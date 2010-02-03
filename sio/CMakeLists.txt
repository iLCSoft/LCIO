#################################
# cmake file for building SIO
# @author Frank Gaede, DESY
# @author Jan Engels, DESY
#################################

# project name
PROJECT( SIO )

# project version
SET( SIO_MAJOR_VERSION 1 )
SET( SIO_MINOR_VERSION 12 )
SET( SIO_PATCH_LEVEL 2 )

# library *nix style versioning
SET( SIO_SOVERSION "${SIO_MAJOR_VERSION}.${SIO_MINOR_VERSION}" )
SET( SIO_VERSION "${SIO_SOVERSION}.${SIO_PATCH_LEVEL}" )

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

#IF( BUILD_WITH_DCAP )
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
ADD_DEFINITIONS( "-Wall -ansi -pedantic" )
#----- need long long for int64 for now ------
ADD_DEFINITIONS( "-Wno-long-long -Wno-sign-compare -fno-strict-aliasing" )

ADD_LIBRARY( lib_SIO ${SIO_SRCS} )

SET_TARGET_PROPERTIES( lib_SIO PROPERTIES
    # create *nix style library versions + symbolic links
    VERSION ${SIO_VERSION}
    SOVERSION ${SIO_SOVERSION}
    # allow creating static and shared libs without conflicts
    CLEAN_DIRECT_OUTPUT 1
    # avoid conflicts between library and binary target names
    OUTPUT_NAME sio
)

TARGET_LINK_LIBRARIES( lib_SIO ${ZLIB_LIBRARIES} )

# install library
INSTALL( TARGETS lib_SIO DESTINATION lib PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE )

