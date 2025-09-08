
set(USE_EXTERNAL_CATCH2 AUTO CACHE STRING "Link against an external Catch2 v3 static library, otherwise build it locally")
set_property(CACHE USE_EXTERNAL_CATCH2 PROPERTY STRINGS AUTO ON OFF)

set(CATCH2_MIN_VERSION 3.5.0)
if(USE_EXTERNAL_CATCH2)
  if (USE_EXTERNAL_CATCH2 STREQUAL AUTO)
    find_package(Catch2 ${CATCH2_MIN_VERSION})
  else()
    find_package(Catch2 ${CATCH2_MIN_VERSION} REQUIRED)
  endif()
endif()

if(NOT Catch2_FOUND)
  message(STATUS "Fetching local copy of Catch2 library for unit-tests...")
  # Build Catch2 with the default flags, to avoid generating warnings when we
  # build it
  set(CXX_FLAGS_CMAKE_USED ${CMAKE_CXX_FLAGS})
  set(CMAKE_CXX_FLAGS ${CXX_FLAGS_CMAKE_DEFAULTS})
  Include(FetchContent)
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v${CATCH2_MIN_VERSION}
    )
  FetchContent_MakeAvailable(Catch2)
  set(CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras ${CMAKE_MODULE_PATH})

  # Disable clang-tidy on external contents
  set_target_properties(Catch2 PROPERTIES CXX_CLANG_TIDY "")

  # Hack around the fact, that the include directories are not declared as
  # SYSTEM for the targets defined this way. Otherwise warnings can still occur
  # in Catch2 code when templates are evaluated (which happens quite a bit)
  get_target_property(CATCH2_IF_INC_DIRS Catch2 INTERFACE_INCLUDE_DIRECTORIES)
  set_target_properties(Catch2 PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${CATCH2_IF_INC_DIRS}")

  # Reset the flags
  set(CMAKE_CXX_FLAGS ${CXX_FLAGS_CMAKE_USED})
endif()


add_executable(unittests_lcio check_collections.cpp)
target_link_libraries(unittests_lcio PRIVATE Catch2::Catch2WithMain LCIO::lcio)
include(Catch)
catch_discover_tests(unittests_lcio
  TEST_PREFIX "ut_"
  PROPERTIES
    ENVIRONMENT LD_LIBRARY_PATH=$<TARGET_FILE_DIR:LCIO::lcio>:$ENV{LD_LIBRARY_PATH}
)
