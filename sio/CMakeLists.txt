#################################
# cmake file for building SIO
# @author Frank Gaede, DESY
# @author Jan Engels, DESY
#################################

# project name
PROJECT( SIO )

# project version
SET( SIO_VERSION_MAJOR ${LCIO_VERSION_MAJOR} )
SET( SIO_VERSION_MINOR ${LCIO_VERSION_MINOR} )
SET( SIO_VERSION_PATCH ${LCIO_VERSION_PATCH} )

# library *nix style versioning
SET( SIO_SOVERSION "${SIO_VERSION_MAJOR}.${SIO_VERSION_MINOR}" )
SET( SIO_VERSION "${SIO_SOVERSION}.${SIO_VERSION_PATCH}" )

#INCLUDE(FindZLIB)
FIND_PACKAGE( ZLIB REQUIRED )
#ZLIB_INCLUDE_DIR - where to find zlib.h, etc.
#ZLIB_LIBRARIES   - List of libraries when using zlib.
#ZLIB_FOUND       - True if zlib found.

INCLUDE( TestBigEndian )
TEST_BIG_ENDIAN( BIG_ENDIAN )

IF( NOT BIG_ENDIAN )
    ADD_DEFINITIONS( "-D__LITTLE_ENDIAN__" )
ENDIF()

#IF( LCIO_DCAP )
#    INCLUDE_DIRECTORIES( ${DCAP_INCLUDE_DIRS} )
#    ADD_DEFINITIONS( "-DSIO_USE_DCAP" )
#ENDIF()

INCLUDE_DIRECTORIES(
    ${SIO_SOURCE_DIR}/include
    ${ZLIB_INCLUDE_DIR}
)

SET (SIO_SRCS
    ${SIO_SOURCE_DIR}/src/SIO_block.cc
    ${SIO_SOURCE_DIR}/src/SIO_blockManager.cc
    ${SIO_SOURCE_DIR}/src/SIO_functions.cc
    ${SIO_SOURCE_DIR}/src/SIO_record.cc
    ${SIO_SOURCE_DIR}/src/SIO_recordManager.cc
    ${SIO_SOURCE_DIR}/src/SIO_stream.cc
    ${SIO_SOURCE_DIR}/src/SIO_streamManager.cc
)

#------ require proper C++ -------
#ADD_DEFINITIONS( "-Wall -ansi -pedantic" )
#----- need long long for int64 for now ------
#ADD_DEFINITIONS( "-Wno-long-long -Wno-sign-compare -fno-strict-aliasing" )


ADD_SHARED_LIBRARY( sio ${SIO_SRCS} )
TARGET_LINK_LIBRARIES( sio ${ZLIB_LIBRARIES} )
INSTALL_SHARED_LIBRARY( sio DESTINATION lib )

