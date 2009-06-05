#include "IMPL/SimTrackerHitImpl.h"

using namespace EVENT ;


namespace IMPL {
  
  SimTrackerHitImpl::SimTrackerHitImpl() :
    _cellID(0),
    _dEdx(0),
    _time(0),
    _particle(0),
    _pathLength(0){
    _pos[0] = 0. ;
    _pos[1] = 0. ;
    _pos[2] = 0. ;
    _p[0] = 0. ;
    _p[1] = 0. ;
    _p[2] = 0. ;
  }

  SimTrackerHitImpl::~SimTrackerHitImpl(){  
  } 

  int SimTrackerHitImpl::getCellID() const {  return _cellID ;}

  const double * SimTrackerHitImpl::getPosition() const { return _pos ; }

  float SimTrackerHitImpl::getdEdx() const { return _dEdx ; }

  float SimTrackerHitImpl::getTime() const { return _time ; }

  EVENT::MCParticle * SimTrackerHitImpl::getMCParticle() const { return _particle ; }

  float SimTrackerHitImpl::getPathLength() const { return _pathLength ; }

  const float* SimTrackerHitImpl::getMomentum() const { return _p ; }

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

  void SimTrackerHitImpl::setMomentum( float p[3] )  { 

    setMomentum( p[0], p[1], p[2] ) ; 
  }

  void SimTrackerHitImpl::setMomentum( float px, float py, float pz ) {

    checkAccess("SimTrackerHitImpl::setMomentum") ;
    _p[0] = px ;
    _p[1] = py ;
    _p[2] = pz ;

  }
 	
  void SimTrackerHitImpl::setPathLength(float pathLength){
    checkAccess("SimTrackerHitImpl::setPathLength") ;
    _pathLength = pathLength ;
  }



} // namespace IMPL
