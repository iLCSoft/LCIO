#include "IMPL/ReconstructedParticleImpl.h"
#include <algorithm>


using namespace DATA ;
using namespace EVENT ;

namespace IMPL{


  ReconstructedParticleImpl::ReconstructedParticleImpl(){
    for(int i=0 ; i < NCOVARIANCE ; i++ ) { _cov.push_back( 0.0 ) ;  }
  }
  ReconstructedParticleImpl::~ReconstructedParticleImpl(){
    // delete the pids owned by this particle
    for(  ParticleIDVec::iterator iter = _pid.begin() ; iter != _pid.end() ; iter++){
      delete *iter ;
    }
  }
 
  int ReconstructedParticleImpl::getTypeFlag() const { return  _typeFlag ; }
  const float* ReconstructedParticleImpl::getMomentum() const { return  _momentum ; }
  float ReconstructedParticleImpl::getEnergy() const { return  _energy ; }
  const DATA::FloatVec & ReconstructedParticleImpl::getCovMatrix() const { return _cov   ; }
  float ReconstructedParticleImpl::getMass() const { return  _mass ; }
  float ReconstructedParticleImpl::getCharge() const { return  _charge ; }
  const float* ReconstructedParticleImpl::getReferencePoint() const { return  _reference ; }

  const DATA::ParticleIDDataVec & ReconstructedParticleImpl::getParticleIDsData() const {
    const ParticleIDDataVec* pidData = reinterpret_cast<const ParticleIDDataVec*>
      ( &_pid ) ;
    return *pidData  ;
  }
  const EVENT::ParticleIDVec & ReconstructedParticleImpl::getParticleIDs() const { return  _pid ; }

  const DATA::ReconstructedParticleDataVec & ReconstructedParticleImpl::getParticlesData() const { 
    // reinterpret pointers to ReconstructedParticle as pointers to ReconstructedParticleData
    const ReconstructedParticleDataVec* pData = reinterpret_cast<const ReconstructedParticleDataVec*>
      ( &_particles ) ;
    return *pData  ;
  }
  const EVENT::ReconstructedParticleVec& ReconstructedParticleImpl::getParticles() const { return _particles ; } 
  const DATA::FloatVec & ReconstructedParticleImpl::getParticleWeights() const { return  _particleWeights ; }

  const DATA::ClusterDataVec & ReconstructedParticleImpl::getClustersData() const { 
    // pointers to ClusterData and Cluster are the same (provided non virtual inheritance)
    const ClusterDataVec* clustersData = reinterpret_cast<const ClusterDataVec*>( &_clusters ) ;
    return *clustersData  ;
  }
  const EVENT::ClusterVec& ReconstructedParticleImpl::getClusters() const { return _clusters  ; } 
  const DATA::FloatVec & ReconstructedParticleImpl::getClusterWeights() const { return _clusterWeights  ; }

  const DATA::TrackDataVec & ReconstructedParticleImpl::getTracksData() const { 
    // pointers to TrackData and Track are the same (provided non virtual inheritance)
    const TrackDataVec* tracksData = reinterpret_cast<const TrackDataVec*>( &_tracks ) ;
    return *tracksData  ;
  }
  const EVENT::TrackVec& ReconstructedParticleImpl::getTracks() const { return _tracks  ; } 
  const DATA::FloatVec & ReconstructedParticleImpl::getTrackWeights() const { return  _trackWeights ; }

  const DATA::MCParticleDataVec & ReconstructedParticleImpl::getMCParticlesData() const { 
    const MCParticleDataVec* mcpData = reinterpret_cast<const MCParticleDataVec*>( &_mcParticles ) ;
    return *mcpData  ;
  }
  const EVENT::MCParticleVec& ReconstructedParticleImpl::getMCParticles() const { return _mcParticles  ; }
  const DATA::FloatVec & ReconstructedParticleImpl::getMCParticleWeights() const { return  _mcParticleWeights ; }
  

  void ReconstructedParticleImpl::setTypeFlag( int typeFlag){
    checkAccess("ReconstructedParticleImpl::setTypeFlag" );
    _typeFlag  = typeFlag ;
  }
  void ReconstructedParticleImpl::setMomentum( const float* momentum ){
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

  void ReconstructedParticleImpl::setCovMatrix( const DATA::FloatVec& cov ){
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

  void ReconstructedParticleImpl::addParticleID( const ParticleID* pid ){
    checkAccess("ReconstructedParticleImpl::addParticleID" );
    _pid.push_back( pid ) ;
    // sort wrt. probability
    sort( _pid.begin() , _pid.end() , PIDSort()  ) ;
  }

  void ReconstructedParticleImpl::addParticle( const ReconstructedParticle* particle , float weight = 1.0 ){
    checkAccess("ReconstructedParticleImpl::addParticle" );
    _particles.push_back( particle ) ;
    _particleWeights.push_back( weight ) ;
  }

  void ReconstructedParticleImpl::addCluster( const Cluster* cluster, float weight = 1.0 ){
    checkAccess("ReconstructedParticleImpl::addCluster" );
    _clusters.push_back( cluster ) ;
    _clusterWeights.push_back( weight ) ;
  }

  void ReconstructedParticleImpl::addTrack( const Track* track, float weight = 1.0 ){
    checkAccess("ReconstructedParticleImpl::addTrack" );
    _tracks.push_back( track ) ;
    _trackWeights.push_back( weight ) ;
  }
  
  void ReconstructedParticleImpl::addMCParticle( const MCParticle* mcParticle , float weight = 1.0 ){
    checkAccess("ReconstructedParticleImpl::addMCParticle" );
    _mcParticles.push_back( mcParticle ) ;
    _mcParticleWeights.push_back( weight ) ;
  }


}; // end namespace
