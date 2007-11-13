#################################
# cmake file for building LCIO
# @author Frank Gaede, DESY
# @author Jan Engels, DESY
#################################

INCLUDE_DIRECTORIES (
    "${LCIO_SOURCE_DIR}/sio/include"
    "${LCIO_SOURCE_DIR}/src/cpp/include"
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
  ./src/SIO/SIOUnpack.cc
  ./src/SIO/SIOVertexHandler.cc
  ./src/SIO/SIOWriter.cc
  ./src/SIO/SIOReader.cc
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
  ./src/UTIL/CellIDEncoder.cc
)

#------ require proper C++ -------
ADD_DEFINITIONS( -Wall -ansi -pedantic )
#----- need long long for int64 for now ------
#FIXME: should establish wether we are on a 32bit or 64 bit machine ....
ADD_DEFINITIONS( -Wno-long-long )

#---- special flags for cfortran code : --------
SET_SOURCE_FILES_PROPERTIES( 
  ${LCIO_CPPFORT_SRCS} PROPERTIES 
  COMPILE_FLAGS "-Df2cFortran -Wno-unused"
)

ADD_LIBRARY( lib_${PROJECT_NAME}F77 STATIC
  ${LCIO_CPPFORT_SRCS} 
)

SET_TARGET_PROPERTIES( lib_${PROJECT_NAME}F77 PROPERTIES
    # create *nix style library versions + symbolic links
    #VERSION ${${PROJECT_NAME}_VERSION}
    #SOVERSION ${${PROJECT_NAME}_SOVERSION}
    # allow creating static and shared libs without conflicts
    CLEAN_DIRECT_OUTPUT 1
    # avoid conflicts between library and binary target names
    OUTPUT_NAME lcioF77
)

ADD_CUSTOM_TARGET( lib DEPENDS lib_${PROJECT_NAME} lib_${PROJECT_NAME}F77 lib_SIO )

ADD_LIBRARY( lib_${PROJECT_NAME} 
  ${LCIO_IMPL_SRCS}
  ${LCIO_IOIMPL_SRCS}
  ${LCIO_UTIL_SRCS}
  ${LCIO_SIO_SRCS}
)

SET_TARGET_PROPERTIES( lib_${PROJECT_NAME} PROPERTIES
    # create *nix style library versions + symbolic links
    VERSION ${${PROJECT_NAME}_VERSION}
    SOVERSION ${${PROJECT_NAME}_SOVERSION}
    # allow creating static and shared libs without conflicts
    CLEAN_DIRECT_OUTPUT 1
    # avoid conflicts between library and binary target names
    OUTPUT_NAME lcio
)

TARGET_LINK_LIBRARIES( lib_LCIO lib_SIO )
TARGET_LINK_LIBRARIES( lib_LCIOF77 lib_LCIO )

# create symbolic bin target for calling targets bin_XXX
ADD_CUSTOM_TARGET( bin )

# ==== a macro for adding binaries from the EXAMPLE directory =====
MACRO( ADD_LCIO_BIN file )
  ADD_EXECUTABLE( bin_${file} ./src/EXAMPLE/${file}.cc )
  ADD_DEPENDENCIES( bin bin_${file} )
#  MESSAGE( STATUS " in ADD_LCIO_BIN : " ${file} " " ./src/EXAMPLE/${file}.cc )
  SET_TARGET_PROPERTIES( bin_${file} PROPERTIES OUTPUT_NAME ${file} )
  TARGET_LINK_LIBRARIES( bin_${file} lib_${PROJECT_NAME} readline ncurses)
  INSTALL( TARGETS bin_${file} DESTINATION bin )
ENDMACRO()
#===================================================================

ADD_LCIO_BIN( simjob ) 
ADD_LCIO_BIN( anajob ) 
ADD_LCIO_BIN( recjob ) 
ADD_LCIO_BIN( dumpevent ) 
ADD_LCIO_BIN( copyfix ) 
ADD_LCIO_BIN( stdhepjob ) 


IF( BUILD_LCIO_SHELL )
 ADD_LCIO_BIN( lsh ) 
 TARGET_LINK_LIBRARIES( bin_lsh readline ncurses)
ENDIF()

IF( BUILD_LCIO_TESTJOBS )
  ADD_LCIO_BIN( readcalibration ) 
  ADD_LCIO_BIN( testvtx ) 
  ADD_LCIO_BIN( lcrtrelation )

  # ctest jobs (call with 'make test')
  ADD_TEST( t_simjob "${EXECUTABLE_OUTPUT_PATH}/simjob" )
  ADD_TEST( t_recjob "${EXECUTABLE_OUTPUT_PATH}/recjob" simjob.slcio recjob.slcio )
  ADD_TEST( t_anajob "${EXECUTABLE_OUTPUT_PATH}/anajob" recjob.slcio )
  
  SET_TESTS_PROPERTIES( t_simjob PROPERTIES PASS_REGULAR_EXPRESSION
      "created  10 runs with  100 events" )
  SET_TESTS_PROPERTIES( t_recjob PROPERTIES PASS_REGULAR_EXPRESSION
      "added collection: 'SomeClusters' and 'SomeTracks' to   100 events and added one extra MCParticle to each event." )
  SET_TESTS_PROPERTIES( t_anajob PROPERTIES PASS_REGULAR_EXPRESSION
      "100 events read from files:" )
ENDIF()


#------------- install the library and the (public) header files -------

INSTALL( TARGETS lib_${PROJECT_NAME} DESTINATION lib PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE )

INSTALL( TARGETS lib_${PROJECT_NAME}F77 DESTINATION lib PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE )

INSTALL(DIRECTORY ${LCIO_SOURCE_DIR}/src/cpp/include/ DESTINATION include 
  PATTERN "*~" EXCLUDE
  #PATTERN "*empty_ignore.h" EXCLUDE
  PATTERN "*SIO*" EXCLUDE
  PATTERN "*CPPFORT*" EXCLUDE
  PATTERN "*CVS*" EXCLUDE)

