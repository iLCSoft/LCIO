#include "IMPL/TrackerHitImpl.h"

using namespace EVENT ;

namespace IMPL {
  
  TrackerHitImpl::TrackerHitImpl() :
    _cellID(0),
    _dEdx(0),
    _time(0),
    _particle(0){
  }

  TrackerHitImpl::~TrackerHitImpl(){  
  } 

  int TrackerHitImpl::getCellID() const {  return _cellID ;}

  const double * TrackerHitImpl::getPosition() const { return _pos ; }

  float TrackerHitImpl::getdEdx() const { return _dEdx ; }

  float TrackerHitImpl::getTime() const { return _time ; }

  const EVENT::MCParticle * TrackerHitImpl::getMCParticle() const {  return _particle ; }

  void TrackerHitImpl::setCellID( int id) { _cellID = id ; }

  void TrackerHitImpl::setPosition( double pos[3]){ 
    _pos[0] = pos[0] ; 
    _pos[1] = pos[1] ; 
    _pos[2] = pos[2] ; 
  }

  void TrackerHitImpl::setdEdx( float dedx )  { _dEdx = dedx ; }

  void TrackerHitImpl::setTime( float t )  {  _time = t ; }

  void TrackerHitImpl::setMCParticle( const EVENT::MCParticle* particle)  { _particle = particle; }

}; // namespace IMPL
