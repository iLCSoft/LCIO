#include "SIO/SIORandomAccessHandler.h"

// -- lcio headers
#include "SIO/LCIORandomAccess.h"
#include "SIO/LCSIO.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO  {

  SIORandomAccessHandler::SIORandomAccessHandler() :
    sio::block( LCSIO::AccessBlockName, LCSIO::blockVersion() ),
    _randomAccess( std::make_shared<LCIORandomAccess>() ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIORandomAccessHandler::setRandomAccess( std::shared_ptr<LCIORandomAccess> ra ) {
    _randomAccess = ra ;
  }

  //----------------------------------------------------------------------------

  std::shared_ptr<LCIORandomAccess> SIORandomAccessHandler::randomAccess() const {
    return _randomAccess ;
  }

  //----------------------------------------------------------------------------

  void SIORandomAccessHandler::read( sio::read_device &device, sio::version_type vers ) {
    LCSIO::checkVersion( vers ) ;
    SIO_SDATA( device ,  _randomAccess->_minRunEvt.RunNum ) ;
    SIO_SDATA( device ,  _randomAccess->_minRunEvt.EvtNum ) ;
    SIO_SDATA( device ,  _randomAccess->_maxRunEvt.RunNum ) ;
    SIO_SDATA( device ,  _randomAccess->_maxRunEvt.EvtNum ) ;
    SIO_SDATA( device ,  _randomAccess->_nRunHeaders ) ;
    SIO_SDATA( device ,  _randomAccess->_nEvents ) ;
    SIO_SDATA( device ,  _randomAccess->_recordsAreInOrder ) ;
    SIO_SDATA( device ,  _randomAccess->_indexLocation ) ;
    SIO_SDATA( device ,  _randomAccess->_prevLocation ) ;
    SIO_SDATA( device ,  _randomAccess->_nextLocation ) ;
    SIO_SDATA( device ,  _randomAccess->_firstRecordLocation ) ;
    // read record size - ignored for now - need to read if record size changes in the future ...
    int recSize ;
    SIO_SDATA( device, recSize ) ;
  }

  //----------------------------------------------------------------------------

  void SIORandomAccessHandler::write( sio::write_device &device ) {
    SIO_SDATA( device ,  _randomAccess->_minRunEvt.RunNum ) ;
    SIO_SDATA( device ,  _randomAccess->_minRunEvt.EvtNum ) ;
    SIO_SDATA( device ,  _randomAccess->_maxRunEvt.RunNum ) ;
    SIO_SDATA( device ,  _randomAccess->_maxRunEvt.EvtNum ) ;
    SIO_SDATA( device ,  _randomAccess->_nRunHeaders ) ;
    SIO_SDATA( device ,  _randomAccess->_nEvents ) ;
    SIO_SDATA( device ,  _randomAccess->_recordsAreInOrder ) ;
    SIO_SDATA( device ,  _randomAccess->_indexLocation ) ;
    SIO_SDATA( device ,  _randomAccess->_prevLocation ) ;
    SIO_SDATA( device ,  _randomAccess->_nextLocation ) ;
    SIO_SDATA( device ,  _randomAccess->_firstRecordLocation ) ;
    // write the current size of the LCIORandomAccess record as last word - incl. check at bits 31-16: 0xabcd
    int recSize = ( 0xabcd0000 | LCSIO::RandomAccessSize ) ;
    SIO_SDATA( device, recSize ) ;
  }

}
