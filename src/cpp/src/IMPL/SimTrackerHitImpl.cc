#include "IMPL/SimTrackerHitImpl.h"

using namespace EVENT ;

namespace IMPL {
  
  SimTrackerHitImpl::SimTrackerHitImpl() :
    _cellID(0),
    _dEdx(0),
    _time(0),
    _particle(0){
  }

  SimTrackerHitImpl::~SimTrackerHitImpl(){  
  } 

  int SimTrackerHitImpl::getCellID() const {  return _cellID ;}

  const double * SimTrackerHitImpl::getPosition() const { return _pos ; }

  float SimTrackerHitImpl::getdEdx() const { return _dEdx ; }

  float SimTrackerHitImpl::getTime() const { return _time ; }

  DATA::MCParticleData * SimTrackerHitImpl::getMCParticleData() const {  return _particle ; }

  EVENT::MCParticle * SimTrackerHitImpl::getMCParticle() const throw (DataNotAvailableException) { 
    if( _particle == 0 ) 
      throw DataNotAvailableException("SimTrackerHitImpl::getMCParticle: no MCPArticle contribution") ;
    return _particle ; 
  }

  void SimTrackerHitImpl::setCellID( int id) {
    checkAccess("SimTrackerHitImpl::setCellID") ;
    _cellID = id ; 
  }

  void SimTrackerHitImpl::setPosition( double pos[3]){ 
    checkAccess("SimTrackerHitImpl::setPosition") ;
    _pos[0] = pos[0] ; 
    _pos[1] = pos[1] ; 
    _pos[2] = pos[2] ; 
  }

  void SimTrackerHitImpl::setdEdx( float dedx )  {
    checkAccess("SimTrackerHitImpl::setdEdx") ;
    _dEdx = dedx ; 
  }

  void SimTrackerHitImpl::setTime( float t )  { 
    checkAccess("SimTrackerHitImpl::setTime") ;
    _time = t ; 
  }

  void SimTrackerHitImpl::setMCParticle( EVENT::MCParticle* particle)  {
    checkAccess("SimTrackerHitImpl::setMCParticle") ;
    _particle = particle; 
  }

}; // namespace IMPL
