#include "IMPL/TrackImpl.h"


using namespace EVENT ;

namespace IMPL {
  
  TrackImpl::TrackImpl() :
    _type(0),
    _d0(0),
    _phi(0),
    _omega(0),
    _z0(0),
     _tanLambda(0),
    _isReferencePointPCA(0),
    _chi2(0),
    _ndf(0),
    _dEdx(0),
    _dEdxError(0),
    _radiusOfInnermostHit(0) { 

    for(int i=0 ; i < NCOVMATRIX ; i++ ) {
      _covMatrix.push_back( 0.0 ) ; 
    }
    _reference[0] = 0.0 ;
    _reference[1] = 0.0 ;
    _reference[2] = 0.0 ;

    _type.set( BIT_ISREFERENCEPOINTDCA ) ;

  }

  TrackImpl::~TrackImpl() { 

//     for( IndexMap::const_iterator iter = _indexMap.begin() ; iter != _indexMap.end() ; iter++ ){
//       delete iter->second ;
//     }

  } 

  //  const std::string & TrackImpl::getType() const { return _type ; }

  int TrackImpl::getType() const { return _type.to_ulong() ; }

//   bool TrackImpl::testType(int bitIndex) const {
//     return _type.test( bitIndex ) ;
//   }

  float TrackImpl::getD0() const { return _d0 ;}
  float TrackImpl::getPhi() const { return _phi ; }
  float TrackImpl::getOmega() const { return _omega ; }
  float TrackImpl::getZ0() const { return _z0 ;}
  float TrackImpl::getTanLambda() const { return _tanLambda ;}

  const FloatVec& TrackImpl::getCovMatrix() const { return _covMatrix ; }
  const float* TrackImpl::getReferencePoint() const { return _reference ; }

  bool  TrackImpl::isReferencePointPCA() const { 
    //    return _isReferencePointPCA ;
    return _type.test( BIT_ISREFERENCEPOINTDCA ) ;
  }

  float TrackImpl::getChi2() const { return _chi2 ;}
  int   TrackImpl::getNdf() const { return _ndf ;}
  float TrackImpl::getdEdx() const { return _dEdx ; }
  float TrackImpl::getdEdxError() const { return _dEdxError ; }


  float TrackImpl::getRadiusOfInnermostHit() const {
    return _radiusOfInnermostHit ;
  }

  const IntVec & TrackImpl::getSubdetectorHitNumbers() const {
    return _subdetectorHitNumbers ;
  }
  const TrackerHitVec & TrackImpl::getTrackerHits() const {
    return _hits ;
  }

  const TrackVec & TrackImpl::getTracks() const {
    return _tracks ;
  } 
  
  void  TrackImpl::setTypeBit( int  index, bool val){  
    checkAccess("TrackImpl::setTypeBit") ;
    _type.set( index, val  )  ;
  }

  //  void  TrackImpl::setType( const std::string&  type ){  
  void  TrackImpl::setType( int  type ){  
    checkAccess("TrackImpl::setType") ;
   
//     int highWord = ( _type.to_ulong() & 0xFFFF0000 ) ;
//     int lowWord = ( 0x0000FFFF & type ) ;
//     _type = highWord | lowWord ; 

    _type = type ;
  } 

  void  TrackImpl::setD0( float d0 ){
    checkAccess("TrackImpl::setD0") ;
    _d0 = d0  ;
  } 
  void  TrackImpl::setPhi( float phi ){ 
    checkAccess("TrackImpl::setPhi") ;
    _phi = phi ; 
  } 
  void  TrackImpl::setOmega( float omega ) { 
    checkAccess("TrackImpl::setOmega") ;
    _omega = omega  ;
  } 
  void  TrackImpl::setZ0( float z0 ){
    checkAccess("TrackImpl::setZ0") ;
    _z0 = z0 ; 
  } 
  void  TrackImpl::setTanLambda( float tanLambda ){
    checkAccess("TrackImpl::setTanLambda") ;
    _tanLambda = tanLambda ; 
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

  void  TrackImpl::setIsReferencePointPCA( bool val){ 
    checkAccess("TrackImpl::setIsReferencePointPCA") ;
    //    _isReferencePointPCA = val ;
    _type.set( BIT_ISREFERENCEPOINTDCA , val ) ;

  } 

  void  TrackImpl::setChi2( float chi2 ){ 
    checkAccess("TrackImpl::setChi2") ;
    _chi2 = chi2 ; 
  } 
  void  TrackImpl::setNdf( int ndf ){ 
    checkAccess("TrackImpl::setNdf") ;
    _ndf = ndf  ; 
  } 
  void  TrackImpl::setdEdx( float dEdx ){ 
    checkAccess("TrackImpl::setdEdx") ;
    _dEdx = dEdx ; 
  } 
  void  TrackImpl::setdEdxError( float dEdxError ){
    checkAccess("TrackImpl::setdEdxError") ;
    _dEdxError = dEdxError  ;
  }   

  void TrackImpl::addHit( TrackerHit* hit) {
    _hits.push_back( hit ) ;
  }

  void  TrackImpl::addTrack( EVENT::Track* trk ) {
    checkAccess("TrackImpl::addTrack") ;
    _tracks.push_back( trk ) ;
  }

  IntVec & TrackImpl::subdetectorHitNumbers(){
    checkAccess("TrackImpl::subdetectorHitNumbers") ;
    return _subdetectorHitNumbers ;
  }

  void  TrackImpl::setRadiusOfInnermostHit( float r ){
    checkAccess("TrackImpl::setRadiusOfInnermostHit") ;
    _radiusOfInnermostHit = r ;
  }

} // namespace IMPL


