#include "EVENT/LCIO.h"

namespace EVENT{

  // the standard requires a definition of static const ints 
  //  ( might be needed if values are used as reference arguments ) 
  const int LCIO::MAJORVERSION ;
  const int LCIO::MINORVERSION  ;
  const int LCIO::CHBIT_LONG ;
  const int LCIO::CHBIT_BARREL ;
  const int LCIO::CHBIT_ID1 ;
  const int LCIO::CHBIT_PDG  ;
  const int LCIO::CHBIT_STEP  ;
  const int LCIO::RCHBIT_LONG   ;
  const int LCIO::RCHBIT_BARREL ;
  const int LCIO::RCHBIT_ID1    ;
  const int LCIO::RCHBIT_NO_PTR ;
  const int LCIO::RCHBIT_TIME ;
  const int LCIO::RCHBIT_ENERGY_ERROR  ;
  const int LCIO::TRAWBIT_ID1 ;
  const int LCIO::TRAWBIT_CM  ;
  const int LCIO::RTHBIT_ID1  ;
  const int LCIO::RTHPBIT_ID1  ;
  const int LCIO::RTHZBIT_ID1  ;
  const int LCIO::THBIT_BARREL   ;
  const int LCIO::THBIT_MOMENTUM ;
  const int LCIO::THBIT_ID1  ;
  const int LCIO::TRBIT_HITS ;
  const int LCIO::CLBIT_HITS ;
  const int LCIO::TPCBIT_RAW ;
  const int LCIO::TPCBIT_NO_PTR ;
  const int LCIO::LCREL_WEIGHTED ;
  const int LCIO::GOBIT_FIXED ;
  const int LCIO::READ_ONLY ;
  const int LCIO::UPDATE   ;
  const int LCIO::ERROR   ;
  const int LCIO::SUCCESS ;
  const int LCIO::WRITE_NEW ;
  const int LCIO::WRITE_APPEND ;



  const char* LCIO::LCEVENT = "LCEvent" ;
  const char* LCIO::LCCOLLECTION = "LCCollection" ;
  const char* LCIO::LCRUNHEADER = "LCRunHeader" ;
  const char* LCIO::MCPARTICLE = "MCParticle" ;
  const char* LCIO::SIMCALORIMETERHIT = "SimCalorimeterHit" ;
  const char* LCIO::RAWCALORIMETERHIT = "RawCalorimeterHit" ;
  const char* LCIO::CALORIMETERHIT = "CalorimeterHit" ;
  const char* LCIO::SIMTRACKERHIT = "SimTrackerHit" ;
  const char* LCIO::TPCHIT = "TPCHit" ;
  const char* LCIO::TRACKERRAWDATA = "TrackerRawData" ;
  const char* LCIO::TRACKERDATA = "TrackerData" ;
  const char* LCIO::TRACKERPULSE = "TrackerPulse" ;
  const char* LCIO::TRACKERHIT = "TrackerHit" ;
  const char* LCIO::TRACKERHITPLANE = "TrackerHitPlane" ;
  const char* LCIO::TRACKERHITZCYLINDER = "TrackerHitZCylinder" ;
  const char* LCIO::LCSTRVEC = "LCStrVec" ;
  const char* LCIO::LCFLOATVEC = "LCFloatVec" ;
  const char* LCIO::LCINTVEC = "LCIntVec" ;
  const char* LCIO::TRACK= "Track" ; 
  const char* LCIO::TRACKSTATE= "TrackState" ; 
  const char* LCIO::CLUSTER = "Cluster" ; 
  const char* LCIO::RECONSTRUCTEDPARTICLE = "ReconstructedParticle" ; 
  const char* LCIO::LCRELATION = "LCRelation" ; 
  const char* LCIO::LCGENERICOBJECT = "LCGenericObject" ; 
  const char* LCIO::PARTICLEID = "ParticleID" ; 
  const char* LCIO::VERTEX = "Vertex" ; 
  
  const char* LCIO::CellIDEncoding = "CellIDEncoding" ; 
}
