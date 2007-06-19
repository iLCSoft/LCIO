#################################
# cmake file for building LCIO
# @author Frank Gaede, DESY
# @author Jan Engels, DESY
#################################

# cmake minimum required version
CMAKE_MINIMUM_REQUIRED( VERSION 2.4 )
#SET( CMAKE_BACKWARDS_COMPATIBILITY 2.4 )
MARK_AS_ADVANCED(CMAKE_BACKWARDS_COMPATIBILITY)

# allow more human readable "if then else" constructs
SET( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE )

# project name
PROJECT( LCIO )

# project options
OPTION( BUILD_SHARED_LIBS "Set this to OFF to build static libraries " ON )
OPTION( BUILD_LCIO_TESTJOBS "Set to ON to build LCIO testjobs" OFF )
OPTION( BUILD_F77_TESTJOBS "Set to ON to build LCIO F77 testjobs" OFF )
OPTION( INSTALL_DOC "Set to ON to install Documentation" ON )

# project version
SET( ${PROJECT_NAME}_MAJOR_VERSION 1 )
SET( ${PROJECT_NAME}_MINOR_VERSION 8 )
SET( ${PROJECT_NAME}_PATCH_LEVEL 3 )
SET( ${PROJECT_NAME}_VERSION
    "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_LEVEL}" )
SET( ${PROJECT_NAME}_SOVERSION
    "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}" )

# set default cmake build type to RelWithDebInfo
IF( NOT CMAKE_BUILD_TYPE )
    SET( CMAKE_BUILD_TYPE "RelWithDebInfo" )
ENDIF()

# set default install prefix to project root directory
#IF( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
IF( CMAKE_INSTALL_PREFIX STREQUAL "/usr/local" )
    SET( CMAKE_INSTALL_PREFIX "${${PROJECT_NAME}_SOURCE_DIR}" )
ENDIF()

# add library install path to the rpath list
SET( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )
MARK_AS_ADVANCED( CMAKE_INSTALL_RPATH )

# append link pathes to rpath list
SET( CMAKE_INSTALL_RPATH_USE_LINK_PATH 1 )
MARK_AS_ADVANCED( CMAKE_INSTALL_RPATH_USE_LINK_PATH )

# output directories
SET( EXECUTABLE_OUTPUT_PATH "${${PROJECT_NAME}_BINARY_DIR}/bin" CACHE PATH
    "EXECUTABLE_OUTPUT_PATH" FORCE )
SET( LIBRARY_OUTPUT_PATH "${${PROJECT_NAME}_BINARY_DIR}/lib" CACHE PATH
    "LIBRARY_OUTPUT_PATH" FORCE )
MARK_AS_ADVANCED( EXECUTABLE_OUTPUT_PATH LIBRARY_OUTPUT_PATH )

# generate header files with ant aid.generate
IF( NOT EXISTS "${${PROJECT_NAME}_SOURCE_DIR}/src/cpp/include/EVENT/LCIO.h" )
    # check if java is available
    # FIXME put this stuff into a module FindJDK.cmake?
    
    IF( NOT DEFINED ENV{JDK_HOME} AND NOT DEFINED ENV{JAVA_HOME} )
        MESSAGE( STATUS "Autodetecting java..." )
        FIND_PACKAGE( Java )
        IF( JAVA_RUNTIME )
            SET( java_bin "${JAVA_RUNTIME}" )
            IF( UNIX )
                # initialize flow-control variable
                SET( link_found TRUE )
                
                # dereference links
                WHILE( link_found )
                    # check if the java binary is a symbolic link
                    EXEC_PROGRAM( file ARGS "${java_bin}"
                        OUTPUT_VARIABLE out_tmp
                        RETURN_VALUE out_ret )
                    IF( out_ret )
                        MESSAGE( SEND_ERROR "Error dereferencing links to Java Home!!" )
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
            # get java_path out of java binary
            STRING( REGEX REPLACE "(.*)\\/bin\\/java$" "\\1" java_path ${java_bin} )
        ELSE()
            MESSAGE( FATAL_ERROR "Java was not found in your system!!" )
        ENDIF()
    ELSE()
        # in case JDK_HOME or JAVA_HOME already set ensure that both variables
        # are set correctly (JDK_HOME as well as JAVA_HOME)
        IF( DEFINED ENV{JDK_HOME} AND DEFINED ENV{JAVA_HOME} )
            IF( ENV{JDK_HOME} STREQUAL "$ENV{JAVA_HOME}" )
                SET( java_path "$ENV{JDK_HOME}" )
            ELSE()
                MESSAGE( SEND_ERROR 
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
        SET( java_bin "${java_path}/bin/java" )
        IF( NOT EXISTS "${java_bin}" )
            MESSAGE( FATAL_ERROR
                "Java runtime not found in: ${java_bin}" )
        ENDIF()
    ENDIF()

    # parse the output of java -version
    EXEC_PROGRAM( "${java_bin}" ARGS "-version"
            OUTPUT_VARIABLE out_tmp
            RETURN_VALUE out_ret )
    IF( out_ret )
        MESSAGE( SEND_ERROR "Error executing java -version!!" )
    ENDIF()
    STRING( REGEX REPLACE ".* version \"([0-9]+\\.[0-9]+\\.[0-9]+).*"
            "\\1" java_version "${out_tmp}" )
    
    # display info
    MESSAGE( STATUS "JDK_HOME and JAVA_HOME set to ${java_path}" )
    MESSAGE( STATUS "Java version ${java_version} configured successfully!" )
    
    # set environment variables
    SET( ENV{JDK_HOME} "${java_path}" )
    SET( ENV{JAVA_HOME} "${java_path}" )
    SET( ENV{LCIO} "${${PROJECT_NAME}_SOURCE_DIR}" )
    SET( ENV{PATH} "${${PROJECT_NAME}_SOURCE_DIR}/tools:${java_path}/bin:$ENV{PATH}" )

    # now generate header files
    MESSAGE( STATUS "Generating ${PROJECT_NAME} header files with ant aid.generate..." )
    EXEC_PROGRAM( ant "${${PROJECT_NAME}_SOURCE_DIR}"
        ARGS aid.generate
        OUTPUT_VARIABLE out_tmp
        RETURN_VALUE out_ret )
    # check if everything went ok
    IF( NOT out_ret )
        MESSAGE( STATUS "Header files successfully generated..." )
    ELSE()
        MESSAGE( SEND_ERROR "Error generating header files!!" )
        MESSAGE( FATAL_ERROR "${out_tmp}" )
    ENDIF()
ENDIF()

# input directories
ADD_SUBDIRECTORY( sio sio )
ADD_SUBDIRECTORY( src/cpp lcio )
ADD_SUBDIRECTORY( src/f77 f77 )

# find doc tools
FIND_PACKAGE( Doxygen )
FIND_PACKAGE( LATEX )

IF( LATEX_COMPILER AND MAKEINDEX_COMPILER AND PDFLATEX_COMPILER
    AND DVIPS_CONVERTER AND LATEX2HTML_CONVERTER AND DOXYGEN_FOUND )
    
    # api documentation
    ADD_CUSTOM_COMMAND(
        OUTPUT  "${${PROJECT_NAME}_SOURCE_DIR}/doc/doxygen_api/html"
        COMMAND "${DOXYGEN_EXECUTABLE}"
        WORKING_DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/doc/doxygen_api"
        COMMENT "Building API Documentation..."
        VERBATIM )
    
    ADD_CUSTOM_TARGET( doc_api
            DEPENDS "${${PROJECT_NAME}_SOURCE_DIR}/doc/doxygen_api/html" )

    # user manual
    ADD_CUSTOM_COMMAND(
        OUTPUT  "${${PROJECT_NAME}_BINARY_DIR}/manual"
        # first we copy directory from source tree to binary tree to
        # keep the source tree unchanged
        COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${${PROJECT_NAME}_SOURCE_DIR}/src/latex/manual"
            "${${PROJECT_NAME}_BINARY_DIR}/manual"
        VERBATIM )

    ADD_CUSTOM_COMMAND(
        OUTPUT  "${${PROJECT_NAME}_SOURCE_DIR}/doc/manual.pdf"
        # copy directory from source tree to binary tree
        COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${${PROJECT_NAME}_SOURCE_DIR}/src/latex/manual"
            "${${PROJECT_NAME}_BINARY_DIR}/manual"
        COMMAND "${LATEX_COMPILER}" ARGS -interaction=batchmode manual.tex
        COMMAND "${LATEX_COMPILER}" ARGS -interaction=batchmode manual.tex
        COMMAND "${DVIPS_CONVERTER}" ARGS -q manual.dvi
        COMMAND "${PDFLATEX_COMPILER}" ARGS -interaction=batchmode manual.tex
        COMMAND "${LATEX2HTML_CONVERTER}" ARGS
            -mkdir -dir manual_html -verbosity 0 -info 0 -no_auto_link -split 0 -no_navigation manual.tex
        # copy files to doc directory
        COMMAND "${CMAKE_COMMAND}" -E copy "manual.pdf"
            "${${PROJECT_NAME}_SOURCE_DIR}/doc"
        COMMAND "${CMAKE_COMMAND}" -E copy "manual.ps"
            "${${PROJECT_NAME}_SOURCE_DIR}/doc"
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "manual_html"
            "${${PROJECT_NAME}_SOURCE_DIR}/doc/manual_html"
        DEPENDS "${${PROJECT_NAME}_BINARY_DIR}/manual"
        WORKING_DIRECTORY "${${PROJECT_NAME}_BINARY_DIR}/manual"
        COMMENT "Building Documentation - Users Manual..."
        VERBATIM )

    ADD_CUSTOM_TARGET( doc_manual
            DEPENDS "${${PROJECT_NAME}_SOURCE_DIR}/doc/manual.pdf" )

    # reference manual
    # FIXME this is not working correctly
    #ADD_CUSTOM_COMMAND(
    #    OUTPUT  "${${PROJECT_NAME}_SOURCE_DIR}/doc/doxygen_api/latex/refman.dvi"
    #    COMMAND "${LATEX_COMPILER}" ARGS -interaction=batchmode refman.tex
    #    COMMAND "${LATEX_COMPILER}" ARGS -interaction=batchmode refman.tex
    #    COMMAND "${DVIPS_CONVERTER}" ARGS -q refman.dvi
    #    # FIXME psnup should be detected
    #    COMMAND "psnup" ARGS -q -2 refman.ps ${${PROJECT_NAME}_SOURCE_DIR}/doc/lciorefman.ps
    #    COMMAND "${PDFLATEX_COMPILER}" ARGS -interaction=batchmode refman.tex
    #    WORKING_DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/doc/doxygen_api/latex"
    #    COMMENT "Building Documentation - Reference Manual..."
    #    VERBATIM )

    #ADD_CUSTOM_TARGET( doc_refman
    #        DEPENDS
    #            "${${PROJECT_NAME}_SOURCE_DIR}/doc/doxygen_api/html"
    #            "${${PROJECT_NAME}_SOURCE_DIR}/doc/doxygen_api/latex/refman.dvi" )
    
    # add custom target for building all documentation
    ADD_CUSTOM_TARGET( doc )
    #ADD_DEPENDENCIES( doc doc_api doc_manual doc_refman )
    ADD_DEPENDENCIES( doc doc_api doc_manual )

ELSE()
    IF( NOT DOXYGEN_FOUND )
        MESSAGE( SEND_ERROR "Doxygen not found in your system!!" )
    ELSE()
        MESSAGE( SEND_ERROR "Latex or one of its tools (dvips, pdflatex, latex2html, makeindex) not found in your system!!" )
    ENDIF()
    IF( INSTALL_DOC )
        MESSAGE( STATUS "INSTALL_DOC forced to OFF" )
        SET( INSTALL_DOC OFF )
    ENDIF()
ENDIF()


# install documentation rules
IF( INSTALL_DOC )
    # make sure documentation is built before make install is called
    INSTALL( CODE "EXEC_PROGRAM(${CMAKE_BUILD_TOOL} ${${PROJECT_NAME}_BINARY_DIR} ARGS doc)" )

    # install documentation
    INSTALL( DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/doc"
            DESTINATION .
            PATTERN "*CVS*" EXCLUDE )
ENDIF()

# create uninstall configuration file 
CONFIGURE_FILE( "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
                "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
                IMMEDIATE @ONLY )

# create uninstall target
ADD_CUSTOM_TARGET( uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" )

# create configuration file from .in file
CONFIGURE_FILE( "${${PROJECT_NAME}_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in"
                "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY )

# install configuration file
INSTALL( FILES "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
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
    "Build shared libraries flag" FORCE )
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
    "Set to ON to install Documentation" FORCE )

# export library dependencies (keep this as the last line in the file)
EXPORT_LIBRARY_DEPENDENCIES( "${PROJECT_NAME}LibDeps.cmake" )
