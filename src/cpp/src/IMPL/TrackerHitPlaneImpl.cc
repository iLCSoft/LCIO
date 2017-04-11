#include "IMPL/TrackerHitPlaneImpl.h"
#include "EVENT/LCIO.h"
#include "UTIL/LCWarning.h"

using namespace EVENT ;

namespace IMPL {
  
  TrackerHitPlaneImpl::TrackerHitPlaneImpl() :
    _cellID0(0),
    _cellID1(0),
    _type(0),
    _du(0),
    _dv(0),
    _EDep(0),
    _EDepError(0),
    _time(0),
    _quality(0),
    _cov( TRKHITPLANENCOVMATRIX ),
    _rawHits(0)
     {}
  
  TrackerHitPlaneImpl::~TrackerHitPlaneImpl(){  
  } 
  
  
  const FloatVec & TrackerHitPlaneImpl::getCovMatrix() const {
    
    // FIXME: compute errors in (x,y,z) 
    return _cov ;
  }
  
  // DEPRECATED. use getEDep()
  float TrackerHitPlaneImpl::getdEdx() const {

      UTIL::LCWarning::getInstance().printWarning( "TRACKERHIT_DEPRECATED_GETDEDX" ) ;

      //return _dEdx ;
      return getEDep();
  }

  void TrackerHitPlaneImpl::setCellID0(int id0){
    checkAccess("TrackerHitPlaneImpl::setCellID0") ;
    _cellID0 = id0 ;
  }

  void TrackerHitPlaneImpl::setCellID1(int id1){
    checkAccess("TrackerHitPlaneImpl::setCellID1") ;
    _cellID1 = id1 ;
  }

  void TrackerHitPlaneImpl::setType(int type) { 
    checkAccess("TrackerHitPlaneImpl::setType") ;
    _type= type ; 
  }

  void TrackerHitPlaneImpl::setPosition( const double pos[3]){ 
    checkAccess("TrackerHitPlaneImpl::setPosition") ;
    _pos[0] = pos[0] ; 
    _pos[1] = pos[1] ; 
    _pos[2] = pos[2] ; 
  }

  void TrackerHitPlaneImpl::setU( float theta, float phi){ 
    checkAccess("TrackerHitPlaneImpl::setU") ;
    _u[0] = theta ;
    _u[1] = phi ;
  } 

  void TrackerHitPlaneImpl::setV( float theta, float phi){ 
    checkAccess("TrackerHitPlaneImpl::setV") ;
    _v[0] = theta ;
    _v[1] = phi ;
  } 

  void TrackerHitPlaneImpl::setdU( float du ){ 
    checkAccess("TrackerHitPlaneImpl::setdU") ;
    _du = du ;
  } 

  void TrackerHitPlaneImpl::setdV( float dv ){ 
    checkAccess("TrackerHitPlaneImpl::setdV") ;
    _dv = dv ;
  } 
  
  void TrackerHitPlaneImpl::setEDep( float e )  { 
    checkAccess("TrackerHitPlaneImpl::setEDep") ;
    _EDep = e ; 
  }

  void TrackerHitPlaneImpl::setEDepError( float e )  { 
    checkAccess("TrackerHitPlaneImpl::setEDepError") ;
    _EDepError = e ; 
  }

  void TrackerHitPlaneImpl::setTime( float t )  { 
    checkAccess("TrackerHitPlaneImpl::setTime") ;
    _time = t ; 
  }

  void TrackerHitPlaneImpl::setQuality( int quality )  { 
    checkAccess("TrackerHitPlaneImpl::setQuality") ;
    _quality = quality ; 
  }

  void TrackerHitPlaneImpl::setQualityBit( int bit , bool val ) {
    checkAccess("TrackerHitPlaneImpl::setQuality") ;

    if( val )
      _quality |=  ( 1 << bit )  ;
    else
      _quality &= ~( 1 << bit ) ;
  }

  //void TrackerHitPlaneImpl::setCovMatrix( const FloatVec& cov ){
  //  checkAccess("TrackerHitPlaneImpl::setCovMatrix") ;
  //  for(int i=0;i<TRKHITPLANENCOVMATRIX;i++){
  //    _cov[i] = cov[i] ;
  //  }
  //}

  //void TrackerHitPlaneImpl::setCovMatrix( float cov[TRKHITPLANENCOVMATRIX]  ){
  //  checkAccess("TrackerHitPlaneImpl::setCovMatrix") ;
  //  for(int i=0;i<TRKHITPLANENCOVMATRIX;i++){
  //    _cov[i] = cov[i] ;
  //  }
  //}



} // namespace IMPL
