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
#include "SIO/SIOLCVecHandler.h"
#include "SIO/SIOReconstructedParticleHandler.h"
#include "SIO/SIOLCRelationHandler.h"
#include "SIO/SIOLCGenericObjectHandler.h"
#include "SIO/SIOTPCHitHandler.h"
#include "SIO/SIOTrackerRawDataHandler.h"
#include "SIO/SIOTrackerDataHandler.h"
#include "SIO/SIOTrackerPulseHandler.h"
#include "SIO/SIOVertexHandler.h"

#include "EVENT/LCIO.h"

namespace SIO {

  SIOHandlerMgr::SIOHandlerMgr(){

    // add instances for all types to the map
    _map[ EVENT::LCIO::MCPARTICLE     ] = new SIOParticleHandler ;
    _map[ EVENT::LCIO::SIMCALORIMETERHIT ] = new SIOSimCalHitHandler ;
    _map[ EVENT::LCIO::RAWCALORIMETERHIT ] = new SIORawCalHitHandler ;
    _map[ EVENT::LCIO::CALORIMETERHIT ] = new SIOCalHitHandler ;
    _map[ EVENT::LCIO::SIMTRACKERHIT ] = new SIOSimTrackHitHandler ;
    _map[ EVENT::LCIO::TPCHIT ] = new SIOTPCHitHandler ;
    _map[ EVENT::LCIO::TRACKERRAWDATA ] = new SIOTrackerRawDataHandler ;
    _map[ EVENT::LCIO::TRACKERDATA ] = new SIOTrackerDataHandler ;
    _map[ EVENT::LCIO::TRACKERPULSE ] = new SIOTrackerPulseHandler ;
    _map[ EVENT::LCIO::TRACKERHIT ] = new SIOTrackerHitHandler ;
    _map[ EVENT::LCIO::TRACKERHITPLANE ] = new SIOTrackerHitPlaneHandler ;
    _map[ EVENT::LCIO::TRACKERHITZCYLINDER ] = new SIOTrackerHitZCylinderHandler ;
    _map[ EVENT::LCIO::TRACK ] = new SIOTrackHandler ;
    _map[ EVENT::LCIO::CLUSTER ] = new SIOClusterHandler ;
    _map[ EVENT::LCIO::RECONSTRUCTEDPARTICLE ] = new SIOReconstructedParticleHandler ;
    _map[ EVENT::LCIO::LCRELATION] = new SIOLCRelationHandler ;
    _map[ EVENT::LCIO::VERTEX] = new SIOVertexHandler ;
    // generic arrays/vectors
    _map[ EVENT::LCIO::LCSTRVEC ] = new SIOStrVecHandler ;
    _map[ EVENT::LCIO::LCFLOATVEC ] = new SIOFloatVecHandler ;
    _map[ EVENT::LCIO::LCINTVEC ] = new SIOIntVecHandler ;
    _map[ EVENT::LCIO::LCGENERICOBJECT ] = new SIOLCGenericObjectHandler ;
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


  bool SIOHandlerMgr::registerHandler( const std::string& type, SIOObjectHandler* handler ) {
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
