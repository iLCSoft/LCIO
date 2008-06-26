#################################
# cmake file for building SIO
# @author Frank Gaede, DESY
# @author Jan Engels, DESY
#################################

# project name
PROJECT( SIO )

# project version
SET( SIO_MAJOR_VERSION 1 )
SET( SIO_MINOR_VERSION 10 )
SET( SIO_PATCH_LEVEL 1 )

# library *nix style versioning
SET( ${PROJECT_NAME}_SOVERSION
    "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}" )
SET( ${PROJECT_NAME}_VERSION
    "${${PROJECT_NAME}_SOVERSION}.${${PROJECT_NAME}_PATCH_LEVEL}" )

#INCLUDE(FindZLIB)
FIND_PACKAGE( ZLIB REQUIRED )
#ZLIB_INCLUDE_DIR - where to find zlib.h, etc.
#ZLIB_LIBRARIES   - List of libraries when using zlib.
#ZLIB_FOUND       - True if zlib found.

#MESSAGE(STATUS "ZLIB_LIBRARIES: " ${ZLIB_LIBRARIES})

INCLUDE( TestBigEndian )
TEST_BIG_ENDIAN( BIG_ENDIAN )

IF( BUILD_WITH_DCAP )
ADD_DEFINITIONS( "-D SIO_USE_DCAP" )
ENDIF()

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
#FIXME: should establish wether we are on a 32bit or 64 bit machine ....
ADD_DEFINITIONS( "-Wno-long-long" )

ADD_DEFINITIONS( "-D_FILE_OFFSET_BITS=64 -Wno-sign-compare" )

ADD_LIBRARY( lib_${PROJECT_NAME} ${SIO_SRCS} )

SET_TARGET_PROPERTIES( lib_${PROJECT_NAME} PROPERTIES
    # create *nix style library versions + symbolic links
    VERSION ${${PROJECT_NAME}_VERSION}
    SOVERSION ${${PROJECT_NAME}_SOVERSION}
    # allow creating static and shared libs without conflicts
    CLEAN_DIRECT_OUTPUT 1
    # avoid conflicts between library and binary target names
    OUTPUT_NAME sio
)

TARGET_LINK_LIBRARIES( lib_${PROJECT_NAME} ${ZLIB_LIBRARIES} )

# install library
INSTALL( TARGETS lib_${PROJECT_NAME} DESTINATION lib PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE )
