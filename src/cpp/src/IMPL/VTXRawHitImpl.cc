#include "IMPL/VTXRawHitImpl.h"

//#include "Exceptions.h"

#include <algorithm>

using namespace EVENT ;

namespace IMPL{

  VTXRawHitImpl::VTXRawHitImpl() : 
    _moduleID(0) ,
    _row(0),
    _column(0),
    _adcCounts(0) {
  }    
  
  /// Destructor.
  VTXRawHitImpl::~VTXRawHitImpl() {
  }
  
  void VTXRawHitImpl::setModuleID( int moduleID ) {
    checkAccess("VTXRawHitImpl::setModuleID") ;
    _moduleID = moduleID ;
  }

  void VTXRawHitImpl::setRow( int row ) {
    checkAccess("VTXRawHitImpl::setRow") ;
    _row = row ;
  } 

  void VTXRawHitImpl::setColumn( int column ) {
    checkAccess("VTXRawHitImpl::setColumn") ;
    _column = column ;
  } 

  void VTXRawHitImpl::setADCCounts( int adcCounts ) {
    checkAccess("VTXRawHitImpl::setADCCounts") ;
    _adcCounts = adcCounts ;
  } 


} // namespace IMPL
