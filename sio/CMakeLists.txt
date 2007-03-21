CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
PROJECT(SIO)

SET(SIO_MAJOR_VERSION 1)
SET(SIO_MINOR_VERSION 8)
SET(SIO_PATCH_LEVEL 1)

SET(SIO_VERSION "v01-08-01")


#INCLUDE(FindZLIB)
FIND_PACKAGE(ZLIB REQUIRED)
#ZLIB_INCLUDE_DIR - where to find zlib.h, etc.
#ZLIB_LIBRARIES   - List of libraries when using zlib.
#ZLIB_FOUND       - True if zlib found.

#MESSAGE(STATUS "ZLIB_LIBRARIES: " ${ZLIB_LIBRARIES})

INCLUDE(TestBigEndian)
TEST_BIG_ENDIAN(BIG_ENDIAN)

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
ADD_DEFINITIONS( -D_FILE_OFFSET_BITS=64 -Wno-sign-compare )

#IF(NOT CMAKE_BUILD_TYPE)
#  SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
#      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
#      FORCE)
#ENDIF(NOT CMAKE_BUILD_TYPE)

ADD_LIBRARY(sio ${SIO_SRCS})
TARGET_LINK_LIBRARIES(sio ${ZLIB_LIBRARIES})

#---- export zlib for the LCIOConfig.cmake -----
SET( ZLIB ${ZLIB_LIBRARIES} )
INSTALL(TARGETS sio DESTINATION lib)

