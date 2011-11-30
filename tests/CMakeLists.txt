####################################################
# cmake file for compiling and running lcio tests
# J. Engels - Desy IT
####################################################

##############################################################################
##### TESTS
##############################################################################



# global target for building tests
ADD_CUSTOM_TARGET( tests )

# lcio.jar needed for java tests
ADD_DEPENDENCIES( tests jar )

# simjob/anajob/recjob needed for c++ tests
ADD_DEPENDENCIES( tests examples )

#ADD_TEST( t_simjob "${EXECUTABLE_OUTPUT_PATH}/simjob" )


# ==== ctest stuff (call with ctest or 'make test') ==========================


# ------ c++ tests -----------------------------------------------------------
ADD_TEST( t_c_sim "${EXECUTABLE_OUTPUT_PATH}/simjob" c_sim.slcio )
ADD_TEST( t_c_ana_c_sim "${EXECUTABLE_OUTPUT_PATH}/anajob" c_sim.slcio )
ADD_TEST( t_c_rec_c_sim "${EXECUTABLE_OUTPUT_PATH}/recjob" c_sim.slcio c_rec.slcio )
ADD_TEST( t_c_ana_c_rec "${EXECUTABLE_OUTPUT_PATH}/anajob" c_rec.slcio )

SET_TESTS_PROPERTIES( t_c_sim PROPERTIES PASS_REGULAR_EXPRESSION "created[ ]+10[ ]+runs with[ ]+100[ ]+events" )
SET_TESTS_PROPERTIES( t_c_ana_c_sim PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
SET_TESTS_PROPERTIES( t_c_rec_c_sim PROPERTIES PASS_REGULAR_EXPRESSION "'SomeClusters' and 'SomeTracks'.*to[ ]+100[ ]+events" )
SET_TESTS_PROPERTIES( t_c_ana_c_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
# ----------------------------------------------------------------------------

# ------ java tests ----------------------------------------------------------
ADD_TEST( t_j_sim ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runSimJob.sh j_sim.slcio )
ADD_TEST( t_j_ana_j_sim ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runAnalysisJob.sh j_sim.slcio )
ADD_TEST( t_j_rec_j_sim ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runRecJob.sh j_sim.slcio j_rec.slcio )
ADD_TEST( t_j_ana_j_rec ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runAnalysisJob.sh j_rec.slcio )

SET_TESTS_PROPERTIES( t_j_sim PROPERTIES PASS_REGULAR_EXPRESSION "created[ ]+10[ ]+runs with[ ]+100[ ]+events" )
SET_TESTS_PROPERTIES( t_j_ana_j_sim PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
SET_TESTS_PROPERTIES( t_j_rec_j_sim PROPERTIES PASS_REGULAR_EXPRESSION "Analyzed[ ]+100[ ]+events" )
SET_TESTS_PROPERTIES( t_j_ana_j_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
# ----------------------------------------------------------------------------


# ------ c++/java ana/sim mixed tests ----------------------------------------
ADD_TEST( t_c_ana_j_sim "${EXECUTABLE_OUTPUT_PATH}/anajob" j_sim.slcio )
ADD_TEST( t_c_ana_j_rec "${EXECUTABLE_OUTPUT_PATH}/anajob" j_rec.slcio )
ADD_TEST( t_j_ana_c_sim ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runAnalysisJob.sh c_sim.slcio )
ADD_TEST( t_j_ana_c_rec ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runAnalysisJob.sh c_rec.slcio )

SET_TESTS_PROPERTIES( t_c_ana_j_sim PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
SET_TESTS_PROPERTIES( t_c_ana_j_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
SET_TESTS_PROPERTIES( t_j_ana_c_sim PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
SET_TESTS_PROPERTIES( t_j_ana_c_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
# ----------------------------------------------------------------------------



# ------ c++/java sim/rec mixed tests ----------------------------------------
ADD_TEST( t_c_rec_j_sim "${EXECUTABLE_OUTPUT_PATH}/recjob" j_sim.slcio j2c_rec.slcio )
ADD_TEST( t_j_rec_c_sim ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runRecJob.sh c_sim.slcio c2j_rec.slcio )
ADD_TEST( t_c_ana_j2c_rec "${EXECUTABLE_OUTPUT_PATH}/anajob" j2c_rec.slcio )
ADD_TEST( t_c_ana_c2j_rec "${EXECUTABLE_OUTPUT_PATH}/anajob" c2j_rec.slcio )

SET_TESTS_PROPERTIES( t_c_rec_j_sim PROPERTIES PASS_REGULAR_EXPRESSION "'SomeClusters' and 'SomeTracks'.*to[ ]+100[ ]+events" )
SET_TESTS_PROPERTIES( t_j_rec_c_sim PROPERTIES PASS_REGULAR_EXPRESSION "Analyzed 100 events" )
SET_TESTS_PROPERTIES( t_c_ana_j2c_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
SET_TESTS_PROPERTIES( t_c_ana_c2j_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
# ----------------------------------------------------------------------------


# ------ java sio dump tests -------------------------------------------------
ADD_TEST( t_j_sio_c_sim ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runSIODump.sh ${PROJECT_SOURCE_DIR}/doc/lcio.xml c_sim.slcio )
ADD_TEST( t_j_sio_c_rec ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runSIODump.sh ${PROJECT_SOURCE_DIR}/doc/lcio.xml c_rec.slcio )
ADD_TEST( t_j_sio_j_sim ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runSIODump.sh ${PROJECT_SOURCE_DIR}/doc/lcio.xml j_sim.slcio )
ADD_TEST( t_j_sio_j_rec ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runSIODump.sh ${PROJECT_SOURCE_DIR}/doc/lcio.xml j_rec.slcio )
ADD_TEST( t_j_sio_j2c_rec ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runSIODump.sh ${PROJECT_SOURCE_DIR}/doc/lcio.xml j2c_rec.slcio )
ADD_TEST( t_j_sio_c2j_rec ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runSIODump.sh ${PROJECT_SOURCE_DIR}/doc/lcio.xml c2j_rec.slcio )
SET_TESTS_PROPERTIES( t_j_sio_c_sim PROPERTIES PASS_REGULAR_EXPRESSION "Read 231 records with 0 warnings" )
SET_TESTS_PROPERTIES( t_j_sio_c_rec PROPERTIES PASS_REGULAR_EXPRESSION "Read 213 records with 0 warnings" )
SET_TESTS_PROPERTIES( t_j_sio_j_sim PROPERTIES PASS_REGULAR_EXPRESSION "Read 213 records with 0 warnings" )
SET_TESTS_PROPERTIES( t_j_sio_j_rec PROPERTIES PASS_REGULAR_EXPRESSION "Read 213 records with 0 warnings" )
SET_TESTS_PROPERTIES( t_j_sio_j2c_rec PROPERTIES PASS_REGULAR_EXPRESSION "Read 213 records with 0 warnings" )
SET_TESTS_PROPERTIES( t_j_sio_c2j_rec PROPERTIES PASS_REGULAR_EXPRESSION "Read 213 records with 0 warnings" )
# ----------------------------------------------------------------------------


IF( BUILD_F77_TESTJOBS )

    ADD_DEPENDENCIES( tests f77 )
    
    # ------ fortran tests ---------------------------------------------------
    ADD_TEST( t_f_sim "${EXECUTABLE_OUTPUT_PATH}/simjob_F" f_sim.slcio )
    ADD_TEST( t_f_ana_f_sim "${EXECUTABLE_OUTPUT_PATH}/anajob_F" f_sim.slcio )
    ADD_TEST( t_f_rec_f_sim "${EXECUTABLE_OUTPUT_PATH}/recjob_F" f_sim.slcio f_rec.slcio )
    ADD_TEST( t_f_ana_f_rec "${EXECUTABLE_OUTPUT_PATH}/anajob_F" f_rec.slcio )

    SET_TESTS_PROPERTIES( t_f_sim PROPERTIES PASS_REGULAR_EXPRESSION "created[ ]+10[ ]+runs with[ ]+100[ ]+events" )
    SET_TESTS_PROPERTIES( t_f_ana_f_sim PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    SET_TESTS_PROPERTIES( t_f_rec_f_sim PROPERTIES PASS_REGULAR_EXPRESSION "'SomeClusters' and 'SomeTracks'.*to[ ]+100[ ]+events" )
    SET_TESTS_PROPERTIES( t_f_ana_f_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    # ------------------------------------------------------------------------
 

    # ------ c++/fortran ana/sim mixed tests ---------------------------------
    ADD_TEST( t_c_ana_f_sim "${EXECUTABLE_OUTPUT_PATH}/anajob" f_sim.slcio )
    ADD_TEST( t_c_ana_f_rec "${EXECUTABLE_OUTPUT_PATH}/anajob" f_rec.slcio )
    ADD_TEST( t_f_ana_c_sim "${EXECUTABLE_OUTPUT_PATH}/anajob_F" c_sim.slcio )
    ADD_TEST( t_f_ana_c_rec "${EXECUTABLE_OUTPUT_PATH}/anajob_F" c_rec.slcio )

    SET_TESTS_PROPERTIES( t_c_ana_f_sim PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    SET_TESTS_PROPERTIES( t_c_ana_f_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    SET_TESTS_PROPERTIES( t_f_ana_c_sim PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    SET_TESTS_PROPERTIES( t_f_ana_c_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    # ------------------------------------------------------------------------

    # ------ java/fortran ana/sim mixed tests --------------------------------
    ADD_TEST( t_j_ana_f_sim ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runAnalysisJob.sh f_sim.slcio )
    ADD_TEST( t_j_ana_f_rec ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runAnalysisJob.sh f_rec.slcio )
    ADD_TEST( t_f_ana_j_sim "${EXECUTABLE_OUTPUT_PATH}/anajob_F" j_sim.slcio )
    ADD_TEST( t_f_ana_j_rec "${EXECUTABLE_OUTPUT_PATH}/anajob_F" j_rec.slcio )

    SET_TESTS_PROPERTIES( t_j_ana_f_sim PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    SET_TESTS_PROPERTIES( t_j_ana_f_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    SET_TESTS_PROPERTIES( t_f_ana_j_sim PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    SET_TESTS_PROPERTIES( t_f_ana_j_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    # ------------------------------------------------------------------------


    
    # ------ c++/fortran sim/rec mixed tests ---------------------------------
    #ADD_TEST( t_c_rec_f_sim "${EXECUTABLE_OUTPUT_PATH}/recjob" f_sim.slcio f2c_rec.slcio )
    #ADD_TEST( t_f_rec_c_sim "${EXECUTABLE_OUTPUT_PATH}/recjob_F" c_sim.slcio c2f_rec.slcio )
    #ADD_TEST( t_c_ana_f2c_rec "${EXECUTABLE_OUTPUT_PATH}/anajob" f2c_rec.slcio )
    #ADD_TEST( t_c_ana_c2f_rec "${EXECUTABLE_OUTPUT_PATH}/anajob" c2f_rec.slcio )

    #SET_TESTS_PROPERTIES( t_c_rec_f_sim PROPERTIES PASS_REGULAR_EXPRESSION "'SomeClusters' and 'SomeTracks'.*to[ ]+100[ ]+events" )
    #SET_TESTS_PROPERTIES( t_f_rec_c_sim PROPERTIES PASS_REGULAR_EXPRESSION "Analyzed 100 events" )
    #SET_TESTS_PROPERTIES( t_c_ana_f2c_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    #SET_TESTS_PROPERTIES( t_c_ana_c2f_rec PROPERTIES PASS_REGULAR_EXPRESSION "100[ ]+events read from files" )
    # ------------------------------------------------------------------------

    ADD_TEST( t_j_sio_f_sim ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runSIODump.sh ${PROJECT_SOURCE_DIR}/doc/lcio.xml f_sim.slcio )
    ADD_TEST( t_j_sio_f_rec ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runSIODump.sh ${PROJECT_SOURCE_DIR}/doc/lcio.xml f_rec.slcio )
    SET_TESTS_PROPERTIES( t_j_sio_f_sim PROPERTIES PASS_REGULAR_EXPRESSION "Read 231 records with 0 warnings" )
    SET_TESTS_PROPERTIES( t_j_sio_f_rec PROPERTIES PASS_REGULAR_EXPRESSION "Read 213 records with 0 warnings" )

ENDIF()


IF( BUILD_ROOTDICT )

    ADD_DEPENDENCIES( tests lcioDict )

    #SET( ROOTDICT_EXAMPLES_DIR "${PROJECT_SOURCE_DIR}/examples/cpp/rootDict" )

    # copy directory to keep source tree clean
    #EXECUTE_PROCESS( COMMAND "${CMAKE_COMMAND}" -E copy_directory
    #    "${ROOTDICT_EXAMPLES_DIR}"
    #    "${PROJECT_BINARY_DIR}/rootDictTests"
    #)
    #ADD_TEST(
    #    NAME t_root_anajob 
    #    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/rootDictTests"
    #    COMMAND ${SH} "${LCIO_ENV_INIT}" ${ROOT_EXECUTABLE} -l -b -q anajobTest.C
    #)

    IF(APPLE)
        SET( LD_LIBRARY_PATH_VAR DYLD_LIBRARY_PATH )
        # hack needed for mac, creates symlinks:
        # liblcio.so -> liblcio.dylib
        # liblcioDict.so -> liblcioDict.dylib
        # gSystem->Load( liblcio.dylib ) doesn't work (only looks for .so extension)
        EXECUTE_PROCESS(
            COMMAND "${CMAKE_COMMAND}" -E create_symlink liblcio.dylib liblcio.so 
            COMMAND "${CMAKE_COMMAND}" -E create_symlink liblcioDict.dylib liblcioDict.so 
            WORKING_DIRECTORY "${LIBRARY_OUTPUT_PATH}"
        )
    ELSE()
        SET( LD_LIBRARY_PATH_VAR LD_LIBRARY_PATH )
    ENDIF()
    SET( LD_LIBRARY_PATH_CONTENTS $ENV{${LD_LIBRARY_PATH_VAR}} )
    #MESSAGE( STATUS "LD_LIBRARY_PATH_CONTENTS: ${LD_LIBRARY_PATH_CONTENTS}" )

    # need to create a wrapper which sets LD_LIBRARY_PATH so that root macros
    # are able to find lcio libraries in the build directory. Otherwise
    # 'make test' would only work after calling 'make install'
    SET( ROOT_WRAPPER ${LD_LIBRARY_PATH_VAR}=${LIBRARY_OUTPUT_PATH}:${LD_LIBRARY_PATH_CONTENTS} ${ROOT_EXECUTABLE} )

    ADD_TEST( t_root_anajob ${SH} "${LCIO_ENV_INIT}" ${ROOT_WRAPPER} -l -b -q ${PROJECT_SOURCE_DIR}/tests/rootDictTests/anajobTest.C )
    ADD_TEST( t_root_writeTree ${SH} "${LCIO_ENV_INIT}" ${ROOT_WRAPPER} -l -b -q ${PROJECT_SOURCE_DIR}/tests/rootDictTests/writeTreeTest.C )
    ADD_TEST( t_root_writeEventTree ${SH} "${LCIO_ENV_INIT}" ${ROOT_WRAPPER} -l -b -q ${PROJECT_SOURCE_DIR}/tests/rootDictTests/writeEventTreeTest.C )
    ADD_TEST( t_root_readEventTree ${SH} "${LCIO_ENV_INIT}" ${ROOT_WRAPPER} -l -b -q ${PROJECT_SOURCE_DIR}/tests/rootDictTests/readEventTreeTest.C )

    SET_TESTS_PROPERTIES( t_root_anajob PROPERTIES PASS_REGULAR_EXPRESSION "50 events read from file" )
    SET_TESTS_PROPERTIES( t_root_writeTree PROPERTIES PASS_REGULAR_EXPRESSION "50 events read from file" )
    SET_TESTS_PROPERTIES( t_root_writeEventTree PROPERTIES PASS_REGULAR_EXPRESSION "50 events read from file" )
    SET_TESTS_PROPERTIES( t_root_readEventTree PROPERTIES PASS_REGULAR_EXPRESSION "50 events read from file" )

ENDIF()


# -------- compiler definitions ----------------------------------------------
#ADD_DEFINITIONS( "-Wall -ansi -pedantic" )
#ADD_DEFINITIONS( "-Wno-long-long -fno-strict-aliasing" )


# -------- include directories -----------------------------------------------
INCLUDE_DIRECTORIES( BEFORE "${LCIO_AID_HEADERS_OUTPUT_DIR}" )
INCLUDE_DIRECTORIES( BEFORE "${LCIO_CXX_HEADERS_DIR}" )





# ==== add tests from the TESTS directory ====================================
MACRO( ADD_LCIO_TEST file )
  IF( BUILD_TESTING )
    ADD_EXECUTABLE( ${file} ../src/cpp/src/TESTS/${file}.cc )
  ELSE()
    ADD_EXECUTABLE( ${file} EXCLUDE_FROM_ALL ../src/cpp/src/TESTS/${file}.cc )
  ENDIF()
  ADD_DEPENDENCIES( tests ${file} )
  TARGET_LINK_LIBRARIES( ${file} lcio )
  ADD_TEST( t_${file} "${EXECUTABLE_OUTPUT_PATH}/${file}" )
  SET_TESTS_PROPERTIES( t_${file} PROPERTIES PASS_REGULAR_EXPRESSION "TEST_PASSED" )
ENDMACRO()




# ===== TESTS in src/cpp/src/TESTS ===========================================

ADD_LCIO_TEST( test_example ) 
ADD_LCIO_TEST( test_calohit )
ADD_LCIO_TEST( test_cluster )
ADD_LCIO_TEST( test_tracks )
ADD_LCIO_TEST( test_trackstate )
ADD_LCIO_TEST( test_trackerhit )
ADD_LCIO_TEST( test_trackerhitplane )
ADD_LCIO_TEST( test_trackerhitzcylinder )
ADD_LCIO_TEST( test_trackerpulse )
ADD_LCIO_TEST( test_randomaccess )  # needs output from t_c_sim

ADD_TEST( t_j_sio_calohit ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runSIODump.sh ${PROJECT_SOURCE_DIR}/doc/lcio.xml calohit.slcio )
SET_TESTS_PROPERTIES( t_j_sio_calohit PROPERTIES PASS_REGULAR_EXPRESSION "Read 23 records with 0 warnings" )

ADD_TEST( t_j_sio_cluster ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runSIODump.sh ${PROJECT_SOURCE_DIR}/doc/lcio.xml cluster.slcio )
SET_TESTS_PROPERTIES( t_j_sio_cluster PROPERTIES PASS_REGULAR_EXPRESSION "Read 9 records with 0 warnings" )

ADD_TEST( t_j_sio_tracks ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runSIODump.sh ${PROJECT_SOURCE_DIR}/doc/lcio.xml tracks.slcio )
SET_TESTS_PROPERTIES( t_j_sio_tracks PROPERTIES PASS_REGULAR_EXPRESSION "Read 9 records with 0 warnings" )

ADD_TEST( t_j_sio_trackerhits ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runSIODump.sh ${PROJECT_SOURCE_DIR}/doc/lcio.xml trackerhits.slcio )
SET_TESTS_PROPERTIES( t_j_sio_trackerhits PROPERTIES PASS_REGULAR_EXPRESSION "Read 23 records with 0 warnings" )

ADD_TEST( t_j_sio_trackerpulses ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runSIODump.sh ${PROJECT_SOURCE_DIR}/doc/lcio.xml trackerpulses.slcio )
SET_TESTS_PROPERTIES( t_j_sio_trackerpulses PROPERTIES PASS_REGULAR_EXPRESSION "Read 23 records with 0 warnings" )

ADD_TEST( t_j_sio_trackerhitplane ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runSIODump.sh ${PROJECT_SOURCE_DIR}/doc/lcio.xml trackerhitplane.slcio )
SET_TESTS_PROPERTIES( t_j_sio_trackerhitplane PROPERTIES PASS_REGULAR_EXPRESSION "Read 9 records with 0 warnings" )

ADD_TEST( t_j_sio_trackerhitzcylinder ${SH} "${LCIO_ENV_INIT}" ${PROJECT_SOURCE_DIR}/bin/runSIODump.sh ${PROJECT_SOURCE_DIR}/doc/lcio.xml trackerhitzcylinder.slcio )
SET_TESTS_PROPERTIES( t_j_sio_trackerhitzcylinder PROPERTIES PASS_REGULAR_EXPRESSION "Read 9 records with 0 warnings" )


FIND_PACKAGE( CLHEP QUIET )
IF( CLHEP_FOUND )
    INCLUDE_DIRECTORIES( ${CLHEP_INCLUDE_DIRS} ) 
    ADD_LCIO_TEST( test_fourvector) 
ELSE()
    MESSAGE( STATUS "cannot build test_fourvector: clhep not found" )
ENDIF( CLHEP_FOUND )


# ==== add some tests for checking number of hits in lcio collections ========
ADD_TEST( NAME t_c_check_colhit_args1 COMMAND bin_lcio_check_colhits )
SET_TESTS_PROPERTIES( t_c_check_colhit_args1 PROPERTIES PASS_REGULAR_EXPRESSION "error: wrong number of arguments" )
ADD_TEST( NAME t_c_check_colhit_args2 COMMAND bin_lcio_check_colhits MCParticle )
SET_TESTS_PROPERTIES( t_c_check_colhit_args2 PROPERTIES PASS_REGULAR_EXPRESSION "error: wrong number of arguments" )

ADD_TEST( NAME t_c_check_colhit_noopts COMMAND bin_lcio_check_colhits MCParticle c_sim.slcio )
SET_TESTS_PROPERTIES( t_c_check_colhit_noopts PROPERTIES PASS_REGULAR_EXPRESSION "found 10100 MCParticle hits in 100 events" )

# one option at a time
ADD_TEST( NAME t_c_check_colhit_minhits1 COMMAND bin_lcio_check_colhits --minhits 101 MCParticle c_sim.slcio )
ADD_TEST( NAME t_c_check_colhit_minhits2 COMMAND bin_lcio_check_colhits --minhits 102 MCParticle c_sim.slcio )
SET_TESTS_PROPERTIES( t_c_check_colhit_minhits2 PROPERTIES WILL_FAIL TRUE )

ADD_TEST( NAME t_c_check_colhit_maxhits1 COMMAND bin_lcio_check_colhits --maxhits 101 MCParticle c_sim.slcio )
ADD_TEST( NAME t_c_check_colhit_maxhits2 COMMAND bin_lcio_check_colhits --maxhits 100 MCParticle c_sim.slcio )
SET_TESTS_PROPERTIES( t_c_check_colhit_maxhits2 PROPERTIES WILL_FAIL TRUE )

ADD_TEST( NAME t_c_check_colhit_exphits1 COMMAND bin_lcio_check_colhits --exphits 101 MCParticle c_sim.slcio )
ADD_TEST( NAME t_c_check_colhit_exphits2 COMMAND bin_lcio_check_colhits --exphits 100 MCParticle c_sim.slcio )
SET_TESTS_PROPERTIES( t_c_check_colhit_exphits2 PROPERTIES WILL_FAIL TRUE )

# more options at once
ADD_TEST( NAME t_c_check_colhit_abshiterror1 COMMAND bin_lcio_check_colhits --exphits 100 --abshiterror 2 MCParticle c_sim.slcio c_rec.slcio )
ADD_TEST( NAME t_c_check_colhit_abshiterror2 COMMAND bin_lcio_check_colhits --exphits 100 --abshiterror 1 MCParticle c_sim.slcio c_rec.slcio )
SET_TESTS_PROPERTIES( t_c_check_colhit_abshiterror2 PROPERTIES WILL_FAIL TRUE )

ADD_TEST( NAME t_c_check_colhit_relhiterror1 COMMAND bin_lcio_check_colhits --exphits 100 --relhiterror .02 MCParticle c_sim.slcio c_rec.slcio )
ADD_TEST( NAME t_c_check_colhit_relhiterror2 COMMAND bin_lcio_check_colhits --exphits 100 --relhiterror .0001 MCParticle c_sim.slcio c_rec.slcio )
SET_TESTS_PROPERTIES( t_c_check_colhit_relhiterror2 PROPERTIES WILL_FAIL TRUE )

ADD_TEST( NAME t_c_check_colhit_absevterror1 COMMAND bin_lcio_check_colhits --exphits 101 --absevterror 100 MCParticle c_sim.slcio c_rec.slcio )
ADD_TEST( NAME t_c_check_colhit_absevterror2 COMMAND bin_lcio_check_colhits --exphits 101 --absevterror 99 MCParticle c_sim.slcio c_rec.slcio )
SET_TESTS_PROPERTIES( t_c_check_colhit_absevterror2 PROPERTIES WILL_FAIL TRUE )

ADD_TEST( NAME t_c_check_colhit_relevterror1 COMMAND bin_lcio_check_colhits --exphits 101 --relevterror .5 MCParticle c_sim.slcio c_rec.slcio )
ADD_TEST( NAME t_c_check_colhit_relevterror2 COMMAND bin_lcio_check_colhits --exphits 101 --relevterror .49 MCParticle c_sim.slcio c_rec.slcio )
SET_TESTS_PROPERTIES( t_c_check_colhit_relevterror2 PROPERTIES WILL_FAIL TRUE )

ADD_TEST( NAME t_c_check_colhit_maxevents1 COMMAND bin_lcio_check_colhits --maxevents 150 -m101 -M102 MCParticle c_sim.slcio c_rec.slcio )
SET_TESTS_PROPERTIES( t_c_check_colhit_maxevents1 PROPERTIES PASS_REGULAR_EXPRESSION "found 15200 MCParticle hits in 150 events" )

ADD_TEST( NAME t_c_check_colhit_maxevents2 COMMAND bin_lcio_check_colhits -x 101 -s 50 -n 50 MCParticle c_sim.slcio c_rec.slcio )
ADD_TEST( NAME t_c_check_colhit_maxevents3 COMMAND bin_lcio_check_colhits -x 102 -s 98 -n 10 --absevterror 2 MCParticle c_sim.slcio c_rec.slcio )

# ============================================================================

