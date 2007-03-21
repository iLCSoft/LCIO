
INCLUDE_DIRECTORIES (
	${LCIO_SOURCE_DIR}/sio/include
	${LCIO_SOURCE_DIR}/src/cpp/include
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

#---- special flags for cfortran code : --------
SET_SOURCE_FILES_PROPERTIES( 
  ${LCIO_CPPFORT_SRCS} PROPERTIES 
  COMPILE_FLAGS "-Df2cFortran -Wno-unused"
  )

ADD_LIBRARY(lcio 
  ${LCIO_IMPL_SRCS} 
  ${LCIO_IOIMPL_SRCS}  
  ${LCIO_CPPFORT_SRCS} 
  ${LCIO_UTIL_SRCS} 
  ${LCIO_SIO_SRCS} 
  )

TARGET_LINK_LIBRARIES(lcio sio)

# ==== a macro for adding binaries from the EXAMPLE directory =====
MACRO(ADD_LCIO_BIN file)
  ADD_EXECUTABLE( ${file} ./src/EXAMPLE/${file}.cc)
#  MESSAGE( STATUS " in ADD_LCIO_BIN : " ${file} " " ./src/EXAMPLE/${file}.cc )
  TARGET_LINK_LIBRARIES(${file} lcio )
  INSTALL(TARGETS ${file} DESTINATION bin)
ENDMACRO(ADD_LCIO_BIN)
#===================================================================

ADD_LCIO_BIN( simjob ) 
ADD_LCIO_BIN( anajob ) 
ADD_LCIO_BIN( recjob ) 
ADD_LCIO_BIN( dumpevent ) 
ADD_LCIO_BIN( copyfix ) 

OPTION(BUILD_LCIO_TESTJOBS FALSE)

IF( BUILD_LCIO_TESTJOBS)
  ADD_LCIO_BIN( readcalibration ) 
  ADD_LCIO_BIN( stdhepjob ) 
  ADD_LCIO_BIN( testvtx ) 
  ADD_LCIO_BIN( lcrtrelation ) 
ENDIF( BUILD_LCIO_TESTJOBS)


#------------- install the library and the (public) header files -------

INSTALL(TARGETS lcio DESTINATION lib)

INSTALL(DIRECTORY ${LCIO_SOURCE_DIR}/src/cpp/include/ DESTINATION include 
  PATTERN "*~" EXCLUDE
  PATTERN "*empty_ignore.h" EXCLUDE
  PATTERN "*SIO*" EXCLUDE
  PATTERN "*CPPFORT*" EXCLUDE
  PATTERN "*CVS*" EXCLUDE)


