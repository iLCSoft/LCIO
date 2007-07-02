#################################
# cmake file for building LCIO
# @author Frank Gaede, DESY
# @author Jan Engels, DESY
#################################

########################################################
# CMake compatibility issues: don't modify this, please!
CMAKE_MINIMUM_REQUIRED( VERSION 2.4.6 )
#SET( CMAKE_BACKWARDS_COMPATIBILITY 2.4.6 )
MARK_AS_ADVANCED(CMAKE_BACKWARDS_COMPATIBILITY)
# allow more human readable "if then else" constructs
SET( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE )
########################################################

# project name
PROJECT( LCIO )

# project version
SET( ${PROJECT_NAME}_MAJOR_VERSION 1 )
SET( ${PROJECT_NAME}_MINOR_VERSION 8 )
SET( ${PROJECT_NAME}_PATCH_LEVEL 3 )

# project options
OPTION( BUILD_SHARED_LIBS "Set to OFF to build static libraries" ON )
OPTION( BUILD_LCIO_TESTJOBS "Set to ON to build LCIO testjobs" OFF )
OPTION( BUILD_F77_TESTJOBS "Set to ON to build LCIO F77 testjobs" OFF )
OPTION( INSTALL_DOC "Set to OFF to skip build/install Documentation" ON )

# set default cmake build type to RelWithDebInfo
IF( NOT CMAKE_BUILD_TYPE )
    SET( CMAKE_BUILD_TYPE "RelWithDebInfo" )
ENDIF()

# set default install prefix to project root directory
IF( CMAKE_INSTALL_PREFIX STREQUAL "/usr/local" )
    SET( CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}" )
ENDIF()

# check for java
# FIXME put this stuff into a module FindJDK.cmake?
IF( NOT DEFINED ENV{JDK_HOME} AND NOT DEFINED ENV{JAVA_HOME} )
    MESSAGE( STATUS "Autodetecting java..." )
    FIND_PACKAGE( Java )
    IF( JAVA_RUNTIME AND JAVA_COMPILE AND JAVA_ARCHIVE )
        SET( java_bin "${JAVA_RUNTIME}" )
        IF( UNIX AND NOT APPLE )
            # initialize flow-control variable
            SET( link_found TRUE )
            
            # dereference links
            WHILE( link_found )
                # check if the java binary is a symbolic link
                EXEC_PROGRAM( file ARGS "${java_bin}"
                    OUTPUT_VARIABLE out_tmp
                    RETURN_VALUE out_ret )
                IF( out_ret )
                    MESSAGE( STATUS "Error dereferencing links to Java Home!!" )
                    MESSAGE( FATAL_ERROR "${out_tmp}" )
                ENDIF()
                # set variable if link is found
                STRING( REGEX MATCH " symbolic link to " link_found "${out_tmp}" )
                IF( link_found )
                    # get the file to where the link points to
                    STRING( REGEX REPLACE ".* symbolic link to (.*)" "\\1" out_regex "${out_tmp}" )
                    MESSAGE( STATUS "Java binary ${java_bin} is a symbolic link to ${out_regex}" )
                    SET( java_bin "${out_regex}" )
                ENDIF()
            ENDWHILE()
        ENDIF()
        
        # extract java_path out of full path to java runtime
        STRING( REGEX REPLACE "(.*)\\/[^/]+\\/java$" "\\1" java_path ${java_bin} )
        # extract java_bin_path out of full path to java runtime
        STRING( REGEX REPLACE "(.*)\\/java$" "\\1" java_bin_path ${java_bin} )
    ELSE()
        MESSAGE( FATAL_ERROR "Java was not found in your system!!" )
    ENDIF()
ELSE()
    # in case JDK_HOME or JAVA_HOME already set ensure that both variables
    # are set correctly (JDK_HOME as well as JAVA_HOME)
    IF( DEFINED ENV{JDK_HOME} AND DEFINED ENV{JAVA_HOME} )
        IF( "$ENV{JDK_HOME}" STREQUAL "$ENV{JAVA_HOME}" )
            SET( java_path "$ENV{JDK_HOME}" )
        ELSE()
            MESSAGE( STATUS 
                "JDK_HOME and JAVA_HOME are set to different paths!!" )
            MESSAGE( STATUS "JDK_HOME: $ENV{JDK_HOME}" )
            MESSAGE( STATUS "JAVA_HOME: $ENV{JAVA_HOME}" )
            MESSAGE( FATAL_ERROR 
                "Please unset one of them or set both correctly!!" )
        ENDIF()
    ELSE()
        IF( DEFINED ENV{JDK_HOME} )
            SET( java_path "$ENV{JDK_HOME}" )
        ENDIF()
        IF( DEFINED ENV{JAVA_HOME} )
            SET( java_path "$ENV{JAVA_HOME}" ) 
        ENDIF()
    ENDIF()

    # find java, javac and jar
    FIND_PROGRAM( java_bin
        java
        ${java_path}/bin
        NO_DEFAULT_PATH )
  
    FIND_PROGRAM( javac_bin
        javac
        ${java_path}/bin
        NO_DEFAULT_PATH )
    
    FIND_PROGRAM( jar_bin
        jar
        ${java_path}/bin
        NO_DEFAULT_PATH )
    
    # abort if not found
    IF( java_bin-NOTFOUND )
        MESSAGE( FATAL_ERROR "Could not find java!!" )
    ENDIF()
    IF( javac_bin-NOTFOUND )
        MESSAGE( FATAL_ERROR "Could not find javac!!" )
    ENDIF()
    IF( jar_bin-NOTFOUND )
        MESSAGE( FATAL_ERROR "Could not find jar!!" )
    ENDIF()

    # extract bin path from full path
    STRING( REGEX REPLACE "(.*)\\/java$" "\\1" java_bin_path ${java_bin} )
ENDIF()

# parse the output of java -version
EXEC_PROGRAM( "${java_bin}" ARGS "-version"
        OUTPUT_VARIABLE out_tmp
        RETURN_VALUE out_ret )
IF( out_ret )
    MESSAGE( STATUS "Error executing java -version!!" )
ENDIF()
STRING( REGEX REPLACE ".* version \"([0-9]+\\.[0-9]+\\.[0-9]+).*"
        "\\1" java_version "${out_tmp}" )

# display info
MESSAGE( STATUS "JDK_HOME and JAVA_HOME set to ${java_path}" )
MESSAGE( STATUS "Java version ${java_version} configured successfully!" )

# set environment variables for
# generating header files with ant aid.generate
SET( ENV{JDK_HOME} "${java_path}" )
SET( ENV{JAVA_HOME} "${java_path}" )
SET( ENV{LCIO} "${CMAKE_SOURCE_DIR}" )
SET( ENV{PATH} "${CMAKE_SOURCE_DIR}/tools:${java_bin_path}:$ENV{PATH}" )

# generate header files with ant aid.generate
IF( NOT EXISTS "${CMAKE_SOURCE_DIR}/src/cpp/include/EVENT/LCIO.h" )

    MESSAGE( STATUS "Generating header files with ant aid.generate..." )
    EXEC_PROGRAM( ant "${CMAKE_SOURCE_DIR}"
        ARGS aid.generate
        OUTPUT_VARIABLE out_tmp
        RETURN_VALUE out_ret )
    # check if everything went ok
    IF( NOT out_ret )
        MESSAGE( STATUS "Header files successfully generated..." )
    ELSE()
        MESSAGE( STATUS "Error generating header files!!" )
        MESSAGE( FATAL_ERROR "${out_tmp}" )
    ENDIF()
ENDIF()

# generate shell script for setting environment
# and calling ant aid
FILE( WRITE "${CMAKE_BINARY_DIR}/script.sh"
            "export LCIO=${CMAKE_SOURCE_DIR}\n"
            "export JDK_HOME=${java_path}\n"
            "export JAVA_HOME=${java_path}\n"
            "export PATH=${java_path}/bin:${CMAKE_SOURCE_DIR}/tools:$PATH\n"
            "ant $1" )

# find shell
FIND_PROGRAM( SH
  sh
  ${CYGWIN_INSTALL_PATH}/bin
  /bin
  /usr/bin
  /usr/local/bin
  /sbin
  # FIXME add path to MacOS here
)
MARK_AS_ADVANCED( SH )

IF( SH-NOTFOUND )
    MESSAGE( STATUS "Unix Shell not found - cannot enable jar target!!" )
ELSE()

    # LCIO - JAVA
    ADD_CUSTOM_COMMAND(
        OUTPUT "${CMAKE_SOURCE_DIR}/lib/lcio.jar"
        COMMAND ${SH} ARGS "${CMAKE_BINARY_DIR}/script.sh" aid
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT "Building LCIO Java..."
        VERBATIM )

    # create java target
    ADD_CUSTOM_TARGET( jar DEPENDS "${CMAKE_SOURCE_DIR}/lib/lcio.jar" )
    
    #ADD_CUSTOM_COMMAND(
    #    OUTPUT "${CMAKE_BINARY_DIR}/src/cpp/include/EVENT/LCIO.h"
    #    COMMAND ${SH} ARGS "${CMAKE_BINARY_DIR}/script.sh" aid.generate
    #    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    #    COMMENT "Generating header files with ant aid.generate..."
    #    VERBATIM )
    #
    #ADD_CUSTOM_TARGET( headers DEPENDS
    #        "${CMAKE_BINARY_DIR}/src/cpp/include/EVENT/LCIO.h" )
    
    # create "always out-of-date" headers target
    ADD_CUSTOM_TARGET( headers
        COMMAND ${SH} "${CMAKE_BINARY_DIR}/script.sh" aid.generate
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT "Generating header files with ant aid.generate..."
        VERBATIM )

ENDIF()

# DOCUMENTATION
FIND_PACKAGE( Doxygen )
FIND_PACKAGE( LATEX )

IF( LATEX_COMPILER AND MAKEINDEX_COMPILER AND PDFLATEX_COMPILER
    AND DVIPS_CONVERTER AND LATEX2HTML_CONVERTER AND DOXYGEN_FOUND )
    
    # api documentation
    ADD_CUSTOM_COMMAND(
        OUTPUT  "${CMAKE_SOURCE_DIR}/doc/doxygen_api/html"
        COMMAND "${DOXYGEN_EXECUTABLE}"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/doc/doxygen_api"
        COMMENT "Building API Documentation..."
        VERBATIM )
    
    ADD_CUSTOM_TARGET( doc_api
            DEPENDS "${CMAKE_SOURCE_DIR}/doc/doxygen_api/html" )

    # user manual
    ADD_CUSTOM_COMMAND(
        OUTPUT  "${CMAKE_BINARY_DIR}/manual"
        # first we copy directory from source tree to binary tree to
        # keep the source tree unchanged
        COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${CMAKE_SOURCE_DIR}/src/latex/manual"
            "${CMAKE_BINARY_DIR}/manual"
        VERBATIM )

    ADD_CUSTOM_COMMAND(
        OUTPUT  "${CMAKE_SOURCE_DIR}/doc/manual.pdf"
        COMMAND "${LATEX_COMPILER}" ARGS -interaction=batchmode manual.tex
        COMMAND "${LATEX_COMPILER}" ARGS -interaction=batchmode manual.tex
        COMMAND "${DVIPS_CONVERTER}" ARGS -q -o manual.ps manual.dvi
        COMMAND "${PDFLATEX_COMPILER}" ARGS -interaction=batchmode manual.tex
        COMMAND "${LATEX2HTML_CONVERTER}" ARGS
            -mkdir -dir manual_html -verbosity 0 -info 0 -no_auto_link -split 0 -no_navigation manual.tex
        # copy files to doc directory
        COMMAND "${CMAKE_COMMAND}" -E copy "manual.pdf"
            "${CMAKE_SOURCE_DIR}/doc"
        COMMAND "${CMAKE_COMMAND}" -E copy "manual.ps"
            "${CMAKE_SOURCE_DIR}/doc"
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "manual_html"
            "${CMAKE_SOURCE_DIR}/doc/manual_html"
        DEPENDS "${CMAKE_BINARY_DIR}/manual"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/manual"
        COMMENT "Building Documentation - Users Manual..."
        VERBATIM )

    ADD_CUSTOM_TARGET( doc_manual
            DEPENDS "${CMAKE_SOURCE_DIR}/doc/manual.pdf" )

    # reference manual
    # FIXME this is not working correctly: latex returns error!
    #ADD_CUSTOM_COMMAND(
    #    OUTPUT  "${CMAKE_SOURCE_DIR}/doc/doxygen_api/latex/refman.dvi"
    #    COMMAND "${LATEX_COMPILER}" ARGS -interaction=batchmode refman.tex
    #    COMMAND "${LATEX_COMPILER}" ARGS -interaction=batchmode refman.tex
    #    COMMAND "${DVIPS_CONVERTER}" ARGS -q -o refman.ps refman.dvi
    #    # FIXME psnup should be detected
    #    COMMAND "psnup" ARGS -q -2 refman.ps ${CMAKE_SOURCE_DIR}/doc/lciorefman.ps
    #    COMMAND "${PDFLATEX_COMPILER}" ARGS -interaction=batchmode refman.tex
    #    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/doc/doxygen_api/latex"
    #    COMMENT "Building Documentation - Reference Manual..."
    #    VERBATIM )

    #ADD_CUSTOM_TARGET( doc_refman
    #        DEPENDS
    #            "${CMAKE_SOURCE_DIR}/doc/doxygen_api/html"
    #            "${CMAKE_SOURCE_DIR}/doc/doxygen_api/latex/refman.dvi" )
    
    # add custom target for building all documentation
    ADD_CUSTOM_TARGET( doc )
    #ADD_DEPENDENCIES( doc doc_api doc_manual doc_refman )
    ADD_DEPENDENCIES( doc doc_api doc_manual )

ELSE()
    IF( NOT DOXYGEN_FOUND )
        MESSAGE( STATUS "Doxygen not found in your system!!" )
    ELSE()
        MESSAGE( STATUS "Latex or one of its tools (dvips, pdflatex, latex2html, makeindex) not found in your system!!" )
    ENDIF()
    IF( INSTALL_DOC )
        MESSAGE( STATUS "INSTALL_DOC forced to OFF" )
        SET( INSTALL_DOC OFF )
    ENDIF()
ENDIF()

# install documentation rules
IF( INSTALL_DOC )
    # make sure documentation is built before make install is called
    INSTALL( CODE "EXEC_PROGRAM(${CMAKE_BUILD_TOOL} ${CMAKE_BINARY_DIR} ARGS doc)" )

    # install documentation
    INSTALL( DIRECTORY "${CMAKE_SOURCE_DIR}/doc"
            DESTINATION .
            PATTERN "*CVS*" EXCLUDE )
ENDIF()

##########################################################################################

# library *nix style versioning
SET( ${PROJECT_NAME}_SOVERSION
    "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}" )
SET( ${PROJECT_NAME}_VERSION
    "${${PROJECT_NAME}_SOVERSION}.${${PROJECT_NAME}_PATCH_LEVEL}" )

# add library install path to the rpath list
SET( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )
MARK_AS_ADVANCED( CMAKE_INSTALL_RPATH )

# append link pathes to rpath list
SET( CMAKE_INSTALL_RPATH_USE_LINK_PATH 1 )
MARK_AS_ADVANCED( CMAKE_INSTALL_RPATH_USE_LINK_PATH )

# output directories
SET( EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin" CACHE PATH
    "EXECUTABLE_OUTPUT_PATH" FORCE )
SET( LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib" CACHE PATH
    "LIBRARY_OUTPUT_PATH" FORCE )
MARK_AS_ADVANCED( EXECUTABLE_OUTPUT_PATH LIBRARY_OUTPUT_PATH )

# DEPENDENCIES: this code has to be placed before adding any library or
# executable so that these are linked properly against the dependencies
IF( DEFINED ${PROJECT_NAME}_DEPENDS OR DEFINED BUILD_WITH OR DEFINED LINK_WITH )
    # load macro
    IF( NOT EXISTS "${CMAKE_MODULE_PATH}/MacroCheckDeps.cmake" )
        MESSAGE( FATAL_ERROR
            "\nSorry, could not find MacroCheckDeps.cmake...\n"
            "Please set CMAKE_MODULE_PATH correctly with: "
            "cmake -DCMAKE_MODULE_PATH=<path_to_cmake_modules>" )
    ENDIF()
    INCLUDE( "${CMAKE_MODULE_PATH}/MacroCheckDeps.cmake" )
    CHECK_DEPS()
ENDIF()

# input directories
ADD_SUBDIRECTORY( sio sio )
ADD_SUBDIRECTORY( src/cpp lcio )
IF( BUILD_F77_TESTJOBS )
    ADD_SUBDIRECTORY( src/f77 f77 )
ENDIF()

# create uninstall configuration file 
CONFIGURE_FILE( "${CMAKE_SOURCE_DIR}/cmake_uninstall.cmake.in"
                "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
                IMMEDIATE @ONLY )

# create uninstall target
ADD_CUSTOM_TARGET( uninstall "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake" )

# create configuration file from .in file
CONFIGURE_FILE( "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in"
                "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY )

# install configuration file
INSTALL( FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        DESTINATION . )

# display status message for important variables
MESSAGE( STATUS )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS "BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS}" )
MESSAGE( STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}" )
MESSAGE( STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}" )
MESSAGE( STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}" )
MESSAGE( STATUS "BUILD_LCIO_TESTJOBS = ${BUILD_LCIO_TESTJOBS}" )
MESSAGE( STATUS "BUILD_F77_TESTJOBS = ${BUILD_F77_TESTJOBS}" )
MESSAGE( STATUS "INSTALL_DOC = ${INSTALL_DOC}" )
MESSAGE( STATUS "Change a value with: cmake -D<Variable>=<Value>" )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS )

# force some variables that could be defined in the command line
# to be written to cache
SET( BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS}" CACHE BOOL
    "Set to OFF to build static libraries" FORCE )
SET( CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}" CACHE PATH
    "Where to install ${PROJECT_NAME}" FORCE )
SET( CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE )
SET( CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" CACHE PATH
    "Path to custom CMake Modules" FORCE )
SET( BUILD_LCIO_TESTJOBS "${BUILD_LCIO_TESTJOBS}" CACHE BOOL
    "Set to ON to build LCIO testbobs" FORCE )
SET( BUILD_F77_TESTJOBS "${BUILD_F77_TESTJOBS}" CACHE BOOL
    "Set to ON to build LCIO F77 testbobs" FORCE )
SET( INSTALL_DOC "${INSTALL_DOC}" CACHE BOOL
    "Set to OFF to skip build/install Documentation" FORCE )

# export build settings
INCLUDE( CMakeExportBuildSettings )
CMAKE_EXPORT_BUILD_SETTINGS( "${PROJECT_NAME}BuildSettings.cmake" )

# export library dependencies (keep this as the last line in the file)
EXPORT_LIBRARY_DEPENDENCIES( "${PROJECT_NAME}LibDeps.cmake" )
