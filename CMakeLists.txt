CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
PROJECT(LCIO)

SET(LCIO_MAJOR_VERSION 1)
SET(LCIO_MINOR_VERSION 8)
SET(LCIO_PATCH_LEVEL 1)

SET(LCIO_VERSION "v01-08-01")

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

#------ require proper C++ -------
ADD_DEFINITIONS(-Wall -ansi  -pedantic)
#----- need long long for int64 for now ------
#FIXME: should establish wether we are on a 32bit or 64 bit machine ....
ADD_DEFINITIONS( -Wno-long-long )


#----- if no install prefix set install in source directory -----
IF( ${CMAKE_INSTALL_PREFIX} STREQUAL "/usr/local")
  SET( CMAKE_INSTALL_PREFIX  ${LCIO_SOURCE_DIR} )
ENDIF( ${CMAKE_INSTALL_PREFIX} STREQUAL "/usr/local")

#---- add own find modules to global module search path -------
SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${LCIO_SOURCE_DIR}/CMake )
 
#------ optionally we can build with CLHEP -------
SET(CLHEP_DIR "" CACHE PATH "if specified LCIO is build with CLHEP" )
IF( CLHEP_DIR )
  FIND_PACKAGE("CLHEP" REQUIRED )
  ADD_DEFINITIONS(-DUSE_CLHEP)
  INCLUDE_DIRECTORIES ( ${CLHEP_INCLUDE_DIRS} ) 
ENDIF( CLHEP_DIR )

#------------------------------------------------


ADD_SUBDIRECTORY(sio sio)
ADD_SUBDIRECTORY(src/cpp lcio)

 
# --- get all dependencies of sio (actually zlib only)
FOREACH( dep ${sio_LIB_DEPENDS} )
 set( siodeps ${siodeps} ${dep} )
ENDFOREACH(  dep ${sio_LIB_DEPENDS} ) 

#----- siodeps is used in LCIOConfig.cmake.in :
CONFIGURE_FILE(${LCIO_SOURCE_DIR}/LCIOConfig.cmake.in
               ${LCIO_BINARY_DIR}/LCIOConfig.cmake @ONLY )

#----- install configure file for dependend packages
INSTALL(FILES  ${LCIO_BINARY_DIR}/LCIOConfig.cmake DESTINATION .)


#EXPORT_LIBRARY_DEPENDENCIES( lciodep.cmake)

 
