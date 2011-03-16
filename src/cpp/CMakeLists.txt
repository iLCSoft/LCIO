#################################
# cmake file for building LCIO
# @author Frank Gaede, DESY
# @author Jan Engels, DESY
#################################

INCLUDE_DIRECTORIES ( BEFORE "${AID_GEN_CXX_HEADERS_DIR}" )

INCLUDE_DIRECTORIES (
    "${LCIO_SOURCE_DIR}/sio/include"
    "${LCIO_CXX_HEADERS_DIR}"
#    "${AID_GEN_CXX_HEADERS_DIR}"
)

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
  ./src/IMPL/TrackerDataImpl.cc
  ./src/IMPL/TrackerHitImpl.cc
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
  ./src/UTIL/CellIDDecoder.cc
  ./src/UTIL/IndexMap.cc
  ./src/UTIL/LCRelationNavigator.cc
  ./src/UTIL/LCSplitWriter.cc
  ./src/UTIL/LCStdHepRdr.cc
  ./src/UTIL/LCTime.cc
  ./src/UTIL/Operators.cc
  ./src/UTIL/lStdHep.cc
  ./src/UTIL/lXDR.cc
  ./src/UTIL/LCTOOLS.cc
  ./src/UTIL/LCWarning.cc
  ./src/UTIL/CellIDEncoder.cc
  ./src/UTIL/CollectionParameterMap.cc
  ./src/UTIL/PIDHandler.cc
)


# ==== a macro for generating header files ==================
#AID_GEN_CXX_HEADERS_DIR must be set
MACRO( ADD_AID_HEADER aid_header_full_path aid_header_base_path )

    #MESSAGE( STATUS "ADD_AID_HEADER: ${aid_header_full_path} ${aid_header_base_path}" )
    #MESSAGE( STATUS "ADD_AID_HEADER: ${aid_header_base_path}" )

    IF( NOT AID_GEN_CXX_HEADERS_DIR )
        MESSAGE( FATAL_ERROR "AID_GEN_CXX_HEADERS_DIR not set" )
    ENDIF()

    GET_FILENAME_COMPONENT( header_basename ${aid_header_base_path} NAME_WE )
    GET_FILENAME_COMPONENT( header_basepath ${aid_header_base_path} PATH )

    #MESSAGE( STATUS "ADD_AID_HEADER: ${header_basepath}/${header_basename}" )

    SET( gen_header_file "${AID_GEN_CXX_HEADERS_DIR}/${header_basepath}/${header_basename}.h" )

    LIST( APPEND AID_HEADERS_DEP_LIST "${gen_header_file}" )
    LIST( APPEND AID_HEADERS_DEP_LIST "${aid_header_full_path}" )

    ADD_CUSTOM_COMMAND(
        OUTPUT "${gen_header_file}"
        COMMAND ${SH} ARGS "${LCIO_ENV_INIT}" java -cp
        '${LCIO_SOURCE_DIR}/tools/freehep-base.jar:${LCIO_SOURCE_DIR}/tools/freehep-aid.jar:${LCIO_SOURCE_DIR}/tools/freehep-buildtools.jar'
        org.freehep.aid.Aid -directory '${AID_GEN_CXX_HEADERS_DIR}/' -property config CPPHeaderGenerator "${aid_header_full_path}" > /dev/null
        #COMMAND ${SH} ARGS "${LCIO_ENV_INIT}" ant aid.generate
        DEPENDS "${aid_header_full_path}"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        COMMENT "Generating header file ${header_basepath}/${header_basename}.h"
    )

    #SET_SOURCE_FILES_PROPERTIES( "${gen_header_file}" PROPERTIES HEADER_FILE_ONLY TRUE )
    #SET_SOURCE_FILES_PROPERTIES( "${gen_header_file}" PROPERTIES GENERATED TRUE )

    #ADD_ROOT_DICT_OBJ( ${gen_header_file} ${header_basepath}/${header_basename}.h )

ENDMACRO()


# EVENT aid files
FILE( GLOB aid_headers "${LCIO_AID_HEADERS_DIR}/EVENT/*.aid" )

FOREACH( aid_file ${aid_headers} )

    FILE( RELATIVE_PATH aid_file_basepath ${LCIO_AID_HEADERS_DIR} ${aid_file} )

    ADD_AID_HEADER( ${aid_file} ${aid_file_basepath} )

ENDFOREACH()

SET( EVENT_AID_HEADERS_DEP_LIST ${AID_HEADERS_DEP_LIST} )
SET( AID_HEADERS_DEP_LIST )

# IO aid files
FILE( GLOB aid_headers "${LCIO_AID_HEADERS_DIR}/IO/*.aid" )

FOREACH( aid_file ${aid_headers} )

    FILE( RELATIVE_PATH aid_file_basepath ${LCIO_AID_HEADERS_DIR} ${aid_file} )

    ADD_AID_HEADER( ${aid_file} ${aid_file_basepath} )

ENDFOREACH()

SET( IO_AID_HEADERS_DEP_LIST ${AID_HEADERS_DEP_LIST} )
SET( AID_HEADERS_DEP_LIST )

SET_SOURCE_FILES_PROPERTIES( ${LCIO_CPPFORT_SRCS} PROPERTIES OBJECT_DEPENDS
    "${EVENT_AID_HEADERS_DEP_LIST};${IO_AID_HEADERS_DEP_LIST}" )

SET_SOURCE_FILES_PROPERTIES( ${LCIO_IMPL_SRCS} PROPERTIES OBJECT_DEPENDS "${EVENT_AID_HEADERS_DEP_LIST}" )
SET_SOURCE_FILES_PROPERTIES( ${LCIO_IOIMPL_SRCS} PROPERTIES OBJECT_DEPENDS "${IO_AID_HEADERS_DEP_LIST}" )



#------ require proper C++ -------
ADD_DEFINITIONS( "-Wall -ansi -pedantic" )
#----- need long long for int64 for now ------
ADD_DEFINITIONS( "-Wno-long-long -fno-strict-aliasing" )

#---- special flags for cfortran code : --------
SET_SOURCE_FILES_PROPERTIES( 
  ${LCIO_CPPFORT_SRCS} PROPERTIES 
  COMPILE_FLAGS "-Df2cFortran -Wno-unused"
)

ADD_LIBRARY( lib_LCIOF77 STATIC
  ${LCIO_CPPFORT_SRCS} 
)

SET_TARGET_PROPERTIES( lib_LCIOF77 PROPERTIES
    # create *nix style library versions + symbolic links
    #VERSION ${LCIO_VERSION}
    #SOVERSION ${LCIO_SOVERSION}
    # allow creating static and shared libs without conflicts
    CLEAN_DIRECT_OUTPUT 1
    # avoid conflicts between library and binary target names
    OUTPUT_NAME lcioF77
)

ADD_CUSTOM_TARGET( lib DEPENDS lib_LCIO lib_LCIOF77 lib_SIO )

ADD_LIBRARY( lib_LCIO 
  ${LCIO_IMPL_SRCS}
  ${LCIO_IOIMPL_SRCS}
  ${LCIO_UTIL_SRCS}
  ${LCIO_SIO_SRCS}
)

SET_TARGET_PROPERTIES( lib_LCIO PROPERTIES
    # create *nix style library versions + symbolic links
    VERSION ${LCIO_VERSION}
    SOVERSION ${LCIO_SOVERSION}
    # allow creating static and shared libs without conflicts
    CLEAN_DIRECT_OUTPUT 1
    # avoid conflicts between library and binary target names
    OUTPUT_NAME lcio
)

TARGET_LINK_LIBRARIES( lib_LCIO lib_SIO )
TARGET_LINK_LIBRARIES( lib_LCIOF77 lib_LCIO )


# create symbolic target for calling targets bin_XXX
ADD_CUSTOM_TARGET( bin )

# ==== a macro for adding binaries from the EXAMPLE directory =====
MACRO( ADD_LCIO_BIN file )
  ADD_EXECUTABLE( bin_${file} ./src/EXAMPLE/${file}.cc )
  ADD_DEPENDENCIES( bin bin_${file} )
#  MESSAGE( STATUS " in ADD_LCIO_BIN : " ${file} " " ./src/EXAMPLE/${file}.cc )
  SET_TARGET_PROPERTIES( bin_${file} PROPERTIES OUTPUT_NAME ${file} )
  TARGET_LINK_LIBRARIES( bin_${file} lib_LCIO )
  INSTALL( TARGETS bin_${file} DESTINATION bin )
ENDMACRO()
#===================================================================

ADD_LCIO_BIN( simjob ) 
ADD_LCIO_BIN( anajob ) 
ADD_LCIO_BIN( recjob ) 
ADD_LCIO_BIN( dumpevent ) 
ADD_LCIO_BIN( copyfix ) 
ADD_LCIO_BIN( stdhepjob ) 
ADD_LCIO_BIN( addRandomAccess )
ADD_LCIO_BIN( lcio_event_counter ) 



IF( BUILD_LCIO_EXAMPLES )
  ADD_LCIO_BIN( lcrtrelation )
  ADD_LCIO_BIN( readcalibration )

  # LCIO SHELL needs curses and readline libraries
  FIND_PACKAGE( Curses )
  FIND_LIBRARY( READLINE_LIBRARY
    NAMES readline
    PATHS
      /usr/local/lib
      /usr/local/lib64
      /usr/lib
      /usr/lib64
      /lib
      /lib64
  )
  IF( CURSES_LIBRARY AND READLINE_LIBRARY )
    ADD_LCIO_BIN( lsh )
    TARGET_LINK_LIBRARIES( bin_lsh ${CURSES_LIBRARY} ${READLINE_LIBRARY} )
  ELSE()
    MESSAGE( STATUS "Warning: didn't find curses and/or readline libraries. Cannot build lsh" )
  ENDIF()
ENDIF()



# create symbolic target for calling targets test_XXX
ADD_CUSTOM_TARGET( tests )

# call make tests before make install
#INSTALL( CODE "EXEC_PROGRAM(${CMAKE_BUILD_TOOL} ${PROJECT_BINARY_DIR} ARGS tests)" )

# custom command to generate jar library
ADD_CUSTOM_COMMAND(
    OUTPUT "${PROJECT_SOURCE_DIR}/lib/lcio.jar"
    COMMAND ${SH} "${LCIO_ENV_INIT}" ant aid
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    COMMENT "generating lcio.jar"
)

# create target to create jar library
ADD_CUSTOM_TARGET( lib_jar DEPENDS "${PROJECT_SOURCE_DIR}/lib/lcio.jar" )

# jar lib is needed for java tests
ADD_DEPENDENCIES( tests lib_jar )

ADD_DEPENDENCIES( tests bin )

# ==== a macro for adding binaries from the TESTS directory =====
MACRO( ADD_LCIO_CTEST file )
  IF( BUILD_LCIO_TESTS )
    ADD_EXECUTABLE( ${file} ./src/TESTS/${file}.cc )
  ELSE()
    ADD_EXECUTABLE( ${file} EXCLUDE_FROM_ALL ./src/TESTS/${file}.cc )
  ENDIF()
  ADD_DEPENDENCIES( tests ${file} )
  TARGET_LINK_LIBRARIES( ${file} lib_LCIO )
  ADD_TEST( t_${file} "${EXECUTABLE_OUTPUT_PATH}/${file}" )
  SET_TESTS_PROPERTIES( t_${file} PROPERTIES
      PASS_REGULAR_EXPRESSION "TEST_PASSED" )
ENDMACRO()
#===================================================================


#ADD_TEST( t_simjob "${EXECUTABLE_OUTPUT_PATH}/simjob" )


#------------- install the library and the (public) header files -------

INSTALL( TARGETS lib_LCIO DESTINATION lib PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE )

INSTALL( TARGETS lib_LCIOF77 DESTINATION lib PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE )

INSTALL(DIRECTORY ${LCIO_CXX_HEADERS_DIR}/ DESTINATION include 
  PATTERN "*~" EXCLUDE
  PATTERN "*.txt" EXCLUDE
  #PATTERN "*empty_ignore.h" EXCLUDE
  PATTERN "*/SIO*" EXCLUDE
  PATTERN "*/CPPFORT*" EXCLUDE
  PATTERN "*/EVENT*" EXCLUDE
  PATTERN "*/IO" EXCLUDE # watchout: */IO* pattern also excludes IOIMPL
  PATTERN "*/CVS*" EXCLUDE
  )

# install generated c++ headers (from aid files)
IF( "${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.6 )
    FILE(MAKE_DIRECTORY ${AID_GEN_CXX_HEADERS_DIR})
ENDIF()
INSTALL(DIRECTORY ${AID_GEN_CXX_HEADERS_DIR}/ DESTINATION include)



# =================== ctest stuff (call with ctest or 'make test') ===============================================


# ------ c++ tests -----------------------------------------------------
ADD_TEST( t_c_sim "${EXECUTABLE_OUTPUT_PATH}/simjob" c_sim.slcio )
ADD_TEST( t_c_ana_c_sim "${EXECUTABLE_OUTPUT_PATH}/anajob" c_sim.slcio )
ADD_TEST( t_c_rec_c_sim "${EXECUTABLE_OUTPUT_PATH}/recjob" c_sim.slcio c_rec.slcio )
ADD_TEST( t_c_ana_c_rec "${EXECUTABLE_OUTPUT_PATH}/anajob" c_rec.slcio )

SET_TESTS_PROPERTIES( t_c_sim PROPERTIES PASS_REGULAR_EXPRESSION "created[ ]+10[ ]+runs with[ ]+100[ ]+events" )
SET_TESTS_PROPERTIES( t_c_ana_c_sim PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
SET_TESTS_PROPERTIES( t_c_rec_c_sim PROPERTIES PASS_REGULAR_EXPRESSION "'SomeClusters' and 'SomeTracks'.*to[ ]+100[ ]+events" )
SET_TESTS_PROPERTIES( t_c_ana_c_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
# ----------------------------------------------------------------------

# ------ java tests -----------------------------------------------------
ADD_TEST( t_j_sim ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runSimJob.sh j_sim.slcio )
ADD_TEST( t_j_ana_j_sim ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runAnalysisJob.sh j_sim.slcio )
ADD_TEST( t_j_rec_j_sim ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runRecJob.sh j_sim.slcio j_rec.slcio )
ADD_TEST( t_j_ana_j_rec ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runAnalysisJob.sh j_rec.slcio )

SET_TESTS_PROPERTIES( t_j_sim PROPERTIES PASS_REGULAR_EXPRESSION "created[ ]+10[ ]+runs with[ ]+100[ ]+events" )
SET_TESTS_PROPERTIES( t_j_ana_j_sim PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
SET_TESTS_PROPERTIES( t_j_rec_j_sim PROPERTIES PASS_REGULAR_EXPRESSION "Analyzed[ ]+100[ ]+events" )
SET_TESTS_PROPERTIES( t_j_ana_j_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
# ------------------------------------------------------------------------------------------------------------------------------


# ------ c++/java ana/sim mixed tests -----------------------------------------------------
ADD_TEST( t_c_ana_j_sim "${EXECUTABLE_OUTPUT_PATH}/anajob" j_sim.slcio )
ADD_TEST( t_c_ana_j_rec "${EXECUTABLE_OUTPUT_PATH}/anajob" j_rec.slcio )
ADD_TEST( t_j_ana_c_sim ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runAnalysisJob.sh c_sim.slcio )
ADD_TEST( t_j_ana_c_rec ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runAnalysisJob.sh c_rec.slcio )

SET_TESTS_PROPERTIES( t_c_ana_j_sim PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
SET_TESTS_PROPERTIES( t_c_ana_j_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
SET_TESTS_PROPERTIES( t_j_ana_c_sim PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
SET_TESTS_PROPERTIES( t_j_ana_c_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
# ------------------------------------------------------------------------------------------------------------------------------



# ------ c++/java sim/rec mixed tests -----------------------------------------------------
ADD_TEST( t_c_rec_j_sim "${EXECUTABLE_OUTPUT_PATH}/recjob" j_sim.slcio j2c_rec.slcio )
ADD_TEST( t_j_rec_c_sim ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runRecJob.sh c_sim.slcio c2j_rec.slcio )
ADD_TEST( t_c_ana_j2c_rec "${EXECUTABLE_OUTPUT_PATH}/anajob" j2c_rec.slcio )
ADD_TEST( t_c_ana_c2j_rec "${EXECUTABLE_OUTPUT_PATH}/anajob" c2j_rec.slcio )

SET_TESTS_PROPERTIES( t_c_rec_j_sim PROPERTIES PASS_REGULAR_EXPRESSION "'SomeClusters' and 'SomeTracks'.*to[ ]+100[ ]+events" )
SET_TESTS_PROPERTIES( t_j_rec_c_sim PROPERTIES PASS_REGULAR_EXPRESSION "Analyzed 100 events" )
SET_TESTS_PROPERTIES( t_c_ana_j2c_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
SET_TESTS_PROPERTIES( t_c_ana_c2j_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
# ------------------------------------------------------------------------------------------------------------------------------



# TODO runSIODump.sh and runrandomtests.sh


IF( BUILD_F77_TESTJOBS )

    ADD_DEPENDENCIES( tests f77 )
    
    # ------ fortran tests ------------------------------------------------------------
    ADD_TEST( t_f_sim "${EXECUTABLE_OUTPUT_PATH}/simjob_F" f_sim.slcio )
    ADD_TEST( t_f_ana_f_sim "${EXECUTABLE_OUTPUT_PATH}/anajob_F" f_sim.slcio )
    ADD_TEST( t_f_rec_f_sim "${EXECUTABLE_OUTPUT_PATH}/recjob_F" f_sim.slcio f_rec.slcio )
    ADD_TEST( t_f_ana_f_rec "${EXECUTABLE_OUTPUT_PATH}/anajob_F" f_rec.slcio )

    SET_TESTS_PROPERTIES( t_f_sim PROPERTIES PASS_REGULAR_EXPRESSION "created[ ]+10[ ]+runs with[ ]+100[ ]+events" )
    SET_TESTS_PROPERTIES( t_f_ana_f_sim PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    SET_TESTS_PROPERTIES( t_f_rec_f_sim PROPERTIES PASS_REGULAR_EXPRESSION "'SomeClusters' and 'SomeTracks'.*to[ ]+100[ ]+events" )
    SET_TESTS_PROPERTIES( t_f_ana_f_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    # ---------------------------------------------------------------------------------
 

    # ------ c++/fortran ana/sim mixed tests -----------------------------------------------------
    ADD_TEST( t_c_ana_f_sim "${EXECUTABLE_OUTPUT_PATH}/anajob" f_sim.slcio )
    ADD_TEST( t_c_ana_f_rec "${EXECUTABLE_OUTPUT_PATH}/anajob" f_rec.slcio )
    ADD_TEST( t_f_ana_c_sim "${EXECUTABLE_OUTPUT_PATH}/anajob_F" c_sim.slcio )
    ADD_TEST( t_f_ana_c_rec "${EXECUTABLE_OUTPUT_PATH}/anajob_F" c_rec.slcio )

    SET_TESTS_PROPERTIES( t_c_ana_f_sim PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    SET_TESTS_PROPERTIES( t_c_ana_f_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    SET_TESTS_PROPERTIES( t_f_ana_c_sim PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    SET_TESTS_PROPERTIES( t_f_ana_c_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    # ------------------------------------------------------------------------------------------------------------------------------

    # ------ java/fortran ana/sim mixed tests -----------------------------------------------------
    ADD_TEST( t_j_ana_f_sim ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runAnalysisJob.sh f_sim.slcio )
    ADD_TEST( t_j_ana_f_rec ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runAnalysisJob.sh f_rec.slcio )
    ADD_TEST( t_f_ana_j_sim "${EXECUTABLE_OUTPUT_PATH}/anajob_F" j_sim.slcio )
    ADD_TEST( t_f_ana_j_rec "${EXECUTABLE_OUTPUT_PATH}/anajob_F" j_rec.slcio )

    SET_TESTS_PROPERTIES( t_j_ana_f_sim PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    SET_TESTS_PROPERTIES( t_j_ana_f_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    SET_TESTS_PROPERTIES( t_f_ana_j_sim PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    SET_TESTS_PROPERTIES( t_f_ana_j_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    # ------------------------------------------------------------------------------------------------------------------------------


    
    # ------ c++/fortran sim/rec mixed tests -----------------------------------------------------
    #ADD_TEST( t_c_rec_f_sim "${EXECUTABLE_OUTPUT_PATH}/recjob" f_sim.slcio f2c_rec.slcio )
    #ADD_TEST( t_f_rec_c_sim "${EXECUTABLE_OUTPUT_PATH}/recjob_F" c_sim.slcio c2f_rec.slcio )
    #ADD_TEST( t_c_ana_f2c_rec "${EXECUTABLE_OUTPUT_PATH}/anajob" f2c_rec.slcio )
    #ADD_TEST( t_c_ana_c2f_rec "${EXECUTABLE_OUTPUT_PATH}/anajob" c2f_rec.slcio )

    #SET_TESTS_PROPERTIES( t_c_rec_f_sim PROPERTIES PASS_REGULAR_EXPRESSION "'SomeClusters' and 'SomeTracks'.*to[ ]+100[ ]+events" )
    #SET_TESTS_PROPERTIES( t_f_rec_c_sim PROPERTIES PASS_REGULAR_EXPRESSION "Analyzed 100 events" )
    #SET_TESTS_PROPERTIES( t_c_ana_f2c_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    #SET_TESTS_PROPERTIES( t_c_ana_c2f_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    # ------------------------------------------------------------------------------------------------------------------------------

ENDIF()


# ===== TESTS in src/cpp/src/TESTS =================
ADD_LCIO_CTEST( test_example ) 
ADD_LCIO_CTEST( test_calohit )
ADD_LCIO_CTEST( test_trackerhit )
ADD_LCIO_CTEST( test_trackerpulse )
ADD_LCIO_CTEST( test_randomaccess) 

FIND_PACKAGE( CLHEP REQUIRED) 
IF( CLHEP_FOUND )
ADD_LCIO_CTEST( test_fourvector) 
INCLUDE_DIRECTORIES( ${CLHEP_INCLUDE_DIRS} ) 
ENDIF( CLHEP_FOUND )
# ==================================================

