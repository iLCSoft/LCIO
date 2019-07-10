#include "SIO/SIOCollectionHandler.h"

// -- lcio headers
#include "SIO/LCSIO.h"
#include "EVENT/LCIO.h"
#include "SIO/SIOObjectHandler.h"
#include "SIO/SIOLCParameters.h"
#include "IOIMPL/LCCollectionIOVec.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO {

  SIOCollectionHandler::SIOCollectionHandler( const std::string& colname, std::shared_ptr<SIOObjectHandler> handler ) :
    sio::block( colname, LCSIO::blockVersion() ),
    _handler( handler ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  const std::string &SIOCollectionHandler::type() const{
    return _handler->collectionType() ;
  }

  //----------------------------------------------------------------------------

  void SIOCollectionHandler::setCollection( EVENT::LCCollection *col ) {
    _collection = col ;
  }

  //----------------------------------------------------------------------------

  void SIOCollectionHandler::read( sio::read_device &device, sio::version_type vers ) {
    IOIMPL::LCCollectionIOVec* ioCol = dynamic_cast<IOIMPL::LCCollectionIOVec*>( _collection ) ;
    if( nullptr == ioCol ) {
      SIO_THROW( sio::error_code::invalid_argument, "Expected collection of type LCCollectionIOVec!" ) ;
    }
    _handler->initReading( device, _collection, vers ) ;
    // read out number of objects in the collection
    int nObj ;
    SIO_DATA( device ,  &nObj , 1  ) ;
    // reserve the space for the pointers to all objects
    ioCol->resize( nObj ) ;
    if( _handler->flag() & ( 1 << EVENT::LCCollection::BITSubset ) ) {
      // read out only pointers
      for( int i=0 ; i< nObj ; i ++ ) {
        SIO_PNTR( device , &(*ioCol)[i]  ) ;
      }
    }
    else {
      // std::cout << "Reading a full collection" << std::endl ;
      // read out all objects in the collection
      for( int i=0 ; i< nObj ; i ++ ) {
        (*ioCol)[i] = _handler->create() ;
        try {
          _handler->read( device , (*ioCol)[i] , vers ) ;
        }
        catch( std::exception &e ) {
          std::stringstream ss ; ss << "Couldn't read out object of type '" << _handler->collectionType() << "' at index " << i ;
          SIO_RETHROW( e, sio::error_code::io_failure, ss.str() ) ;
        }
        catch( ... ) {
          std::stringstream ss ; ss << "Couldn't read out object of type '" << _handler->collectionType() << "' at index " << i ;
          SIO_THROW( sio::error_code::io_failure, ss.str() ) ;
        }
      }
    }
  }

  //----------------------------------------------------------------------------

  void SIOCollectionHandler::write( sio::write_device &device ) {
    _handler->initWriting( device, _collection ) ;
    // write number of objects
  	int nObj = _collection->getNumberOfElements() ;
  	SIO_DATA( device,  &nObj , 1  ) ;
    if( _handler->flag() & ( 1 << EVENT::LCCollection::BITSubset ) ) {
      // write only pointers
      for( int i=0 ; i< nObj ; i ++ ) {
        auto ptr = _collection->getElementAt(i) ;
        SIO_PNTR( device , &ptr ) ;
      }
    }
    else {
      // write all the objects
    	for( int i=0 ; i< nObj ; i ++ ) {
        _handler->write( device, _collection->getElementAt(i) ) ;
    	}
    }

  }

} // namespace
