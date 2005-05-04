#include "IMPL/SiliconRawHitImpl.h"

//#include "Exceptions.h"

#include <algorithm>

using namespace EVENT ;

namespace IMPL{

  SiliconRawHitImpl::SiliconRawHitImpl() : 
    _cellID0(0) ,
    _cellID1(0),
    _timeStamp(0),
    _adcCounts(0) {
  }    
  
  /// Destructor.
  SiliconRawHitImpl::~SiliconRawHitImpl() {
  }
  
  void SiliconRawHitImpl::setCellID0( int cellID0 ) {
    checkAccess("SiliconRawHitImpl::setCellID0") ;
    _cellID0 = cellID0 ;
  }

  void SiliconRawHitImpl::setCellID1( int cellID1 ) {
    checkAccess("SiliconRawHitImpl::setCellID1") ;
    _cellID1 = cellID1 ;
  } 

  void SiliconRawHitImpl::setTimeStamp( int timeStamp ) {
    checkAccess("SiliconRawHitImpl::setTimeStamp") ;
    _timeStamp = timeStamp ;
  } 

  void SiliconRawHitImpl::setADCCounts( int adcCounts ) {
    checkAccess("SiliconRawHitImpl::setADCCounts") ;
    _adcCounts = adcCounts ;
  } 


} // namespace IMPL
