
#include "IOIMPL/LCEventLazyImpl.h"
#include "SIO/LCIORecords.h"
#include "SIO/SIOParticleHandler.h"
#include "SIO/SIOCollectionHandler.h"

// -- std headers
#include <algorithm>

// -- sio headers
#include <sio/api.h>
#include <sio/block.h>
#include <sio/compression/zlib.h>

namespace IOIMPL {

  EVENT::LCCollection *LCEventLazyImpl::getCollection(const std::string & name) const {
    const_cast<LCEventLazyImpl*>(this)->unpackEvent() ;
    return LCEventImpl::getCollection( name ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCCollection *LCEventLazyImpl::takeCollection(const std::string & name) const {
    const_cast<LCEventLazyImpl*>(this)->unpackEvent() ;
    return LCEventImpl::takeCollection( name ) ;
  }
  
  //----------------------------------------------------------------------------
  
  void LCEventLazyImpl::removeCollection(const std::string & name) {
    std::remove_if( _blocks.begin(), _blocks.end(), [&]( const sio::block_ptr &blk ){ return (blk->name() == name) ; } ) ;
    LCEventImpl::removeCollection( name ) ;
  }
  
  //----------------------------------------------------------------------------
  
  void LCEventLazyImpl::setBuffer( const sio::record_info &recinfo, sio::buffer &&recordBuffer ) {
    if( nullptr != _recordBuffer ) {
      throw IO::IOException( "LCEventLazyImpl::setBuffer: tried to setup a SIO buffer but already set !" ) ;
    }
    // take the buffer and shrink the memory to fit its size
    _recordBuffer.reset( new sio::buffer( std::move( recordBuffer ) ) ) ;
    _recordBuffer->shrink() ;
    _recordInfo = recinfo ;
    // setup the block now as collections may be added before unpacking
    setupBlocks() ;
  }
  
  //----------------------------------------------------------------------------
  
  void LCEventLazyImpl::unpackEvent() {
    if( _unpacked ) {
      return ;
    }
    if( nullptr == _recordBuffer ) {
      throw IO::IOException( "LCEventLazyImpl::unpackEvent: SIO buffer not set !" ) ;
    }
    const bool compressed = sio::api::is_compressed( _recordInfo._options ) ;
    // uncompress and unpack
    if( compressed ) {
      // uncompress
      sio::buffer compBuffer( _recordInfo._uncompressed_length ) ;
      sio::zlib_compression compressor ;
      compressor.uncompress( _recordBuffer->span( _recordInfo._header_length ), compBuffer ) ;
      // unpack the event
      sio::api::read_blocks( compBuffer.span(), _blocks ) ;
    }
    else {
      sio::api::read_blocks( _recordBuffer->span( _recordInfo._header_length ), _blocks ) ;      
    }
    // the event record has been unpacked. Release the buffer memory
    _recordBuffer->clear( true ) ;
    _unpacked = true ;
    postProcessEvent() ;
  }
  
  //----------------------------------------------------------------------------
  
  void LCEventLazyImpl::postProcessEvent() {
    // restore the daughter relations from the parent relations
    SIO::SIOParticleHandler::restoreParentDaughterRelations( this ) ;
    // check subset collection's pointers
    char* rColChar = getenv ("LCIO_IGNORE_NULL_IN_SUBSET_COLLECTIONS") ;
    if( nullptr != rColChar ) {
      return;
    }
    const std::vector< std::string >* strVec = this->getCollectionNames() ;
    for( auto name = strVec->begin() ; name != strVec->end() ; name++) {
      EVENT::LCCollection* col = this->getCollection( *name ) ;
      if( col->isSubset() ) {
        for( int i=0,N=col->getNumberOfElements() ; i<N ; ++i ) {
          if( col->getElementAt( i ) == 0 ) {
            std::stringstream sts ;
            sts << " LCEventLazyImpl::postProcessEvent: null pointer in subset collection "
                << *name << " at position: " << i  << std::endl ;
            throw EVENT::Exception( sts.str()  ) ;
          }
        }
      }
    }
  }
  
  //----------------------------------------------------------------------------
  
  void LCEventLazyImpl::setupBlocks() {
    // auto collectionNames = this->getCollectionNames();
    for ( auto i = _colMap.begin() ; i != _colMap.end() ; i++ ) {
      auto handler = _eventHandlerMgr.getHandler( i->second->getTypeName() ) ;
      auto block = std::make_shared<SIO::SIOCollectionHandler>( i->first, handler ) ;
      block->setCollection( i->second );
      _blocks.push_back( block ) ;
    }
  }

}
