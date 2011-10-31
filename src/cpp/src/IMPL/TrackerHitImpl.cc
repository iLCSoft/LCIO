#include "IMPL/TrackerHitImpl.h"
#include "EVENT/LCIO.h"
#include "UTIL/LCWarning.h"

using namespace EVENT ;

namespace IMPL {
  
  TrackerHitImpl::TrackerHitImpl() :
    _cellID0(0),
    _cellID1(0),
    _type(0),
    //_dEdx(0),
    _EDep(0),
    _EDepError(0),
    _time(0),
    _quality(0),
    _rawHits(0) {
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


  int TrackerHitImpl::getCellID0() const {
    return _cellID0 ;
  }

  int TrackerHitImpl::getCellID1() const {
    return _cellID1 ;
  }

  const double* TrackerHitImpl::getPosition() const {  return _pos ; } 

  const FloatVec & TrackerHitImpl::getCovMatrix() const {
    return _cov ;
  }

  // DEPRECATED. use getEDep()
  float TrackerHitImpl::getdEdx() const {

      UTIL::LCWarning::getInstance().printWarning( "TRACKERHIT_DEPRECATED_GETDEDX" ) ;

      //return _dEdx ;
      return getEDep();
  }

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

  void TrackerHitImpl::setCellID0(int id0){
    checkAccess("TrackerHitImpl::setCellID0") ;
    _cellID0 = id0 ;
  }

  void TrackerHitImpl::setCellID1(int id1){
    checkAccess("TrackerHitImpl::setCellID1") ;
    _cellID1 = id1 ;
  }

  void TrackerHitImpl::setType(int type) { 
    checkAccess("TrackerHitImpl::setType") ;
    _type= type ; 
  }

  void TrackerHitImpl::setPosition( const double pos[3]){ 
    checkAccess("TrackerHitImpl::setPosition") ;
    _pos[0] = pos[0] ; 
    _pos[1] = pos[1] ; 
    _pos[2] = pos[2] ; 
  }

  // DEPRECATED. use setEDep()
  void TrackerHitImpl::setdEdx( float dedx )  {

    UTIL::LCWarning::getInstance().printWarning( "TRACKERHIT_DEPRECATED_SETDEDX" ) ;

    //checkAccess("TrackerHitImpl::setdEdx") ;
    //_dEdx = dedx ; 
    setEDep( dedx ) ;
  }

  void TrackerHitImpl::setEDep( float e )  { 
    checkAccess("TrackerHitImpl::setEDep") ;
    _EDep = e ; 
  }

  void TrackerHitImpl::setEDepError( float e )  { 
    checkAccess("TrackerHitImpl::setEDepError") ;
    _EDepError = e ; 
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
  void TrackerHitImpl::setCovMatrix( const float cov[TRKHITNCOVMATRIX]  ){
    checkAccess("TrackerHitImpl::setCovMatrix") ;
    for(int i=0;i<TRKHITNCOVMATRIX;i++){
      _cov[i] = cov[i] ;
    }
  }


} // namespace IMPL
