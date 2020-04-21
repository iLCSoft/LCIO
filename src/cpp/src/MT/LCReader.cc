#include "MT/LCReader.h"

// -- lcio headers
#include "SIO/LCSIO.h"
#include "SIO/SIOParticleHandler.h"
#include "SIO/LCIORecords.h"
#include "SIO/LCIORandomAccessMgr.h"
#include "IOIMPL/LCEventIOImpl.h"
#include "IOIMPL/LCEventLazyImpl.h"
#include "IOIMPL/LCRunHeaderIOImpl.h"
#include "Exceptions.h"
#include "SIO/RunEventMap.h"
#include "SIO/SIOHandlerMgr.h"

// -- sio headers
#include "sio/definitions.h"
#include "sio/buffer.h"
#include <sio/exception.h>
#include <sio/api.h>
#include <sio/compression/zlib.h>

// -- std headers
#include <sys/stat.h>
#include <assert.h>
#include <limits>

typedef EVENT::long64 long64 ;

namespace MT {

  LCReader::LCReader( int lcReaderFlag ) :
    _rawBuffer( std::make_shared<sio::buffer>(1*sio::mbyte) ),
    _compBuffer( std::make_shared<sio::buffer>(2*sio::mbyte) ),
    _eventHandlerMgr( std::make_shared<SIO::SIOHandlerMgr>() ),
    _readEventMap( lcReaderFlag & LCReader::directAccess ),
    _lazyUnpack( lcReaderFlag & LCReader::lazyUnpack ),
    _raMgr(std::make_shared<SIO::LCIORandomAccessMgr>()) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void LCReader::open( const std::string& filename ) {
    if( _stream.is_open() ) {
      _stream.close() ;
    }
    _stream.open( filename , std::ios::binary ) ;
    if( not _stream.is_open() ) {
      SIO_THROW( sio::error_code::not_open, "Couldn't open input stream '" + filename + "'" ) ;
    }
    if( _readEventMap ) {
      getEventMap() ;
    }
    // We are in single file mode ...
    if( _myFilenames.empty() ) {
      _myFilenames.push_back( filename ) ;
    }
  }

  //----------------------------------------------------------------------------

  void LCReader::open( const std::vector<std::string>& filenames ) {
    if( filenames.empty() ) {
      throw IO::IOException( "[LCReader::open()] Provided file list is empty") ;
    }
    struct stat fileinfo ;
    std::string missing_files ;
    // JE: first we check if all files exist
    for(unsigned int i=0 ; i < filenames.size(); i++) {
      if ( ::stat( filenames[i].c_str(), &fileinfo ) != 0 ) {
	       missing_files += filenames[i] ;
	       missing_files += "  " ;
      }
    }
    // JE: if not raise IOException
    if( missing_files.size() != 0 ) {
      throw IO::IOException( std::string( "[LCReader::open()] File(s) not found:  " + missing_files )) ;
    }
    _myFilenames = filenames ;
    _currentFileIndex = 0 ;
    open( _myFilenames[0] ) ;
  }

  //----------------------------------------------------------------------------

  std::unique_ptr<EVENT::LCRunHeader> LCReader::readNextRunHeader( int accessMode ) {
    std::unique_ptr<EVENT::LCRunHeader> runhdr = nullptr ;
    auto validator = [&]( const sio::record_info &recinfo ) {
      // we want a run header record only
      return ( recinfo._name == SIO::LCSIO::RunRecordName ) ;
    } ;
    auto processor = [&]( const sio::record_info &recinfo, const sio::buffer_span& recdata ) {
      const bool compressed = sio::api::is_compressed( recinfo._options ) ;
      if( compressed ) {
        _compBuffer->resize( recinfo._uncompressed_length ) ;
        sio::zlib_compression compressor ;
        compressor.uncompress( recdata, *_compBuffer ) ;
      }
      auto rundata = compressed ? _compBuffer->span() : recdata ;
      auto runheader = std::make_unique<IOIMPL::LCRunHeaderIOImpl>() ;
      SIO::SIORunHeaderRecord::readBlocks( rundata, runheader.get() ) ;
      runheader->setReadOnly( accessMode == EVENT::LCIO::READ_ONLY ) ;
      runheader->parameters().setValue( "LCIOFileName" ,  _myFilenames[ _currentFileIndex  ] ) ;
      runhdr = std::move(runheader) ;
      return false ; // only one record
    } ;
    try {
      sio::api::read_records( _stream, *_rawBuffer, validator , processor ) ;
    }
    catch( sio::exception &e ) {
      // reached end of file. Need to close the current and open the next if available
      if( e.code() == sio::error_code::eof ) {
        if( !_myFilenames.empty()  && _currentFileIndex+1 < _myFilenames.size()  ) {
          close() ;
          open( _myFilenames[ ++_currentFileIndex  ] ) ;
          try {
            return readNextRunHeader(accessMode) ;            
          }
          catch( sio::exception &e2 ) {
            if( e2.code() == sio::error_code::eof ) {
              return nullptr ;
            }
            SIO_RETHROW( e2, e2.code(), "Couldn't read next run header!" ) ;
          }
        }
        return nullptr ;
      }
      SIO_RETHROW( e, e.code(), "Couldn't read next run header!" ) ;
    }
    return runhdr ;
  }

  //----------------------------------------------------------------------------

  std::unique_ptr<EVENT::LCEvent> LCReader::readNextEvent( int accessMode )  {
    IOIMPL::LCEventLazyImpl* lazyEvent = nullptr ;
    std::unique_ptr<IOIMPL::LCEventIOImpl> event = nullptr ;
    auto validator = [&]( const sio::record_info &recinfo ) {
      return ( recinfo._name == SIO::LCSIO::HeaderRecordName || recinfo._name == SIO::LCSIO::EventRecordName ) ;
    } ;
    auto processor = [&]( const sio::record_info &recinfo, const sio::buffer_span& recdata ) {
      const bool compressed = sio::api::is_compressed( recinfo._options ) ;
      // do not run uncompression if we have an event record and lazy unpacking... 
      const bool uncomp = ( compressed && not (recinfo._name == SIO::LCSIO::EventRecordName && _lazyUnpack) ) ;
      if( uncomp ) {
        _compBuffer->resize( recinfo._uncompressed_length ) ;
        sio::zlib_compression compressor ;
        compressor.uncompress( recdata, *_compBuffer ) ;
      }
      auto data = uncomp ? _compBuffer->span() : recdata ;
      if( recinfo._name == SIO::LCSIO::HeaderRecordName ) {
        if( _lazyUnpack ) {
          lazyEvent = new IOIMPL::LCEventLazyImpl() ;
          event.reset( lazyEvent ) ;
        }
        else {
          event = std::make_unique<IOIMPL::LCEventIOImpl>() ;
        }
        SIO::SIOEventHeaderRecord::readBlocks( data, event.get(), _readCollectionNames ) ;
        return true ;
      }
      else if( recinfo._name == SIO::LCSIO::EventRecordName ) {
        if( nullptr == event ) {
          throw IO::IOException( "SIOReader::readNextEvent: Event record before an EventHeader record ..." ) ;
        }
        if( _lazyUnpack ) {
          _bufferMaxSize = std::max( _bufferMaxSize, _rawBuffer->size() ) ;
          // move the buffer to the event
          lazyEvent->setBuffer( recinfo, std::move(*_rawBuffer) ) ;
          // re-allocate a new valid buffer
          *_rawBuffer = sio::buffer( _bufferMaxSize ) ;
        }
        else {
          SIO::SIOEventRecord::readBlocks( data, event.get(), *_eventHandlerMgr ) ;
        }
        return false ;
      }
      return false ;
    } ;
    try {
      sio::api::read_records( _stream, *_rawBuffer, validator, processor ) ;
    }
    catch( sio::exception &e ) {
      // reached end of file. Need to close the current and open the next if available
      if( e.code() == sio::error_code::eof ) {
        if( !_myFilenames.empty()  && _currentFileIndex+1 < _myFilenames.size()  ) {
          close() ;
          open( _myFilenames[ ++_currentFileIndex  ] ) ;
          try {
            return readNextEvent(accessMode) ;            
          }
          catch( sio::exception &e2 ) {
            if( e2.code() == sio::error_code::eof ) {
              return nullptr ;
            }
            SIO_RETHROW( e2, e2.code(), "Couldn't read next event!" ) ;
          }
        }
        return nullptr ;
      }
      SIO_RETHROW( e, e.code(), "Couldn't read next event!" ) ;
    }
    if( nullptr != event ) {
      event->setAccessMode( accessMode ) ;
      if( not _lazyUnpack ) {
        postProcessEvent( event.get() ) ;        
      }
    }
    return event ;
  }

  //----------------------------------------------------------------------------

  int LCReader::getNumberOfEvents()  {
    // create the event map if needed (i.e. not opened in direct access mode)
    if( ! _readEventMap ) {
      _readEventMap = true ;
      getEventMap() ;
    }
    return _raMgr->getEventMap()->getNumberOfEventRecords()  ;
  }

  //----------------------------------------------------------------------------

  int LCReader::getNumberOfRuns() {
    // create the event map if needed (i.e. not opened in direct access mode)
    if( ! _readEventMap ){
      _readEventMap = true ;
      getEventMap() ;
    }
    return _raMgr->getEventMap()->getNumberOfRunRecords() ;
  }

  //----------------------------------------------------------------------------

  void LCReader::getRuns(EVENT::IntVec & runs) {
    int nRun = this->getNumberOfRuns() ;
    runs.resize( nRun ) ;
    auto map = _raMgr->getEventMap() ;
    auto it = map->begin() ;

    for(int i=0 ; i <nRun ; ++i, ++it) {
      runs[i] = it->first.RunNum ;
      assert(  it->first.EvtNum  == -1 ) ;
    }
  }

  //----------------------------------------------------------------------------

  void LCReader::getEvents(EVENT::IntVec & events) {
    int nRun = this->getNumberOfRuns() ;
    int nEvt = this->getNumberOfEvents() ;
    events.resize(  2 * nEvt ) ;
    auto map = _raMgr->getEventMap() ;
    auto it = map->begin() ;
    // This line should be equivalent to the one commented after
    std::advance( it, nRun );
    // for(int i=0 ; i <nRun ; ++i , ++it ) ;
    for(int i=0 ; i < nEvt ; ++i , ++it ) {
      events[ 2*i     ] = it->first.RunNum ;
      events[ 2*i + 1 ] = it->first.EvtNum ;
      assert( it->first.EvtNum   != -1 ) ;
    }
  }

  //----------------------------------------------------------------------------

  void LCReader::setReadCollectionNames( const std::vector<std::string>& colnames ) {
    _readCollectionNames = colnames ;
  }

  //----------------------------------------------------------------------------

  void LCReader::skipNEvents( int n ) {
    if( n < 1 ) { // nothing to skip
      return ;
    }
    int eventsSkipped = 0 ;
    try {
      // we skip n event header records
      sio::api::skip_records( _stream, [&] ( const sio::record_info &recinfo ) {
        if( recinfo._name == SIO::LCSIO::HeaderRecordName ) {
          ++eventsSkipped ;
        }
        return ( eventsSkipped < n ) ;
      } ) ;
      // we need to skip one more record here which is an event record
      sio::api::skip_n_records( _stream, 1 ) ;
    }
    catch( sio::exception &e ) {
      if( e.code() != sio::error_code::eof ) {
        SIO_RETHROW( e, e.code(), "SIOReader::skipNEvents: Couldn't skip events" ) ;
      }
    }
  }

  //----------------------------------------------------------------------------

  std::unique_ptr<EVENT::LCRunHeader> LCReader::readRunHeader( int runNumber, int accessMode ) {
    if( _readEventMap ) {
      EVENT::long64 pos = _raMgr->getPosition( SIO::RunEvent( runNumber, -1 ) ) ;
      if( pos != SIO::RunEventMap::npos ) {
	      _stream.seekg( pos ) ;
        if( not _stream.good() ) {
          throw IO::IOException( "[SIOReader::readRunHeader()] Can't seek stream to requested position" ) ;
        }
	      return readNextRunHeader( accessMode ) ;
      }
      else {
	      return nullptr ;
      }
    }
    else {  // no event map ------------------
      std::cout << " WARNING : LCReader::readRunHeader(run) called but not in direct access Mode  - " << std::endl
		            << " Too avoid this WARNING create the LCReader with: " << std::endl
		            << "       LCFactory::getInstance()->createLCReader( IO::LCReader::directAccess ) ; " << std::endl ;
    }
    return nullptr ;
  }

  //----------------------------------------------------------------------------

  std::unique_ptr<EVENT::LCEvent> LCReader::readEvent( int runNumber, int evtNumber, int accessMode ) {
    if( _readEventMap ) {
      EVENT::long64 pos = _raMgr->getPosition( SIO::RunEvent( runNumber,evtNumber ) ) ;
      if( pos != SIO::RunEventMap::npos ) {
	      _stream.seekg( pos ) ;
	      if( not _stream.good() ) {
	        throw IO::IOException( "[LCReader::readEvent()] Can't seek stream to requested position" ) ;
        }
	      return readNextEvent( accessMode ) ;
      }
      else {
	      return 0 ;
      }
    }
    else {  // no event map ------------------
      std::cout << " WARNING : LCReader::readEvent(run,evt) called but not in direct access Mode  - " << std::endl
		            << " use fast skip mechanism instead ..." << std::endl
		            << " Too avoid this WARNING create the LCReader with: " << std::endl
		            << "       LCFactory::getInstance()->createLCReader( IO::LCReader::directAccess ) ; " << std::endl ;
      // ---- OLD code with fast skip -----------
      bool evtFound = false ;
      // look for the specific event in the stream directly. Very slow ...
      std::unique_ptr<IOIMPL::LCEventIOImpl> event = nullptr ;
      IOIMPL::LCEventLazyImpl* lazyEvent = nullptr ;
      auto validator = [&]( const sio::record_info &recinfo ) {
        return ( recinfo._name == SIO::LCSIO::HeaderRecordName || recinfo._name == SIO::LCSIO::EventRecordName ) ;
      } ;
      auto processor = [&]( const sio::record_info &recinfo, const sio::buffer_span& recdata ) {
        const bool compressed = sio::api::is_compressed( recinfo._options ) ;
        const bool uncomp = ( compressed && not (recinfo._name == SIO::LCSIO::EventRecordName && _lazyUnpack) ) ;
        if( uncomp ) {
          _compBuffer->resize( recinfo._uncompressed_length ) ;
          sio::zlib_compression compressor ;
          compressor.uncompress( recdata, *_compBuffer ) ;
        }
        auto data = uncomp ? _compBuffer->span() : recdata ;
        if( recinfo._name == SIO::LCSIO::HeaderRecordName ) {
          if( _lazyUnpack ) {
            lazyEvent = new IOIMPL::LCEventLazyImpl() ;
            event.reset( lazyEvent ) ;
          }
          else {
            event = std::make_unique<IOIMPL::LCEventIOImpl>() ;
          }
          SIO::SIOEventHeaderRecord::readBlocks( data, event.get(), _readCollectionNames ) ;
          return true ;
        }
        else if( recinfo._name == SIO::LCSIO::EventRecordName ) {
          if( nullptr == event ) {
            throw IO::IOException( "SIOReader::readNextEvent: Event record before an EventHeader record ..." ) ;
          }
          // if we've found the requested event, we unpack the blocks then
          if( event->getEventNumber() == evtNumber && event->getRunNumber() == runNumber ) {
            if( _lazyUnpack ) {
              _bufferMaxSize = std::max( _bufferMaxSize, _rawBuffer->size() ) ;
              // move the buffer to the event
              lazyEvent->setBuffer( recinfo, std::move(*_rawBuffer) ) ;
              // re-allocate a new valid buffer
              *_rawBuffer = sio::buffer( _bufferMaxSize ) ;
            }
            else {
              SIO::SIOEventRecord::readBlocks( data, event.get(), *_eventHandlerMgr ) ;
            }
            evtFound = true ;
            // event found ! stop here !
            return false ;
          }
          // not the correct event number, continue looking ...
          return true ;
        }
        return false ;
      } ;
      try {
        sio::api::read_records( _stream, *_rawBuffer, validator, processor ) ;
      }
      catch( sio::exception &e ) {
        if( e.code() != sio::error_code::eof ) {
          SIO_RETHROW( e, e.code(), "Exception caucht while searching for event!" ) ;
        }
      }
      if( not evtFound ) {
        return nullptr ;
      }
      event->setAccessMode( EVENT::LCIO::READ_ONLY ) ;
      if( not _lazyUnpack ) {
        postProcessEvent( event.get() ) ;
      }
      return event ;
    } //-- end fast skip
  }

  //----------------------------------------------------------------------------

  void LCReader::close() {
    _raMgr->clear() ;
    _readEventMap = false ;
  }

  //----------------------------------------------------------------------------

  void LCReader::readStream( const LCReaderListenerList & listeners ) {
    readStream( listeners, std::numeric_limits<int>::max() ) ;
  }

  //----------------------------------------------------------------------------

  void LCReader::readStream( const LCReaderListenerList & listeners, int maxRecord ) {
    const bool readUntilEOF = (maxRecord == std::numeric_limits<int>::max()) ;
    std::set<std::string> records = { SIO::LCSIO::RunRecordName, SIO::LCSIO::HeaderRecordName, SIO::LCSIO::EventRecordName } ;
    std::shared_ptr<IOIMPL::LCEventIOImpl> event = nullptr ;
    std::shared_ptr<IOIMPL::LCEventLazyImpl> lazyEvent = nullptr ;
    int recordsRead = 0 ;
    // loop over records in the stream. SIO does it nicely for us
    auto validator = [&] ( const sio::record_info &recinfo ) {
      return ( records.find( recinfo._name ) != records.end() ) ;
    } ;
    auto processor = [&] ( const sio::record_info &recinfo, const sio::buffer_span& recdata ) {
      const bool compressed = sio::api::is_compressed( recinfo._options ) ;
      const bool uncomp = ( compressed && not (recinfo._name == SIO::LCSIO::EventRecordName && _lazyUnpack) ) ;
      if( uncomp ) {
        _compBuffer->resize( recinfo._uncompressed_length ) ;
        sio::zlib_compression compressor ;
        compressor.uncompress( recdata, *_compBuffer ) ;
      }
      auto data = uncomp ? _compBuffer->span() : recdata ;
      // LCRunHeader case
      if( recinfo._name == SIO::LCSIO::RunRecordName ) {
        recordsRead++ ;
        auto runheader = std::make_shared<IOIMPL::LCRunHeaderIOImpl>() ;
        SIO::SIORunHeaderRecord::readBlocks( data, runheader.get() ) ;
        runheader->parameters().setValue( "LCIOFileName" ,  _myFilenames[ _currentFileIndex  ] ) ;
        auto iter = listeners.begin() ;
        while( iter != listeners.end() ){
          runheader->setReadOnly( false ) ;
          (*iter)->processRunHeader( runheader ) ;
          iter++ ;
        }
      }
      // Event header case. Setup the event for the next record
      else if( recinfo._name == SIO::LCSIO::HeaderRecordName ) {
        if( _lazyUnpack ) {
          lazyEvent = std::make_shared<IOIMPL::LCEventLazyImpl>() ;
          event = lazyEvent ;
        }
        else {
          event = std::make_shared<IOIMPL::LCEventIOImpl>() ;
        }
        SIO::SIOEventHeaderRecord::readBlocks( data, event.get(), _readCollectionNames ) ;
      }
      else if( recinfo._name == SIO::LCSIO::EventRecordName ) {
        if( nullptr == event ) {
          throw IO::IOException( "SIOReader::readStream: Event record before an EventHeader record ..." ) ;
        }
        recordsRead++ ;
        if( _lazyUnpack ) {
          _bufferMaxSize = std::max( _bufferMaxSize, _rawBuffer->size() ) ;
          // move the buffer to the event
          lazyEvent->setBuffer( recinfo, std::move(*_rawBuffer) ) ;
          // re-allocate a new valid buffer
          *_rawBuffer = sio::buffer( _bufferMaxSize ) ;
        }
        else {
          SIO::SIOEventRecord::readBlocks( data, event.get(), *_eventHandlerMgr ) ;
        }
        auto iter = listeners.begin() ;
        while( iter != listeners.end() ) {
          if( not _lazyUnpack ) {
            postProcessEvent( event.get() ) ;
          }
          event->setAccessMode( EVENT::LCIO::UPDATE ) ;
          (*iter)->processEvent( event ) ;
          iter++ ;
        }
        // free ressources here
        event = nullptr ;
      }
      return (recordsRead < maxRecord) ;
    } ;
    try {
      sio::api::read_records( _stream, *_rawBuffer, validator , processor ) ;
    }
    catch( sio::exception &e ) {
      if( e.code() != sio::error_code::eof ) {
        SIO_RETHROW( e, e.code(), "SIOReader::readStream: Couldn't read stream" ) ;
      }
      else {
        // we caught an eof exception here
        bool nextFileAvailable = (!_myFilenames.empty()  && _currentFileIndex+1 < _myFilenames.size()) ;
        if( nextFileAvailable ) {
          close() ;
          open( _myFilenames[ ++_currentFileIndex  ] ) ;
          if( readUntilEOF ) {
            // read all
            readStream( listeners, maxRecord ) ;
          }
          else {
            // read the remaining number of records
            readStream( listeners, maxRecord - recordsRead ) ;
          }
          return ;
        }
        else {
          if( readUntilEOF ) {
            return ;
          }
            std::stringstream message ;
            message << "SIOReader::readStream(int maxRecord) : EOF before "
              << maxRecord << " records read from file" << std::ends ;
            throw IO::EndOfDataException( message.str() )  ;
          }
        }
      }
    }

  //----------------------------------------------------------------------------

  void LCReader::readNextRecord( const LCReaderListenerList & listeners ) {
    readStream( listeners, 1 );
  }

  //----------------------------------------------------------------------------

  void LCReader::readStream( LCReaderListener *listener ) {
    readStream( LCReaderListenerList{listener} ) ;
  }

  //----------------------------------------------------------------------------

  void LCReader::readStream( LCReaderListener *listener, int maxRecord ) {
    readStream( LCReaderListenerList{listener}, maxRecord );
  }

  //----------------------------------------------------------------------------

  void LCReader::readNextRecord( LCReaderListener *listener ) {
    readNextRecord( LCReaderListenerList{listener} );
  }

  //----------------------------------------------------------------------------

  void LCReader::postProcessEvent( EVENT::LCEvent *evt ) {
    // restore the daughter relations from the parent relations
    SIO::SIOParticleHandler::restoreParentDaughterRelations( evt ) ;
    // check subset collection's pointers
    char* rColChar = getenv ("LCIO_IGNORE_NULL_IN_SUBSET_COLLECTIONS") ;
    if( nullptr != rColChar ) {
      return;
    }
    const std::vector< std::string >* strVec = evt->getCollectionNames() ;
    for( auto name = strVec->begin() ; name != strVec->end() ; name++) {
      EVENT::LCCollection* col = evt->getCollection( *name ) ;
      if( col->isSubset() ) {
        for( int i=0,N=col->getNumberOfElements() ; i<N ; ++i ) {
          if( col->getElementAt( i ) == 0 ) {
            std::stringstream sts ;
            sts << " SIOReader::postProcessEvent: null pointer in subset collection "
                << *name << " at position: " << i  << std::endl ;
            throw EVENT::Exception( sts.str()  ) ;
          }
        }
      }
    }
  }

  //----------------------------------------------------------------------------

  void LCReader::getEventMap() {
    _raMgr->createEventMap( _stream ) ;
  }

} // namespace
