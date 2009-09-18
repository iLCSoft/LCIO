#include "UTIL/LCIOTypeInfo.h"


namespace UTIL{



  template<> const char* lctypename<EVENT::MCParticle>() { return EVENT::LCIO::MCPARTICLE ; }
  template<> const char* lctypename<EVENT::SimCalorimeterHit>() { return EVENT::LCIO::SIMCALORIMETERHIT ; }
  template<> const char* lctypename<EVENT::CalorimeterHit>() { return EVENT::LCIO::CALORIMETERHIT ; }
  template<> const char* lctypename<EVENT::RawCalorimeterHit>() { return EVENT::LCIO::RAWCALORIMETERHIT ; }
  template<> const char* lctypename<EVENT::SimTrackerHit>() { return EVENT::LCIO::SIMTRACKERHIT ; }
  template<> const char* lctypename<EVENT::TrackerRawData>() { return EVENT::LCIO::TRACKERRAWDATA ; }
  template<> const char* lctypename<EVENT::TrackerData>() { return EVENT::LCIO::TRACKERDATA ; }
  template<> const char* lctypename<EVENT::TrackerPulse>() { return EVENT::LCIO::TRACKERPULSE ; }
  template<> const char* lctypename<EVENT::TrackerHit>() { return EVENT::LCIO::TRACKERHIT ; }
  template<> const char* lctypename<EVENT::LCStrVec>() { return EVENT::LCIO::LCSTRVEC ; }
  template<> const char* lctypename<EVENT::LCFloatVec>() { return EVENT::LCIO::LCFLOATVEC ; }
  template<> const char* lctypename<EVENT::LCIntVec>() { return EVENT::LCIO::LCINTVEC ; }
  template<> const char* lctypename<EVENT::Track>() { return EVENT::LCIO::TRACK ; }
  template<> const char* lctypename<EVENT::Cluster>() { return EVENT::LCIO::CLUSTER ; }
  template<> const char* lctypename<EVENT::ReconstructedParticle>() { return EVENT::LCIO::RECONSTRUCTEDPARTICLE ; }
  template<> const char* lctypename<EVENT::LCRelation>() { return EVENT::LCIO::LCRELATION ; }
  template<> const char* lctypename<EVENT::LCGenericObject>() { return EVENT::LCIO::LCGENERICOBJECT ; }
  template<> const char* lctypename<EVENT::Vertex>() { return EVENT::LCIO::VERTEX ; }


  template<> const char* lctypename<EVENT::LCObject>(const EVENT::LCObject* o) { 
    
    if( dynamic_cast<const EVENT::MCParticle*>(o) != 0 ) return  lctypename<EVENT::MCParticle>() ;
    
    if( dynamic_cast<const EVENT::SimCalorimeterHit*>(o) != 0 ) return  lctypename<EVENT::SimCalorimeterHit>() ;
    if( dynamic_cast<const EVENT::CalorimeterHit*>(o) != 0 ) return   lctypename<EVENT::CalorimeterHit>() ;
    if( dynamic_cast<const EVENT::RawCalorimeterHit*>(o) != 0 ) return   lctypename<EVENT::RawCalorimeterHit>() ;
    if( dynamic_cast<const EVENT::SimTrackerHit*>(o) != 0 ) return   lctypename<EVENT::SimTrackerHit>() ;
    if( dynamic_cast<const EVENT::TrackerRawData*>(o) != 0 ) return   lctypename<EVENT::TrackerRawData>() ;
    if( dynamic_cast<const EVENT::TrackerData*>(o) != 0 ) return   lctypename<EVENT::TrackerData>() ;
    if( dynamic_cast<const EVENT::TrackerPulse*>(o) != 0 ) return   lctypename<EVENT::TrackerPulse>() ;
    if( dynamic_cast<const EVENT::TrackerHit*>(o) != 0 ) return   lctypename<EVENT::TrackerHit>() ;
    if( dynamic_cast<const EVENT::LCStrVec*>(o) != 0 ) return   lctypename<EVENT::LCStrVec>() ;
    if( dynamic_cast<const EVENT::LCFloatVec*>(o) != 0 ) return   lctypename<EVENT::LCFloatVec>() ;
    if( dynamic_cast<const EVENT::LCIntVec*>(o) != 0 ) return   lctypename<EVENT::LCIntVec>() ;
    if( dynamic_cast<const EVENT::Track*>(o) != 0 ) return   lctypename<EVENT::Track>() ;
    if( dynamic_cast<const EVENT::Cluster*>(o) != 0 ) return   lctypename<EVENT::Cluster>() ;
    if( dynamic_cast<const EVENT::ReconstructedParticle*>(o) != 0 ) return   lctypename<EVENT::ReconstructedParticle>() ;
    if( dynamic_cast<const EVENT::LCRelation*>(o) != 0 ) return   lctypename<EVENT::LCRelation>() ;
    if( dynamic_cast<const EVENT::LCGenericObject*>(o) != 0 ) return   lctypename<EVENT::LCGenericObject>() ;
    if( dynamic_cast<const EVENT::Vertex*>(o) != 0 ) return   lctypename<EVENT::Vertex>() ;
    
    return "UNKNOWN" ;
  }



}
