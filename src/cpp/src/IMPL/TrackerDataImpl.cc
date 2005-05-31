#include "IMPL/TrackerDataImpl.h"

//#include "Exceptions.h"

#include <algorithm>

using namespace EVENT ;

namespace IMPL{

  TrackerDataImpl::TrackerDataImpl() : 
    _cellID0(0) ,
    _cellID1(0) ,
    _time(0){
  }    

  /// Destructor.
  TrackerDataImpl::~TrackerDataImpl() {
  }
  
  void TrackerDataImpl::setCellID0( int cellID0 ) {
    checkAccess("TrackerDataImpl::setCellID0") ;
    _cellID0 = cellID0 ;
  }

  void TrackerDataImpl::setCellID1( int cellID1 ) {
    checkAccess("TrackerDataImpl::setCellID1") ;
    _cellID1 = cellID1 ;
  }

  void TrackerDataImpl::setTime( float time ) {
    checkAccess("TrackerDataImpl::setTime") ;
    _time = time ;
    
  } 

  void TrackerDataImpl::setChargeValues( const FloatVec & charge ) {
    checkAccess("TrackerDataImpl::setChargeValues") ;
    _charge.clear() ;
    _charge.resize( charge.size() ) ;
    std::copy( charge.begin() , charge.end() , _charge.begin() ) ;
  }

  FloatVec & TrackerDataImpl::chargeValues() {
    checkAccess("TrackerDataImpl::chargeValues") ;
    return _charge ;
  }  
  

} // namespace IMPL
