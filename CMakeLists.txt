########################################################
# cmake file for building LCIO
# @author Frank Gaede, DESY
# @author Jan Engels, DESY
CMAKE_MINIMUM_REQUIRED( VERSION 3.14 FATAL_ERROR )
########################################################


# project name
PROJECT( LCIO LANGUAGES CXX )
include(GNUInstallDirs)

# project version
SET( LCIO_VERSION_MAJOR 2 )
SET( LCIO_VERSION_MINOR 22 )
SET( LCIO_VERSION_PATCH 6 )

### set correct LCIO version in relevant files  ############################

configure_file (
  "${PROJECT_SOURCE_DIR}/cmake/LCIO.aid.in"
  "${PROJECT_SOURCE_DIR}/src/aid/EVENT/LCIO.aid"
  )
configure_file (
  "${PROJECT_SOURCE_DIR}/cmake/LCIO.h.in"
  "${PROJECT_SOURCE_DIR}/src/cpp/include/pre-generated/EVENT/LCIO.h"
  )
configure_file (
  "${PROJECT_SOURCE_DIR}/cmake/lcio.properties.in"
  "${PROJECT_SOURCE_DIR}/config/lcio.properties"
  )
configure_file (
  "${PROJECT_SOURCE_DIR}/cmake/lcio_namespaces.h.in"
  "${PROJECT_SOURCE_DIR}/src/cpp/include/lcio.h"
  )
configure_file (
  "${PROJECT_SOURCE_DIR}/cmake/index.html.in"
  "${PROJECT_SOURCE_DIR}/doc/index.html"
  )
configure_file (
  "${PROJECT_SOURCE_DIR}/cmake/lcio.xml.in"
  "${PROJECT_SOURCE_DIR}/doc/lcio.xml"
  )
configure_file (
  "${PROJECT_SOURCE_DIR}/cmake/manual.tex.in"
  "${PROJECT_SOURCE_DIR}/src/latex/manual/manual.tex"
  )
configure_file (
  "${PROJECT_SOURCE_DIR}/cmake/pom.xml.in"
  "${PROJECT_SOURCE_DIR}/pom.xml"
  )



### DEPENDENCIES ############################################################

LIST( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )

option(LCIO_SET_RPATH   "Link libraries with built-in RPATH (run-time search path)" ON)

#FIND_PACKAGE( ILCUTIL COMPONENTS ILCSOFT_CMAKE_MODULES REQUIRED )

# load default settings from ILCSOFT_CMAKE_MODULES
INCLUDE( ilcsoft_default_settings )

# ``-DCMAKE_CXX_STANDARD=<standard>`` when invoking CMake
SET(CMAKE_CXX_STANDARD 17 CACHE STRING "")

if(NOT CMAKE_CXX_STANDARD MATCHES "17|20|23")
  MESSAGE(FATAL_ERROR "Unsupported C++ standard: ${CMAKE_CXX_STANDARD} (need >= 17)")
endif()
SET( CMAKE_CXX_STANDARD_REQUIRED ON )

# lcio.jar
OPTION( INSTALL_JAR "Set to OFF to skip build/install lcio.jar" OFF )

# allow C++ only builds
OPTION( LCIO_GENERATE_HEADERS "Set to OFF to use pre-generated headers (no java needed)" OFF)

IF( INSTALL_JAR )
    INSTALL( CODE "EXECUTE_PROCESS( COMMAND ${CMAKE_BUILD_TOOL} jar)" )
    INSTALL( FILES "${PROJECT_SOURCE_DIR}/lib/lcio.jar" DESTINATION lib )# OPTIONAL )
ENDIF()


# ROOT
OPTION( BUILD_ROOTDICT "Set to ON to build ROOT dictionary" ON )
IF( BUILD_ROOTDICT )
    FIND_PACKAGE( ROOT 6.04 REQUIRED ) # look for ROOT versions >= 5.27.06
    INCLUDE(${ROOT_USE_FILE})
ENDIF()


# install python stuff (for out-of-source-build)
INSTALL( DIRECTORY "${PROJECT_SOURCE_DIR}/src/python/" DESTINATION python )
INSTALL( DIRECTORY "${PROJECT_SOURCE_DIR}/examples/python/" DESTINATION python/examples )
# install executable python utilities
IF( BUILD_ROOTDICT )
  INSTALL( PROGRAMS "${PROJECT_SOURCE_DIR}/examples/python/anajob.py" TYPE BIN RENAME anajob.py )
ENDIF()


# JAVA (needed to generate c++ headers)
if( INSTALL_JAR OR LCIO_GENERATE_HEADERS)
  INCLUDE( FindJAVA4LCIO )
  IF( NOT JAVA_FOUND )
    MESSAGE( FATAL_ERROR "Failed to find Java! Please set JAVA_DIR=/path/to/java \n"
      "set INSTALL_JAR AND LCIO_GENERATE_HEADERS to OFF for C++ only build" )
  ENDIF()
endif()


# DCAP
#OPTION( LCIO_DCAP "Set to ON to build LCIO with dcap (dCache) support" OFF )
#MESSAGE( STATUS "LCIO_DCAP = ${LCIO_DCAP}" )
#IF( LCIO_DCAP )
#    FIND_LIBRARY( DCAP_LIBRARIES NAMES dcap PATHS ${DCAP_DIR}/lib NO_DEFAULT_PATH )
#    IF( NOT DCAP_DIR )
#        FIND_LIBRARY( DCAP_LIBRARIES NAMES dcap )
#    ENDIF()
#    FIND_PATH( DCAP_INCLUDE_DIRS NAMES dcap.h PATHS ${DCAP_DIR}/include NO_DEFAULT_PATH )
#    IF( NOT DCAP_DIR )
#        FIND_PATH( DCAP_INCLUDE_DIRS NAMES dcap.h )
#    ENDIF()
#
#    IF( DCAP_LIBRARIES AND DCAP_INCLUDE_DIRS )
#        SET( DCAP_FOUND TRUE )
#        LINK_LIBRARIES( ${DCAP_LIBRARIES} )
#    ELSE()
#        MESSAGE( SEND_ERROR "dcap not found. please set DCAP_DIR=/path/to/dcap or option LCIO_DCAP to OFF" )
#    ENDIF()
#ENDIF()


# UNIX SHELL (needed for calling LCIO_ENV_INIT script)
FIND_PROGRAM( SH sh ${CYGWIN_INSTALL_PATH}/bin /bin /usr/bin /usr/local/bin /sbin )
MARK_AS_ADVANCED( SH )
IF( NOT SH )
    MESSAGE( FATAL_ERROR "unix shell not found" )
ENDIF()




### GENERATE ENVIRONMENT SCRIPT FOR CALLING ANT #############################

SET( LCIO_ENV_INIT "${PROJECT_BINARY_DIR}/lcio_env_init.sh" )

IF( NOT EXISTS ${LCIO_ENV_INIT} )
    MESSAGE( STATUS "Generating lcio_env_init.sh ..." )
    
    FILE( WRITE "${LCIO_ENV_INIT}"
"
#!/usr/bin/env bash

export LCIO=${PROJECT_SOURCE_DIR}
export LD_LIBRARY_PATH=${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}:$LD_LIBRARY_PATH
export PYTHONPATH=${PROJECT_SOURCE_DIR}/src/python:$PYTHONPATH
export JDK_HOME=${JAVA_HOME}
export JAVA_HOME=${JAVA_HOME}
export PATH=${PROJECT_SOURCE_DIR}/tools:${JAVA_BIN_PATH}:$PATH
eval $*
"
    )

ENDIF()

# Deal with SIO (we need at least 0.1 since there are no targets before)
FIND_PACKAGE( SIO 0.2 QUIET )

IF( NOT SIO_FOUND )
  MESSAGE( STATUS "SIO not found on your system. Using builtin sio" )
  include(FetchContent)
  FetchContent_Declare(sio_extern
  GIT_REPOSITORY https://github.com/iLCSoft/SIO.git
  GIT_TAG v00-02
  GIT_SHALLOW 1
#  FIND_PACKAGE_ARGS
)

  FetchContent_MakeAvailable(sio_extern)

  get_target_property(SIO_VERSION SIO::sio VERSION)
  get_target_property(SIO_INCLUDE_DIRS SIO::sio INCLUDE_DIRECTORIES)
ENDIF()

MESSAGE( STATUS "Using SIO (${SIO_VERSION})" )
MESSAGE( STATUS "SIO library: ${SIO_LIBRARIES}" )
MESSAGE( STATUS "SIO includes: ${SIO_INCLUDE_DIRS}" )

INCLUDE_DIRECTORIES( SYSTEM ${SIO_INCLUDE_DIRS} )

FIND_PACKAGE( Threads REQUIRED )

### GENERATE HEADERS ( USING ANT ) ##########################################

SET( LCIO_CXX_HEADERS_DIR "${LCIO_SOURCE_DIR}/src/cpp/include" )

# directories where ant generates c++ and java headers
if( INSTALL_JAR )
  SET( LCIO_JAVA_HEADERS_DIR "${LCIO_SOURCE_DIR}/src/java/hep/lcio" )
endif()

if( LCIO_GENERATE_HEADERS )
  SET( LCIO_AID_HEADERS_OUTPUT_DIR "${LCIO_BINARY_DIR}/include" )
else()
  SET( LCIO_AID_HEADERS_OUTPUT_DIR "${LCIO_SOURCE_DIR}/src/cpp/include/pre-generated" )
endif()


SET( LCIO_AID_HEADERS_DIR "${LCIO_SOURCE_DIR}/src/aid" )


## create "always out-of-date" headers target
#ADD_CUSTOM_TARGET( headers_force
#    COMMAND ${SH} "${LCIO_ENV_INIT}" ant aid.generate
#    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
#    COMMENT "Generating header files with ant aid.generate..."
#)

## custom command to generate headers with ant
#ADD_CUSTOM_COMMAND(
#    OUTPUT "${LCIO_CXX_HEADERS_DIR}/EVENT/LCIO.h"
#    COMMAND ${SH} "${LCIO_ENV_INIT}" ant aid.generate
#    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
#    COMMENT "Generating header files with ant aid.generate..."
#)
#
## create target to create headers with ant
#ADD_CUSTOM_TARGET( headers DEPENDS ${LCIO_CXX_HEADERS_DIR}/EVENT/LCIO.h )


### BUILD JAR LIBRARY (USING ANT OR MAVEN) ##################################
if( INSTALL_JAR )

  SET( LCIO_JAVA_BUILD_CMD "ant aid" )
  OPTION( LCIO_JAVA_USE_MAVEN "Set to ON to build/install lcio.jar using maven" OFF )
  IF( LCIO_JAVA_USE_MAVEN )
    SET( LCIO_JAVA_BUILD_CMD "mvn clean install" )
  ENDIF()

  # create "always out-of-date" jar target
  ADD_CUSTOM_TARGET( jar_force
    COMMAND ${SH} "${LCIO_ENV_INIT}" "${LCIO_JAVA_BUILD_CMD}"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    COMMENT "Building lcio jar library..."
    )



  # custom command to generate jar library
  ADD_CUSTOM_COMMAND(
    OUTPUT "${PROJECT_SOURCE_DIR}/lib/lcio.jar"
    COMMAND ${SH} "${LCIO_ENV_INIT}" "${LCIO_JAVA_BUILD_CMD}"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    COMMENT "generating lcio.jar"
    )
  
  # create target to create jar library
  ADD_CUSTOM_TARGET( jar DEPENDS ${PROJECT_SOURCE_DIR}/lib/lcio.jar )
  


  ### REMOVE RESTS GENERATED BY ANT IN THE SOURCE-TREE ########################
  
  # additional stuff to clean with 'make clean'
  # does not work with regexp's, e.g. ${LCIO_JAVA_HEADERS_DIR}/event/*.java
  SET_DIRECTORY_PROPERTIES( PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
    #"${PROJECT_SOURCE_DIR}/lib/lcio.jar;${LCIO_CXX_HEADERS_DIR}/EVENT;${LCIO_CXX_HEADERS_DIR}/IO"
    "${PROJECT_SOURCE_DIR}/lib/lcio.jar"
    )
  
  # keep source tree clean
  ADD_CUSTOM_TARGET( uninstall_headers
    COMMAND "${CMAKE_COMMAND}" -E remove "${LCIO_JAVA_HEADERS_DIR}/io/*.java"
    COMMAND "${CMAKE_COMMAND}" -E remove "${LCIO_JAVA_HEADERS_DIR}/event/*.java"
    #COMMAND "${CMAKE_COMMAND}" -E remove_directory "${LCIO_CXX_HEADERS_DIR}/EVENT"
    #COMMAND "${CMAKE_COMMAND}" -E remove_directory "${LCIO_CXX_HEADERS_DIR}/IO"
    
    COMMAND ${SH} "${LCIO_ENV_INIT}" ant clean
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    )


  ADD_DEPENDENCIES( uninstall uninstall_headers )

endif() 

### SUBDIRECTORIES ##########################################################

# replaced by standard cmake variable BUILD_TESTING
#OPTION( BUILD_LCIO_TESTS "Set to ON to build LCIO tests under src/cpp/src/TESTS" OFF )

OPTION( BUILD_LCIO_EXAMPLES "Set to ON to build LCIO examples" ON )

# lcio library
ADD_SUBDIRECTORY( ./src/cpp lcio )


# directory containing lcio tests
ADD_SUBDIRECTORY( ./tests )


### DOCUMENTATION ###########################################################

OPTION( INSTALL_DOC "Set to OFF to skip build/install Documentation" OFF )
IF( INSTALL_DOC )
    ADD_SUBDIRECTORY( ./doc )
    INSTALL( CODE "EXECUTE_PROCESS( COMMAND ${CMAKE_BUILD_TOOL} doc)" )
ENDIF()



# display some variables and write them to cache
DISPLAY_STD_VARIABLES()

MESSAGE( STATUS "BUILD_LCIO_EXAMPLES = ${BUILD_LCIO_EXAMPLES}" )
MESSAGE( STATUS "BUILD_F77_TESTJOBS = ${BUILD_F77_TESTJOBS}" )
MESSAGE( STATUS "BUILD_ROOTDICT = ${BUILD_ROOTDICT}" )
MESSAGE( STATUS "INSTALL_JAR = ${INSTALL_JAR}" )
MESSAGE( STATUS "LCIO_JAVA_USE_MAVEN = ${LCIO_JAVA_USE_MAVEN}" )
MESSAGE( STATUS "SIO_PRINT_DEBUG = ${SIO_PRINT_DEBUG}" )
MESSAGE( STATUS "" )


# generate and install following configuration files
GENERATE_PACKAGE_CONFIGURATION_FILES( LCIOConfig.cmake LCIOConfigVersion.cmake )

INSTALL( FILES cmake/MacroCheckPackageLibs.cmake cmake/MacroCheckPackageVersion.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake)

install(EXPORT ${PROJECT_NAME}Targets
  NAMESPACE ${PROJECT_NAME}::
  FILE "${PROJECT_NAME}Targets.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/"
  )

