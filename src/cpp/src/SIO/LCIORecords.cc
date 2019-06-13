#include "SIO/LCIORecords.h"

#include <SIO/SIORunHeaderHandler.h>
#include <SIO/SIOEventHandler.h>
#include <SIO/SIOCollectionHandler.h>
#include <SIO/SIORandomAccessHandler.h>
#include <SIO/SIOIndexHandler.h>
#include <EVENT/LCEvent.h>
#include <Exceptions.h>

namespace SIO {
  
  // sio::record_ptr LCIORecords::createEventHeaderRecord( const EVENT::LCEvent *event ) const {
  //   auto eventHeaderRecord = std::make_shared<SIO_record>( LCSIO_HEADERRECORDNAME );
  //   eventHeaderRecord->add_block<SIOEventHandler>( LCSIO_HEADERBLOCKNAME );
  //   auto block = eventHeaderRecord->get_block_as<SIOEventHandler>( LCSIO_HEADERBLOCKNAME );
  //   block->setEvent( event );
  //   return eventHeaderRecord;
  // }
  // 
  // //----------------------------------------------------------------------------
  // 
  // sio::record_ptr LCIORecords::createEventHeaderRecord( IOIMPL::LCEventIOImpl **event, const std::vector<std::string> &readCol ) const {
  //   auto eventHeaderRecord = std::make_shared<SIO_record>( LCSIO_HEADERRECORDNAME );
  //   eventHeaderRecord->add_block<SIOEventHandler>( LCSIO_HEADERBLOCKNAME );
  //   auto block = eventHeaderRecord->get_block_as<SIOEventHandler>( LCSIO_HEADERBLOCKNAME );
  //   block->setEventPtr( event );
  //   block->setReadCollectionNames( readCol );
  //   return eventHeaderRecord;
  // }
  // 
  // //----------------------------------------------------------------------------
  // 
  // sio::record_ptr LCIORecords::createEventRecord( const EVENT::LCEvent *event ) const {
  //   // create the event record
  //   auto eventRecord = std::make_shared<SIO_record>( LCSIO_EVENTRECORDNAME );
  //   // loop over collections and setup the blocks for writing
  //   auto collectionNames = event->getCollectionNames();
  //   for( auto collectionName : *collectionNames ) {
  //     auto collection = event->getCollection( collectionName );
  //     if( collection->isTransient() ) {
  //       continue;
  //     }
  //     try {
  //       eventRecord->add_block<SIOCollectionHandler>( collectionName, collection->getTypeName(), nullptr );        
  //     }
  //     catch(EVENT::Exception& ex) {
  //       continue;
  //     }
  //     auto block = eventRecord->get_block_as<SIOCollectionHandler>( collectionName );
  //     auto handler = _handlerMgr.getHandler( collection->getTypeName() );
  //     block->setHandler( handler );
  //     block->setCollection( collection );
  //   }
  //   return eventRecord;    
  // }
  // 
  // //----------------------------------------------------------------------------
  // 
  // sio::record_ptr LCIORecords::createEventRecord(IOIMPL::LCEventIOImpl **event) const {
  //   // create the event record
  //   auto eventRecord = std::make_shared<SIO_record>( LCSIO_EVENTRECORDNAME );
  //   // loop over collections and setup the blocks for reading/writing an event
  //   auto collectionNames = (*event)->getCollectionNames();
  //   for( auto collectionName : *collectionNames ) {
  //     auto collection = (*event)->getCollection( collectionName );
  //     try {
  //       eventRecord->add_block<SIOCollectionHandler>( collectionName, collection->getTypeName(), event );
  //     }
  //     catch(EVENT::Exception& ex) {
  //       continue;
  //     }
  //     auto block = eventRecord->get_block_as<SIOCollectionHandler>( collectionName );
  //     auto handler = _handlerMgr.getHandler( collection->getTypeName() );
  //     block->setHandler( handler );
  //     block->setEvent( event );
  //   }
  //   return eventRecord;
  // }
  // 
  // //----------------------------------------------------------------------------
  // 
  // sio::record_ptr LCIORecords::createRunRecord( const EVENT::LCRunHeader *runHeader ) const {
  //   auto runRecord = std::make_shared<SIO_record>( LCSIO_RUNRECORDNAME );
  //   runRecord->add_block<SIORunHeaderHandler>( LCSIO_RUNBLOCKNAME );
  //   runRecord->get_block_as<SIORunHeaderHandler>( LCSIO_RUNBLOCKNAME )->setRunHeader( runHeader );
  //   return runRecord;
  // }
  // 
  // //----------------------------------------------------------------------------
  // 
  // sio::record_ptr LCIORecords::createRunRecord( IOIMPL::LCRunHeaderIOImpl **runHeader ) const {
  //   auto runRecord = std::make_shared<SIO_record>( LCSIO_RUNRECORDNAME );
  //   runRecord->add_block<SIORunHeaderHandler>( LCSIO_RUNBLOCKNAME, runHeader );
  //   return runRecord;
  // }
  // 
  // //----------------------------------------------------------------------------
  // 
  // sio::record_ptr LCIORecords::createRandomAccessRecord( LCIORandomAccessMgr *raMgr ) const {
  //   auto raRecord = std::make_shared<SIO_record>( LCSIO_ACCESSRECORDNAME );
  //   raRecord->add_block<SIORandomAccessHandler>( LCSIO_ACCESSBLOCKNAME, raMgr );
  //   return raRecord;
  // }
  // 
  // //----------------------------------------------------------------------------
  // 
  // sio::record_ptr LCIORecords::createIndexRecord( LCIORandomAccessMgr *raMgr ) const {
  //   auto indexRecord = std::make_shared<SIO_record>( LCSIO_INDEXRECORDNAME );
  //   indexRecord->add_block<SIOIndexHandler>( LCSIO_INDEXBLOCKNAME, raMgr );
  //   return indexRecord;
  // }
  
} // namespace 
 
