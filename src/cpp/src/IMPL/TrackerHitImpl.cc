#include "IMPL/TrackerHitImpl.h"
#include "EVENT/LCIO.h"

using namespace EVENT ;

namespace IMPL {
  
  TrackerHitImpl::TrackerHitImpl() :
    _type(0),
    _dEdx(0),
    _time(0),
    _quality(0) {
    _pos[0] = 0. ;
    _pos[1] = 0. ;
    _pos[2] = 0. ;
    
    _cov.resize( TRKHITNCOVMATRIX ) ;
    //     for(int i=0;i<TRKHITNCOVMATRIX;i++){
    //       _cov.push_back(0.0) ;
    //     }
  }
  
  TrackerHitImpl::~TrackerHitImpl(){  
  } 

  const double* TrackerHitImpl::getPosition() const {  return _pos ; } 

  const FloatVec & TrackerHitImpl::getCovMatrix() const {
    return _cov ;
  }

  float TrackerHitImpl::getdEdx() const { return _dEdx ; }

  float TrackerHitImpl::getTime() const { return _time ; }


  const EVENT::LCObjectVec & TrackerHitImpl::getRawHits() const {
    return _rawHits ;
  }

  EVENT::LCObjectVec & TrackerHitImpl::rawHits() {
    checkAccess("TrackerHitImpl::rawHits") ;
    return _rawHits ;
  }

//   const std::string & TrackerHitImpl::getType() const {
//     static std::string tpcHitType( LCIO::TPCHIT ) ;
//     static std::string unknown( "Unknown" ) ;
//     TPCHit* tpchit = dynamic_cast<TPCHit*>( _rawHit ) ;
//     if( tpchit != 0 ) 
//       return tpcHitType ;
//     else
//       return unknown ;
//   }

  int TrackerHitImpl::getType() const {
    return _type ;
  }


  void TrackerHitImpl::setType(int type) { 
    checkAccess("TrackerHitImpl::setType") ;
    _type= type ; 
  }

  void TrackerHitImpl::setPosition( double pos[3]){ 
    checkAccess("TrackerHitImpl::setPosition") ;
    _pos[0] = pos[0] ; 
    _pos[1] = pos[1] ; 
    _pos[2] = pos[2] ; 
  }

  void TrackerHitImpl::setdEdx( float dedx )  {
    checkAccess("TrackerHitImpl::setdEdx") ;
    _dEdx = dedx ; 
  }

  void TrackerHitImpl::setTime( float t )  { 
    checkAccess("TrackerHitImpl::setTime") ;
    _time = t ; 
  }

  void TrackerHitImpl::setQuality( int quality )  { 
    checkAccess("TrackerHitImpl::setQuality") ;
    _quality = quality ; 
  }

  void TrackerHitImpl::setQualityBit( int bit , bool val ) {
    checkAccess("TrackerHitImpl::setQuality") ;

    if( val )
      _quality |=  ( 1 << bit )  ;
    else
      _quality &= ~( 1 << bit ) ;
  }

  void TrackerHitImpl::setCovMatrix( const FloatVec& cov ){
    checkAccess("TrackerHitImpl::setCovMatrix") ;
    for(int i=0;i<TRKHITNCOVMATRIX;i++){
      _cov[i] = cov[i] ;
    }
  }
  void TrackerHitImpl::setCovMatrix( float cov[TRKHITNCOVMATRIX]  ){
    checkAccess("TrackerHitImpl::setCovMatrix") ;
    for(int i=0;i<TRKHITNCOVMATRIX;i++){
      _cov[i] = cov[i] ;
    }
  }


} // namespace IMPL
