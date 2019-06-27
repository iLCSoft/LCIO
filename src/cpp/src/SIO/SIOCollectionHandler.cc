#include "SIO/SIOCollectionHandler.h"

// -- lcio headers
#include "SIO/LCSIO.h"
#include "EVENT/LCIO.h"
#include "SIO/SIOObjectHandler.h"
#include "SIO/SIOLCParameters.h"
#include "IOIMPL/LCCollectionIOVec.h"

// -- sio headers
#include <sio/io_device.h>

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
    // read out collection parameters
    unsigned int flag ;
    SIO_DATA( device ,  &flag , 1  ) ;
    if( vers > SIO_VERSION_ENCODE( 1, 1 ) ) {
  	  SIOLCParameters::read( device, _collection->parameters(), vers ) ;
    }
    _handler->setFlag( flag ) ;
    // read out number of objects in the collection
    int nObj ;
    SIO_DATA( device ,  &nObj , 1  ) ;
    // reserve the space for the pointers to all objects
    ioCol->resize( nObj ) ;
    if( flag & ( 1 << EVENT::LCCollection::BITSubset ) ) {
      // read out only pointers
      for( int i=0 ; i< nObj ; i ++ ) {
        auto lcobj = (*ioCol)[i] ;
        SIO_PNTR( device , lcobj  ) ;
      }
    }
    else {
      // read out all objects in the collection
      for( int i=0 ; i< nObj ; i ++ ) {
        (*ioCol)[i] = _handler->create() ;
        auto lcobj = (*ioCol)[i] ;
        try {
          _myHandler->read( device , lcobj , vers ) ;
        }
        catch( std::exception &e ) {
          SIO_RETHROW( e, sio::error_code::io_failure, "Couldn't read out object of type '" + _handler->collectionType() + "'" ) ;
        }
        catch( ... ) {
          SIO_THROW( sio::error_code::io_failure, "Couldn't read out object of type '" + _handler->collectionType() + "'" ) ;
        }
      }
    }
  }
  
  //----------------------------------------------------------------------------
  
  void SIOCollectionHandler::write( sio::write_device &device ) {
    // handle collection flag
    auto flag = _collection->getFlag() ;
    _handler->setFlag( _collection->getFlag() ) ;
    SIO_DATA( device , &flag , 1 ) ;
    // write collection parameter
    SIOLCParameters::write( device , _collection->getParameters() ) ;
    // write number of objects
  	int nObj = _collection->getNumberOfElements() ;
  	SIO_DATA( stream,  &nObj , 1  ) ;
    if( flag & ( 1 << EVENT::LCCollection::BITSubset ) ) {
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
