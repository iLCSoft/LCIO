#include "SIO/SIORunHeaderHandler.h"
#include "SIO/SIOLCParameters.h"
#include "SIO/LCSIO.h"

#include "EVENT/LCRunHeader.h"
#include "EVENT/LCIO.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO {

  SIORunHeaderHandler::SIORunHeaderHandler() :
    sio::block( LCSIO::RunBlockName, LCSIO::blockVersion() ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIORunHeaderHandler::setRunHeader( EVENT::LCRunHeader* rh ) {
    _runHeader = rh ;
  }

  //----------------------------------------------------------------------------

  void SIORunHeaderHandler::read( sio::read_device &device, sio::version_type vers ) {
    LCSIO::checkVersion( vers ) ;
    auto rhdr = dynamic_cast<IMPL::LCRunHeaderImpl*>( _runHeader ) ;
    // run number
    int rnTmp ;
    SIO_SDATA( device, rnTmp ) ;
    rhdr->setRunNumber( rnTmp ) ;
    // detector name
    std::string detNameTmp ;
    SIO_SDATA( device,  detNameTmp ) ;
    rhdr->setDetectorName( detNameTmp  )  ;
    // run description
    std::string descTmp ;
    SIO_SDATA( device,  descTmp ) ;
    rhdr->setDescription( descTmp  )  ;
    // active sub detector names
    int nSDN ;
    SIO_SDATA( device, nSDN ) ;
    for( int i=0; i<nSDN ; i++ ) {
      std::string sdnTmp ;
      SIO_SDATA( device,  sdnTmp ) ;
      rhdr->addActiveSubdetector( sdnTmp ) ;
    }
    // parameters
    if( vers > SIO_VERSION_ENCODE( 1, 1 ) ) {
      SIOLCParameters::read( device, rhdr->parameters(), vers ) ;
    }
  }

  //----------------------------------------------------------------------------

  void SIORunHeaderHandler::write( sio::write_device &device ) {
    const std::vector<std::string>* strVec = _runHeader->getActiveSubdetectors() ;
    int nSDN = strVec->size() ;
    SIO_SDATA( device, _runHeader->getRunNumber() ) ;
  	SIO_SDATA( device, _runHeader->getDetectorName() ) ;
  	SIO_SDATA( device, _runHeader->getDescription() ) ;
    SIO_SDATA( device, nSDN ) ;
    for( auto detstr : *strVec ) {
      SIO_SDATA( device, detstr ) ;
    }
    SIOLCParameters::write( device , _runHeader->getParameters() ) ;
  }

}
