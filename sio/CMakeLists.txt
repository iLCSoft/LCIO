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

IF( NOT BIG_ENDIAN )
    ADD_DEFINITIONS( "-D__LITTLE_ENDIAN__" )
ENDIF()

# Build the SIO library
FILE( GLOB_RECURSE SIO_SRCS src/*.cc )

ADD_DEFINITIONS( "-DSIO_LOGLVL=0" )
ADD_SHARED_LIBRARY( sio ${SIO_SRCS} )
TARGET_INCLUDE_DIRECTORIES( sio PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
TARGET_INCLUDE_DIRECTORIES( sio SYSTEM PUBLIC ${ZLIB_INCLUDE_DIR} )
TARGET_LINK_LIBRARIES( sio ${ZLIB_LIBRARIES} )
INSTALL_SHARED_LIBRARY( sio DESTINATION ${CMAKE_INSTALL_LIBDIR} )

# Install the sio headers as well. Necessary for the python bindings and also
# for cases where others want to link against LCIO / this version of SIO
INSTALL(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/sio DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# mimic the SIOConfig.cmake variables for the LCIO project
SET( SIO_VERSION                  "${SIO_VERSION}"                       CACHE STRING "The SIO version" )
SET( SIO_LIBRARIES                "sio"                                  CACHE STRING "The SIO library" )
SET( SIO_INCLUDE_DIRS             "${CMAKE_CURRENT_SOURCE_DIR}/include"  CACHE STRING "The path to SIO include directories" )
SET( CHECK_PACKAGE_SIO_LIBRARY    "sio"                                  CACHE STRING "The name of the builtin SIO library to check when searching for LCIO package" )
