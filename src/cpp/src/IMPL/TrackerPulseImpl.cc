#include "IMPL/TrackerPulseImpl.h"

//#include "Exceptions.h"

#include <algorithm>

using namespace EVENT ;

namespace IMPL{

  TrackerPulseImpl::TrackerPulseImpl() : 
    _cellID0(0) ,
    _cellID1(0) ,
    _time(0),
    _charge(0),
    _quality(0),
    _corrData(0) {
  }    
  
  /// Destructor.
  TrackerPulseImpl::~TrackerPulseImpl() {
  }
  
  void TrackerPulseImpl::setCellID0( int cellID0 ) {
    checkAccess("TrackerPulseImpl::setCellID0") ;
    _cellID0 = cellID0 ;
  }

  void TrackerPulseImpl::setCellID1( int cellID1 ) {
    checkAccess("TrackerPulseImpl::setCellID1") ;
    _cellID1 = cellID1 ;
  }

  void TrackerPulseImpl::setTime( float time ) {
    checkAccess("TrackerPulseImpl::setTime") ;
    _time = time ;
    
  } 
  void TrackerPulseImpl::setCharge( float charge ) {
    checkAccess("TrackerPulseImpl::setCharge") ;
    _charge = charge ;
    
  } 
  void TrackerPulseImpl::setQuality( int quality ) {
    checkAccess("TrackerPulseImpl::setQuality") ;
    _quality = quality ;
  }

  void TrackerPulseImpl::setQualityBit( int bit , bool val ) {
    checkAccess("TrackerPulseImpl::setQuality") ;
    
    if( val ) 
      _quality |=  ( 1 << bit )  ;
    else
      _quality &= ~( 1 << bit ) ; 
  }

  void TrackerPulseImpl::setTrackerData( TrackerData * corrData ) {
    checkAccess("TrackerPulseImpl::setTrackerData") ;
    _corrData = corrData ;
  }

} // namespace IMPL
