#################################
# cmake file for building LCIO
# @author Frank Gaede, DESY
# @author Jan Engels, DESY
#################################

SET( DICT_OUTPUT_DIR "${LCIO_BINARY_DIR}/dict" )
SET_DIRECTORY_PROPERTIES( PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${DICT_OUTPUT_DIR}" )

## create output directory
# since DICT_OUTPUT_DIR is part of ADDITIONAL_MAKE_CLEAN_FILES it needs to be
# created when make clean && make dict is called
# however making it a custom command and adding DEPENDS ${DICT_OUTPUT_DIR} in
# the custom command to create the dict header files has the problem of
# calling: cmake .. && make (will regenerate everything again)
# solution: create directory in the custom command itself (before the generate command)
#FILE( MAKE_DIRECTORY ${DICT_OUTPUT_DIR} ) # problem if calling make clean && make dict
#ADD_CUSTOM_COMMAND( # problem if calling cmake .. && make
#  OUTPUT  "${DICT_OUTPUT_DIR}"
#  COMMAND "${CMAKE_COMMAND}"
#  ARGS -E make_directory "${DICT_OUTPUT_DIR}"
#  COMMENT "create directory ${DICT_OUTPUT_DIR}"
#)

# returns ROOT_DICT_LIB_SRCS: list including all generated srcs
##
MACRO( ADD_ROOT_DICT_OBJ header_full_path header_base_path )

    #MESSAGE( STATUS "ADD_ROOT_DICT_OBJ: ${header_full_path} ${header_base_path}" )
    #MESSAGE( STATUS "ADD_ROOT_DICT_OBJ: ${header_base_path}" )

    IF( NOT DICT_OUTPUT_DIR )
        MESSAGE( FATAL_ERROR "DICT_OUTPUT_DIR not set" )
    ENDIF()

    # generate a basename from the header + relative path, e.g.:
    # include/EVENT/TrackerPulse.h  -->>  include_EVENT_TrackerPulse
    #GET_FILENAME_COMPONENT( header_basename ${header_base_path} NAME_WE )
    STRING( REPLACE "/" "_" dict_basename ${header_base_path} )
    STRING( REPLACE ".h" "" dict_basename ${dict_basename} )

    SET( dict_linkdef "${dict_basename}_Linkdef.h" )
    SET( dict_linkdef_file "${DICT_OUTPUT_DIR}/${dict_linkdef}" )

    SET( dict_src "${dict_basename}_dict.cxx" )
    SET( dict_src_file "${DICT_OUTPUT_DIR}/${dict_src}" )

    SET( dict_hdr "${dict_basename}_dict.h" )
    SET( dict_hdr_file "${DICT_OUTPUT_DIR}/${dict_hdr}" )

    # generate dict header file
    ADD_CUSTOM_COMMAND(
        OUTPUT  ${dict_linkdef_file}
        # cmake 2.4 doesn't know make_directory
        #COMMAND ${CMAKE_COMMAND} ARGS -E make_directory "${DICT_OUTPUT_DIR}"
        COMMAND mkdir ARGS -p ${DICT_OUTPUT_DIR}
        COMMAND ${CMAKE_COMMAND}
        ARGS -E echo "\\#pragma link C++ defined_in \\\"${header_base_path}\\\"\; >${dict_linkdef_file}"
        WORKING_DIRECTORY "${LCIO_BINARY_DIR}"
        DEPENDS ${header_full_path} 
        COMMENT "generating ${dict_linkdef}"
    )

    # generate dict source file
    ADD_CUSTOM_COMMAND(
        OUTPUT  ${dict_src_file}
        #COMMAND ${ROOT_HOME}/bin/rootcint
        #COMMAND $ENV{ROOTSYS}/bin/rootcint
        COMMAND ${SH} ${ROOT_ENV_INIT} ${ROOT_CINT}
        ARGS -f ${dict_src_file} -c -DHANDLE_LCIO_EXCEPTIONS=IGNORED_FOR_CINT
        -I${AID_GEN_CXX_HEADERS_DIR} -I${LCIO_CXX_HEADERS_DIR} ${header_full_path} ${dict_linkdef_file}
	COMMAND ${CMAKE_COMMAND}
        ARGS -E echo "  >>${dict_hdr_file}"
	COMMAND ${CMAKE_COMMAND}
        ARGS -E echo "//+++++++ fix an issue with unique global variables generated from cint \\(ROOT 5.27.06\\)  >>${dict_hdr_file}"
	COMMAND ${CMAKE_COMMAND}
        ARGS -E echo "\\#undef _ROOT_DICT_NAME >>${dict_hdr_file}"
	COMMAND ${CMAKE_COMMAND}
        ARGS -E echo "\\#undef _R__JOIN_ >>${dict_hdr_file}"
	COMMAND ${CMAKE_COMMAND}
        ARGS -E echo "\\#undef _R__UNIQUE_ >>${dict_hdr_file}"
	COMMAND ${CMAKE_COMMAND}
        ARGS -E echo "\\#define _ROOT_DICT_NAME ${dict_basename} >>${dict_hdr_file}"
	COMMAND ${CMAKE_COMMAND}
        ARGS -E echo "\\#define _R__JOIN_\\(X,Y,Z\\) _NAME3_\\(X,Y,Z\\)  >>${dict_hdr_file}"
	COMMAND ${CMAKE_COMMAND}
        ARGS -E echo "\\#define _R__UNIQUE_\\(X\\) _R__JOIN_\\( _ROOT_DICT_NAME , X, __LINE__\\)  >>${dict_hdr_file}"
        WORKING_DIRECTORY "${LCIO_BINARY_DIR}"
        DEPENDS ${dict_linkdef_file}
        COMMENT "generating ${dict_src}"
    )

    LIST( APPEND ROOT_DICT_LIB_SRCS ${dict_src_file} )
ENDMACRO()
#===================================================================

INCLUDE( ${LCIO_SOURCE_DIR}/FindROOT.cmake )

# should rootcint be detected in FindROOT.cmake ?
SET( ROOT_CINT ROOT_CINT-NOTFOUND )
FIND_PROGRAM( ROOT_CINT
    rootcint
    ${ROOT_HOME}/bin
    NO_DEFAULT_PATH
)


IF( ROOT_FOUND AND ROOT_CINT )

    #SET( ENV{ROOTSYS} "${ROOT_HOME}" )
    #SET( ENV{PATH} $ENV{ROOTSYS}/bin:$ENV{PATH} )
    #SET( ENV{LD_LIBRARY_PATH} $ENV{ROOTSYS}/lib:$ENV{LD_LIBRARY_PATH} )

    # FIXME variables must be set by FindROOT.cmake
    #SET( cint_script "${PROJECT_BINARY_DIR}/rootcint.cmake" )
    #FILE( WRITE "${cint_script}" "# little script to run rootcint\n" )
    #FILE( APPEND "${cint_script}" "SET( ENV{ROOTSYS} ${ROOT_HOME} )\n" )
    #FILE( APPEND "${cint_script}" "SET( ENV{PATH} \$ENV{ROOTSYS}/bin:\$ENV{PATH} )\n" )
    #FILE( APPEND "${cint_script}" "SET( ENV{LD_LIBRARY_PATH} \$ENV{ROOTSYS}/lib:\$ENV{LD_LIBRARY_PATH} )\n" )
    #FILE( APPEND "${cint_script}" "EXEC_PROGRAM( rootcint -f \${dict_src_file} -c -DHANDLE_LCIO_EXCEPTIONS=IGNORED_FOR_CINT -Iinclude \${lcio_hdr_rel_path} \${dict_linkdef_file} )\n" )

    # generate shell script for setting root environment
    SET( ROOT_ENV_INIT "${PROJECT_BINARY_DIR}/root_env_init.sh" )
    IF( NOT EXISTS ${ROOT_ENV_INIT} )
        MESSAGE( STATUS "Generating ROOT env init script..." )
        FILE( WRITE "${ROOT_ENV_INIT}"
            "# script to initialize root environment\n"
            "export ROOTSYS=${ROOT_HOME}\n"
            "export PATH=$ROOTSYS/bin:$PATH\n"
            "export LD_LIBRARY_PATH=$ROOTSYS/lib:$LD_LIBRARY_PATH\n"
            "eval $*\n"
        )
     ENDIF()


    # EVENT aid files
    FILE( GLOB aid_headers
        "${LCIO_AID_HEADERS_DIR}/EVENT/*.aid"
        "${LCIO_AID_HEADERS_DIR}/IO/*.aid"
    )

    FOREACH( aid_file ${aid_headers} )

        FILE( RELATIVE_PATH aid_file_basepath ${LCIO_AID_HEADERS_DIR} ${aid_file} )
        STRING( REPLACE ".aid" ".h" header_basepath ${aid_file_basepath} )

        SET( gen_header_file "${AID_GEN_CXX_HEADERS_DIR}/${header_basepath}" )

        ADD_ROOT_DICT_OBJ( ${gen_header_file} ${header_basepath} )

    ENDFOREACH()


    INCLUDE_DIRECTORIES ( BEFORE "${AID_GEN_CXX_HEADERS_DIR}" )

    INCLUDE_DIRECTORIES (
        "${LCIO_SOURCE_DIR}/sio/include"
        "${LCIO_CXX_HEADERS_DIR}"
    )


    #------ require proper C++ -------
    ADD_DEFINITIONS( "-Wall -ansi" )
    #----- need long long for int64 for now ------
    ADD_DEFINITIONS( "-Wno-long-long -fno-strict-aliasing" )


    # list of headers
    FILE( GLOB_RECURSE lcio_headers ${LCIO_CXX_HEADERS_DIR}/*.h )

    # list of headers to exclude
    FILE( GLOB_RECURSE exclude_headers 
        ${LCIO_CXX_HEADERS_DIR}/empty_ignore.h
        ${LCIO_CXX_HEADERS_DIR}/EVENT/*.h
        ${LCIO_CXX_HEADERS_DIR}/IO/*.h
        ${LCIO_CXX_HEADERS_DIR}/SIO/*.h
        ${LCIO_CXX_HEADERS_DIR}/CPPFORT/*.h
        ${LCIO_CXX_HEADERS_DIR}/UTIL/CellIDDecoder.h
        ${LCIO_CXX_HEADERS_DIR}/UTIL/LCWarning.h
        ${LCIO_CXX_HEADERS_DIR}/UTIL/Operators.h
        ${LCIO_CXX_HEADERS_DIR}/UTIL/LCFourVector.h
    )

    # remove items to exclude
    LIST( REMOVE_ITEM lcio_headers ${exclude_headers} )


    FOREACH( lcio_hdr ${lcio_headers} )

      #MESSAGE( STATUS "processing header file: ${lcio_hdr}" )
      
      # get the relative path
      #STRING( REPLACE "${LCIO_CXX_HEADERS_DIR}" "" lcio_hdr_rel_path ${lcio_hdr} )
      FILE( RELATIVE_PATH lcio_hdr_rel_path "${LCIO_CXX_HEADERS_DIR}" ${lcio_hdr} )

      #MESSAGE( STATUS "header file relpath: ${lcio_hdr_rel_path}" )

      ADD_ROOT_DICT_OBJ( ${lcio_hdr} ${lcio_hdr_rel_path} )
     
    ENDFOREACH()

    INCLUDE_DIRECTORIES( "${DICT_OUTPUT_DIR}" )
    INCLUDE_DIRECTORIES( "${ROOT_INCLUDE_DIRS}" )

    ADD_LIBRARY( lib_LCIODICT ${ROOT_DICT_LIB_SRCS} ) # ${LCIO_RIO_SRCS} )
    INSTALL( TARGETS lib_LCIODICT DESTINATION lib )


    IF( APPLE )
     SET( ROOT_DICT_LINK_FLAGS "-single_module -undefined dynamic_lookup -bind_at_load")
    #     ADD_LIBRARY( lib_LCIO ${LCIO_RIO_SRCS} )
    ELSE( APPLE )
     SET( ROOT_DICT_LINK_FLAGS " " )
    ENDIF( APPLE )

    SET_TARGET_PROPERTIES( lib_LCIODICT PROPERTIES
      # create *nix style library versions + symbolic links
      VERSION ${LCIO_VERSION}
      SOVERSION ${LCIO_SOVERSION}
      # allow creating static and shared libs without conflicts
      CLEAN_DIRECT_OUTPUT 1
      # avoid conflicts between library and binary target names
      OUTPUT_NAME lcioDict
      LINKER_FLAGS ${ROOT_DICT_LINK_FLAGS}
      )

    ADD_CUSTOM_TARGET( dict DEPENDS  lib_LCIODICT )

    TARGET_LINK_LIBRARIES( lib_LCIODICT ${ROOT_LIBRARIES} lib_LCIO)

ELSE() 

    MESSAGE( STATUS " NO ROOT found :-( " ) 

ENDIF()
#----------------------------------------------

