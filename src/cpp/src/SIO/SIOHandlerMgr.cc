#include "SIO/SIOHandlerMgr.h"

#include "SIO/SIOSimCalHitHandler.h" 
#include "SIO/SIOCalHitHandler.h" 
#include "SIO/SIOSimTrackHitHandler.h" 
#include "SIO/SIOTrackHandler.h" 
#include "SIO/SIOClusterHandler.h" 
#include "SIO/SIOTPCHitHandler.h" 
#include "SIO/SIOParticleHandler.h" 
#include "SIO/SIOFloatVecHandler.h" 
#include "SIO/SIOIntVecHandler.h" 

#include "EVENT/LCIO.h"

using namespace EVENT ;

namespace SIO {
  
  SIOHandlerMgr* SIOHandlerMgr::_me  = 0 ;
  
  
  SIOHandlerMgr::SIOHandlerMgr(){
    
    // add instances for all types to the map
    
    _map[ LCIO::MCPARTICLE     ] = new SIOParticleHandler ;
    _map[ LCIO::SIMCALORIMETERHIT ] = new SIOSimCalHitHandler ;
    _map[ LCIO::CALORIMETERHIT ] = new SIOCalHitHandler ;
    _map[ LCIO::SIMTRACKERHIT ] = new SIOSimTrackHitHandler ;
    _map[ LCIO::TPCHIT ] = new SIOTPCHitHandler ;
    _map[ LCIO::TRACK ] = new SIOTrackHandler ;
    _map[ LCIO::CLUSTER ] = new SIOClusterHandler ;
    // generic arrays/vectors
    _map[ LCIO::LCFLOATVEC ] = new SIOFloatVecHandler ;
    _map[ LCIO::LCINTVEC ] = new SIOIntVecHandler ;

  } 
  
  SIOHandlerMgr::~SIOHandlerMgr(){
    // just called at end of program ...
  }
  
  SIOHandlerMgr* SIOHandlerMgr::instance(){
	
    if( ! _me  ){
      _me  = new SIOHandlerMgr ;
    }
    return _me ;
  }
  
  SIOObjectHandler* SIOHandlerMgr::getHandler( const std::string& type ){
    
    if(  _map.find( type ) == _map.end( ) ) 
      return 0 ;
    
    return _map[ type ] ;
  }
  
  
  bool SIOHandlerMgr::registerHandler( const std::string& type, SIOObjectHandler* handler ){
	
    // check if type name does not yet exist  
    if(  _map.find( type ) != _map.end( ) ) 
      return false ;
    
    _map[ type ] = handler ;
    
    return true ;
  }
  
}; // namespace
