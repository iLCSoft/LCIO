#include "SIO/SIOReader.h"

// -- lcio headers
#include "SIO/LCSIO.h"
#include "SIO/SIOParticleHandler.h"
#include "Exceptions.h"

#include <sio/exception.h>
#include <sio/api.h>
#include <sio/compression/zlib.h>

#include <sys/stat.h>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <assert.h>

namespace SIO {


  SIOReader::SIOReader( int lcReaderFlag ) :
    _readEventMap( lcReaderFlag & IO::LCReader::directAccess  ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  SIOReader::~SIOReader() {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIOReader::setReadCollectionNames(const std::vector<std::string>& colnames){
    _readCollectionNames = colnames ;
  }

  //----------------------------------------------------------------------------

  void SIOReader::open(const std::vector<std::string>& filenames) {
    if( filenames.empty() ) {
      throw IO::IOException( "[SIOReader::open()] Provided file list is empty") ;
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
      throw IO::IOException( std::string( "[SIOReader::open()] File(s) not found:  " + missing_files )) ;
    }
    _myFilenames = filenames ;
    _currentFileIndex = 0 ;
    open( _myFilenames[0] ) ;
  }

  //----------------------------------------------------------------------------

  void SIOReader::open(const std::string& filename)  {
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

  void SIOReader::getEventMap() {
    _raMgr.createEventMap( _stream ) ;
  }

  //----------------------------------------------------------------------------

  int SIOReader::getNumberOfEvents()  {
    // create the event map if needed (i.e. not opened in direct access mode)
    if( ! _readEventMap ) {
      _readEventMap = true ;
      getEventMap() ;
    }
    return _raMgr.getEventMap()->getNumberOfEventRecords() ;
  }

  //----------------------------------------------------------------------------

  int SIOReader::getNumberOfRuns()  {
    // create the event map if needed (i.e. not opened in direct access mode)
    if( ! _readEventMap ) {
      _readEventMap = true ;
      getEventMap() ;
    }
    return _raMgr.getEventMap()->getNumberOfRunRecords() ;
  }

  //----------------------------------------------------------------------------

  void SIOReader::getRuns(EVENT::IntVec & runs) {
    int nRun = this->getNumberOfRuns() ;
    runs.resize( nRun ) ;
    auto map = _raMgr.getEventMap() ;
    auto it = map->begin() ;

    for(int i=0 ; i <nRun ; ++i, ++it) {
      runs[i] = it->first.RunNum ;
      assert(  it->first.EvtNum  == -1 ) ;
    }
  }

  //----------------------------------------------------------------------------

  void SIOReader::getEvents(EVENT::IntVec & events) {
    int nRun = this->getNumberOfRuns() ;
    int nEvt = this->getNumberOfEvents() ;
    events.resize(  2 * nEvt ) ;
    auto map = _raMgr.getEventMap() ;
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

  // void SIOReader::readRecord( const sio::record_map &records , sio::record_read_result &readResult ) {
  //   if( _stream->state() == SIO_STATE_OPEN ) {
  //     // read the next record from the stream
  //     readResult = _stream->read_next_record( records ) ;
  //     if( ! (readResult._status & 1) ) {
  //       if( readResult._status & SIO_STREAM_EOF ) {
  //         // if we have a list of filenames open the next file
  //         if( !_myFilenames.empty()  && _currentFileIndex+1 < _myFilenames.size() ) {
  //           close() ;
  //           open( _myFilenames[ ++_currentFileIndex  ] ) ;
  //           readRecord( records, readResult ) ;
  //           return ;
  //         }
  //         throw EndOfDataException("EOF") ;
  //       }
  //       throw IOException( std::string(" io error on stream: ") + _stream->file_name() ) ;
  //     }
  //   }
  //   else {
  //     throw IOException( " stream not opened" ) ;
  //   }
  // }

  //----------------------------------------------------------------------------

  EVENT::LCRunHeader* SIOReader::readNextRunHeader()  {
    return readNextRunHeader( EVENT::LCIO::READ_ONLY ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCRunHeader* SIOReader::readNextRunHeader(int accessMode) {
    try {
      sio::api::read_records( _stream, _rawBuffer,
        [&]( const sio::record_info &recinfo ) {
          // we want a run header record only
          return ( recinfo._name == LCSIO::RunRecordName ) ;
        },
        [&]( const sio::record_info &recinfo, const sio::buffer_span& recdata ) {
          const bool compressed = sio::api::is_compressed( recinfo._options ) ;
          if( compressed ) {
            _compBuffer.resize( recinfo._uncompressed_length ) ;
            sio::zlib_compression compressor ;
            compressor.uncompress( recdata, _compBuffer ) ;
          }
          auto rundata = compressed ? _compBuffer.span() : recdata ;
          auto runheader = std::make_shared<IOIMPL::LCRunHeaderIOImpl>() ;
          SIORunHeaderRecord::readBlocks( rundata, runheader.get() ) ;
          runheader->setReadOnly( accessMode == EVENT::LCIO::READ_ONLY ) ;
          runheader->parameters().setValue( "LCIOFileName" ,  _myFilenames[ _currentFileIndex  ] ) ;
          _runHeader = runheader ;
          return false ; // only one record
        }
      ) ;      
    }
    catch( sio::exception &e ) {
      if( e.code() == sio::error_code::eof ) {
        return nullptr ;
      }
      SIO_RETHROW( e, e.code(), "Couldn't read next run header!" ) ;
    }

    return _runHeader.get() ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCEvent* SIOReader::readNextEvent()  {
    return readNextEvent( EVENT::LCIO::READ_ONLY ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCEvent* SIOReader::readNextEvent(int accessMode)  {
    std::shared_ptr<IOIMPL::LCEventIOImpl> event = nullptr ;
    try {
      sio::api::read_records( _stream, _rawBuffer,
        [&]( const sio::record_info &recinfo ) {
          return ( recinfo._name == LCSIO::HeaderRecordName || recinfo._name == LCSIO::EventRecordName ) ;
        },
        [&]( const sio::record_info &recinfo, const sio::buffer_span& recdata ) {
          const bool compressed = sio::api::is_compressed( recinfo._options ) ;
          if( compressed ) {
            _compBuffer.resize( recinfo._uncompressed_length ) ;
            sio::zlib_compression compressor ;
            compressor.uncompress( recdata, _compBuffer ) ;
          }
          auto data = compressed ? _compBuffer.span() : recdata ;
          if( recinfo._name == LCSIO::HeaderRecordName ) {
            event = std::make_shared<IOIMPL::LCEventIOImpl>() ;
            SIOEventHeaderRecord::readBlocks( data, event.get(), _readCollectionNames ) ;
            return true ;
          }
          else if( recinfo._name == LCSIO::EventRecordName ) {
            if( nullptr == event ) {
              throw IO::IOException( "SIOReader::readNextEvent: Event record before an EventHeader record ..." ) ;
            }
            SIOEventRecord::readBlocks( data, event.get(), _eventHandlerMgr ) ;
            return false ;
          }
          return false ;
        }
      ) ;
    }
    catch( sio::exception &e ) {
      if( e.code() == sio::error_code::eof ) {
        return nullptr ;
      }
      SIO_RETHROW( e, e.code(), "Couldn't read next event!" ) ;
    }
    event->setAccessMode( accessMode ) ;
    _event = event ;
    postProcessEvent( _event.get() ) ;
    return _event.get() ;
  }

  //----------------------------------------------------------------------------

  void SIOReader::skipNEvents( int n ) {
    if( n < 1 ) { // nothing to skip
      return ;
    }
    int eventsSkipped = 0 ;
    try {
      // we skip n event header records
      sio::api::skip_records( _stream, [&] ( const sio::record_info &recinfo ) {
        if( recinfo._name == LCSIO::HeaderRecordName ) {
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

  EVENT::LCRunHeader * SIOReader::readRunHeader(int runNumber) {
    return readRunHeader( runNumber, EVENT::LCIO::READ_ONLY ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCRunHeader * SIOReader::readRunHeader(int runNumber, int accessMode) {
    if( _readEventMap ) {
      EVENT::long64 pos = _raMgr.getPosition( RunEvent( runNumber, -1 ) ) ;
      if( pos != RunEventMap::npos ) {
	      _stream.seekg( pos ) ;
        if( not _stream.good() ) {
          throw IO::IOException( "[SIOReader::readRunHeader()] Can't seek stream to requested position" ) ;
        }
	      return readNextRunHeader( accessMode ) ;
      }
      else {
	      return 0 ;
      }
    }
    else {  // no event map ------------------
      std::cout << " WARNING : LCReader::readRunHeader(run) called but not in direct access Mode  - " << std::endl
		            << " Too avoid this WARNING create the LCReader with: " << std::endl
		            << "       LCFactory::getInstance()->createLCReader( IO::LCReader::directAccess ) ; " << std::endl ;
    }
    return 0 ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCEvent * SIOReader::readEvent(int runNumber, int evtNumber) {
    return readEvent( runNumber, evtNumber , EVENT::LCIO::READ_ONLY ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCEvent * SIOReader::readEvent(int runNumber, int evtNumber, int accessMode) {
    if( _readEventMap ) {
      EVENT::long64 pos = _raMgr.getPosition(  RunEvent( runNumber,evtNumber ) ) ;
      if( pos != RunEventMap::npos ) {
	      _stream.seekg( pos ) ;
	      if( not _stream.good() ) {
	        throw IO::IOException( "[SIOReader::readEvent()] Can't seek stream to requested position" ) ;
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
      bool runFound = false ;
      bool evtFound = false ;
      // check current run - if any
      if( _runHeader != 0 ) {
	       if( _runHeader->getRunNumber() == runNumber ) runFound = true ;
      }
      // skip through run headers until run found or EOF
      while ( ! runFound ) {
	      if( readNextRunHeader() == 0 ) break ;
	      runFound = ( _runHeader->getRunNumber() == runNumber ) ;
      }
      if( !runFound ) {
	      return 0 ;
      }
      // look for the specific event in the stream directly. Very slow ...
      std::shared_ptr<IOIMPL::LCEventIOImpl> event = nullptr ;
      try {
        sio::api::read_records( _stream, _rawBuffer,
          [&]( const sio::record_info &recinfo ) {
            return ( recinfo._name == LCSIO::HeaderRecordName || recinfo._name == LCSIO::EventRecordName ) ;
          },
          [&]( const sio::record_info &recinfo, const sio::buffer_span& recdata ) {
            const bool compressed = sio::api::is_compressed( recinfo._options ) ;
            if( compressed ) {
              _compBuffer.resize( recinfo._uncompressed_length ) ;
              sio::zlib_compression compressor ;
              compressor.uncompress( recdata, _compBuffer ) ;
            }
            auto data = compressed ? _compBuffer.span() : recdata ;
            if( recinfo._name == LCSIO::HeaderRecordName ) {
              event = std::make_shared<IOIMPL::LCEventIOImpl>() ;
              SIOEventHeaderRecord::readBlocks( data, event.get(), _readCollectionNames ) ;
              return true ;
            }
            else if( recinfo._name == LCSIO::EventRecordName ) {
              if( nullptr == event ) {
                throw IO::IOException( "SIOReader::readNextEvent: Event record before an EventHeader record ..." ) ;
              }
              // if we've found the requested event, we unpack the blocks then
              if( event->getEventNumber() == evtNumber ) {
                SIOEventRecord::readBlocks( data, event.get(), _eventHandlerMgr ) ;
                evtFound = true ;
                // event found ! stop here !
                return false ;
              }
              // not the correct event number, continue looking ...
              return true ;
            }
            return false ;
          }
        ) ;
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
      _event = event ;
      postProcessEvent( _event.get() ) ;
      return _event.get() ;
    } //-- end fast skip
  }

  //----------------------------------------------------------------------------

  void SIOReader::close() {
    _raMgr.clear() ;
    _readEventMap = false ;
  }

  //----------------------------------------------------------------------------

  void SIOReader::registerLCEventListener(IO::LCEventListener * ls) {
    _evtListeners.insert( _evtListeners.end() , ls );
  }

  //----------------------------------------------------------------------------

  void SIOReader::removeLCEventListener(IO::LCEventListener * ls) {
    _evtListeners.erase( _evtListeners.find( ls )  );
  }

  //----------------------------------------------------------------------------

  void SIOReader::registerLCRunListener(IO::LCRunListener * ls) {
    _runListeners.insert( _runListeners.end() , ls );
  }

  //----------------------------------------------------------------------------

  void SIOReader::removeLCRunListener(IO::LCRunListener * ls) {
    _runListeners.erase( _runListeners.find( ls ) );
  }

  //----------------------------------------------------------------------------

  void SIOReader::readStream() {
    readStream( std::numeric_limits<int>::max() ) ;
  }

  //----------------------------------------------------------------------------
  void SIOReader::readStream(int maxRecord) {
    const bool readUntilEOF = (maxRecord == std::numeric_limits<int>::max()) ;
    std::set<std::string> records = { LCSIO::RunRecordName, LCSIO::HeaderRecordName, LCSIO::EventRecordName } ;
    std::shared_ptr<IOIMPL::LCEventIOImpl> event = nullptr ;
    int recordsRead = 0 ;
    // loop over records in the stream. SIO does it nicely for us
    try {
      sio::api::read_records( _stream, _rawBuffer,
        [&] ( const sio::record_info &recinfo ) {
          return ( records.find( recinfo._name ) != records.end() ) ;
        },
        [&] ( const sio::record_info &recinfo, const sio::buffer_span& recdata ) {
          const bool compressed = sio::api::is_compressed( recinfo._options ) ;
          if( compressed ) {
            _compBuffer.resize( recinfo._uncompressed_length ) ;
            sio::zlib_compression compressor ;
            compressor.uncompress( recdata, _compBuffer ) ;
          }
          auto data = compressed ? _compBuffer.span() : recdata ;
          // LCRunHeader case
          if( recinfo._name == LCSIO::RunRecordName ) {
            recordsRead++ ;
            auto runheader = std::make_shared<IOIMPL::LCRunHeaderIOImpl>() ;
            SIORunHeaderRecord::readBlocks( data, runheader.get() ) ;
            runheader->parameters().setValue( "LCIOFileName" ,  _myFilenames[ _currentFileIndex  ] ) ;
            std::set<IO::LCRunListener*>::iterator iter = _runListeners.begin() ;
            while( iter != _runListeners.end() ){
              runheader->setReadOnly( false ) ;
              (*iter)->modifyRunHeader( runheader.get() ) ;
              runheader->setReadOnly( true ) ;
              (*iter)->processRunHeader( runheader.get() ) ;
              iter++ ;
            }
          }
          // Event header case. Setup the event for the next record
          else if( recinfo._name == LCSIO::HeaderRecordName ) {
            event = std::make_shared<IOIMPL::LCEventIOImpl>() ;
            SIOEventHeaderRecord::readBlocks( data, event.get(), _readCollectionNames ) ;
          }
          else if( recinfo._name == LCSIO::EventRecordName ) {
            if( nullptr == event ) {
              throw IO::IOException( "SIOReader::readStream: Event record before an EventHeader record ..." ) ;
            }
            recordsRead++ ;
            SIOEventRecord::readBlocks( data, event.get(), _eventHandlerMgr ) ;
            std::set<IO::LCEventListener*>::iterator iter = _evtListeners.begin() ;
            while( iter != _evtListeners.end() ) {
              postProcessEvent( event.get() ) ;
              // fg20070813 changed order of update and process (needed for
              // Marlin modifying processors )
              event->setAccessMode( EVENT::LCIO::UPDATE ) ;
              (*iter)->modifyEvent( event.get() ) ;
              event->setAccessMode( EVENT::LCIO::READ_ONLY ) ; // set the proper acces mode
              (*iter)->processEvent( event.get() ) ;
              iter++ ;
            }
            // free ressources here
            event = nullptr ;
          }
          return ( recordsRead < maxRecord ) ;
        }
      ) ;
    }
    catch( sio::exception &e ) {
      if( e.code() != sio::error_code::eof ) {
        SIO_RETHROW( e, e.code(), "SIOReader::readStream: Couldn't read stream" ) ;
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

  //----------------------------------------------------------------------------

  void  SIOReader::postProcessEvent( EVENT::LCEvent *event ) {
    // restore the daughter relations from the parent relations
    SIOParticleHandler::restoreParentDaughterRelations( event ) ;
    // check subset collection's pointers
    char* rColChar = getenv ("LCIO_IGNORE_NULL_IN_SUBSET_COLLECTIONS") ;
    if( nullptr != rColChar ) {
      return;
    }
    const std::vector< std::string >* strVec = event->getCollectionNames() ;
    for( auto name = strVec->begin() ; name != strVec->end() ; name++) {
    	EVENT::LCCollection* col = event->getCollection( *name ) ;
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

} // namespace
