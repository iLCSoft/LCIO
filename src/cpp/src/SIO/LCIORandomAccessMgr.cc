#include "SIO/LCIORandomAccessMgr.h"

#include "SIO/LCSIO.h"
#include "SIO/SIORandomAccessHandler.h"
#include "SIO/SIOIndexHandler.h"
#include "SIO/SIOEventHandler.h"
#include "SIO/SIORunHeaderHandler.h"
#include "IOIMPL/LCEventIOImpl.h"
#include "IOIMPL/LCRunHeaderIOImpl.h"
#include "Exceptions.h"

// -- std headers
#include <sstream>
#include <stdio.h>
#include <string.h>

// -- sio headers
#include <sio/api.h>
#include <sio/compression/zlib.h>

namespace SIO {

  void LCIORandomAccessMgr::clear() {
    _runEvtMap->clear() ;
    _list.clear() ;
    _fileRecord = nullptr ;
  }

  //----------------------------------------------------------------------------

  std::shared_ptr<LCIORandomAccess> LCIORandomAccessMgr::createFromEventMap() const {
    auto ra = std::make_shared<LCIORandomAccess>() ;
    ra->_minRunEvt =   _runEvtMap->minRunEvent() ;
    ra->_maxRunEvt =   _runEvtMap->maxRunEvent() ;
    ra->_nRunHeaders = _runEvtMap->getNumberOfRunRecords() ;
    ra->_nEvents =     _runEvtMap->getNumberOfEventRecords() ;
    ra->_recordsAreInOrder =  true ;  //  ???? how is this defined ????
    ra->_indexLocation = 0 ;
    ra->_prevLocation = 0 ;
    ra->_nextLocation = 0 ;
    ra->_firstRecordLocation = 0 ;
    return ra ;
  }

  //----------------------------------------------------------------------------

  void LCIORandomAccessMgr::createFileRecord() {
    if( nullptr == _fileRecord ) {
      _fileRecord = std::make_shared<LCIORandomAccess>() ;
      _fileRecord->_minRunEvt = RunEvent( 2147483647L, 2147483647L ) ; // 2**31-1  - largest 32 bit signed
      _fileRecord->_maxRunEvt = 0 ;
      _fileRecord->_nRunHeaders = 0 ;
      _fileRecord->_nEvents = 0 ;
      _fileRecord->_recordsAreInOrder = 1 ;
      _fileRecord->_indexLocation = 0 ; // defines file record
      _fileRecord->_prevLocation = 9223372036854775807LL ; // 2**63-1  - largest 64 bit signed
      _fileRecord->_nextLocation = 0 ;
      _fileRecord->_firstRecordLocation = 0 ;
    }

    for( auto i = _list.begin() ; i != _list.end() ; ++i ) {
      auto ra = *i ;
    	_fileRecord->_minRunEvt = ( ra->_minRunEvt < _fileRecord->_minRunEvt ?  ra->_minRunEvt : _fileRecord->_minRunEvt  ) ;
    	_fileRecord->_maxRunEvt = ( ra->_maxRunEvt > _fileRecord->_maxRunEvt ?  ra->_maxRunEvt : _fileRecord->_maxRunEvt  ) ;
    	_fileRecord->_nRunHeaders += ra->_nRunHeaders ;
    	_fileRecord->_nEvents += ra->_nEvents ;
    	_fileRecord->_recordsAreInOrder = ( _fileRecord->_recordsAreInOrder * ra->_recordsAreInOrder  ) ; // should be 0 if any record is 0
    	_fileRecord->_indexLocation = 0 ; // defines file record
    	if( ra->_nextLocation > _fileRecord->_nextLocation ) {
    	  _fileRecord->_nextLocation  =  ra->_nextLocation ;
      }
    	if( 0 < ra->_prevLocation  &&  ra->_prevLocation < _fileRecord->_prevLocation ) {
    	  _fileRecord->_prevLocation  =  ra->_prevLocation ;
      }
    }
  }

  //----------------------------------------------------------------------------

  void  LCIORandomAccessMgr::addLCIORandomAccess( std::shared_ptr<LCIORandomAccess> ra ) {
    _list.push_back( ra );
  }

  //----------------------------------------------------------------------------

  bool LCIORandomAccessMgr::readLCIORandomAccessAt( sio::ifstream& stream , long64 pos) {
    seekStream( stream, pos ) ;
    return readLCIORandomAccess( stream ) ;
  }

  //----------------------------------------------------------------------------

  bool LCIORandomAccessMgr::readLCIORandomAccess( sio::ifstream &stream ) {
    // first extract the record from the stream (header + data)
    sio::record_info recinfo ;
    try {
      sio::api::read_record( stream, recinfo, _rawBuffer ) ;
    }
    catch( sio::exception &e ) {
      // RE: Need to clear stream state.
      // Maybe the record is not there and we want to move ahead
      stream.clear() ;
      seekStream( stream, 0 ) ;
      return false ;
    }
    // we got a record but it's not an LCIORandomAccess record...
    if( recinfo._name != LCSIO::AccessRecordName ) {
      return false ;
    }
    // setup the rando access block
    sio::block_list blocks {} ;
    auto raBlock = std::make_shared<SIORandomAccessHandler>() ;
    blocks.push_back( raBlock ) ;
    // get a valid buffer span for reading
    auto recordData = _rawBuffer.span( recinfo._header_length, recinfo._data_length ) ;
    // read the record blocks
    sio::api::read_blocks( recordData, blocks ) ;
    // get our random access object and add it to our list
    addLCIORandomAccess( raBlock->randomAccess() ) ;
    return true ;
  }

  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------

  bool LCIORandomAccessMgr::readLCIOIndexAt( sio::ifstream &stream , long64 pos) {
    seekStream( stream, pos ) ;
    return readLCIOIndex( stream ) ;
  }

  //----------------------------------------------------------------------------

  bool LCIORandomAccessMgr::readLCIOIndex( sio::ifstream &stream ) {
    SIO_DEBUG( "LCIORandomAccessMgr::readLCIOIndex: Reading at " << stream.tellg() );
    // first extract the record from the stream (header + data)
    sio::record_info recinfo ;
    try {
      sio::api::read_record( stream, recinfo, _rawBuffer ) ;
    }
    catch( sio::exception &e ) {
      // no way to extract a record !
      // RE: Need to clear stream state.
      // Maybe the record is not there and we want to move ahead
      stream.clear() ;
      seekStream( stream, 0 ) ;
      return false ;
    }
    // we got a record but it's not an LCIOIndex record...
    if( recinfo._name != LCSIO::IndexRecordName ) {
      return false ;
    }
    // setup the rando access block
    sio::block_list blocks {} ;
    auto indexBlock = std::make_shared<SIOIndexHandler>() ;
    indexBlock->setRunEventMap( _runEvtMap ) ;
    blocks.push_back( indexBlock ) ;
    const bool compressed = sio::api::is_compressed( recinfo._options ) ;
    if( compressed ) {
      sio::buffer compBuffer( recinfo._uncompressed_length ) ;
      sio::zlib_compression compressor ;
      compressor.uncompress( _rawBuffer.span( recinfo._header_length, recinfo._data_length ), compBuffer ) ;
      // read the record blocks
      sio::api::read_blocks( compBuffer.span(), blocks ) ;
    }
    else {
      // read the record blocks
      sio::api::read_blocks( _rawBuffer.span( recinfo._header_length, recinfo._data_length ), blocks ) ;
    }
    return true ;
  }

  //----------------------------------------------------------------------------

  bool LCIORandomAccessMgr::initAppend( sio::ifstream & stream ) {
    // check if the last record is LCIORandomAccess  (the file record )
    if( ! readLCIORandomAccessAt( stream , -LCSIO::RandomAccessSize ) ) {
      recreateEventMap( stream ) ;
      return false;
    }
    // store the file record separately
    _fileRecord = _list.back() ;
    _list.pop_back()  ;
    // now read first LCIORandomAccess record
    readLCIORandomAccessAt( stream , _fileRecord->_nextLocation ) ; // start of last LCIORandomAccessRecord
    return true ;
  }

  //----------------------------------------------------------------------------

  bool LCIORandomAccessMgr::createEventMap( sio::ifstream & stream ) {
    // check if the last record is LCIORandomAccess ( the file record )
    if( ! readLCIORandomAccessAt( stream , -LCSIO::RandomAccessSize) )  {
      return recreateEventMap( stream ) ;
    }
    // store the file record separately
    _fileRecord = _list.back() ;
    _list.pop_back()  ;

    auto raPos = _fileRecord->_nextLocation ;
    // start of last LCIORandomAccessRecord
    readLCIORandomAccessAt( stream ,  raPos  ) ;
    auto ra = lastLCIORandomAccess() ;
    // and then read all remaining LCIORandomAccess records
    raPos = ra->getPrevLocation() ;
    auto indexPos = ra->getIndexLocation() ;
    readLCIOIndexAt( stream , indexPos ) ;

    while( raPos != 0 ) {
      if( readLCIORandomAccessAt( stream , raPos) ) {
        ra = lastLCIORandomAccess() ;
      	raPos = ra->getPrevLocation() ;
      	auto idxPos = ra->getIndexLocation() ;
      	readLCIOIndexAt( stream , idxPos ) ;
      }
      else {
	       throw IO::IOException( std::string( "[LCIORandomAccessMgr::ReadEventMap()] Could not read previous LCIORandomAccess record" ) ) ;
      }
    }
    seekStream( stream, 0 ) ;// go to start of file
    return true ;
  }

  //----------------------------------------------------------------------------

  bool LCIORandomAccessMgr::recreateEventMap( sio::ifstream & stream ) {

    std::cout << " LCIORandomAccessMgr::getEventMap() recreating event map for direct access (old file)" << std::endl ;
    // read the next record from the stream
    if( not stream.is_open() ) {
      throw IO::IOException( "LCIORandomAccessMgr::recreateEventMap: stream is not open" ) ;
    }
    // go to start of file
    seekStream( stream, 0 ) ;
    std::set<std::string> recordlist = { LCSIO::HeaderRecordName, LCSIO::RunRecordName } ;
    while( true ) {
      sio::record_info recinfo {} ;
      try {
        sio::api::skip_records( stream, [&]( const sio::record_info &ri ){
          return ( recordlist.find(ri._name) != recordlist.end() ) ;
        }) ;
        // we read something valid here. Read the record from the stream
        sio::api::read_record( stream, recinfo, _rawBuffer ) ;
        // Get the record data
        auto recdata = _rawBuffer.span( recinfo._header_length, recinfo._data_length ) ;
        // deal with zlib uncompression
        const bool compressed = sio::api::is_compressed( recinfo._options ) ;
        if( compressed ) {
          sio::zlib_compression compressor ;
          _compressBuffer.resize( recinfo._uncompressed_length ) ;
          compressor.uncompress( recdata, _compressBuffer ) ;
        }
        // get the correct buffer depending of compression settings
        auto databuf = compressed ? _compressBuffer.span() : recdata ;
        // event case
        if( recinfo._name == LCSIO::HeaderRecordName ) {
          auto event = std::make_shared<IOIMPL::LCEventIOImpl>() ;
          // pass a dummy list of read collection with only '__dummy__' inside.
          // We only need the event and run numbers there, no need to settup all collections
          SIOEventHeaderRecord::readBlocks( databuf, event.get(), {"__dummy__"} ) ;
          _runEvtMap->add( RunEvent( event->getRunNumber() , event->getEventNumber() ) , static_cast<long64>(recinfo._file_start) ) ;
        }
        // run case
        else if( recinfo._name == LCSIO::RunRecordName ) {
          auto run = std::make_shared<IOIMPL::LCRunHeaderIOImpl>() ;
          SIORunHeaderRecord::readBlocks( recdata, run.get() ) ;
          _runEvtMap->add( RunEvent( run->getRunNumber() , -1 ) , static_cast<long64>(recinfo._file_start) ) ;
        }
      }
      catch( sio::exception &e ) {
        // reached end of file
        if( e.code() == sio::error_code::eof ) {
          // RE: clear EOF bit in stream. Normal exit in this case
          stream.clear() ;
          break ;
        }
        SIO_RETHROW( e, e.code(), "Couldn't recreate event map !" ) ;
      }
    } // while
    // go to start of file
    seekStream( stream, 0 ) ;
    return true ;
  }

  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------

  void LCIORandomAccessMgr::writeRandomAccessRecords( sio::ofstream & stream ) {
    // nothing to write ?
    if( _runEvtMap->empty() ) {
      return ;
    }
    if( not stream.is_open() ) {
      throw IO::IOException( "[LCIORandomAccessMgr::writeRandomAccessRecords] stream not opened" ) ;
    }
    sio::block_list blocks {} ;

    // 1) write the first index in the file
    auto indexHandler = std::make_shared<SIOIndexHandler>() ;
    indexHandler->setRunEventMap( _runEvtMap ) ;
    blocks.push_back( indexHandler ) ;
    // write in the buffer first, then the buffer to the file
    auto recinfo = sio::api::write_record( LCSIO::IndexRecordName, _rawBuffer, blocks, 0 ) ;
    sio::api::write_record( stream, _rawBuffer.span(), recinfo ) ;

    // 2) create the LCIORandomAccess object ( linked list of records )
    auto ra = createFromEventMap() ;
    ra->setIndexLocation( recinfo._file_start ) ;
    long64 thisPos = stream.tellp() ;
    ra->setFirstRecordLocation(  thisPos  ) ;
    auto lRa  = lastLCIORandomAccess() ;
    SIO_DEBUG( "LCIORandomAccessMgr::writeRandomAccessRecords: lRa is " << lRa );
    long64 prevPos = (  lRa ? lRa->getFirstRecordLocation() : 0 ) ;
    ra->setPreviousLocation(  prevPos ) ;
    addLCIORandomAccess( ra ) ;

    // 3) Write the random access record
    blocks.clear() ;
    auto raHandler = std::make_shared<SIORandomAccessHandler>() ;
    raHandler->setRandomAccess( lastLCIORandomAccess() ) ;
    blocks.push_back( raHandler ) ;
    // write in the buffer first, then the buffer to the file
    recinfo = sio::api::write_record( LCSIO::AccessRecordName, _rawBuffer, blocks, 0 ) ;
    sio::api::write_record( stream, _rawBuffer.span(), recinfo ) ;

    // 4) Create the file record
    createFileRecord() ;
    if( thisPos > _fileRecord->_nextLocation ) {
      _fileRecord->_nextLocation  =  thisPos ;
    }
    if( _fileRecord->_prevLocation > thisPos ) {
      _fileRecord->_prevLocation  =  thisPos ;
    }

    // 5) Write the file random access record to stream
    raHandler->setRandomAccess( _fileRecord ) ;
    // write in the buffer first, then the buffer to the file
    recinfo = sio::api::write_record( LCSIO::AccessRecordName, _rawBuffer, blocks, 0 ) ;
    sio::api::write_record( stream, _rawBuffer.span(), recinfo ) ;

    SIO_DEBUG( "Random access manager: =====\n" << *this );
  }

  //----------------------------------------------------------------------------

  void LCIORandomAccessMgr::seekStream( sio::ifstream &stream, long64 pos ) {
    if( not stream.is_open() ) {
      throw IO::IOException( "[LCIORandomAccessMgr::seekStream] Stream not open") ;
    }
    if( pos < 0 ) {
      stream.seekg( 0 , std::ios_base::end ) ;
      auto endg = stream.tellg() ;
      if( endg < -pos ) {
        std::stringstream s ;  s << "[LCIORandomAccessMgr::seekStream] Can't seek stream to " << pos ;
        throw IO::IOException( s.str() ) ;
      }
      std::streampos tpos = -pos ;
      // pos is negative, so addition make sense here !
      stream.seekg( endg - tpos , std::ios_base::beg ) ;
    }
    else {
      stream.seekg( pos ) ;
    }
    if( not stream.good() ) {
      std::stringstream s ;  s << "[LCIORandomAccessMgr::seekStream] Can't seek stream to " << pos << ". rdstate is: " << stream.rdstate() ;
      throw IO::IOException( s.str() ) ;
    }
  }

  //----------------------------------------------------------------------------

  std::ostream& operator<<(std::ostream& os, const LCIORandomAccessMgr& ra ) {
    os << " LCIORandomAccessMgr:  ----------------------- " << std::endl   ;
    for( auto i = ra._list.begin() ; i != ra._list.end() ; ++i ) {
      os << **i ;
    }
    os  <<  *(ra._runEvtMap) << std::endl ;
    return os ;
  }
}
