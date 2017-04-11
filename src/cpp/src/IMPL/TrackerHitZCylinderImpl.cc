#include "IMPL/TrackerHitZCylinderImpl.h"
#include "EVENT/LCIO.h"
#include "UTIL/LCWarning.h"

using namespace EVENT ;

namespace IMPL {
  
  TrackerHitZCylinderImpl::TrackerHitZCylinderImpl() :
    _cellID0(0),
    _cellID1(0),

    _type(0),
    _drphi(0),
    _dz(0),
    _EDep(0),
    _EDepError(0),
    _time(0),
    _quality(0),
    _cov(TRKHITZCYLNCOVMATRIX ), 
    _rawHits(0) {
  }
  
  TrackerHitZCylinderImpl::~TrackerHitZCylinderImpl(){  
  } 
  
  
  const FloatVec & TrackerHitZCylinderImpl::getCovMatrix() const {
    
    // FIXME: compute errors in (x,y,z) 
    return _cov ;
  }
  
  // DEPRECATED. use getEDep()
  float TrackerHitZCylinderImpl::getdEdx() const {

      UTIL::LCWarning::getInstance().printWarning( "TRACKERHIT_DEPRECATED_GETDEDX" ) ;

      //return _dEdx ;
      return getEDep();
  }

  void TrackerHitZCylinderImpl::setCellID0(int id0){
    checkAccess("TrackerHitZCylinderImpl::setCellID0") ;
    _cellID0 = id0 ;
  }

  void TrackerHitZCylinderImpl::setCellID1(int id1){
    checkAccess("TrackerHitZCylinderImpl::setCellID1") ;
    _cellID1 = id1 ;
  }


  void TrackerHitZCylinderImpl::setType(int type) { 
    checkAccess("TrackerHitZCylinderImpl::setType") ;
    _type= type ; 
  }

  void TrackerHitZCylinderImpl::setPosition( const double pos[3]){ 
    checkAccess("TrackerHitZCylinderImpl::setPosition") ;
    _pos[0] = pos[0] ; 
    _pos[1] = pos[1] ; 
    _pos[2] = pos[2] ; 
  }

  void TrackerHitZCylinderImpl::setCenter( float xc, float yc){ 
    checkAccess("TrackerHitZCylinderImpl::setCenter") ;
    _center[0] = xc ;
    _center[1] = yc ;
  } 
  
  //void TrackerHitZCylinderImpl::setR( float r ){ 
  //  checkAccess("TrackerHitZCylinderImpl::setR") ;
  //  _r = r ;
  //} 
  
  void TrackerHitZCylinderImpl::setdRPhi(float drphi ){ 
    checkAccess("TrackerHitZCylinderImpl::setdRPhi") ;
    _drphi = drphi ;
  } 

  void TrackerHitZCylinderImpl::setdZ( float dz ){ 
    checkAccess("TrackerHitZCylinderImpl::setdZ") ;
    _dz = dz ;
  } 
  
  void TrackerHitZCylinderImpl::setEDep( float e )  { 
    checkAccess("TrackerHitZCylinderImpl::setEDep") ;
    _EDep = e ; 
  }

  void TrackerHitZCylinderImpl::setEDepError( float e )  { 
    checkAccess("TrackerHitZCylinderImpl::setEDepError") ;
    _EDepError = e ; 
  }

  void TrackerHitZCylinderImpl::setTime( float t )  { 
    checkAccess("TrackerHitZCylinderImpl::setTime") ;
    _time = t ; 
  }

  void TrackerHitZCylinderImpl::setQuality( int quality )  { 
    checkAccess("TrackerHitZCylinderImpl::setQuality") ;
    _quality = quality ; 
  }

  void TrackerHitZCylinderImpl::setQualityBit( int bit , bool val ) {
    checkAccess("TrackerHitZCylinderImpl::setQuality") ;

    if( val )
      _quality |=  ( 1 << bit )  ;
    else
      _quality &= ~( 1 << bit ) ;
  }

  //void TrackerHitZCylinderImpl::setCovMatrix( const FloatVec& cov ){
  //  checkAccess("TrackerHitZCylinderImpl::setCovMatrix") ;
  //  for(int i=0;i<TRKHITZCYLNCOVMATRIX;i++){
  //    _cov[i] = cov[i] ;
  //  }
  //}

  //void TrackerHitZCylinderImpl::setCovMatrix( float cov[TRKHITZCYLNCOVMATRIX]  ){
  //  checkAccess("TrackerHitZCylinderImpl::setCovMatrix") ;
  //  for(int i=0;i<TRKHITZCYLNCOVMATRIX;i++){
  //    _cov[i] = cov[i] ;
  //  }
  //}



} // namespace IMPL
