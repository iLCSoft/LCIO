#include "MT/LCReader.h"

// -- lcio headers
#include "SIO/LCSIO.h"
#include "SIO/SIOParticleHandler.h"
#include "SIO/LCIORecords.h"
#include "SIO/LCIORandomAccessMgr.h"
#include "IOIMPL/LCEventIOImpl.h"
#include "IOIMPL/LCRunHeaderIOImpl.h"
#include "Exceptions.h"
#include "SIO/RunEventMap.h"

// -- sio headers
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
    _readEventMap( lcReaderFlag & LCReader::directAccess ),
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

  LCRunHeaderPtr LCReader::readNextRunHeader( int accessMode ) {
    LCRunHeaderPtr runhdr = nullptr ;
    sio::api::read_records( _stream, _rawBuffer,
      [&]( const sio::record_info &recinfo ) {
        // we want a run header record only
        return ( recinfo._name == SIO::LCSIO::RunRecordName ) ;
      },
      [&]( const sio::record_info &recinfo, const sio::buffer_span& recdata ) {
        const bool compressed = sio::api::is_compressed( recinfo._options ) ;
        if( compressed ) {
          sio::zlib_compression compressor ;
          compressor.uncompress( recdata, _compBuffer ) ;
        }
        auto rundata = compressed ? _compBuffer.span() : recdata ;
        auto runheader = std::make_shared<IOIMPL::LCRunHeaderIOImpl>() ;
        SIO::SIORunHeaderRecord::readBlocks( rundata, runheader.get() ) ;
        runheader->setReadOnly( accessMode == EVENT::LCIO::READ_ONLY ) ;
        runheader->parameters().setValue( "LCIOFileName" ,  _myFilenames[ _currentFileIndex  ] ) ;
        runhdr = runheader ;
        return false ; // only one record
      }
    ) ;
    return runhdr ;
  }

  //----------------------------------------------------------------------------

  LCEventPtr LCReader::readNextEvent( int accessMode )  {
    std::shared_ptr<IOIMPL::LCEventIOImpl> event = nullptr ;
    sio::api::read_records( _stream, _rawBuffer,
      [&]( const sio::record_info &recinfo ) {
        return ( recinfo._name == SIO::LCSIO::HeaderRecordName || recinfo._name == SIO::LCSIO::EventRecordName ) ;
      },
      [&]( const sio::record_info &recinfo, const sio::buffer_span& recdata ) {
        const bool compressed = sio::api::is_compressed( recinfo._options ) ;
        if( compressed ) {
          sio::zlib_compression compressor ;
          compressor.uncompress( recdata, _compBuffer ) ;
        }
        auto data = compressed ? _compBuffer.span() : recdata ;
        if( recinfo._name == SIO::LCSIO::HeaderRecordName ) {
          event = std::make_shared<IOIMPL::LCEventIOImpl>() ;
          SIO::SIOEventHeaderRecord::readBlocks( data, event.get(), _readCollectionNames ) ;
          return true ;
        }
        else if( recinfo._name == SIO::LCSIO::EventRecordName ) {
          if( nullptr == event ) {
            throw IO::IOException( "SIOReader::readNextEvent: Event record before an EventHeader record ..." ) ;
          }
          SIO::SIOEventRecord::readBlocks( data, event.get(), _eventHandlerMgr ) ;
          return false ;
        }
        return false ;
      }
    ) ;
    if( nullptr != event ) {
      event->setAccessMode( accessMode ) ;
      postProcessEvent( event.get() ) ;
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

  LCRunHeaderPtr LCReader::readRunHeader( int runNumber, int accessMode ) {
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
	      return 0 ;
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

  LCEventPtr LCReader::readEvent( int runNumber, int evtNumber, int accessMode ) {
    if( _readEventMap ) {
      EVENT::long64 pos = _raMgr->getPosition(  SIO::RunEvent( runNumber,evtNumber ) ) ;
      if( pos != SIO::RunEventMap::npos ) {
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
      // look for the specific event in the stream directly. Very slow ...
      bool evtFound = false ;
      std::shared_ptr<IOIMPL::LCEventIOImpl> event = nullptr ;
      sio::api::read_records( _stream, _rawBuffer,
        [&]( const sio::record_info &recinfo ) {
          return ( recinfo._name == SIO::LCSIO::HeaderRecordName || recinfo._name == SIO::LCSIO::EventRecordName ) ;
        },
        [&]( const sio::record_info &recinfo, const sio::buffer_span& recdata ) {
          const bool compressed = sio::api::is_compressed( recinfo._options ) ;
          if( compressed ) {
            sio::zlib_compression compressor ;
            compressor.uncompress( recdata, _compBuffer ) ;
          }
          auto data = compressed ? _compBuffer.span() : recdata ;
          if( recinfo._name == SIO::LCSIO::HeaderRecordName ) {
            event = std::make_shared<IOIMPL::LCEventIOImpl>() ;
            SIO::SIOEventHeaderRecord::readBlocks( data, event.get(), _readCollectionNames ) ;
            return true ;
          }
          else if( recinfo._name == SIO::LCSIO::EventRecordName ) {
            if( nullptr == event ) {
              throw IO::IOException( "SIOReader::readNextEvent: Event record before an EventHeader record ..." ) ;
            }
            // if we've found the requested event, we unpack the blocks then
            if( event->getEventNumber() == evtNumber && event->getRunNumber() == runNumber ) {
              SIO::SIOEventRecord::readBlocks( data, event.get(), _eventHandlerMgr ) ;
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
      if( not evtFound ) {
        return nullptr ;
      }
      event->setAccessMode( EVENT::LCIO::READ_ONLY ) ;
      postProcessEvent( event.get() ) ;
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
            sio::zlib_compression compressor ;
            compressor.uncompress( recdata, _compBuffer ) ;
          }
          auto data = compressed ? _compBuffer.span() : recdata ;
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
            event = std::make_shared<IOIMPL::LCEventIOImpl>() ;
            SIO::SIOEventHeaderRecord::readBlocks( data, event.get(), _readCollectionNames ) ;
          }
          else if( recinfo._name == SIO::LCSIO::EventRecordName ) {
            if( nullptr == event ) {
              throw IO::IOException( "SIOReader::readStream: Event record before an EventHeader record ..." ) ;
            }
            recordsRead++ ;
            auto iter = listeners.begin() ;
            while( iter != listeners.end() ) {
              postProcessEvent( event.get() ) ;
              event->setAccessMode( EVENT::LCIO::UPDATE ) ;
              (*iter)->processEvent( event ) ;
              iter++ ;
            }
          }
          return ( recordsRead >= maxRecord ) ;
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

  void LCReader::readNextRecord( const LCReaderListenerList & listeners ) {
    readStream( listeners, 1 );
  }

  //----------------------------------------------------------------------------

  void LCReader::readStream( LCReaderListener *listener ) {
    readStream( {listener} ) ;
  }

  //----------------------------------------------------------------------------

  void LCReader::readStream( LCReaderListener *listener, int maxRecord ) {
    readStream( {listener}, maxRecord );
  }

  //----------------------------------------------------------------------------

  void LCReader::readNextRecord( LCReaderListener *listener ) {
    readNextRecord( {listener} );
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
