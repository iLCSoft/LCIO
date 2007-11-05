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
# enable testing
ENABLE_TESTING()
########################################################

# project name
PROJECT( LCIO )

# project version
SET( ${PROJECT_NAME}_MAJOR_VERSION 1 )
SET( ${PROJECT_NAME}_MINOR_VERSION 8 )
SET( ${PROJECT_NAME}_PATCH_LEVEL 5 )

# project options
OPTION( BUILD_SHARED_LIBS "Set to OFF to build static libraries" ON )
OPTION( BUILD_LCIO_TESTJOBS "Set to ON to build LCIO testjobs" OFF )
OPTION( BUILD_LCIO_SHELL "Set to ON to build LCIO shell lsh" OFF )
OPTION( BUILD_F77_TESTJOBS "Set to ON to build LCIO F77 testjobs" OFF )
OPTION( INSTALL_DOC "Set to OFF to skip build/install Documentation" ON )
OPTION( INSTALL_JAR "Set to OFF to skip build/install lcio.jar" ON )

# set default cmake build type to RelWithDebInfo
IF( NOT CMAKE_BUILD_TYPE )
    SET( CMAKE_BUILD_TYPE "RelWithDebInfo" )
ENDIF()

# set default install prefix to project root directory
IF( CMAKE_INSTALL_PREFIX STREQUAL "/usr/local" )
    SET( CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}" )
ENDIF()

# include FindJAVA.cmake for finding Java

INCLUDE( FindJAVA.cmake )

IF( JAVA_FOUND )
    MESSAGE( STATUS "JAVA_HOME set to ${JAVA_HOME}" )
ELSE()
    MESSAGE( FATAL_ERROR "Please set JAVA_HOME correctly!!" )
ENDIF()

# set environment variables for
# generating header files with ant aid.generate
SET( ENV{JDK_HOME} "${JAVA_HOME}" )
SET( ENV{JAVA_HOME} "${JAVA_HOME}" )
SET( ENV{LCIO} "${PROJECT_SOURCE_DIR}" )
SET( ENV{PATH} "${PROJECT_SOURCE_DIR}/tools:${JAVA_BIN_PATH}:$ENV{PATH}" )

# generate header files with ant aid.generate
IF( NOT EXISTS "${PROJECT_SOURCE_DIR}/src/cpp/include/EVENT/LCIO.h" )

    MESSAGE( STATUS "Generating header files with ant aid.generate..." )
    EXEC_PROGRAM( ant "${PROJECT_SOURCE_DIR}"
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

# add custom target for building all documentation
ADD_CUSTOM_TARGET( doc )

# code for *nix only!
IF( UNIX )
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

    IF( SH )
        # generate shell script for setting environment
        # and calling ant aid
        MESSAGE( STATUS "Generating shell script for calling ant..." )
        FILE( WRITE "${PROJECT_BINARY_DIR}/script.sh"
            "export LCIO=${PROJECT_SOURCE_DIR}\n"
            "export JDK_HOME=${JAVA_HOME}\n"
            "export JAVA_HOME=${JAVA_HOME}\n"
            "export PATH=${PROJECT_SOURCE_DIR}/tools:${JAVA_BIN_PATH}:$PATH\n"
            "ant $1" )
     
        # create "always out-of-date" headers target
        ADD_CUSTOM_TARGET( headers
            COMMAND ${SH} "${PROJECT_BINARY_DIR}/script.sh" aid.generate
            WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
            COMMENT "Generating header files with ant aid.generate..."
            VERBATIM )
        
        #ADD_CUSTOM_COMMAND(
        #    OUTPUT "${PROJECT_BINARY_DIR}/src/cpp/include/EVENT/LCIO.h"
        #    COMMAND ${SH} ARGS "${PROJECT_BINARY_DIR}/script.sh" aid.generate
        #    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        #    COMMENT "Generating header files with ant aid.generate..."
        #    VERBATIM )
        #
        #ADD_CUSTOM_TARGET( headers DEPENDS
        #        "${PROJECT_BINARY_DIR}/src/cpp/include/EVENT/LCIO.h" )

        # LCIO - JAVA
        ADD_CUSTOM_COMMAND(
            OUTPUT "${PROJECT_SOURCE_DIR}/lib/lcio.jar"
            COMMAND ${SH} ARGS "${PROJECT_BINARY_DIR}/script.sh" aid
            WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
            COMMENT "Building LCIO Java..."
            VERBATIM )

        # create java target
        ADD_CUSTOM_TARGET( jar DEPENDS "${PROJECT_SOURCE_DIR}/lib/lcio.jar" )

        # create custom target for cleaning class files
        ADD_CUSTOM_TARGET( jar_clean
            COMMAND ${SH} "${PROJECT_BINARY_DIR}/script.sh" clean
            WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
            COMMENT "Cleaning java classes with ant clean..."
            VERBATIM )

        # tell make clean to also erase lcio.jar
        SET_DIRECTORY_PROPERTIES( PROPERTIES
            ADDITIONAL_MAKE_CLEAN_FILES "${PROJECT_SOURCE_DIR}/lib/lcio.jar" )

        #JAVADOC
        IF( JAVA_DOC )
            ADD_CUSTOM_COMMAND(
                OUTPUT "${PROJECT_SOURCE_DIR}/doc/api/index.html"
                COMMAND ${SH} ARGS "${PROJECT_BINARY_DIR}/script.sh" doc
                WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                COMMENT "Building Java API Documentation..."
                VERBATIM )
            
            ADD_CUSTOM_TARGET( doc_java DEPENDS "${PROJECT_SOURCE_DIR}/doc/api/index.html" )
            # tell doc target to call doc_java
            ADD_DEPENDENCIES( doc doc_java )
        ENDIF()
    ELSE()
        MESSAGE( STATUS "Unix Shell needed for calling ant not found!!" )
        IF( INSTALL_DOC )
            MESSAGE( STATUS "WARNING: INSTALL_DOC forced to OFF and doc_java disabled!" )
            SET( INSTALL_DOC OFF )
        ELSE()
            MESSAGE( STATUS "WARNING: doc_java target disabled!" )
        ENDIF()
        IF( INSTALL_JAR )
            MESSAGE( STATUS "WARNING: INSTALL_JAR forced to OFF and jar target disabled!" )
            SET( INSTALL_JAR OFF )
        ELSE()
            MESSAGE( STATUS "WARNING: jar target disabled!" )
        ENDIF()
    ENDIF()
ENDIF()

IF( WIN32 )
    MESSAGE( SEND_ERROR "Windows support not implemented yet!!!" )
    #TODO generate script for calling ant.bat
ENDIF()

# doxygen and latex stuff
FIND_PACKAGE( Doxygen )
FIND_PACKAGE( LATEX )

IF( LATEX_COMPILER AND MAKEINDEX_COMPILER AND PDFLATEX_COMPILER
    AND DVIPS_CONVERTER AND LATEX2HTML_CONVERTER AND DOXYGEN_FOUND )
    
    # c++ documentation
    ADD_CUSTOM_COMMAND(
        OUTPUT  "${PROJECT_SOURCE_DIR}/doc/doxygen_api/html"
        COMMAND "${DOXYGEN_EXECUTABLE}"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/doc/doxygen_api"
        COMMENT "Building C++ API Documentation..."
        VERBATIM )
    
    ADD_CUSTOM_TARGET( doc_cpp
            DEPENDS "${PROJECT_SOURCE_DIR}/doc/doxygen_api/html" )

    # user manual
    ADD_CUSTOM_COMMAND(
        OUTPUT  "${PROJECT_BINARY_DIR}/manual"
        # first we copy directory from source tree to binary tree to
        # keep the source tree unchanged
        COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${PROJECT_SOURCE_DIR}/src/latex/manual"
            "${PROJECT_BINARY_DIR}/manual"
        VERBATIM )

    ADD_CUSTOM_COMMAND(
        OUTPUT  "${PROJECT_SOURCE_DIR}/doc/manual.pdf"
        COMMAND "${LATEX_COMPILER}" ARGS -interaction=batchmode manual.tex
        COMMAND "${LATEX_COMPILER}" ARGS -interaction=batchmode manual.tex
        COMMAND "${DVIPS_CONVERTER}" ARGS -q -o manual.ps manual.dvi
        COMMAND "${PDFLATEX_COMPILER}" ARGS -interaction=batchmode manual.tex
        COMMAND "${LATEX2HTML_CONVERTER}" ARGS
            -mkdir -dir manual_html -verbosity 0 -info 0 -no_auto_link -split 0 -no_navigation manual.tex
        # copy files to doc directory
        COMMAND "${CMAKE_COMMAND}" -E copy "manual.pdf"
            "${PROJECT_SOURCE_DIR}/doc"
        COMMAND "${CMAKE_COMMAND}" -E copy "manual.ps"
            "${PROJECT_SOURCE_DIR}/doc"
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "manual_html"
            "${PROJECT_SOURCE_DIR}/doc/manual_html"
        DEPENDS "${PROJECT_BINARY_DIR}/manual"
        WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/manual"
        COMMENT "Building Documentation - Users Manual..."
        VERBATIM )

    ADD_CUSTOM_TARGET( doc_manual
            DEPENDS "${PROJECT_SOURCE_DIR}/doc/manual.pdf" )

    # reference manual
    # FIXME this is not working correctly: latex returns error!
    #ADD_CUSTOM_COMMAND(
    #    OUTPUT  "${PROJECT_SOURCE_DIR}/doc/doxygen_api/latex/refman.dvi"
    #    COMMAND "${LATEX_COMPILER}" ARGS -interaction=batchmode refman.tex
    #    COMMAND "${LATEX_COMPILER}" ARGS -interaction=batchmode refman.tex
    #    COMMAND "${DVIPS_CONVERTER}" ARGS -q -o refman.ps refman.dvi
    #    # FIXME psnup should be detected
    #    COMMAND "psnup" ARGS -q -2 refman.ps ${PROJECT_SOURCE_DIR}/doc/lciorefman.ps
    #    COMMAND "${PDFLATEX_COMPILER}" ARGS -interaction=batchmode refman.tex
    #    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/doc/doxygen_api/latex"
    #    COMMENT "Building Documentation - Reference Manual..."
    #    VERBATIM )

    #ADD_CUSTOM_TARGET( doc_refman
    #        DEPENDS
    #            "${PROJECT_SOURCE_DIR}/doc/doxygen_api/html"
    #            "${PROJECT_SOURCE_DIR}/doc/doxygen_api/latex/refman.dvi" )
    
    #ADD_DEPENDENCIES( doc doc_cpp doc_manual doc_refman )
    ADD_DEPENDENCIES( doc doc_cpp doc_manual )

ELSE()
    IF( NOT DOXYGEN_FOUND )
        MESSAGE( STATUS "Doxygen not found in your system!!" )
    ELSE()
        MESSAGE( STATUS "Latex or one of its tools (dvips, pdflatex, latex2html, makeindex) not found in your system!!" )
    ENDIF()
    IF( INSTALL_DOC )
        MESSAGE( STATUS "WARNING: INSTALL_DOC forced to OFF" )
        SET( INSTALL_DOC OFF )
    ENDIF()
ENDIF()

# installation rules
IF( INSTALL_JAR )
    # make sure jar is built before calling make install
    INSTALL( CODE "EXEC_PROGRAM(${CMAKE_BUILD_TOOL} ${PROJECT_BINARY_DIR} ARGS jar)" )
    # install jar
    INSTALL( FILES "${PROJECT_SOURCE_DIR}/lib/lcio.jar" DESTINATION lib )
ENDIF()
IF( INSTALL_DOC )
    # make sure documentation is built before make install is called
    INSTALL( CODE "EXEC_PROGRAM(${CMAKE_BUILD_TOOL} ${PROJECT_BINARY_DIR} ARGS doc)" )

    # install documentation
    INSTALL( DIRECTORY "${PROJECT_SOURCE_DIR}/doc"
            DESTINATION .
            PATTERN "*CVS*" EXCLUDE )
ENDIF()

##########################################################################################

# library *nix style versioning
SET( ${PROJECT_NAME}_SOVERSION
    "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}" )
SET( ${PROJECT_NAME}_VERSION
    "${${PROJECT_NAME}_SOVERSION}.${${PROJECT_NAME}_PATCH_LEVEL}" )

# add install path to the rpath list
SET( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )
MARK_AS_ADVANCED( CMAKE_INSTALL_RPATH )

# add install path to the rpath list (apple)
IF( APPLE )
    SET( CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib" )
    MARK_AS_ADVANCED( CMAKE_INSTALL_NAME_DIR )
ENDIF()

# append link pathes to rpath list
SET( CMAKE_INSTALL_RPATH_USE_LINK_PATH 1 )
MARK_AS_ADVANCED( CMAKE_INSTALL_RPATH_USE_LINK_PATH )

# output directories
SET( EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin" CACHE PATH
    "EXECUTABLE_OUTPUT_PATH" FORCE )
SET( LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib" CACHE PATH
    "LIBRARY_OUTPUT_PATH" FORCE )
MARK_AS_ADVANCED( EXECUTABLE_OUTPUT_PATH LIBRARY_OUTPUT_PATH )

# DEPENDENCIES: this code has to be placed before adding any library or
# executable so that these are linked properly against the dependencies
#IF( DEFINED ${PROJECT_NAME}_DEPENDS OR DEFINED BUILD_WITH OR DEFINED LINK_WITH )
#    # load macro
#    IF( NOT EXISTS "${CMAKE_MODULE_PATH}/MacroCheckDeps.cmake" )
#        MESSAGE( FATAL_ERROR
#            "\nSorry, could not find MacroCheckDeps.cmake...\n"
#            "Please set CMAKE_MODULE_PATH correctly with: "
#            "cmake -DCMAKE_MODULE_PATH=<path_to_cmake_modules>" )
#    ENDIF()
#    INCLUDE( "${CMAKE_MODULE_PATH}/MacroCheckDeps.cmake" )
#    CHECK_DEPS()
#ENDIF()

# This code would not be necessary if the DEPENDENCIES code above would not
# be commented out, but this way LCIO does not depend on CMakeModules
IF( DEFINED BUILD_WITH AND BUILD_WITH STREQUAL "CLHEP" )
    IF( NOT DEFINED CLHEP_HOME )
        MESSAGE( FATAL_ERROR "Check for CLHEP: CLHEP_HOME not set!! "
            "Please call cmake with -DCLHEP_HOME=<path_to_CLHEP>" )
    ENDIF()
    MESSAGE( STATUS "Check for CLHEP: ${CLHEP_HOME}" )
    INCLUDE( FindCLHEP.cmake )

    IF( CLHEP_FOUND )
        INCLUDE_DIRECTORIES( ${CLHEP_INCLUDE_DIR} )
        LINK_LIBRARIES( ${CLHEP_LIBRARIES} )
        ADD_DEFINITIONS( ${CLHEP_DEFINITIONS} )
        SEPARATE_ARGUMENTS( CLHEP_DEFINITIONS )
        LIST( APPEND ${PROJECT_NAME}_DEFINITIONS ${CLHEP_DEFINITIONS} )
    ENDIF()

    SET( BUILD_WITH "${BUILD_WITH}" CACHE STRING
        "Build ${PROJECT_NAME} with these optional packages" FORCE )
ENDIF()

# input directories
ADD_SUBDIRECTORY( sio sio )
ADD_SUBDIRECTORY( src/cpp lcio )
IF( BUILD_F77_TESTJOBS )
    ADD_SUBDIRECTORY( src/f77 f77 )
ENDIF()

# create uninstall configuration file 
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake_uninstall.cmake.in"
                "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake"
                IMMEDIATE @ONLY )

# create uninstall target
ADD_CUSTOM_TARGET( uninstall "${CMAKE_COMMAND}" -P "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake" )

# create configuration file from .in file
LIST( APPEND ${PROJECT_NAME}_DEFINITIONS "-Wno-long-long" )
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in"
                "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY )

# install configuration file
INSTALL( FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        DESTINATION . )

# display status message for important variables
MESSAGE( STATUS )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS "BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS}" )
MESSAGE( STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}" )
MESSAGE( STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}" )
MESSAGE( STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}" )
MESSAGE( STATUS "BUILD_LCIO_TESTJOBS = ${BUILD_LCIO_TESTJOBS}" )
MESSAGE( STATUS "BUILD_LCIO_SHELL = ${BUILD_LCIO_SHELL}" )
MESSAGE( STATUS "BUILD_F77_TESTJOBS = ${BUILD_F77_TESTJOBS}" )
MESSAGE( STATUS "INSTALL_DOC = ${INSTALL_DOC}" )
MESSAGE( STATUS "INSTALL_JAR = ${INSTALL_JAR}" )
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
SET( INSTALL_JAR "${INSTALL_JAR}" CACHE BOOL
    "Set to OFF to skip build/install lcio.jar" FORCE )

# export build settings
INCLUDE( CMakeExportBuildSettings )
CMAKE_EXPORT_BUILD_SETTINGS( "${PROJECT_NAME}BuildSettings.cmake" )

# export library dependencies (keep this as the last line in the file)
EXPORT_LIBRARY_DEPENDENCIES( "${PROJECT_NAME}LibDeps.cmake" )
