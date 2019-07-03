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
    _map[ EVENT::LCIO::MCPARTICLE     ] = std::make_shared<SIOParticleHandler>() ;
    _map[ EVENT::LCIO::SIMCALORIMETERHIT ] = std::make_shared<SIOSimCalHitHandler>() ;
    _map[ EVENT::LCIO::RAWCALORIMETERHIT ] = std::make_shared<SIORawCalHitHandler>() ;
    _map[ EVENT::LCIO::CALORIMETERHIT ] = std::make_shared<SIOCalHitHandler>() ;
    _map[ EVENT::LCIO::SIMTRACKERHIT ] = std::make_shared<SIOSimTrackHitHandler>() ;
    _map[ EVENT::LCIO::TPCHIT ] = std::make_shared<SIOTPCHitHandler>() ;
    _map[ EVENT::LCIO::TRACKERRAWDATA ] = std::make_shared<SIOTrackerRawDataHandler>() ;
    _map[ EVENT::LCIO::TRACKERDATA ] = std::make_shared<SIOTrackerDataHandler>() ;
    _map[ EVENT::LCIO::TRACKERPULSE ] = std::make_shared<SIOTrackerPulseHandler>() ;
    _map[ EVENT::LCIO::TRACKERHIT ] = std::make_shared<SIOTrackerHitHandler>() ;
    _map[ EVENT::LCIO::TRACKERHITPLANE ] = std::make_shared<SIOTrackerHitPlaneHandler>() ;
    _map[ EVENT::LCIO::TRACKERHITZCYLINDER ] = std::make_shared<SIOTrackerHitZCylinderHandler>() ;
    _map[ EVENT::LCIO::TRACK ] = std::make_shared<SIOTrackHandler>() ;
    _map[ EVENT::LCIO::CLUSTER ] = std::make_shared<SIOClusterHandler>() ;
    _map[ EVENT::LCIO::RECONSTRUCTEDPARTICLE ] = std::make_shared<SIOReconstructedParticleHandler>() ;
    _map[ EVENT::LCIO::LCRELATION] = std::make_shared<SIOLCRelationHandler>() ;
    _map[ EVENT::LCIO::VERTEX] = std::make_shared<SIOVertexHandler>() ;
    // generic arrays/vectors
    _map[ EVENT::LCIO::LCSTRVEC ] = std::make_shared<SIOStrVecHandler>() ;
    _map[ EVENT::LCIO::LCFLOATVEC ] = std::make_shared<SIOFloatVecHandler>() ;
    _map[ EVENT::LCIO::LCINTVEC ] = std::make_shared<SIOIntVecHandler>() ;
    _map[ EVENT::LCIO::LCGENERICOBJECT ] = std::make_shared<SIOLCGenericObjectHandler>() ;
  }

  //----------------------------------------------------------------------------

  std::shared_ptr<SIOObjectHandler> SIOHandlerMgr::getHandler( const std::string& type ) const {
    auto findIter = _map.find( type );
    if(  findIter == _map.end( ) ) {
      return nullptr ;
    }
    return findIter->second ;
  }

  //----------------------------------------------------------------------------

  void SIOHandlerMgr::registerHandler( const std::string& type, std::unique_ptr<SIOObjectHandler> handler ) {
    if(  _map.find( type ) != _map.end( ) ) {
      throw EVENT::Exception( "SIOHandlerMgr::registerHandler: Handler of type '" + type + "' already exists !" ) ;
    }
    _map[ type ] = std::move(handler) ;
  }

} // namespace
