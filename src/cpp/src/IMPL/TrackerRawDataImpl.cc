#include "IMPL/TrackerRawDataImpl.h"

//#include "Exceptions.h"

#include <algorithm>

using namespace EVENT ;

namespace IMPL{

  TrackerRawDataImpl::TrackerRawDataImpl() : 
    _cellID0(0) ,
    _cellID1(0) ,
    _time(0){
  }    

  /// Destructor.
  TrackerRawDataImpl::~TrackerRawDataImpl() {
  }
  
  void TrackerRawDataImpl::setCellID0( int cellID0 ) {
    checkAccess("TrackerRawDataImpl::setCellID0") ;
    _cellID0 = cellID0 ;
  }

  void TrackerRawDataImpl::setCellID1( int cellID1 ) {
    checkAccess("TrackerRawDataImpl::setCellID1") ;
    _cellID1 = cellID1 ;
  }

  void TrackerRawDataImpl::setTime( int time ) {
    checkAccess("TrackerRawDataImpl::setTime") ;
    _time = time ;
    
  } 

  void TrackerRawDataImpl::setADCValues( const ShortVec & adc ) {
    checkAccess("TrackerRawDataImpl::setChargeVec") ;
    _adc.clear() ;
    _adc.resize( adc.size() ) ;
    std::copy( adc.begin() , adc.end() , _adc.begin() ) ;
  }

  ShortVec & TrackerRawDataImpl::adcValues() {
    checkAccess("TrackerRawDataImpl::adc") ;
    return _adc ;
  }  
  

} // namespace IMPL
