#include "IMPL/TrackerHitImpl.h"
#include "EVENT/LCIO.h"

using namespace EVENT ;

namespace IMPL {
  
  TrackerHitImpl::TrackerHitImpl() :
    //    _cov(TRKHITNCOVMATRIX),
    _dEdx(0),
    _time(0) {
    _pos[0] = 0. ;
    _pos[1] = 0. ;
    _pos[2] = 0. ;
    for(int i=0;i<TRKHITNCOVMATRIX;i++){
      _cov.push_back(0.0) ;
    }
  }

  TrackerHitImpl::~TrackerHitImpl(){  
  } 

  const double* TrackerHitImpl::getPosition() const {  return _pos ; } 

  const FloatVec & TrackerHitImpl::getCovMatrix() const {
    return _cov ;
  }

  float TrackerHitImpl::getdEdx() const { return _dEdx ; }

  float TrackerHitImpl::getTime() const { return _time ; }

  LCObject * TrackerHitImpl::getRawDataHit() const {
    return _rawHit ;
  }

  const std::string & TrackerHitImpl::getType() const {

    static std::string tpcHitType( LCIO::TPCHIT ) ;
    static std::string unknown( "Unknown" ) ;

    TPCHit* tpchit = dynamic_cast<TPCHit*>( _rawHit ) ;
    if( tpchit != 0 ) 
      return tpcHitType ;
    else
      return unknown ;

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
  void TrackerHitImpl::setTPCHit( TPCHit* hit){
    checkAccess("TrackerHitImpl::setTime") ;
    _rawHit = hit ;
  }


}; // namespace IMPL
