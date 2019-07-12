
#include "IOIMPL/LCEventLazyImpl.h"
#include "SIO/LCIORecords.h"

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


  EVENT::LCCollection *LCEventLazyImpl::takeCollection(const std::string & name) const {
    const_cast<LCEventLazyImpl*>(this)->unpackEvent() ;
    return LCEventImpl::takeCollection( name ) ;
  }
  
  void LCEventLazyImpl::removeCollection(const std::string & name) {
    std::remove_if( _blocks.begin(), _blocks.end(), [&]( const sio::block_ptr &blk ){ return (blk->name() == name) ; } ) ;
    LCEventImpl::removeCollection( name ) ;
  }
  
  void LCEventLazyImpl::setBuffer( const sio::record_info &recinfo, sio::buffer &&recordBuffer ) {
    if( nullptr != _recordBuffer ) {
      throw IO::IOException( "LCEventLazyImpl::setBuffer: tried to setup a SIO buffer but already set !" ) ;
    }
    // take the buffer and shrink the memory to fit its size
    _recordBuffer.reset( new sio::buffer( std::move( recordBuffer ) ) ) ;
    _recordBuffer->shrink() ;
    _recordInfo = recinfo ;
    // setup the block now as collections may be added before unpacking
    SIO::SIOEventRecord::setupBlocks( this, _eventHandlerMgr, _blocks ) ;
  }
  
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
      compressor.uncompress( _recordBuffer->span(), compBuffer ) ;
      // unpack the event
      sio::api::read_blocks( compBuffer.span(), _blocks ) ;
    }
    else {
      sio::api::read_blocks( _recordBuffer->span(), _blocks ) ;      
    }
    // the event record has been unpacked. Release the buffer memory
    _recordBuffer->clear( true ) ;
    _unpacked = true ;
  }

}
