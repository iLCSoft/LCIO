#include "IMPL/ClusterImpl.h"
#include <algorithm>
#include "IMPL/ParticleIDImpl.h"


using namespace EVENT ;

namespace IMPL{


  ClusterImpl::ClusterImpl() :
    _type(0),
    _energy(0),
    _theta(0),
    _phi(0) {

    _errpos.resize( NERRPOS ) ;
    _errdir.resize( NERRDIR ) ;
//     for(int i=0 ; i < NERRPOS ; i++ ) { _errpos.push_back( 0.0 ) ;  }
//     for(int i=0 ; i < NERRDIR ; i++ ) { _errdir.push_back( 0.0 ) ;  }
    _position[0] = 0. ;
    _position[1] = 0. ;
    _position[2] = 0. ;
  }

  ClusterImpl::~ClusterImpl(){
    // delete the pids owned by this cluster
    for(  ParticleIDVec::iterator iter = _pid.begin() ; iter != _pid.end() ; iter++){
      delete *iter ;
    }
  }
 
  int ClusterImpl::getType() const{  return _type.to_ulong() ; //return _type ; 
  }

//   bool ClusterImpl::testType(int bitIndex) const {
//     return _type.test( bitIndex ) ;
//   }
  
  float ClusterImpl::getEnergy() const{ return _energy;  }
  const float* ClusterImpl::getPosition() const{ return _position ; }
  const FloatVec & ClusterImpl::getPositionError() const{ return  _errpos ;  }
  float ClusterImpl::getITheta() const{ return _theta ;  }
  float ClusterImpl::getIPhi() const{ return _phi ;  }
  const FloatVec & ClusterImpl::getDirectionError() const{ return _errdir ;  }
  const FloatVec & ClusterImpl::getShape() const{ return _shape ;  }
//   const FloatVec & ClusterImpl::getParticleType() const{ return _particletype ;  }
  const EVENT::ParticleIDVec & ClusterImpl::getParticleIDs() const { return  _pid ; }

  const ClusterVec & ClusterImpl::getClusters() const{
    return _clusters ;
  }
  const EVENT::CalorimeterHitVec & ClusterImpl::getCalorimeterHits() const {
    return _hits ;
  }
  const EVENT::FloatVec & ClusterImpl::getHitContributions() const {
    return _weights ;
  }
  const  EVENT::FloatVec& ClusterImpl::getSubdetectorEnergies() const {
    return _subdetectorEnergies ;
  }


  void ClusterImpl::setType(int type ) {  
    checkAccess("ClusterImpl::setType") ;
    _type = type ; 
  }
  void ClusterImpl::setEnergy(float energy ) { 
    checkAccess("ClusterImpl::setEnergy") ;
    _energy = energy ;  
  }
  void ClusterImpl::setPosition(float* position) { 
    checkAccess("ClusterImpl::setPosition") ;
    for(int i=0;i<3;i++) { _position[i] = position[i]  ;  }
  }
  void ClusterImpl::setPositionError(const FloatVec &errpos) { 
    checkAccess("ClusterImpl::setPosition") ;
    for(int i=0;i<NERRPOS;i++) { _errpos[i] = errpos[i]  ;  }
  }
  void ClusterImpl::setPositionError(const float* errpos) { 
    checkAccess("ClusterImpl::setPositionError") ;
    for(int i=0;i<NERRPOS;i++) { _errpos[i] = errpos[i]  ;  }
  }
  void ClusterImpl::setITheta(float theta) { 
    checkAccess("ClusterImpl::setITheta") ;
    _theta = theta ;  
  }
  void ClusterImpl::setIPhi(float phi){  
    checkAccess("ClusterImpl::setIPhi") ;
    _phi = phi ;  
  }
  void ClusterImpl::setDirectionError(const FloatVec &errdir) { 
    checkAccess("ClusterImpl::setDirectionError") ;
    for(int i=0;i<NERRDIR;i++) { _errdir[i] = errdir[i]  ;  }
  }
  void ClusterImpl::setDirectionError(const float* errdir) { 
    checkAccess("ClusterImpl::setDirectionError") ;
    for(int i=0;i<NERRDIR;i++) { _errdir[i] = errdir[i]  ;  }
  }
//   void ClusterImpl::setShape(const float* shape) { 
//     checkAccess("ClusterImpl::setShape") ;
//     for(int i=0;i<NSHAPE;i++) { _shape[i] = shape[i]  ;  }
//   }
  void ClusterImpl::setShape(const FloatVec &shape) { 
    checkAccess("ClusterImpl::setShape") ;

    copy( shape.begin() , shape.end() , back_inserter( _shape ) ) ;
//     _shape.resize( shape.size() ) ;
//     for(unsigned int i=0;i<shape.size();i++) { _shape[i] = shape[i]  ;  }
  }

//   void ClusterImpl::setEMWeight(float emWeight ) { 
//     checkAccess("ClusterImpl::setEMWeight") ;
//     _particletype[0] = emWeight ; 
//   }
//   void ClusterImpl::setHADWeight(float hadWeight ) {
//     checkAccess("ClusterImpl::setHADWeight") ;
//     _particletype[1] = hadWeight ; 
//   }
//   void ClusterImpl::setMuonWeight(float muonWeight ) {
//     checkAccess("ClusterImpl::setMuonWeight") ;
//     _particletype[2] = muonWeight ;
//   }
  
  void ClusterImpl::addParticleID( ParticleID* pid ){
    checkAccess("ClusterImpl::addParticleID" );
    _pid.push_back( pid ) ;
    // sort wrt. probability
    sort( _pid.begin() , _pid.end() , PIDSort()  ) ;
  }

  void ClusterImpl::addCluster(EVENT::Cluster* cluster){
    checkAccess("ClusterImpl::addCluster") ;
    _clusters.push_back( cluster ) ;
  }
  
  
  void ClusterImpl::addHit(EVENT::CalorimeterHit* hit  , float contribution) {
    _hits.push_back( hit ) ;
    _weights.push_back( contribution ) ;
  }
  
  void  ClusterImpl::setTypeBit( int  index, bool val){  
    checkAccess("ClusterImpl::setTypeBit") ;
    _type.set( index, val  )  ;
  }

  EVENT::FloatVec& ClusterImpl::subdetectorEnergies(){
    checkAccess("ClusterImpl::subdetectorEnergies") ;
    return _subdetectorEnergies ;
  }

} // end namespace
