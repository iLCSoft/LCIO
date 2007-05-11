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

# project version
SET( ${PROJECT_NAME}_MAJOR_VERSION 1 )
SET( ${PROJECT_NAME}_MINOR_VERSION 8 )
SET( ${PROJECT_NAME}_PATCH_LEVEL 2 )
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

# input directories
ADD_SUBDIRECTORY( sio sio )
ADD_SUBDIRECTORY( src/cpp lcio )
ADD_SUBDIRECTORY( src/f77 f77 EXCLUDE_FROM_ALL )

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

# export library dependencies (keep this as the last line in the file)
EXPORT_LIBRARY_DEPENDENCIES( "${PROJECT_NAME}LibDeps.cmake" )
