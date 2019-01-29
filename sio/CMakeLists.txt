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
    ${SIO_SOURCE_DIR}/src/SIO_functions.cc
    ${SIO_SOURCE_DIR}/src/SIO_stream.cc
)

#------ require proper C++ -------
#ADD_DEFINITIONS( "-Wall -ansi -pedantic" )
#----- need long long for int64 for now ------
#ADD_DEFINITIONS( "-Wno-long-long -Wno-sign-compare -fno-strict-aliasing" )
ADD_DEFINITIONS( "-DSIO_PRINT_DEBUG" )

ADD_SHARED_LIBRARY( sio ${SIO_SRCS} )
TARGET_LINK_LIBRARIES( sio ${ZLIB_LIBRARIES} )
INSTALL_SHARED_LIBRARY( sio DESTINATION lib )

# install SIO headers
INSTALL_DIRECTORY( include/ DESTINATION include )

function ( add_test_reg test_name )
  cmake_parse_arguments(ARG "BUILD_EXEC" "" "COMMAND;DEPENDS;EXEC_ARGS;REGEX_PASS;REGEX_PASSED;REGEX_FAIL;REGEX_FAILED;REQUIRES" ${ARGN} )
  set ( missing )
  set ( use_test 1 )

  if ( ${ARG_BUILD_EXEC} )
    add_executable ( ${test_name} tests/${test_name}.cc )
    target_link_libraries( ${test_name} sio )
    target_include_directories( ${test_name} PUBLIC ${SIO_SOURCE_DIR}/tests )
    set_target_properties( ${test_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" )
  endif()

  set ( cmd ${ARG_COMMAND} )
  if ( "${cmd}" STREQUAL "" )
    if( ${ARG_BUILD_EXEC} )
      set ( cmd ${CMAKE_BINARY_DIR}/bin/${test_name} )
    else()
      message( SEND_ERROR "No command no executable provided" )
    endif()
  endif()

  set ( passed ${ARG_REGEX_PASS} ${ARG_REGEX_PASSED} )
  if ( "${passed}" STREQUAL "NONE" )
    unset ( passed )
  elseif ( "${passed}" STREQUAL "" )
    set ( passed "TEST_PASSED" )
  endif()

  set ( failed ${ARG_REGEX_FAIL} ${ARG_REGEX_FAILED} )
  if ( "${failed}" STREQUAL "NONE" )
    unset ( failed )
  endif()

  set ( args ${ARG_EXEC_ARGS} )
  # if ( "${args}" STREQUAL "" )
  #   set ( args ${test_name} )
  # endif()
  add_test(NAME t_${test_name} COMMAND ${cmd} ${args} )
  if ( NOT "${passed}" STREQUAL "" )
    set_tests_properties( t_${test_name} PROPERTIES PASS_REGULAR_EXPRESSION "${passed}" )
  endif()
  if ( NOT "${failed}" STREQUAL "" )
    set_tests_properties( t_${test_name} PROPERTIES FAIL_REGULAR_EXPRESSION "${failed}" )
  endif()
  # Set test dependencies if present
  foreach ( _dep ${ARG_DEPENDS} )
    set_tests_properties( t_${test_name} PROPERTIES DEPENDS t_${_dep} )
  endforeach()
endfunction()


add_test_reg (
  SIO_test_block_wr
  BUILD_EXEC 
  REGEX_FAIL "TEST_FAILED" 
)

add_test_reg (
  SIO_test_functions
  BUILD_EXEC 
  REGEX_FAIL "TEST_FAILED" 
)
