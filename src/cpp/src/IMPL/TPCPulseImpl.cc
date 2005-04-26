#include "IMPL/TPCPulseImpl.h"

//#include "Exceptions.h"

#include <algorithm>

using namespace EVENT ;

namespace IMPL{

  TPCPulseImpl::TPCPulseImpl() : 
    _channelID(0) ,
    _time(0),
    _charge(0),
    _quality(0),
    _corrData(0) {
  }    
  
  /// Destructor.
  TPCPulseImpl::~TPCPulseImpl() {
  }
  
  void TPCPulseImpl::setChannelID( int channelID ) {
    checkAccess("TPCPulseImpl::setChannelID") ;
    _channelID = channelID ;
  }

  void TPCPulseImpl::setTime( float time ) {
    checkAccess("TPCPulseImpl::setTime") ;
    _time = time ;
    
  } 
  void TPCPulseImpl::setCharge( float charge ) {
    checkAccess("TPCPulseImpl::setCharge") ;
    _charge = charge ;
    
  } 
  void TPCPulseImpl::setQuality( int quality ) {
    checkAccess("TPCPulseImpl::setQuality") ;
    _quality = quality ;
  }

  void TPCPulseImpl::setQualityBit( int bit , bool val ) {
    checkAccess("TPCPulseImpl::setQuality") ;
    
    if( val ) 
      _quality |=  ( 1 << bit )  ;
    else
      _quality &= ~( 1 << bit ) ; 
  }

  void TPCPulseImpl::setTPCCorrectedData( TPCCorrectedData * corrData ) {
    checkAccess("TPCPulseImpl::setTPCCorrectedData") ;
    _corrData = corrData ;
  }

} // namespace IMPL
