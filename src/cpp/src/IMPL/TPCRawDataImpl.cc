#include "IMPL/TPCRawDataImpl.h"

//#include "Exceptions.h"

#include <algorithm>

using namespace EVENT ;

namespace IMPL{

  TPCRawDataImpl::TPCRawDataImpl() : 
    _channelID(0) ,
    _time0(0){
  }    

  /// Destructor.
  TPCRawDataImpl::~TPCRawDataImpl() {
  }
  
  void TPCRawDataImpl::setChannelID( int channelID ) {
    checkAccess("TPCRawDataImpl::setChannelID") ;
    _channelID = channelID ;
  }

  void TPCRawDataImpl::setTime0( int time0 ) {
    checkAccess("TPCRawDataImpl::setTime0") ;
    _time0 = time0 ;
    
  } 

  void TPCRawDataImpl::setChargeVec( const ShortVec & charge ) {
    checkAccess("TPCRawDataImpl::setChargeVec") ;
    _charge.clear() ;
    _charge.resize( charge.size() ) ;
    std::copy( charge.begin() , charge.end() , _charge.begin() ) ;
  }

  ShortVec & TPCRawDataImpl::charge() {
    checkAccess("TPCRawDataImpl::charge") ;
    return _charge ;
  }  
  

} // namespace IMPL
