cmake_minimum_required(VERSION 3.14)

project(DownstreamProjectUsingLCIO)

find_package(LCIO REQUIRED)

# Make sure that the LCIO version is set and usableas a version
if (${LCIO_VERSION} VERSION_GREATER "0.0.0")
  message(STATUS "Found LCIO version " ${LCIO_VERSION})
else()
  message(FATAL_ERROR "Cannot determine LCIO_VERSION")
endif()

add_executable(lcio_test_program lcio_test_program.cpp)
target_link_libraries(lcio_test_program PRIVATE LCIO::lcio)
