#include "SIO/LCIORecords.h"

#include <SIO/SIORunHeaderHandler.h>
#include <SIO/SIOEventHandler.h>
#include <SIO/SIOCollectionHandler.h>
#include <SIO/SIORandomAccessHandler.h>
#include <SIO/SIOIndexHandler.h>
#include <EVENT/LCEvent.h>
#include <Exceptions.h>

// -- sio headers
#include <sio/exception.h>
#include <sio/api.h>

namespace SIO {

  void SIOEventHeaderRecord::readBlocks( const sio::buffer_span &buffer, EVENT::LCEvent *event, const std::vector<std::string> &readCol ) {
    sio::block_list blocks {} ;
    auto headerBlock = std::make_shared<SIOEventHandler>() ;
    headerBlock->setEvent( event ) ;
    headerBlock->setCollectionNames( std::set<std::string>( readCol.begin(), readCol.end() ) ) ;
    blocks.push_back( headerBlock ) ;
    sio::api::read_blocks( buffer, blocks ) ;
  }

  //----------------------------------------------------------------------------

  void SIOEventHeaderRecord::writeRecord( sio::buffer &outbuf, EVENT::LCEvent *event, sio::record_info& rec_info, sio::options_type opts ) {
    sio::block_list blocks {} ;
    auto headerBlock = std::make_shared<SIOEventHandler>() ;
    headerBlock->setEvent( event ) ;
    blocks.push_back( headerBlock ) ;
    rec_info = sio::api::write_record( LCSIO::HeaderRecordName, outbuf, blocks, opts ) ;
  }
  
  //----------------------------------------------------------------------------

  void SIOEventHeaderRecord::writeRecord( sio::buffer &outbuf, EVENT::LCEvent *event, sio::record_info& rec_info, const std::set<std::string> &colsOnly, sio::options_type opts ) {
    sio::block_list blocks {} ;
    auto headerBlock = std::make_shared<SIOEventHandler>() ;
    headerBlock->setEvent( event ) ;
    headerBlock->setCollectionNames( colsOnly ) ;
    blocks.push_back( headerBlock ) ;
    rec_info = sio::api::write_record( LCSIO::HeaderRecordName, outbuf, blocks, opts ) ;
  }

  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------

  void SIOEventRecord::setupBlocks( EVENT::LCEvent *event, const SIOHandlerMgr &handlerMgr, sio::block_list &blocks, const std::set<std::string> &colsOnly ) {
    auto collectionNames = event->getCollectionNames();
    for( auto collectionName : *collectionNames ) {
      auto collection = event->getCollection( collectionName ) ;
      // this is never true while reading. Can only appear on writing
      if( collection->isTransient() ) {
        continue;
      }
      if( ( not colsOnly.empty() ) and ( colsOnly.end() == colsOnly.find(collectionName) ) ) {
        continue ;
      }
      auto handler = handlerMgr.getHandler( collection->getTypeName() ) ;
      auto block = std::make_shared<SIOCollectionHandler>( collectionName, handler ) ;
      block->setCollection( collection );
      blocks.push_back( block ) ;
    }
  }

  //----------------------------------------------------------------------------

  void SIOEventRecord::readBlocks( const sio::buffer_span &buffer, EVENT::LCEvent *event, const SIOHandlerMgr &handlerMgr ) {
    sio::block_list blocks {} ;
    SIOEventRecord::setupBlocks( event, handlerMgr, blocks ) ;
    sio::api::read_blocks( buffer, blocks ) ;
  }

  //----------------------------------------------------------------------------

  void SIOEventRecord::writeRecord( sio::buffer &outbuf, EVENT::LCEvent *event, const SIOHandlerMgr &handlerMgr, sio::record_info& rec_info, sio::options_type opts ) {
    sio::block_list blocks {} ;
    SIOEventRecord::setupBlocks( event, handlerMgr, blocks ) ;
    rec_info = sio::api::write_record( LCSIO::EventRecordName, outbuf, blocks, opts ) ;
  }
  
  //----------------------------------------------------------------------------

  void SIOEventRecord::writeRecord( sio::buffer &outbuf, EVENT::LCEvent *event, const SIOHandlerMgr &handlerMgr, sio::record_info& rec_info, const std::set<std::string> &colsOnly, sio::options_type opts ) {
    sio::block_list blocks {} ;
    SIOEventRecord::setupBlocks( event, handlerMgr, blocks, colsOnly ) ;
    rec_info = sio::api::write_record( LCSIO::EventRecordName, outbuf, blocks, opts ) ;
  }

  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------

  void SIORunHeaderRecord::readBlocks( const sio::buffer_span &buffer, EVENT::LCRunHeader *rhdr ) {
    sio::block_list blocks {} ;
    auto runBlock = std::make_shared<SIORunHeaderHandler>() ;
    runBlock->setRunHeader( rhdr ) ;
    blocks.push_back( runBlock ) ;
    sio::api::read_blocks( buffer, blocks ) ;
  }

  //----------------------------------------------------------------------------

  void SIORunHeaderRecord::writeRecord( sio::buffer &outbuf, EVENT::LCRunHeader *rhdr, sio::record_info& rec_info, sio::options_type opts ) {
    sio::block_list blocks {} ;
    auto runBlock = std::make_shared<SIORunHeaderHandler>() ;
    runBlock->setRunHeader( rhdr ) ;
    blocks.push_back( runBlock ) ;
    rec_info = sio::api::write_record( LCSIO::RunRecordName, outbuf, blocks, opts ) ;
  }

} // namespace
