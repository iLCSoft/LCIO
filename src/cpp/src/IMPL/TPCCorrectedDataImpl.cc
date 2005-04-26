#include "IMPL/TPCCorrectedDataImpl.h"

//#include "Exceptions.h"

#include <algorithm>

using namespace EVENT ;

namespace IMPL{

  TPCCorrectedDataImpl::TPCCorrectedDataImpl() : 
    _channelID(0) ,
    _time0(0){
  }    

  /// Destructor.
  TPCCorrectedDataImpl::~TPCCorrectedDataImpl() {
  }
  
  void TPCCorrectedDataImpl::setChannelID( int channelID ) {
    checkAccess("TPCCorrectedDataImpl::setChannelID") ;
    _channelID = channelID ;
  }

  void TPCCorrectedDataImpl::setTime0( int time0 ) {
    checkAccess("TPCCorrectedDataImpl::setTime0") ;
    _time0 = time0 ;
    
  } 

  void TPCCorrectedDataImpl::setChargeVec( const FloatVec & charge ) {
    checkAccess("TPCCorrectedDataImpl::setChargeVec") ;
    _charge.clear() ;
    _charge.resize( charge.size() ) ;
    std::copy( charge.begin() , charge.end() , _charge.begin() ) ;
  }

  FloatVec & TPCCorrectedDataImpl::charge() {
    checkAccess("TPCCorrectedDataImpl::charge") ;
    return _charge ;
  }  
  

} // namespace IMPL
