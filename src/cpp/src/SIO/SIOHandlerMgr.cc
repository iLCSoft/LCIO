#include "SIO/SIOHandlerMgr.h"

#include "SIO/SIOSimCalHitHandler.h" 
#include "SIO/SIORawCalHitHandler.h" 
#include "SIO/SIOCalHitHandler.h" 
#include "SIO/SIOSimTrackHitHandler.h" 
#include "SIO/SIOTrackHandler.h" 
#include "SIO/SIOClusterHandler.h" 
#include "SIO/SIOTPCHitHandler.h" 
#include "SIO/SIOTrackerHitHandler.h" 
#include "SIO/SIOTrackerHitPlaneHandler.h" 
#include "SIO/SIOTrackerHitZCylinderHandler.h" 
#include "SIO/SIOParticleHandler.h" 
#include "SIO/SIOStrVecHandler.h"
#include "SIO/SIOFloatVecHandler.h" 
#include "SIO/SIOIntVecHandler.h" 
#include "SIO/SIOReconstructedParticleHandler.h" 
#include "SIO/SIOLCRelationHandler.h" 
#include "SIO/SIOLCGenericObjectHandler.h" 
#include "SIO/SIOTPCHitHandler.h" 
#include "SIO/SIOTrackerRawDataHandler.h" 
#include "SIO/SIOTrackerDataHandler.h" 
#include "SIO/SIOTrackerPulseHandler.h" 
#include "SIO/SIOVertexHandler.h"

#include "EVENT/LCIO.h"

using namespace EVENT ;

namespace SIO {
  
  SIOHandlerMgr::SIOHandlerMgr(){
    
    // add instances for all types to the map
    _map[ LCIO::MCPARTICLE     ] = new SIOParticleHandler ;
    _map[ LCIO::SIMCALORIMETERHIT ] = new SIOSimCalHitHandler ;
    _map[ LCIO::RAWCALORIMETERHIT ] = new SIORawCalHitHandler ;
    _map[ LCIO::CALORIMETERHIT ] = new SIOCalHitHandler ;
    _map[ LCIO::SIMTRACKERHIT ] = new SIOSimTrackHitHandler ;
    _map[ LCIO::TPCHIT ] = new SIOTPCHitHandler ;
    _map[ LCIO::TRACKERRAWDATA ] = new SIOTrackerRawDataHandler ;
    _map[ LCIO::TRACKERDATA ] = new SIOTrackerDataHandler ;
    _map[ LCIO::TRACKERPULSE ] = new SIOTrackerPulseHandler ;
    _map[ LCIO::TRACKERHIT ] = new SIOTrackerHitHandler ;
    _map[ LCIO::TRACKERHITPLANE ] = new SIOTrackerHitPlaneHandler ;
    _map[ LCIO::TRACKERHITZCYLINDER ] = new SIOTrackerHitZCylinderHandler ;
    _map[ LCIO::TRACK ] = new SIOTrackHandler ;
    _map[ LCIO::CLUSTER ] = new SIOClusterHandler ;
    _map[ LCIO::RECONSTRUCTEDPARTICLE ] = new SIOReconstructedParticleHandler ;
    _map[ LCIO::LCRELATION] = new SIOLCRelationHandler ;
    _map[ LCIO::VERTEX] = new SIOVertexHandler ;
    // generic arrays/vectors
    _map[ LCIO::LCSTRVEC ] = new SIOStrVecHandler ;
    _map[ LCIO::LCFLOATVEC ] = new SIOFloatVecHandler ;
    _map[ LCIO::LCINTVEC ] = new SIOIntVecHandler ;
    _map[ LCIO::LCGENERICOBJECT ] = new SIOLCGenericObjectHandler ;
  } 
  
  SIOHandlerMgr::~SIOHandlerMgr(){
    for( SIOHandlerMap::iterator iter = _map.begin() ; iter  != _map.end() ; iter++ ){
      delete iter->second ;
    }
  }
  
  SIOObjectHandler* SIOHandlerMgr::getHandler( const std::string& type ) const {
    auto findIter = _map.find( type );
    if(  findIter == _map.end( ) ) {
      return nullptr ;
    }
    return findIter->second ;
  }
  
  
  bool SIOHandlerMgr::registerHandler( const std::string& type, SIOObjectHandler* handler ){
	
    // check if type name does not yet exist  
    if(  _map.find( type ) != _map.end( ) ) {
      return false ;
    }
    
    if( nullptr == handler ) {
      return false ;
    }
    
    _map[ type ] = handler ;
    
    return true ;
  }
  
} // namespace
