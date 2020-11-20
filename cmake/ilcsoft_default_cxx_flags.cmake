include(CheckCXXCompilerFlag)

SET(COMPILER_FLAGS -Wall -Wdeprecated -Weffc++ -Wextra -Wformat-security -Wno-long-long -Wno-non-virtual-dtor -Wshadow -Wuninitialized -pedantic -fdiagnostics-color=auto)

# AppleClang/Clang specific warning flags
if(CMAKE_CXX_COMPILER_ID MATCHES "^(Apple)?Clang$")
  set ( COMPILER_FLAGS ${COMPILER_FLAGS} -Winconsistent-missing-override -Wno-c++1z-extensions -Wheader-hygiene )
endif()

FOREACH( FLAG ${COMPILER_FLAGS} )
  STRING(REPLACE "-" "_" FLAG_WORD ${FLAG} )
  STRING(REPLACE "+" "P" FLAG_WORD ${FLAG_WORD} )

  CHECK_CXX_COMPILER_FLAG( "${FLAG}" CXX_FLAG_WORKS_${FLAG_WORD} )
  IF( ${CXX_FLAG_WORKS_${FLAG_WORD}} )
    MESSAGE("Adding ${FLAG} to CXX_FLAGS" )
    SET ( CMAKE_CXX_FLAGS "${FLAG} ${CMAKE_CXX_FLAGS} ")
  ELSE()
    MESSAGE("NOT Adding ${FLAG} to CXX_FLAGS" )
  ENDIF()
ENDFOREACH()

# resolve which linker we use
execute_process(COMMAND ${CMAKE_CXX_COMPILER} -Wl,--version OUTPUT_VARIABLE stdout ERROR_QUIET)
if("${stdout}" MATCHES "GNU ")
  set(LINKER_TYPE "GNU")
else()
  execute_process(COMMAND ${CMAKE_CXX_COMPILER} -Wl,-v ERROR_VARIABLE stderr )
  if(("${stderr}" MATCHES "PROGRAM:ld") AND ("${stderr}" MATCHES "PROJECT:ld64"))
    set(LINKER_TYPE "APPLE")
  else()
    set(LINKER_TYPE "unknown")
    MESSAGE("Detected unknown linker: ${stdout} ${stderr}" )
  endif()
endif()

if("${LINKER_TYPE}" STREQUAL "APPLE")
  SET ( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-undefined,error")
elseif("${LINKER_TYPE}" STREQUAL "GNU")
  SET ( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
else()
  MESSAGE( WARNING "No known linker (GNU or Apple) has been detected, pass no flags to linker" )
endif()
