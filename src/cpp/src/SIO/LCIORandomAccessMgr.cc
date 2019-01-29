#include "SIO/LCIORandomAccessMgr.h"

#include "SIO/LCSIO.h"
#include "SIO/SIORandomAccessHandler.h"
#include "SIO/SIOIndexHandler.h"
#include "SIO/SIOEventHandler.h"
#include "SIO/SIORunHeaderHandler.h"

#include "IOIMPL/LCEventIOImpl.h"
#include "IOIMPL/LCRunHeaderIOImpl.h"

#include "SIO_stream.h"

#include "Exceptions.h"
#include <sstream>

#include <stdio.h>
#include <string.h>

using namespace IO ;

namespace SIO {

  LCIORandomAccessMgr::~LCIORandomAccessMgr() {
    // cleanup 
    for( std::list<LCIORandomAccess* >::iterator i = _list.begin() ; i != _list.end() ; ++i ){
      delete *i ; 
    }
    if( nullptr != _fileRecord ) {
      delete _fileRecord ;
    }
  }
  
  //----------------------------------------------------------------------------

  void LCIORandomAccessMgr::clear() {
    _runEvtMap.clear() ;
    for( std::list<LCIORandomAccess* >::iterator i = _list.begin() ; i != _list.end() ; ++i ) { 
      delete *i ; 
    }
    _list.clear() ;
    if( nullptr != _fileRecord ) {
      delete _fileRecord ;
      _fileRecord = nullptr ;
    }
  }
  
  //----------------------------------------------------------------------------
  
  LCIORandomAccess* LCIORandomAccessMgr::createFromEventMap() {

    LCIORandomAccess* ra = new LCIORandomAccess ;
    
    ra->_minRunEvt =   _runEvtMap.minRunEvent() ;
    ra->_maxRunEvt =   _runEvtMap.maxRunEvent() ;
    ra->_nRunHeaders = _runEvtMap.getNumberOfRunRecords() ;
    ra->_nEvents =     _runEvtMap.getNumberOfEventRecords() ;
    
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
      _fileRecord = new LCIORandomAccess ;
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
    
    for( std::list<LCIORandomAccess* >::const_iterator i = _list.begin() ; i != _list.end() ; ++i ) {
      LCIORandomAccess* ra = *i ;
      
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

  void  LCIORandomAccessMgr::addLCIORandomAccess( LCIORandomAccess* ra ) {
    _list.push_back( ra ); 
  }

  //----------------------------------------------------------------------------

  bool LCIORandomAccessMgr::readLCIORandomAccessAt( SIO_stream* stream , long64 pos) {
    seekStream( stream, pos ) ;
    return readLCIORandomAccess( stream ) ;
  }

  //----------------------------------------------------------------------------

  bool LCIORandomAccessMgr::readLCIORandomAccess( SIO_stream* stream ) {

    auto raRecord = _lcioRecords.createRandomAccessRecord( this ) ;
    raRecord->set_compression(false);
    SIO_DEBUG( "LCIORandomAccessMgr::readLCIORandomAccess: Reading at " << stream->file_tell() );
    auto readResult = stream->read_next_record( {{ raRecord->name(), raRecord }} );
    if( ! (readResult._status & 1) ) {
      readResult._status = stream->reset() ;
      if( readResult._status != SIO_STREAM_SUCCESS ) {
         throw IOException( "IO error while reading LCIORandomAccess from stream" ) ;
      }
      return false ;
    }
    return true ;
  }
  
  //----------------------------------------------------------------------------

  bool LCIORandomAccessMgr::readLCIOIndexAt( SIO_stream* stream , long64 pos) {
    seekStream( stream, pos ) ;
    return readLCIOIndex( stream ) ;
  }
  
  //----------------------------------------------------------------------------

  bool LCIORandomAccessMgr::readLCIOIndex( SIO_stream* stream ) {

    auto raRecord = _lcioRecords.createIndexRecord( this ) ;
    raRecord->set_compression(false);
    auto readResult = stream->read_next_record( {{ LCSIO_INDEXRECORDNAME, raRecord }} );
    if( ! (readResult._status & 1) ) {
      readResult._status = stream->reset() ;
      if( readResult._status != SIO_STREAM_SUCCESS ) {
         throw IOException( "IO error while reading LCIORandomAccess from stream" ) ;
      }
      return false ;
    }
    return true ;
  }

  //----------------------------------------------------------------------------

  bool LCIORandomAccessMgr::initAppend( SIO_stream* stream ) {
    // check if the last record is LCIORandomAccess  (the file record )
    SIO_DEBUG( "LCIORandomAccessMgr::initAppend: reading random access" );
    if( ! readLCIORandomAccessAt( stream , -LCSIO_RANDOMACCESS_SIZE) ) {
      SIO_DEBUG( "LCIORandomAccessMgr::initAppend: No random access" );
      recreateEventMap( stream ) ; 
      return false;
    }
    // store the file record separately 
    _fileRecord = _list.back() ;
    _list.pop_back()  ;
    SIO_DEBUG( "LCIORandomAccessMgr::initAppend: Random access found !" );
    // now read first LCIORandomAccess record 
    readLCIORandomAccessAt( stream , _fileRecord->_nextLocation ) ; // start of last LCIORandomAccessRecord	
    return true ;
  }

  //----------------------------------------------------------------------------

  bool LCIORandomAccessMgr::createEventMap( SIO_stream* stream ) {
    // check if the last record is LCIORandomAccess ( the file record )
    if( ! readLCIORandomAccessAt( stream , -LCSIO_RANDOMACCESS_SIZE) )  {
      return recreateEventMap( stream ) ; 
    }
    // store the file record separately 
    _fileRecord = _list.back() ;
    _list.pop_back()  ;

    EVENT::long64 raPos = _fileRecord->_nextLocation ;
    // start of last LCIORandomAccessRecord	
    readLCIORandomAccessAt( stream ,  raPos  ) ; 
    const LCIORandomAccess* ra = lastLCIORandomAccess() ;
    // and then read all remaining LCIORandomAccess records
    raPos = ra->getPrevLocation() ;
    EVENT::long64 indexPos = ra->getIndexLocation() ;
    readLCIOIndexAt( stream , indexPos ) ;

    while( raPos != 0 ) {
      if( readLCIORandomAccessAt( stream , raPos) ){
        ra = lastLCIORandomAccess() ;
      	raPos = ra->getPrevLocation() ;	
      	EVENT::long64 idxPos = ra->getIndexLocation() ;
      	readLCIOIndexAt( stream , idxPos ) ;
      }
      else {
	       throw IOException( std::string( "[LCIORandomAccessMgr::ReadEventMap()] Could not read previous LCIORandomAccess record" ) ) ;
      }      
    }
    seekStream( stream, 0 ) ;// go to start of file
    return true ;
  }
  
  //----------------------------------------------------------------------------

  bool LCIORandomAccessMgr::recreateEventMap( SIO_stream* stream ) {

    std::cout << " LCIORandomAccessMgr::getEventMap() recreating event map for direct access (old file)" << std::endl ;
    // go to start of file
    seekStream( stream, 0 ) ;

    IOIMPL::LCEventIOImpl* evtPtr {nullptr} ;
    IOIMPL::LCRunHeaderIOImpl* runPtr {nullptr} ;
    auto headerRecord = _lcioRecords.createEventHeaderRecord( evtPtr ) ;
    auto runRecord = _lcioRecords.createRunRecord( runPtr ) ;
    
    sio::record_map records = { {headerRecord->name(), headerRecord}, {runRecord->name(), runRecord} } ;
    
    while( true ) {
      // read the next record from the stream
      if( stream->state() != SIO_STATE_OPEN ) {
        throw IOException( " stream not open" ) ;
      } 
      auto readResult = stream->read_next_record( records ) ;
      if( ! (readResult._status & 1) ) {
        if( readResult._status & SIO_STREAM_EOF ) {
          break ;
        }
        throw IOException( " io error on stream" ) ;
      }

      //--
      int runNum = -1 ;
      int evtNum = -1 ;
      
      if( readResult._record->name() == LCSIO_HEADERRECORDNAME ) {
        runNum = evtPtr->getRunNumber() ;
        evtNum = evtPtr->getEventNumber() ;
      }
      if( readResult._record->name() == LCSIO_RUNRECORDNAME ) {
        runNum = runPtr->getRunNumber() ;
      }
      _runEvtMap.add(   RunEvent( runNum , evtNum ) , readResult._record_begin ) ;
    } // while
    // go to start of file
    seekStream( stream, 0 ) ;
    return true ;
  }
  
  //----------------------------------------------------------------------------
  
  void LCIORandomAccessMgr::writeRandomAccessRecords(SIO_stream* stream) {
    // nothing to write
    if( _runEvtMap.empty() ) { 
      return ;       
    }
    auto raRecord = _lcioRecords.createRandomAccessRecord( this ) ;
    raRecord->set_compression( false ) ;
    auto indexRecord = _lcioRecords.createIndexRecord( this ) ;
    indexRecord->set_compression( false ) ;
    
    if( stream->state() != SIO_STATE_OPEN ){
      throw IOException( "[LCIORandomAccessMgr::writeRandomAccessRecords] stream not opened" ) ;
    }
    
    auto writeResult = stream->write_record( indexRecord );
    
    if( !(writeResult._status & 1)  ) {
      throw IOException( "[LCIORandomAccessMgr::writeRandomAccessRecords] couldn't write LCIOIndex to stream" ) ;      
    }
    // create the LCIORandomAccess object ( linked list of records ) 
    LCIORandomAccess* ra = createFromEventMap() ;
    ra->setIndexLocation( writeResult._record_begin ) ;
    long64 thisPos = stream->file_tell() ;
    ra->setFirstRecordLocation(  thisPos  ) ;
    
    const LCIORandomAccess* lRa  = lastLCIORandomAccess() ;
    SIO_DEBUG( "LCIORandomAccessMgr::writeRandomAccessRecords: lRa is " << lRa );
    long64 prevPos = (  lRa ? lRa->getFirstRecordLocation() : 0 ) ;
    ra->setPreviousLocation(  prevPos ) ;
    
    addLCIORandomAccess( ra ) ; 
    
    // write LCAccess record
    writeResult = stream->write_record( raRecord ) ;
    
    if( !(writeResult._status & 1)  ) {
      throw IOException( "[LCIORandomAccessMgr::writeRandomAccessRecords] couldn't write LCIORandomAccess to stream" ) ;
    }
    
    createFileRecord() ;

    if( thisPos > _fileRecord->_nextLocation ) {
      _fileRecord->_nextLocation  =  thisPos ;
    }
    if( _fileRecord->_prevLocation > thisPos ) {
      _fileRecord->_prevLocation  =  thisPos ;
    }

    addLCIORandomAccess( _fileRecord ) ; 
    
    writeResult = stream->write_record( raRecord ) ;
    SIO_DEBUG( "Random access manager: =====\n" << *this );
    
    if( !(writeResult._status & 1)  ) {
      throw IOException( "[LCIORandomAccessMgr::writeRandomAccessRecords] couldn't write LCIORandomAccess file record to stream" ) ;
    }
    _list.pop_back()  ;
  }
  
  //----------------------------------------------------------------------------
  
  void LCIORandomAccessMgr::seekStream( SIO_stream *stream, long64 pos ) {
    if( stream->state() != SIO_STATE_OPEN ) {
      throw IO::IOException( "[LCIORandomAccessMgr::seekStream] Stream not open") ;
    }
    SIO_DEBUG( "LCIORandomAccessMgr::seekStream: seeking at " << pos << ", current position being " << stream->file_tell() );
    int status ;
    if( pos < 0 ) {
      status = stream->seek( pos , SEEK_END ) ;
    }
    else {
      status = stream->seek( pos ) ;
    }    
    SIO_DEBUG( "LCIORandomAccessMgr::seekStream: current position is now " << stream->file_tell() );
    if( status != SIO_STREAM_SUCCESS ) {
      std::stringstream s ;  s << "[LCIORandomAccessMgr::seekStream] Can't seek stream to " << pos << "  errno : " << errno ;
      throw IO::IOException( s.str() ) ;
    }
  }
  
  //----------------------------------------------------------------------------

  std::ostream& operator<<(std::ostream& os, const LCIORandomAccessMgr& ra ) {
    os << " LCIORandomAccessMgr:  ----------------------- " << std::endl   ;
    for( std::list<LCIORandomAccess* >::const_iterator i = ra._list.begin() ; i != ra._list.end() ; ++i ){
      os << **i ;
    }
    os  <<  ra._runEvtMap << std::endl ;
    return os ;
  }
}
