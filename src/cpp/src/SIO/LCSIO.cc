#include "SIO/LCSIO.h"
#include "EVENT/LCIO.h"

// -- sio headers
#include <sio/version.h>
#include <sio/exception.h>

namespace SIO {

  void LCSIO::checkVersion( sio::version_type versionID ){
    if ( SIO_VERSION_MAJOR( versionID ) < 1 && SIO_VERSION_MINOR( versionID ) < 8 )
      SIO_THROW( sio::error_code::invalid_argument, "Old file versions ( < v00-08 ) no longer supported !") ;    
  }
  
  //----------------------------------------------------------------------------
  
  sio::version_type LCSIO::blockVersion() {
    return SIO_VERSION_ENCODE( EVENT::LCIO::MAJORVERSION, EVENT::LCIO::MINORVERSION ) ;
  }
  
} 
 
