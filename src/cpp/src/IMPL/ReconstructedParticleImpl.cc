#include "IMPL/ReconstructedParticleImpl.h"
#include <algorithm>
#include "IMPL/ParticleIDImpl.h"


using namespace EVENT ;

namespace IMPL{


  ReconstructedParticleImpl::ReconstructedParticleImpl() :
    _type(0) ,
    _energy(0) ,
    _mass(0) ,
    _charge(0),
    _pidUsed(0),
    _goodnessOfPID(0),
    _sv(0)
    //_ev(0)
  {
    _cov.resize( NCOVARIANCE ) ;
    //     for(int i=0 ; i < NCOVARIANCE ; i++ ) { _cov.push_back( 0.0 ) ;  }
    _momentum[0] = 0. ;
    _momentum[1] = 0. ;
    _momentum[2] = 0. ;
    _reference[0] = 0. ;
    _reference[1] = 0. ;
    _reference[2] = 0. ;    
  }

  ReconstructedParticleImpl::~ReconstructedParticleImpl(){
    // delete the pids owned by this particle
    for(  ParticleIDVec::iterator iter = _pid.begin() ; iter != _pid.end() ; iter++){
      delete *iter ;
    }
  }
 
  int ReconstructedParticleImpl::getType() const { return _type ; }
//   bool ReconstructedParticleImpl::isPrimary() const { return _primary ;}

  const double* ReconstructedParticleImpl::getMomentum() const { return  _momentum ; }
  double ReconstructedParticleImpl::getEnergy() const { return  _energy ; }
  const EVENT::FloatVec & ReconstructedParticleImpl::getCovMatrix() const { return _cov   ; }
  double ReconstructedParticleImpl::getMass() const { return  _mass ; }
  float ReconstructedParticleImpl::getCharge() const { return  _charge ; }
  const float* ReconstructedParticleImpl::getReferencePoint() const { return  _reference ; }


  EVENT::ParticleID * ReconstructedParticleImpl::getParticleIDUsed() const { return  _pidUsed ; }
  
  float ReconstructedParticleImpl::getGoodnessOfPID() const{
    return _goodnessOfPID ;
  }
  const EVENT::ParticleIDVec & ReconstructedParticleImpl::getParticleIDs() const { return  _pid ; }

  const EVENT::ReconstructedParticleVec& ReconstructedParticleImpl::getParticles() const { return _particles ; } 
//   const EVENT::FloatVec & ReconstructedParticleImpl::getParticleWeights() const { return  _particleWeights ; }

  const EVENT::ClusterVec& ReconstructedParticleImpl::getClusters() const { return _clusters  ; } 
//   const EVENT::FloatVec & ReconstructedParticleImpl::getClusterWeights() const { return _clusterWeights  ; }

  const EVENT::TrackVec& ReconstructedParticleImpl::getTracks() const { return _tracks  ; } 
//   const EVENT::FloatVec & ReconstructedParticleImpl::getTrackWeights() const { return  _trackWeights ; }

//   const EVENT::MCParticleVec& ReconstructedParticleImpl::getMCParticles() const { return _mcParticles  ; }
//   const EVENT::FloatVec & ReconstructedParticleImpl::getMCParticleWeights() const { return  _mcParticleWeights ; }
  
  EVENT::Vertex * ReconstructedParticleImpl::getStartVertex() const { return _sv  ; }
                                                                                                                                                             
  //EVENT::Vertex * ReconstructedParticleImpl::getEndVertex() const { return _ev  ; }

  EVENT::Vertex * ReconstructedParticleImpl::getEndVertex() const {
    if(getParticles().size()!=0){
      return getParticles().at(0)->getStartVertex();
    }
    return NULL;
  }
                                                                                                                                                          
  void ReconstructedParticleImpl::setStartVertex( EVENT::Vertex *sv ){
    checkAccess("ReconstructedParticleImpl::setStartVertex" );
    _sv = sv;
  }

  /*  
  void ReconstructedParticleImpl::setEndVertex( EVENT::Vertex *ev ){
    checkAccess("ReconstructedParticleImpl::setEndVertex" );
    _ev = ev;
  }
*/

  void ReconstructedParticleImpl::setType(int type){
    checkAccess("ReconstructedParticleImpl::setType" );
    _type = type ;
  }

//   void ReconstructedParticleImpl::setPrimary(bool primary){
//     checkAccess("ReconstructedParticleImpl::setPrimary" );
//     _primary = primary ;
//   }

//   void ReconstructedParticleImpl::setTypeFlag( int typeFlag){
//     checkAccess("ReconstructedParticleImpl::setTypeFlag" );
//     _typeFlag  = typeFlag ;
//   }

  void ReconstructedParticleImpl::setMomentum( const float* momentum ){
    checkAccess("ReconstructedParticleImpl::setMomentum" );
    _momentum[0]  = momentum[0] ;
    _momentum[1]  = momentum[1] ;
    _momentum[2]  = momentum[2] ;
  }

  void ReconstructedParticleImpl::setMomentum( const double* momentum ){
    checkAccess("ReconstructedParticleImpl::setMomentum" );
    _momentum[0]  = momentum[0] ;
    _momentum[1]  = momentum[1] ;
    _momentum[2]  = momentum[2] ;
  }

  void ReconstructedParticleImpl::setEnergy( float energy){
    checkAccess("ReconstructedParticleImpl::setEnergy" );
    _energy  = energy ;
  }

  void ReconstructedParticleImpl::setCovMatrix( const float* cov ){
    checkAccess("ReconstructedParticleImpl::setCovMatrix" );
    for(int i=0;i<NCOVARIANCE;i++) _cov[i] = cov[i] ;
  }

  void ReconstructedParticleImpl::setCovMatrix( const EVENT::FloatVec& cov ){
    checkAccess("ReconstructedParticleImpl::" );
    for(int i=0;i<NCOVARIANCE;i++) _cov[i] = cov[i] ;
  }

  void ReconstructedParticleImpl::setMass( float mass ){
    checkAccess("ReconstructedParticleImpl::setMass" );
    _mass  = mass ;
  }

  void ReconstructedParticleImpl::setCharge( float charge ){
    checkAccess("ReconstructedParticleImpl::setCharge" );
    _charge  = charge ;
  }

  void ReconstructedParticleImpl::setReferencePoint( const float* reference ){
    checkAccess("ReconstructedParticleImpl::setReferencePoint" );
    _reference[0]  = reference[0] ;
    _reference[1]  = reference[1] ;
    _reference[2]  = reference[2] ;
  }

  void ReconstructedParticleImpl::setParticleIDUsed( ParticleID* pidUsed ){
    checkAccess("ReconstructedParticleImpl::setParticleIDUsed" );
    _pidUsed = pidUsed ;
  }

  void ReconstructedParticleImpl::setGoodnessOfPID( float goodness ){
    checkAccess("ReconstructedParticleImpl::setGoodnessOfPID") ;
    _goodnessOfPID = goodness ;
  }

  void ReconstructedParticleImpl::addParticleID( ParticleID* pid ){
    // adding a PID should also be possible w/ read only data
    //    checkAccess("ReconstructedParticleImpl::addParticleID" );
    _pid.push_back( pid ) ;
    // sort wrt. probability
    sort( _pid.begin() , _pid.end() , PIDSort()  ) ;
  }

  void ReconstructedParticleImpl::addParticle( ReconstructedParticle* particle ){
    checkAccess("ReconstructedParticleImpl::addParticle" );
    _particles.push_back( particle ) ;
//     _particleWeights.push_back( weight ) ;
  }

  void ReconstructedParticleImpl::addCluster( Cluster* cluster ){
    checkAccess("ReconstructedParticleImpl::addCluster" );
    _clusters.push_back( cluster ) ;
//     _clusterWeights.push_back( weight ) ;
  }

  void ReconstructedParticleImpl::addTrack( Track* track ){
    checkAccess("ReconstructedParticleImpl::addTrack" );
    _tracks.push_back( track ) ;
//     _trackWeights.push_back( weight ) ;
  }
  
//   void ReconstructedParticleImpl::addMCParticle(  MCParticle* mcParticle , float weight ){
//     checkAccess("ReconstructedParticleImpl::addMCParticle" );
//     _mcParticles.push_back( mcParticle ) ;
//     _mcParticleWeights.push_back( weight ) ;
//   }


} // end namespace
