#include "SIO/SIOObjectHandler.h"
#include "SIO/SIOLCParameters.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO {

  SIOObjectHandler::SIOObjectHandler( const std::string &colType ) :
    _collectionType( colType ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIOObjectHandler::setFlag( unsigned int flag ) {
    _flag = flag ;
  }

  //----------------------------------------------------------------------------

  unsigned int SIOObjectHandler::flag() const {
    return _flag ;
  }

  //----------------------------------------------------------------------------

  const std::string &SIOObjectHandler::collectionType() const {
    return _collectionType ;
  }

  //----------------------------------------------------------------------------

  void SIOObjectHandler::initReading( sio::read_device &device, EVENT::LCCollection *collection, sio::version_type vers ) {
    SIO_SDATA( device ,  _flag ) ;
    if( vers > SIO_VERSION_ENCODE( 1, 1 ) ) {
  	  SIOLCParameters::read( device, collection->parameters(), vers ) ;
    }
  }

  //----------------------------------------------------------------------------

  void SIOObjectHandler::initWriting( sio::write_device &device, EVENT::LCCollection *collection ) {
    SIO_SDATA( device , _flag ) ;
    SIOLCParameters::write( device , collection->getParameters() ) ;
  }

}
