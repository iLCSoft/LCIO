#include "SIO/SIOHandlerMgr.h"

#include "SIO/SIOSimCalHitHandler.h" 
#include "SIO/SIOSimTrackHitHandler.h" 
#include "SIO/SIOParticleHandler.h" 
#include "SIO/SIOFloatVecHandler.h" 

#include "EVENT/LCIO.h"

using namespace EVENT ;

namespace SIO {
  
  SIOHandlerMgr* SIOHandlerMgr::_me  = 0 ;
  
  
  SIOHandlerMgr::SIOHandlerMgr(){
    
    // add instances for all types to the map
    
    _map[ LCIO::MCPARTICLE     ] = new SIOParticleHandler ;
    _map[ LCIO::SIMCALORIMETERHIT ] = new SIOSimCalHitHandler ;
    _map[ LCIO::SIMTRACKERHIT ] = new SIOSimTrackHitHandler ;
    // generic arrays/vectors
    _map[ LCIO::LCFLOATVEC ] = new SIOFloatVecHandler ;

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
