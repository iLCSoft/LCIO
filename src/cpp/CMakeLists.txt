#################################
# cmake file for building LCIO
# @author Frank Gaede, DESY
# @author Jan Engels, DESY
#################################

SET( LCIO_CPPFORT_SRCS
  ./src/CPPFORT/HEPEVT.cc
  ./src/CPPFORT/lccah.cc
  ./src/CPPFORT/lcclu.cc
  ./src/CPPFORT/lccol.cc
  ./src/CPPFORT/lcevt.cc
  ./src/CPPFORT/lcgob.cc
  ./src/CPPFORT/lciof77apiext.cc
  ./src/CPPFORT/lcmcp.cc
  ./src/CPPFORT/lcobv.cc
  ./src/CPPFORT/lcpid.cc
  ./src/CPPFORT/lcrcp.cc
  ./src/CPPFORT/lcrdr.cc
  ./src/CPPFORT/lcrdrrep.cc
  ./src/CPPFORT/lcrel.cc
  ./src/CPPFORT/lcrhd.cc
  ./src/CPPFORT/lcrnv.cc
  ./src/CPPFORT/lcsch.cc
  ./src/CPPFORT/lcsth.cc
  ./src/CPPFORT/lctph.cc
  ./src/CPPFORT/lctrh.cc
  ./src/CPPFORT/lctrk.cc
  ./src/CPPFORT/lcvec.cc
  ./src/CPPFORT/lcvtx.cc
  ./src/CPPFORT/lcwrt.cc
)

SET( LCIO_IMPL_SRCS
  ./src/IMPL/AccessChecked.cc
  ./src/IMPL/CalorimeterHitImpl.cc
  ./src/IMPL/ClusterImpl.cc
  ./src/IMPL/LCCollectionVec.cc
  ./src/IMPL/LCEventImpl.cc
  ./src/IMPL/LCFlagImpl.cc
  ./src/IMPL/LCGenericObjectImpl.cc
  ./src/IMPL/LCIO.cc
  ./src/IMPL/LCIOExceptionHandler.cc
  ./src/IMPL/LCParametersImpl.cc
  ./src/IMPL/LCRunHeaderImpl.cc
  ./src/IMPL/MCParticleImpl.cc
  ./src/IMPL/ParticleIDImpl.cc
  ./src/IMPL/RawCalorimeterHitImpl.cc
  ./src/IMPL/ReconstructedParticleImpl.cc
  ./src/IMPL/SimCalorimeterHitImpl.cc
  ./src/IMPL/SimTrackerHitImpl.cc
  ./src/IMPL/TPCHitImpl.cc
  ./src/IMPL/TrackImpl.cc
  ./src/IMPL/TrackStateImpl.cc
  ./src/IMPL/TrackerDataImpl.cc
  ./src/IMPL/TrackerHitImpl.cc
  ./src/IMPL/TrackerHitPlaneImpl.cc
  ./src/IMPL/TrackerHitZCylinderImpl.cc
  ./src/IMPL/TrackerPulseImpl.cc
  ./src/IMPL/TrackerRawDataImpl.cc
  ./src/IMPL/VertexImpl.cc
)

SET( LCIO_IOIMPL_SRCS
  ./src/IOIMPL/LCFactory.cc
)

SET( LCIO_SIO_SRCS
  ./src/SIO/LCSIO.cc
  ./src/SIO/SIOCalHitHandler.cc
  ./src/SIO/SIOClusterHandler.cc
  ./src/SIO/SIOCollectionHandler.cc
  ./src/SIO/SIOEventHandler.cc
  ./src/SIO/SIOFloatVecHandler.cc
  ./src/SIO/SIOHandlerMgr.cc
  ./src/SIO/SIOIntVecHandler.cc
  ./src/SIO/SIOLCGenericObjectHandler.cc
  ./src/SIO/SIOLCParameters.cc
  ./src/SIO/SIOLCRelationHandler.cc
  ./src/SIO/SIOObjectHandler.cc
  ./src/SIO/SIOParticleHandler.cc
  ./src/SIO/SIORawCalHitHandler.cc
  ./src/SIO/SIOReconstructedParticleHandler.cc
  ./src/SIO/SIORunHeaderHandler.cc
  ./src/SIO/SIOSimCalHitHandler.cc
  ./src/SIO/SIOSimTrackHitHandler.cc
  ./src/SIO/SIOStrVecHandler.cc
  ./src/SIO/SIOTPCHitHandler.cc
  ./src/SIO/SIOTrackHandler.cc
  ./src/SIO/SIOTrackerDataHandler.cc
  ./src/SIO/SIOTrackerHitHandler.cc
  ./src/SIO/SIOTrackerHitZCylinderHandler.cc
  ./src/SIO/SIOTrackerHitPlaneHandler.cc
  ./src/SIO/SIOTrackerPulseHandler.cc
  ./src/SIO/SIOTrackerRawDataHandler.cc
  ./src/SIO/SIOVertexHandler.cc
  ./src/SIO/SIOWriter.cc
  ./src/SIO/SIOReader.cc
  ./src/SIO/SIORandomAccessHandler.cc
  ./src/SIO/SIOIndexHandler.cc
  ./src/SIO/LCIORandomAccess.cc
  ./src/SIO/LCIORandomAccessMgr.cc
  ./src/SIO/RunEventMap.cc
)

SET( LCIO_UTIL_SRCS
  ./src/UTIL/BitField64.cc
  ./src/UTIL/IndexMap.cc
  ./src/UTIL/LCRelationNavigator.cc
  ./src/UTIL/LCSplitWriter.cc
  ./src/UTIL/LCStdHepRdr.cc
  ./src/UTIL/LCStdHepRdrNew.cc
  ./src/UTIL/LCTime.cc
  ./src/UTIL/Operators.cc
  ./src/UTIL/lStdHep.cc
  ./src/UTIL/lXDR.cc
  ./src/UTIL/LCTOOLS.cc
  ./src/UTIL/LCWarning.cc
  ./src/UTIL/CellIDEncoder.cc
  ./src/UTIL/CollectionParameterMap.cc
  ./src/UTIL/PIDHandler.cc
  ./src/UTIL/ILDConf.cc
)


###### macro for generating lcio c++ header files from aid files #############
#
# usage:
#   ADD_LCIO_AID_HEADERS_DIRECTORY( "${LCIO_AID_HEADERS_DIR}/EVENT" )
#
#   the previous command goes into the given directory, finds all .aid
#   header files and generates their corresponding c++ headers as follows:
#       ${LCIO_AID_HEADERS_DIR}/EVENT/LCIO.aid
#       ${LCIO_AID_HEADERS_OUTPUT_DIR}/EVENT/LCIO.h
#
#
#
# input argument:
#   full path to directory containing aid files
#
#
# required variables:
#
#   LCIO_AID_HEADERS_OUTPUT_DIR - path used to store generated c++ headers
#
#   LCIO_AID_HEADERS_DIR        - path used to calculate relative paths
#       for the generated c++ header, i.e. the "subtracted" path from the
#       full path to the .aid files located in the input directory.
#
#
# returns variables:
#
#   AID_HEADERS_LIST - variable containing list of all input .aid files
#   CXX_HEADERS_LIST - variable containing list of all output header files
#
##############################################################################
#SET( LCIO_AID_HEADERS_OUTPUT_DIR "${PROJECT_BINARY_DIR}/include" )
#SET( LCIO_AID_HEADERS_DIR "${LCIO_SOURCE_DIR}/src/aid" )

MACRO( ADD_LCIO_AID_HEADERS_DIRECTORY _input_dir )

    # get list of all .aid files in input directory
    FILE( GLOB _aid_headers "${_input_dir}/*.aid" )

    # clear output variables
    SET( AID_HEADERS_LIST )
    SET( CXX_HEADERS_LIST )

    FOREACH( _aid_header ${_aid_headers} )

        # /path/to/lcio/src/aid/EVENT/MCParticle.aid -> EVENT/MCParticle.aid
        FILE( RELATIVE_PATH _aid_header_relpath "${LCIO_AID_HEADERS_DIR}" "${_aid_header}" )

        # EVENT/MCParticle.aid -> EVENT
        GET_FILENAME_COMPONENT( _cxx_header_basepath ${_aid_header_relpath} PATH )
        # EVENT/MCParticle.aid -> MCParticle
        GET_FILENAME_COMPONENT( _cxx_header_basename ${_aid_header_relpath} NAME_WE )

        # ${LCIO_AID_HEADERS_OUTPUT_DIR}/EVENT/MCParticle.h
        SET( _cxx_header "${LCIO_AID_HEADERS_OUTPUT_DIR}/${_cxx_header_basepath}/${_cxx_header_basename}.h" )

        # debug messages
        #MESSAGE( "_aid_header: ${_aid_header}" )
        #MESSAGE( "_aid_header_relpath: ${_aid_header_relpath}" )
        #MESSAGE( "_cxx_header_basepath: ${_cxx_header_basepath}" )
        #MESSAGE( "_cxx_header_basename: ${_cxx_header_basename}" )
        #MESSAGE( "_cxx header: ${_cxx_header}" )

        LIST( APPEND AID_HEADERS_LIST "${_aid_header}" )
        LIST( APPEND CXX_HEADERS_LIST "${_cxx_header}" )

        ADD_CUSTOM_COMMAND(
            OUTPUT "${_cxx_header}"
            COMMAND ${SH} "${LCIO_ENV_INIT}" java -cp
            '${LCIO_SOURCE_DIR}/tools/freehep-base.jar:${LCIO_SOURCE_DIR}/tools/freehep-aid.jar:${LCIO_SOURCE_DIR}/tools/freehep-buildtools.jar'
            org.freehep.aid.Aid -directory '${LCIO_AID_HEADERS_OUTPUT_DIR}/' -property config CPPHeaderGenerator "${_aid_header}" > /dev/null
            DEPENDS "${_aid_header}"
            WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
            COMMENT "Generating header file ${_cxx_header_basepath}/${_cxx_header_basename}.h"
        )

        #SET_SOURCE_FILES_PROPERTIES( "${_cxx_header}" PROPERTIES HEADER_FILE_ONLY TRUE )
        #SET_SOURCE_FILES_PROPERTIES( "${_cxx_header}" PROPERTIES GENERATED TRUE )

    ENDFOREACH()

ENDMACRO()
##############################################################################



if( LCIO_GENERATE_HEADERS )
  # -------- aid generated c++ headers -----------------------------------------
  ADD_LCIO_AID_HEADERS_DIRECTORY( "${LCIO_AID_HEADERS_DIR}/EVENT" )
  SET( EVENT_AID_HEADERS_LIST ${AID_HEADERS_LIST} )
  SET( EVENT_CXX_HEADERS_LIST ${CXX_HEADERS_LIST} )
  SET( EVENT_AID_HEADERS_DEP_LIST ${AID_HEADERS_LIST};${CXX_HEADERS_LIST} )
  
  ADD_LCIO_AID_HEADERS_DIRECTORY( "${LCIO_AID_HEADERS_DIR}/IO" )
  SET( IO_AID_HEADERS_LIST ${AID_HEADERS_LIST} )
  SET( IO_CXX_HEADERS_LIST ${CXX_HEADERS_LIST} )
  SET( IO_AID_HEADERS_DEP_LIST ${AID_HEADERS_LIST};${CXX_HEADERS_LIST} )
  
  # make source files depend on generated header files + aid files
  SET_SOURCE_FILES_PROPERTIES( ${LCIO_CPPFORT_SRCS} PROPERTIES OBJECT_DEPENDS "${EVENT_AID_HEADERS_DEP_LIST};${IO_AID_HEADERS_DEP_LIST}" )
  SET_SOURCE_FILES_PROPERTIES( ${LCIO_IMPL_SRCS} PROPERTIES OBJECT_DEPENDS "${EVENT_AID_HEADERS_DEP_LIST}" )
  SET_SOURCE_FILES_PROPERTIES( ${LCIO_IOIMPL_SRCS} PROPERTIES OBJECT_DEPENDS "${IO_AID_HEADERS_DEP_LIST}" )
  # ----------------------------------------------------------------------------

else()

  #------FG: if the headers are not generated we need to manually set the header lists
  
  # get list of all .h files in pre-generated directory
  FILE( GLOB  EVENT_CXX_HEADERS_LIST "${LCIO_AID_HEADERS_OUTPUT_DIR}/EVENT/*.h" )
  FILE( GLOB  IO_CXX_HEADERS_LIST "${LCIO_AID_HEADERS_OUTPUT_DIR}/IO/*.h" )

#  MESSAGE( STATUS "********************* IO_CXX_HEADERS_LIST:  ${IO_CXX_HEADERS_LIST}" )
#  MESSAGE( STATUS "********************* EVENT_CXX_HEADERS_LIST:  ${EVENT_CXX_HEADERS_LIST}" )

endif()


# -------- compiler definitions ----------------------------------------------
#ADD_DEFINITIONS( "-Wall -ansi -pedantic" )
#ADD_DEFINITIONS( "-Wno-long-long -fno-strict-aliasing" )



#---- special flags for cfortran code : --------
SET_SOURCE_FILES_PROPERTIES( 
  ${LCIO_CPPFORT_SRCS} PROPERTIES 
  COMPILE_FLAGS "-Df2cFortran -Wno-unused"
)
# ----------------------------------------------------------------------------




# -------- include directories -----------------------------------------------
INCLUDE_DIRECTORIES( BEFORE "${LCIO_AID_HEADERS_OUTPUT_DIR}" )
INCLUDE_DIRECTORIES( BEFORE "${LCIO_SOURCE_DIR}/sio/include" )
INCLUDE_DIRECTORIES( BEFORE "${LCIO_CXX_HEADERS_DIR}" )

# install public headers
INSTALL_DIRECTORY( ${LCIO_CXX_HEADERS_DIR}/ DESTINATION include 
    PATTERN "*.txt" EXCLUDE
    #PATTERN "*empty_ignore.h" EXCLUDE
    PATTERN "*/SIO*" EXCLUDE
    PATTERN "*/CPPFORT*" EXCLUDE
    PATTERN "*/EVENT*" EXCLUDE
    PATTERN "*/pre-generated" EXCLUDE
    PATTERN "*/IO" EXCLUDE # watchout: */IO* pattern also excludes IOIMPL
)

# install aid generated c++ headers
INSTALL_DIRECTORY( ${LCIO_AID_HEADERS_OUTPUT_DIR}/ DESTINATION include )
# ----------------------------------------------------------------------------



# ------- libraries lcio + lcioF77 -------------------------------------------
ADD_LIBRARY( lcioF77 STATIC ${LCIO_CPPFORT_SRCS} )
INSTALL( TARGETS lcioF77 DESTINATION lib )

ADD_SHARED_LIBRARY( lcio
  ${LCIO_IMPL_SRCS}
  ${LCIO_IOIMPL_SRCS}
  ${LCIO_UTIL_SRCS}
  ${LCIO_SIO_SRCS}
)
INSTALL_SHARED_LIBRARY( lcio DESTINATION lib )

TARGET_LINK_LIBRARIES( lcio sio )
TARGET_LINK_LIBRARIES( lcioF77 lcio )

ADD_CUSTOM_TARGET( lib DEPENDS lcio lcioF77 )
# ----------------------------------------------------------------------------




# -------- rootdict library --------------------------------------------------

IF( BUILD_ROOTDICT )

  #fg: for ROOT 6 we need the .src/cpp as include directory  
  INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} )

    # special rootcint definitions for lcio 
#    SET( ROOT_DICT_CINT_DEFINITIONS -DHANDLE_LCIO_EXCEPTIONS=IGNORED_FOR_CINT )

    # load MacroRootDict.cmake from ILCSOFT_CMAKE_MODULES
    INCLUDE( ${ROOT_DICT_MACROS_FILE} )

    # include directories to be passed over to rootcint
    SET( ROOT_DICT_INCLUDE_DIRS
        ${LCIO_AID_HEADERS_OUTPUT_DIR}
        ${LCIO_CXX_HEADERS_DIR}
    )



    # ------ generate dictionary for UTIL classes ------------------
    # get list of ALL LCIO headers in LCIO_CXX_HEADERS_DIR
    FILE( GLOB UTIL_CXX_HEADERS_LIST 
      ${LCIO_CXX_HEADERS_DIR}/*.h 
      ${LCIO_CXX_HEADERS_DIR}/rootDict/*.h 
      ${LCIO_CXX_HEADERS_DIR}/UTIL/*.h
      )

#    FILE( GLOB_RECURSE UTIL_CXX_HEADERS_LIST ${LCIO_CXX_HEADERS_DIR}/UTIL/*.h )

    # list of headers to exclude
    FILE( GLOB_RECURSE exclude_headers 
        ${LCIO_CXX_HEADERS_DIR}/empty_ignore.h 
        ${LCIO_CXX_HEADERS_DIR}/UTIL/LCWarning.h
        ${LCIO_CXX_HEADERS_DIR}/UTIL/LCFourVector.h
    )

    # remove items to exclude
    LIST( REMOVE_ITEM UTIL_CXX_HEADERS_LIST ${exclude_headers} )

    
    MESSAGE( STATUS "********************* UTIL_CXX_HEADERS_LIST:  ${UTIL_CXX_HEADERS_LIST}" )

    # generate dictionary source files!
    GEN_ROOT_DICT_LINKDEF_HEADER( UTIL ${UTIL_CXX_HEADERS_LIST} )
    GEN_ROOT_DICT_SOURCES( UTIL.cxx )
    LIST( APPEND lcio_rootdict_sources ${ROOT_DICT_OUTPUT_SOURCES} )



#  MESSAGE( STATUS "********************* IO_CXX_HEADERS_LIST:  ${IO_CXX_HEADERS_LIST}" )
#  MESSAGE( STATUS "********************* EVENT_CXX_HEADERS_LIST:  ${EVENT_CXX_HEADERS_LIST}" )
#  MESSAGE( STATUS "********************* UTIL_CXX_HEADERS_LIST:  ${UTIL_CXX_HEADERS_LIST}" )


    # ------ generate dictionary for IO classes --------------------
    # IO_CXX_HEADERS_LIST is generated in ADD_LCIO_AID_HEADERS_DIRECTORY
    GEN_ROOT_DICT_LINKDEF_HEADER( IO ${IO_CXX_HEADERS_LIST} )
    GEN_ROOT_DICT_SOURCES( IO.cxx )
    LIST( APPEND lcio_rootdict_sources ${ROOT_DICT_OUTPUT_SOURCES} )



    # ------ generate dictionary for EVENT classes -----------------
    # EVENT_CXX_HEADERS_LIST is generated in ADD_LCIO_AID_HEADERS_DIRECTORY
    GEN_ROOT_DICT_LINKDEF_HEADER( EVENT ${EVENT_CXX_HEADERS_LIST} )
    GEN_ROOT_DICT_SOURCES( EVENT.cxx )
    LIST( APPEND lcio_rootdict_sources ${ROOT_DICT_OUTPUT_SOURCES} )



    # ------ generate dictionary for IMPL classes ------------------
    PREPARE_ROOT_DICT_HEADERS( ${LCIO_CXX_HEADERS_DIR}/IMPL )
    GEN_ROOT_DICT_LINKDEF_HEADER( IMPL ${ROOT_DICT_INPUT_HEADERS} )
    GEN_ROOT_DICT_SOURCES( IMPL.cxx )
    LIST( APPEND lcio_rootdict_sources ${ROOT_DICT_OUTPUT_SOURCES} )



    # ------ generate dictionary for IOIMPL classes ------------------
    PREPARE_ROOT_DICT_HEADERS( ${LCIO_CXX_HEADERS_DIR}/IOIMPL )
    GEN_ROOT_DICT_LINKDEF_HEADER( IOIMPL ${ROOT_DICT_INPUT_HEADERS} )
    GEN_ROOT_DICT_SOURCES( IOIMPL.cxx )
    LIST( APPEND lcio_rootdict_sources ${ROOT_DICT_OUTPUT_SOURCES} )

    # ------ generate dictionary for templates ------------------
    PREPARE_ROOT_DICT_HEADERS( ${LCIO_CXX_HEADERS_DIR}/rootDict )
    GEN_ROOT_DICT_LINKDEF_HEADER( rootDict ${ROOT_DICT_INPUT_HEADERS} )
    GEN_ROOT_DICT_SOURCES( rootDict.cxx )
    LIST( APPEND lcio_rootdict_sources ${ROOT_DICT_OUTPUT_SOURCES} )



    # ------ build lcioDict library ---------------------------------

    INCLUDE_DIRECTORIES( "${ROOT_DICT_OUTPUT_DIR}" )
    INCLUDE_DIRECTORIES( SYSTEM "${ROOT_INCLUDE_DIRS}" )

    ADD_SHARED_LIBRARY( lcioDict ${lcio_rootdict_sources} )
    TARGET_LINK_LIBRARIES( lcioDict ${ROOT_LIBRARIES} lcio )

    IF( APPLE )

      SET_TARGET_PROPERTIES( lcioDict PROPERTIES LINKER_FLAGS
        "-single_module -undefined dynamic_lookup -bind_at_load" )
      
      ADD_CUSTOM_TARGET(dylib2so
	COMMAND ${CMAKE_COMMAND} -E create_symlink liblcio.dylib liblcio.so 
	COMMAND ${CMAKE_COMMAND} -E create_symlink liblcioDict.dylib liblcioDict.so
	WORKING_DIRECTORY  ${CMAKE_INSTALL_PREFIX}/lib 
	DEPENDS ${CMAKE_INSTALL_PREFIX}/lib/liblcio.dylib )

      INSTALL( CODE "EXECUTE_PROCESS( COMMAND ${CMAKE_BUILD_TOOL} dylib2so)" )

    ENDIF()


    ADD_CUSTOM_TARGET( dict DEPENDS lcioDict )

    INSTALL_SHARED_LIBRARY( lcioDict DESTINATION lib )
    
    #fg: for root6 we also need to install the pcm files ...
    SET( ROOT_DICT_PCM_FILES  
      ${ROOT_DICT_OUTPUT_DIR}/EVENT_rdict.pcm
      ${ROOT_DICT_OUTPUT_DIR}/IMPL_rdict.pcm
      ${ROOT_DICT_OUTPUT_DIR}/IOIMPL_rdict.pcm
      ${ROOT_DICT_OUTPUT_DIR}/IO_rdict.pcm
      ${ROOT_DICT_OUTPUT_DIR}/UTIL_rdict.pcm
      ${ROOT_DICT_OUTPUT_DIR}/rootDict_rdict.pcm
      )
    
    message( STATUS "{ROOT_DICT_OUTPUT_DIR} : ${ROOT_DICT_OUTPUT_DIR}  - {ROOT_DICT_PCM_FILES}: ${ROOT_DICT_PCM_FILES} " )

    INSTALL(FILES  ${ROOT_DICT_PCM_FILES} DESTINATION lib)

ENDIF()
# ----------------------------------------------------------------------------




# ==== add examples from the EXAMPLES directory ==============================
ADD_CUSTOM_TARGET( examples )
MACRO( ADD_LCIO_EXAMPLE file )
  ADD_EXECUTABLE( bin_${file} ./src/EXAMPLE/${file}.cc )
  ADD_DEPENDENCIES( examples bin_${file} )
  #MESSAGE( STATUS "ADD_LCIO_EXAMPLE: ${file} ./src/EXAMPLE/${file}.cc" )
  SET_TARGET_PROPERTIES( bin_${file} PROPERTIES OUTPUT_NAME ${file} )
  TARGET_LINK_LIBRARIES( bin_${file} lcio )
  INSTALL( TARGETS bin_${file} DESTINATION bin )
ENDMACRO()

ADD_LCIO_EXAMPLE( simjob ) 
ADD_LCIO_EXAMPLE( anajob ) 
ADD_LCIO_EXAMPLE( recjob ) 
ADD_LCIO_EXAMPLE( dumpevent ) 
ADD_LCIO_EXAMPLE( copyfix ) 
ADD_LCIO_EXAMPLE( stdhepjob ) 
ADD_LCIO_EXAMPLE( stdhepjob_new )
ADD_LCIO_EXAMPLE( addRandomAccess )
ADD_LCIO_EXAMPLE( lcio_event_counter ) 
ADD_LCIO_EXAMPLE( lcio_check_col_elements )
ADD_LCIO_EXAMPLE( lcio_split_file ) 
ADD_LCIO_EXAMPLE( lcio_merge_files ) 
ADD_LCIO_EXAMPLE( readmcparticles ) 

IF( BUILD_LCIO_EXAMPLES )
  ADD_LCIO_EXAMPLE( lcrtrelation )
  ADD_LCIO_EXAMPLE( readcalibration )

  # LCIO SHELL needs curses and readline libraries
  FIND_PACKAGE( Curses QUIET )
  FIND_LIBRARY( READLINE_LIBRARY readline PATHS /usr/local/lib64 /usr/local/lib )
  IF( CURSES_LIBRARY AND READLINE_LIBRARY )
    ADD_LCIO_EXAMPLE( lsh )
    TARGET_LINK_LIBRARIES( bin_lsh ${CURSES_LIBRARY} ${READLINE_LIBRARY} )
  ELSE()
    MESSAGE( STATUS "cannot build lsh: curses and/or readline libraries not found" )
  ENDIF()
ENDIF()
# ============================================================================

