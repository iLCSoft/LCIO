#include "SIO/SIOContext.h"

#include "SIO_functions.h"
#include "SIO_stream.h"
#include "Exceptions.h"

#include <SIO/SIORunHeaderHandler.h>
#include <SIO/SIOEventHandler.h>
#include <SIO/SIORandomAccessHandler.h>

#include <cctype>
#include <cerrno>
#include <iostream>
#include <sstream>

namespace SIO {

  SIOContext::SIOContext() {
    _allRecordNames = {
      {SIORecordType::Event,      LCSIO_EVENTRECORDNAME},
      {SIORecordType::Header,     LCSIO_HEADERRECORDNAME},
      {SIORecordType::Run,        LCSIO_RUNRECORDNAME},
      {SIORecordType::Access,     LCSIO_ACCESSRECORDNAME},
      {SIORecordType::Index,      LCSIO_INDEXRECORDNAME}
    };
  }
  
  //----------------------------------------------------------------------------
  
  void SIOContext::setDefaultCompress( bool flag ) {
    _defaultCompress = flag;
  }
  
  //----------------------------------------------------------------------------
  
  std::string SIOContext::getRecordName( SIORecordType recordType ) const {
    auto findIter = _allRecordNames.find( recordType );
    return ( _allRecordNames.end() == findIter) ? "" : findIter->second;
  }
  
  //----------------------------------------------------------------------------
  
  sio::record_ptr SIOContext::getRecord( SIORecordType recordType ) {
    auto recordName = getRecordName( recordType );
    auto findIter = _records.find( recordName );
    return ( _records.end() == findIter ) ? nullptr : findIter->second;
  }
  
  //----------------------------------------------------------------------------
  
  SIOContext &SIOContext::setCompress( bool flag, SIORecordType recordType ) {
    if( SIORecordType::All == recordType ) {
      auto accessName = getRecordName( SIORecordType::Access );
      for( auto record : _records ) {
        bool compressFlag = ( accessName == record.first ) ? false : flag;
        record.second->set_compression( compressFlag );
      }
    }
    else {
      auto recordName = getRecordName( recordType );
      auto findIter = _records.find( recordName );
      if( _records.end() != findIter ) {
        bool compressFlag = ( SIORecordType::Access == recordType ) ? false : flag;
        findIter->second->set_compression( compressFlag );
      }
    }
    return *this;
  }
  
  //----------------------------------------------------------------------------
  
  // record and block names used in LCIO-SIO
  // #define LCSIO_RUNRECORDNAME    "LCRunHeader"
  // #define LCSIO_RUNBLOCKNAME     "RunHeader"
  // #define LCSIO_EVENTRECORDNAME  "LCEvent"
  // #define LCSIO_EVENTBLOCKNAME   "Event"
  // #define LCSIO_HEADERRECORDNAME "LCEventHeader"
  // #define LCSIO_HEADERBLOCKNAME  "EventHeader"
  // 
  // #define LCSIO_ACCESSRECORDNAME "LCIORandomAccess"
  // #define LCSIO_ACCESSBLOCKNAME  "LCIORandomAccess"
  // #define LCSIO_RANDOMACCESS_SIZE 136 
  // #define LCSIO_INDEXRECORDNAME "LCIOIndex"
  // #define LCSIO_INDEXBLOCKNAME  "LCIOIndex"
  
  void SIOContext::setupLCIORecords() {
    // Event record
    // ATTN: no block prepared here.
    // Each event has a different block setup
    auto eventRecord = std::make_shared<SIO_record>( LCSIO_EVENTRECORDNAME );
    _records[ LCSIO_EVENTRECORDNAME ] = eventRecord;
    // Run header record
    auto runRecord = std::make_shared<SIO_record>( LCSIO_RUNRECORDNAME );
    runRecord->add_block<SIORunHeaderHandler>( LCSIO_RUNBLOCKNAME );
    _records[ LCSIO_RUNRECORDNAME ] = runRecord;
    // Event header record
    auto eventHeaderRecord = std::make_shared<SIO_record>( LCSIO_HEADERRECORDNAME );
    eventHeaderRecord->add_block<SIOEventHandler>( LCSIO_HEADERBLOCKNAME );
    _records[ LCSIO_HEADERRECORDNAME ] = eventHeaderRecord;
    // Index record
    auto indexRecord = std::make_shared<SIO_record>( LCSIO_INDEXRECORDNAME );
    indexRecord->add_block<SIORandomAccessHandler>( LCSIO_INDEXBLOCKNAME );
    _records[ LCSIO_INDEXRECORDNAME ] = indexRecord;
    
  }

} // namespace 
 
