# 
# CMakeLists.txt for building the SIO library only
# For the full SIO package (library, includes, binaries, examples) 
# please visit https:/github.com/iLCSoft/SIO
# 
# @author: Remi Ete, DESY
# 

PROJECT( SIO )

# project version
SET( SIO_VERSION_MAJOR 0 )
SET( SIO_VERSION_MINOR 0 )
SET( SIO_VERSION_PATCH 4 )
SET( SIO_VERSION "${SIO_VERSION_MAJOR}.${SIO_VERSION_MINOR}.${SIO_VERSION_PATCH}" )
SET( SIO_SOVERSION "${SIO_VERSION_MAJOR}.${SIO_VERSION_MINOR}" )

FIND_PACKAGE( ZLIB REQUIRED )

# SIO care about endianess
INCLUDE( TestBigEndian )
TEST_BIG_ENDIAN( BIG_ENDIAN )

# Build the SIO library
FILE( GLOB_RECURSE SIO_SRCS src/*.cc )

ADD_LIBRARY(sio SHARED ${SIO_SRCS})
ADD_LIBRARY(SIO::sio ALIAS sio)

TARGET_LINK_LIBRARIES(sio PRIVATE ZLIB::ZLIB)
TARGET_INCLUDE_DIRECTORIES(sio PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/SIO>
  )
TARGET_COMPILE_DEFINITIONS(sio PUBLIC "-DSIO_LOGLVL=0")
IF( NOT BIG_ENDIAN )
  TARGET_COMPILE_DEFINITIONS(sio PUBLIC "-D__LITTLE_ENDIAN__")
ENDIF()

SET_TARGET_PROPERTIES(sio PROPERTIES
  # create *nix style library versions + symbolic links
  VERSION ${${PROJECT_NAME}_VERSION}
  SOVERSION ${${PROJECT_NAME}_SOVERSION}
  )

# Install the targets and the headers
INSTALL(TARGETS sio
  EXPORT SIOTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR})

INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/sio DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SIO)

# mimic the SIOConfig.cmake variables for the LCIO project
SET( SIO_VERSION                  "${SIO_VERSION}"                       CACHE STRING "The SIO version" )
SET( CHECK_PACKAGE_SIO_LIBRARY    "SIO::sio"                                  CACHE STRING "The name of the builtin SIO library to check when searching for LCIO package" )
