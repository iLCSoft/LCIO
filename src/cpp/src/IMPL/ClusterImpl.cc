#include "IMPL/ClusterImpl.h"


using namespace DATA ;
using namespace EVENT ;

namespace IMPL{


  ClusterImpl::ClusterImpl(){
    for(int i=0 ; i < NERRPOS ; i++ ) { _errpos.push_back( 0.0 ) ;  }
    for(int i=0 ; i < NERRDIR ; i++ ) { _errdir.push_back( 0.0 ) ;  }
    for(int i=0 ; i < NSHAPE ; i++ )  { _shape.push_back( 0.0 ) ;   }
    for(int i=0 ; i < 3 ; i++ )  { _particletype.push_back( 0.0 ) ;   }
  }

  ClusterImpl::~ClusterImpl(){
    for( IndexMap::const_iterator iter = _indexMap.begin() ; iter != _indexMap.end() ; iter++ ){
      delete iter->second->Indices ;
      delete iter->second->Weights ;
      delete iter->second ;
    }
  }
 
  int ClusterImpl::getType() const{ return _type ; }
  float ClusterImpl::getEnergy() const{ return _energy;  }
  const float* ClusterImpl::getPosition() const{ return _position ; }
  const FloatVec & ClusterImpl::getPositionError() const{ return  _errpos ;  }
  float ClusterImpl::getTheta() const{ return _theta ;  }
  float ClusterImpl::getPhi() const{ return _phi ;  }
  const FloatVec & ClusterImpl::getDirectionError() const{ return _errdir ;  }
  const FloatVec & ClusterImpl::getShape() const{ return _shape ;  }
  const FloatVec & ClusterImpl::getParticleType() const{ return _particletype ;  }
  const ClusterDataVec & ClusterImpl::getClustersData() const{ 
    // pointers to ClusterData and Cluster are the same (provided non virtual inheritance)
    const ClusterDataVec* clustersData = reinterpret_cast<const ClusterDataVec*>( &_clusters ) ;
    return *clustersData  ;
  }
  const StringVec & ClusterImpl::getHitCollectionNames() const{ 
    _hitCollectionNames.clear() ;
    
    for( IndexMap::const_iterator iter = _indexMap.begin() ; iter != _indexMap.end() ; iter++ ){
      _hitCollectionNames.push_back ( iter->first ) ;
    }
    return _hitCollectionNames ;
  }
  const IntVec & ClusterImpl::getHitIndicesForCollection(const std::string & colName) const{
    return *_indexMap[ colName ]->Indices ;
  }
  const FloatVec & ClusterImpl::getHitContributionsForCollection(const std::string & colName) const{
    return *_indexMap[ colName ]->Weights ;
  }
  const ClusterVec & ClusterImpl::getClusters() const{
    return _clusters ;
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
  void ClusterImpl::setTheta(float theta) { 
    checkAccess("ClusterImpl::setTheta") ;
    _theta = theta ;  
  }
  void ClusterImpl::setPhi(float phi){  
    checkAccess("ClusterImpl::setPhi") ;
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
  void ClusterImpl::setShape(const float* shape) { 
    checkAccess("ClusterImpl::setShape") ;
    for(int i=0;i<NSHAPE;i++) { _shape[i] = shape[i]  ;  }
  }
  void ClusterImpl::setShape(const FloatVec &shape) { 
    checkAccess("ClusterImpl::setShape") ;
    for(int i=0;i<NSHAPE;i++) { _shape[i] = shape[i]  ;  }
  }
  void ClusterImpl::setEMWeight(float emWeight ) { 
    checkAccess("ClusterImpl::setEMWeight") ;
    _particletype[0] = emWeight ; 
  }
  void ClusterImpl::setHADWeight(float hadWeight ) {
    checkAccess("ClusterImpl::setHADWeight") ;
    _particletype[1] = hadWeight ; 
  }
  void ClusterImpl::setMuonWeight(float muonWeight ) {
    checkAccess("ClusterImpl::setMuonWeight") ;
    _particletype[2] = muonWeight ;
  }
  
  void ClusterImpl::addCluster(EVENT::Cluster* cluster){
    checkAccess("ClusterImpl::addCluster") ;
    _clusters.push_back( cluster ) ;
  }
  

  void  ClusterImpl::addHitIndex( const std::string& colName, int index , float contribution){
     
    checkAccess("ClusterImpl::addHitIndex") ;

    WeightedIndices*  wi = _indexMap[ colName ] ; 
    
    if( wi == 0 ) {
      wi = new WeightedIndices ;
      wi->Indices  = new IntVec ;
      wi->Weights = new FloatVec ;
      _indexMap[ colName ] = wi ;
    }
    wi->Indices->push_back( index )  ;
    wi->Weights->push_back( contribution )  ;
  }    
  

}; // end namespace
