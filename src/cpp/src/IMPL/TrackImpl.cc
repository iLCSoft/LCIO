#include "IMPL/TrackImpl.h"


using namespace DATA ;
using namespace EVENT ;

namespace IMPL {
  
  
  TrackImpl::TrackImpl() { 

    for(int i=0 ; i < NCOVMATRIX ; i++ ) {
      _covMatrix.push_back( 0.0 ) ; 
    }
  }

  TrackImpl::~TrackImpl() { 

    for( IndexMap::const_iterator iter = _indexMap.begin() ; iter != _indexMap.end() ; iter++ ){
      delete iter->second ;
    }

  } 

  int TrackImpl::getType() const { return _type ; }

  float TrackImpl::getMomentum() const { return _p ; }
  float TrackImpl::getTheta() const { return _theta ;}
  float TrackImpl::getPhi() const { return _phi ; }
  float TrackImpl::getD0() const { return _d0 ;}
  float TrackImpl::getZ0() const { return _z0 ;}
  const FloatVec& TrackImpl::getCovMatrix() const { return _covMatrix ; }
  const float* TrackImpl::getReferencePoint() const { return _reference ; }
  float TrackImpl::getChi2() const { return _chi2 ;}
  float TrackImpl::getdEdx() const { return _dEdx ; }
  float TrackImpl::getdEdxError() const { return _dEdxError ; }



  const std::vector<std::string>&  TrackImpl::getHitCollectionNames() const { 

    _hitCollectionNames.clear() ;

    for( IndexMap::const_iterator iter = _indexMap.begin() ; iter != _indexMap.end() ; iter++ ){
      _hitCollectionNames.push_back ( iter->first ) ;
    }
    return _hitCollectionNames ;
  }


  const IntVec& TrackImpl::getHitIndicesForCollection(const std::string & colName) const { 
    return *_indexMap[ colName ] ;
  }
  

  const TrackDataVec & TrackImpl::getTracksData() const {
    // pointers to TrackData and Track are the same (provided non virtual inheritance)
    const TrackDataVec* tracksData = reinterpret_cast<const TrackDataVec*>( &_tracks ) ;
    return *tracksData  ;
  } 

  const TrackVec & TrackImpl::getTracks() const {
    return _tracks ;
  } 
  
    

  void  TrackImpl::setType( int type ){  
    checkAccess("TrackImpl::setType") ;
    _type = type ; 
  } 
  void  TrackImpl::setMomentum( float momentum ) { 
    checkAccess("TrackImpl::setMomentum") ;
    _p = momentum  ;
  } 
  void  TrackImpl::setTheta( float theta ){
    checkAccess("TrackImpl::setTheta") ;
    _theta = theta ; 
  } 
  void  TrackImpl::setPhi( float phi ){ 
    checkAccess("TrackImpl::setPhi") ;
    _phi = phi ; 
  } 
  void  TrackImpl::setD0( float d0 ){
    checkAccess("TrackImpl::setD0") ;
    _d0 = d0  ;
  } 
  void  TrackImpl::setZ0( float z0 ){
    checkAccess("TrackImpl::setZ0") ;
    _z0 = z0 ; 
  } 
  void  TrackImpl::setCovMatrix( float* cov ){ 
    checkAccess("TrackImpl::setCovMatrix") ;
    for(int i=0;i<NCOVMATRIX;i++) {
      _covMatrix[i] = cov[i]  ; 
    }
  } 
  void  TrackImpl::setCovMatrix( const FloatVec& cov ){ 
    checkAccess("TrackImpl::setCovMatrix") ;
    for(int i=0;i<NCOVMATRIX;i++) {
      _covMatrix[i] = cov[i]  ; 
    }
  } 
  
  void  TrackImpl::setReferencePoint( float* rPnt){ 
    checkAccess("TrackImpl::setReferencePoint") ;
    for(int i=0;i<3;i++) {
      _reference[i] = rPnt[i]  ; 
    }
  } 
  void  TrackImpl::setChi2( float chi2 ){ 
    checkAccess("TrackImpl::setChi2") ;
    _chi2 = chi2 ; 
  } 
  void  TrackImpl::setdEdx( float dEdx ){ 
    checkAccess("TrackImpl::setdEdx") ;
    _dEdx = dEdx ; 
  } 
  void  TrackImpl::setdEdxError( float dEdxError ){
    checkAccess("TrackImpl::setdEdxError") ;
    _dEdxError = dEdxError  ;
  }   
  void  TrackImpl::addHitIndex( const std::string& colName, int index ){   

    checkAccess("TrackImpl::addHitIndex") ;
    IntVec* vec = _indexMap[ colName ] ;
    
    if( vec == 0 ){
      vec = new IntVec ;
      _indexMap[ colName ] = vec ;
    }

    vec->push_back( index )  ;
  }    
  void  TrackImpl::addTrack( EVENT::Track* trk ) {
    checkAccess("TrackImpl::addTrack") ;
    _tracks.push_back( trk ) ;
  }
  

//   Track* Track::operator=(LCObject* obj) {
    
//     Track* trk = dynamic_cast<Track*>(obj) ;
//     if(trk==0)
//       throw Exception("Cannot dynamic_cast<Track*>( obj)" ) ;
//     return trk ;
//   }
  

}; // namespace IMPL


