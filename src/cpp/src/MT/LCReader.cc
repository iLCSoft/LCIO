#include "MT/LCReader.h" 

// -- lcio headers
#include "SIO/LCSIO.h" 
#include "SIO/SIOParticleHandler.h"
#include "SIO/LCIORecords.h"
#include "SIO/LCIORandomAccessMgr.h"
#include "IOIMPL/LCEventIOImpl.h"
#include "IOIMPL/LCRunHeaderIOImpl.h"

// -- sio headers
#include "SIO_stream.h" 
#include "SIO_record.h" 

// -- std headers
#include <assert.h>
#include <climits>

typedef EVENT::long64 long64 ;
 
namespace MT {
  
  LCReader::LCReader( int lcReaderFlag ) :
    _readEventMap( lcReaderFlag & LCReader::directAccess ),
    _lcioRecords(std::make_shared<SIO::LCIORecords>()),
    _raMgr(std::make_shared<SIO::LCIORandomAccessMgr>()) {
    /* nop */
  }
  
  //----------------------------------------------------------------------------
  
  void LCReader::open( const std::string& filename ) {
    _stream = std::make_shared<sio::stream>();
    _stream->set_reserve(64 * SIO_KBYTE);
    // Open the stream
    auto status = _stream->open( filename, SIO_MODE_READ ) ;
    if( status != SIO_STREAM_SUCCESS ) {
      throw IO::IOException( std::string( "[MT::LCReader::open()] Can't open stream: " + filename ) ) ;
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
      if ( FSTAT( filenames[i].c_str(), &fileinfo ) != 0 ) {
         missing_files += filenames[i] ;
         missing_files += "  " ;
      }
    }
    // JE: if not raise IOException
    if( missing_files.size() != 0 ) {
      throw IO::IOException( std::string( "[MT::LCReader::open()] File(s) not found:  " + missing_files )) ;
    }
    _myFilenames = filenames ;
    _currentFileIndex = 0 ;
    open( _myFilenames[0] ) ;
  }
  
  //----------------------------------------------------------------------------

  LCRunHeaderPtr LCReader::readNextRunHeader( int accessMode ) {
    IOIMPL::LCRunHeaderIOImpl* runptr = nullptr ;
    auto runRecord = _lcioRecords->createRunRecord( &runptr ) ;
    sio::record_read_result readResult ;
    // this might throw exceptions
    try {
      readRecord( {{runRecord->name(), runRecord}} , readResult ) ;
    }
    catch(IO::EndOfDataException) {
      if ( nullptr != runptr ) {
        delete runptr ;
      }
      return 0 ;
    }
    // set the proper acces mode before returning the run
    if( nullptr != runptr ) {
      runptr->setReadOnly( accessMode == EVENT::LCIO::READ_ONLY ) ;
    }
    // save the current file name in run header parameter: 
    runptr->parameters().setValue( "LCIOFileName" ,  _myFilenames[ _currentFileIndex  ] ) ;
    return LCRunHeaderPtr( runptr ) ;
  }
  
  //----------------------------------------------------------------------------
  
  LCEventPtr LCReader::readNextEvent( int accessMode )  {
    IOIMPL::LCEventIOImpl* evtptr = nullptr ;
    sio::record_read_result readResult ;
    auto headerRecord = _lcioRecords->createEventHeaderRecord( &evtptr, _readCollectionNames ) ;
    try {
      readRecord( {{headerRecord->name(), headerRecord}} , readResult ) ;
    }
    catch(IO::EndOfDataException) {
      if ( nullptr != evtptr ) {
        delete evtptr ;
      }
      return 0 ;
    }
    // Create the record to be read. This also setup the collections to be read
    auto eventRecord = _lcioRecords->createEventRecord( &evtptr ) ;
    try {
      readRecord( {{eventRecord->name(), eventRecord}} , readResult ) ;
    }
    catch(IO::EndOfDataException) {
      if ( nullptr != evtptr ) {
        delete evtptr ;
      }
      return 0 ;
    }
    // Set the proper acces mode before returning the event
    evtptr->setAccessMode( accessMode ) ;
    postProcessEvent(evtptr) ;
    return LCEventPtr(evtptr) ;
  }
  
  //----------------------------------------------------------------------------

  int LCReader::getNumberOfEvents()  {
    // create the event map if needed (i.e. not opened in direct access mode)
    if( ! _readEventMap ) {
      _readEventMap = true ;      
      getEventMap() ;
    }
    return _raMgr->getEventMap().getNumberOfEventRecords()  ;    
  }
  
  //----------------------------------------------------------------------------

  int LCReader::getNumberOfRuns() {
    // create the event map if needed (i.e. not opened in direct access mode)
    if( ! _readEventMap ){  
      _readEventMap = true ;
      getEventMap() ;
    }
    return _raMgr->getEventMap().getNumberOfRunRecords() ;
  }
  
  //----------------------------------------------------------------------------

  void LCReader::getRuns(EVENT::IntVec & runs) {
    int nRun = this->getNumberOfRuns() ;    
    runs.resize( nRun ) ;
    auto map = _raMgr->getEventMap() ;
    auto it = map.begin() ;   
    
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
    auto it = map.begin() ;   
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
    sio::record_read_result readResult ;
    IOIMPL::LCEventIOImpl* evtptr = nullptr ;
    auto headerRecord = _lcioRecords->createEventHeaderRecord( &evtptr, _readCollectionNames ) ;
    while( eventsSkipped++ < n ) {
      try {
        readRecord( {{headerRecord->name(), headerRecord}} , readResult ) ;
      }
      catch(IO::EndOfDataException) {
        if ( nullptr != evtptr ) {
          delete evtptr ;
        }
        return ;
      }
    }
    // Create the record to be read. This also setup the collections to be read
    // WARNING evtptr is first deleted when calling readRecord
    auto eventRecord = _lcioRecords->createEventRecord( &evtptr ) ;
    try {
      readRecord( {{eventRecord->name(), eventRecord}} , readResult ) ;
    }
    catch(IO::EndOfDataException) {
      if ( nullptr != evtptr ) {
        delete evtptr ;
      }
      return ;
    }
    if ( nullptr != evtptr ) {
      delete evtptr ;
    }
  }
  
  //----------------------------------------------------------------------------

  LCRunHeaderPtr LCReader::readRunHeader( int runNumber, int accessMode ) {
    if( _readEventMap ) {
      EVENT::long64 pos = _raMgr->getPosition( SIO::RunEvent( runNumber, -1 ) ) ; 
      if( pos != SIO::RunEventMap::npos ) {
        int status = _stream->seek( pos ) ;
        if( status != SIO_STREAM_SUCCESS ) { 
            throw IO::IOException( std::string( "[SIOReader::readRunHeader()] Can't seek stream to"
            " requested position" ) ) ;
        }
        return readNextRunHeader( accessMode ) ;
      }
      else {
        return nullptr ;
      }
    } 
    else {  // no event map ------------------
      std::cout << " WARNING : LCReader::readRunHeader(run,mode) called but not in direct access Mode  - " << std::endl 
                << " Too avoid this WARNING create the LCReader with: " << std::endl 
                << "       auto r = new MT::LCReader( MT::LCReader::directAccess ) ; " << std::endl ;
    }
    return nullptr ;
  }
  
  //----------------------------------------------------------------------------

  LCEventPtr LCReader::readEvent( int runNumber, int evtNumber, int accessMode ) {
    if( _readEventMap ) {
      EVENT::long64 pos = _raMgr->getPosition( SIO::RunEvent( runNumber, evtNumber ) ) ; 
      if( pos != SIO::RunEventMap::npos ) {	
	      int status = _stream->seek( pos ) ;
	      if( status != SIO_STREAM_SUCCESS ) { 
	        throw IO::IOException( std::string( "[SIOReader::readEvent()] Can't seek stream to"
					  " requested position" ) ) ;
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
		            << "       auto r = new MT::LCReader( MT::LCReader::directAccess ) ; " << std::endl ;
      // ---- OLD code with fast skip -----------
      bool runFound = false ;
      bool evtFound = false ;
      // skip through run headers until run found or EOF
      while ( ! runFound ) {
        auto run = readNextRunHeader() ;
	      if( nullptr == run ) break ; 
	      runFound = ( run->getRunNumber() == runNumber ) ;
      }
      if( !runFound ) {
	      return 0 ;
      }
      // Read event header until we find the requested event number
      sio::record_read_result readResult;
      IOIMPL::LCEventIOImpl* evtptr = nullptr ;
      auto headerRecord = _lcioRecords->createEventHeaderRecord( &evtptr, _readCollectionNames ) ;
      while( ! evtFound ) {
        try {
          readRecord( {{headerRecord->name(), headerRecord}} , readResult ) ;
        }
        catch(IO::EndOfDataException) {
          if ( nullptr != evtptr ) {
            delete evtptr ;
          }
          return 0 ;
        }
        evtFound = ( evtptr->getEventNumber() == evtNumber ) ;
      }
      if( !evtFound ) {
        if ( nullptr != evtptr ) {
          delete evtptr ;
        }
        return 0 ;
      }
      // Create the record to be read. This also setup the collections to be read
      auto eventRecord = _lcioRecords->createEventRecord( &evtptr ) ;
      try {
        readRecord( {{eventRecord->name(), eventRecord}} , readResult ) ;
      }
      catch(IO::EndOfDataException) {
        if ( nullptr != evtptr ) {
          delete evtptr ;
        }
        return 0 ;
      }
      evtptr->setAccessMode( EVENT::LCIO::READ_ONLY ) ;
      postProcessEvent(evtptr) ;
      return LCEventPtr( evtptr ) ;  
    } //-- end fast skip
  }
  
  //----------------------------------------------------------------------------
  
  void LCReader::close() {
    _raMgr->clear() ;
    _readEventMap = false ;
  }
  
  //----------------------------------------------------------------------------
  
  void LCReader::readStream( const LCReaderListenerList & listeners ) {
    readStream( listeners, INT_MAX ) ;
  }
  
  //----------------------------------------------------------------------------

  void LCReader::readStream( const LCReaderListenerList & listeners, int maxRecord ) {
    const bool readUntilEOF = (maxRecord == INT_MAX) ;
    // here we need to read all the records on the stream
    // and then notify the listeners depending on the type ....
    // set all known records to unpack
    // read records and notify listeners
    int recordsRead = 0 ;
    sio::record_read_result readResult;
    while( recordsRead < maxRecord ) {
      IOIMPL::LCEventIOImpl* evtptr = nullptr ;
      IOIMPL::LCRunHeaderIOImpl* runptr = nullptr ;
      auto runRecord = _lcioRecords->createRunRecord( &runptr ) ;
      auto headerRecord = _lcioRecords->createEventHeaderRecord( &evtptr, _readCollectionNames ) ;
      sio::record_map records = {
        {runRecord->name(), runRecord} , 
        {headerRecord->name(), headerRecord}
      };
      try {
        readRecord( records , readResult ) ;
      }
      catch(IO::EndOfDataException) {
        if ( nullptr != evtptr ) {
          delete evtptr ;
        }
        if ( nullptr != runptr ) {
          delete runptr ;
        }
        // only throw exception if a 'finite' number of records was 
        // specified that couldn't be read from the file
        if( readUntilEOF ) {
          return ;
        }
        else {
          std::stringstream message ;
          message << "SIOReader::readStream(int maxRecord) : EOF before " 
            << maxRecord << " records read from file" << std::ends ;
          throw IO::EndOfDataException( message.str())  ;
        }
      }
      // Next record is an event. Setup the record and read it
      if( readResult._record->name() == LCSIO_HEADERRECORDNAME ) {
        auto eventRecord = _lcioRecords->createEventRecord( &evtptr ) ;
        try {
          readRecord( {{eventRecord->name(), eventRecord}} , readResult ) ;
        }
        catch(IO::EndOfDataException) {
          if ( nullptr != evtptr ) {
            delete evtptr ;
          }
          if ( nullptr != runptr ) {
              delete runptr ;
          }
          // only throw exception if a 'finite' number of records was 
          // specified that couldn't be read from the file
          if( readUntilEOF ) {
            return ;
          }
          else {
            std::stringstream message ;
            message << "SIOReader::readStream(int maxRecord) : EOF before " 
              << maxRecord << " records read from file" << std::ends ;
            throw IO::EndOfDataException( message.str())  ;
          }
        }
      }
      // notify LCRunListeners
      if( readResult._record->name() == LCSIO_RUNRECORDNAME ) {
        recordsRead++ ;
        LCRunHeaderPtr hdrsptr( runptr ) ;
        auto iter = listeners.begin() ;
        while( iter != listeners.end() ) {
          runptr->setReadOnly( false ) ;
          (*iter)->processRunHeader( hdrsptr ) ;          
          iter++ ;
        }
      }
      // notify LCEventListeners 
      if( readResult._record->name() == LCSIO_EVENTRECORDNAME ) {
        recordsRead++ ;
        LCEventPtr evtsptr( evtptr ) ;
        auto iter = listeners.begin() ;
        while( iter != listeners.end() ) {
          postProcessEvent(evtptr) ;
          evtptr->setAccessMode( EVENT::LCIO::UPDATE ) ;
          (*iter)->processEvent( evtsptr ) ;
          iter++ ;    
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

  void LCReader::readRecord( const sio::record_map &records , sio::record_read_result &readResult ) {
    if( _stream->state() == SIO_STATE_OPEN ) {
      // read the next record from the stream
      readResult = _stream->read_next_record( records ) ;
      if( ! (readResult._status & 1) ) {
        if( readResult._status & SIO_STREAM_EOF ) {
          // if we have a list of filenames open the next file
          if( !_myFilenames.empty()  && _currentFileIndex+1 < _myFilenames.size() ) {
            close() ;
            open( _myFilenames[ ++_currentFileIndex  ] ) ;
            readRecord( records, readResult ) ;
            return ;
          }
          throw IO::EndOfDataException("EOF") ;
        }
        throw IO::IOException( std::string(" io error on stream: ") + _stream->file_name() ) ;
      }
    }
    else {
      throw IO::IOException( " stream not opened" ) ;
    }
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
    _raMgr->createEventMap( _stream.get() ) ;
  }

} // namespace
