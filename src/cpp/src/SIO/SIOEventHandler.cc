#include "SIO/SIOEventHandler.h"
#include "SIO/LCSIO.h"

#include "EVENT/LCEvent.h"
#include "EVENT/LCIO.h"
#include "IOIMPL/LCEventIOImpl.h"
#include "IOIMPL/LCCollectionIOVec.h"
#include "SIO/SIOLCParameters.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO  {


  SIOEventHandler::SIOEventHandler() :
    sio::block( LCSIO::HeaderBlockName, LCSIO::blockVersion() ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIOEventHandler::setEvent( EVENT::LCEvent* evt ) {
    _event = evt ;
  }

  //----------------------------------------------------------------------------

  void SIOEventHandler::setCollectionNames(const std::set<std::string>& colnames) {
    _colSubSet = colnames ;
  }

  //----------------------------------------------------------------------------

  void SIOEventHandler::read( sio::read_device &device, sio::version_type vers ) {
    LCSIO::checkVersion( vers ) ;
    auto evt = dynamic_cast<IOIMPL::LCEventIOImpl*>( _event ) ;
    SIO_SDATA( device ,  evt->_runNumber ) ;
    SIO_SDATA( device ,  evt->_eventNumber ) ;
    SIO_SDATA( device ,  evt->_timeStamp ) ;
    std::string detName ;
    SIO_SDATA( device, detName ) ;
    evt->setDetectorName( detName  )  ;
    int nCol ;
    SIO_SDATA( device, nCol ) ;
    // read collection types and names
    // not needed for the event record
    // but SIO crashes if block is not read completely ...
    for( int i=0; i<nCol ; i++ ) {
      std::string colName, colType ;
      SIO_SDATA( device,  colName ) ;
      SIO_SDATA( device,  colType ) ;
      std::string::size_type idx ;
      if( ( idx = colType.rfind( SIOEventHandler::SubsetPostfix ) ) != std::string::npos ) {
        colType = std::string( colType , 0 , idx ) ;
      }
      // if we have a list with the sub set of collection names to be read we only add these to the event
      if( _colSubSet.empty() || _colSubSet.find( colName ) !=  _colSubSet.end() ) {
        evt->addCollection( new IOIMPL::LCCollectionIOVec( colType ) , colName) ;
      }
    }
    // read parameters
    if( vers > SIO_VERSION_ENCODE( 1, 1 ) ) {
      SIOLCParameters::read( device, evt->parameters(), vers ) ;
    }
  }

  //----------------------------------------------------------------------------

  void SIOEventHandler::write( sio::write_device &device ) {
    auto colNames = _event->getCollectionNames() ;
    unsigned int nCol = 0 ;
    for(unsigned int i=0 ; i < colNames->size() ; i++ ) {
      if( ( not _colSubSet.empty() ) and ( _colSubSet.end() == _colSubSet.find( (*colNames)[i] ) ) ) {
        continue ;
      }
      if( _event->getCollection( (*colNames)[i] )->isTransient() ) {
        continue ;  
      }
      nCol++ ;
    }
    SIO_SDATA( device, _event->getRunNumber() ) ;
  	SIO_SDATA( device, _event->getEventNumber() ) ;
  	SIO_SDATA( device, _event->getTimeStamp()  ) ;
  	SIO_SDATA( device, _event->getDetectorName() ) ;
    SIO_SDATA( device, nCol ) ;
    for(unsigned int i=0 ; i < colNames->size() ; i++ ) {
      auto col = _event->getCollection( (*colNames)[i] ) ;
      if( col->isTransient() ) {
        continue ;
      }
      if( ( not _colSubSet.empty() ) and ( _colSubSet.end() == _colSubSet.find( (*colNames)[i] ) ) ) {
        continue ;
      }
      std::string colType( col->getTypeName() ) ;
      if( col->isSubset() ) {
        colType += SIOEventHandler::SubsetPostfix ;
      }
      SIO_SDATA( device, (*colNames)[i] ) ;
      SIO_SDATA( device,  colType ) ;
    }
    SIOLCParameters::write( device , _event->getParameters() ) ;
  }

}
