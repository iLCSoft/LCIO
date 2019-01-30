#include "SIO/SIOReader.h" 


#include "SIO/LCSIO.h"
#include "SIO/SIOEventHandler.h" 
#include "SIO/SIOCollectionHandler.h"

#include "SIO/SIORunHeaderHandler.h"
#include "SIO/SIOParticleHandler.h"

#include "SIO/SIORandomAccessHandler.h"
#include "SIO/SIOIndexHandler.h"

#include "LCIOSTLTypes.h"

#include "SIO_stream.h" 
#include "SIO_record.h" 
#include "IMPL/LCIOExceptionHandler.h"


#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <climits>

using namespace EVENT ;
using namespace IO ;
using namespace IOIMPL ;
using namespace IMPL ;

#include <assert.h>

typedef EVENT::long64 long64 ;
 
namespace SIO {


  SIOReader::SIOReader( int lcReaderFlag ) :
    _readEventMap( lcReaderFlag & LCReader::directAccess  ) {
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
      throw IOException( "[SIOReader::open()] Provided file list is empty") ;
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
      throw IOException( std::string( "[SIOReader::open()] File(s) not found:  " + missing_files )) ;
    }
    _myFilenames = filenames ;
    _currentFileIndex = 0 ;
    open( _myFilenames[0] ) ;
  }
  
  //----------------------------------------------------------------------------

  void SIOReader::open(const std::string& filename)  {
    _stream = std::make_shared<sio::stream>();
    _stream->set_reserve(64 * SIO_KBYTE);
    // Open the stream
    auto status = _stream->open( filename, SIO_MODE_READ ) ;
    if( status != SIO_STREAM_SUCCESS ) {
      throw IOException( std::string( "[SIOReader::open()] Can't open stream: " + filename ) ) ;
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
    _raMgr.createEventMap( _stream.get() ) ;
  }

  //----------------------------------------------------------------------------

  int SIOReader::getNumberOfEvents()  {
    // create the event map if needed (i.e. not opened in direct access mode)
    if( ! _readEventMap ) {
      _readEventMap = true ;      
      getEventMap() ;
    }
    return _raMgr.getEventMap().getNumberOfEventRecords()  ;    
  }
  
  //----------------------------------------------------------------------------

  int SIOReader::getNumberOfRuns()  {
    // create the event map if needed (i.e. not opened in direct access mode)
    if( ! _readEventMap ){  
      _readEventMap = true ;
      getEventMap() ;
    }
    return _raMgr.getEventMap().getNumberOfRunRecords() ;
  }
  
  //----------------------------------------------------------------------------

  void SIOReader::getRuns(EVENT::IntVec & runs) {
    int nRun = this->getNumberOfRuns() ;    
    runs.resize( nRun ) ;
    const RunEventMap& map = _raMgr.getEventMap() ;
    auto it = map.begin() ;   
    
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
    const RunEventMap& map = _raMgr.getEventMap() ;
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

  void SIOReader::readRecord( const sio::record_map &records , sio::record_read_result &readResult ) {
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
          throw EndOfDataException("EOF") ;
        }
        throw IOException( std::string(" io error on stream: ") + _stream->file_name() ) ;
      }
    }
    else {
      throw IOException( " stream not opened" ) ;
    }
  }
  
  //----------------------------------------------------------------------------

  LCRunHeader* SIOReader::readNextRunHeader()  {
    return readNextRunHeader( LCIO::READ_ONLY ) ;
  }
  
  //----------------------------------------------------------------------------

  LCRunHeader* SIOReader::readNextRunHeader(int accessMode) {
    auto runRecord = _lcioRecords.createRunRecord( &_run ) ;
    sio::record_read_result readResult ;
    // this might throw exceptions
    try {
      readRecord( {{runRecord->name(), runRecord}} , readResult ) ;
    }
    catch(EndOfDataException) {
      return 0 ;
    }
    // set the proper acces mode before returning the run
    if( _run != 0 ) {
      _run->setReadOnly( accessMode == LCIO::READ_ONLY ) ;
    }
    // save the current file name in run header parameter: 
    _run->parameters().setValue( "LCIOFileName" ,  _myFilenames[ _currentFileIndex  ] ) ;
    return _run ;
  }
  
  //----------------------------------------------------------------------------
  
  // void SIOReader::setUpHandlers(){
  // 
  //   // use event _evt to setup the block readers from header information ....
  //   const std::vector<std::string>* strVec = _evt->getCollectionNames() ;
  //   for( std::vector<std::string>::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
  // 
  //     const LCCollection* col = _evt->getCollection( *name ) ;
  // 
  // 
  //     // check if block handler exists in manager
  //     SIOCollectionHandler* ch = dynamic_cast<SIOCollectionHandler*>
  //     ( SIO_blockManager::get( name->c_str() )  ) ;
  // 
  //     // check if it is still the correct block handler
  //     if( ch != 0 && ch->getTypeName().compare(col->getTypeName()) != 0 ){
  //       // The type changed, so destroy the old handler and get a new one.
  //       std::string chName = ch->getTypeName();
  //       std::string colName= col->getTypeName();
  //       std::cout << "ch name: " << chName << " col name: " << colName << std::endl;
  //       delete ch;
  //       ch = 0;
  //     }
  // 
  //     // if not, create a new block handler
  //     if( ch == 0 ) {
  // 
  //       // create collection handler for event
  //       try{
  //         ch =  new SIOCollectionHandler( *name, col->getTypeName() , &_evt )  ;
  //         // calls   SIO_blockManager::add( ch )  in the c'tor !
  //       }
  //       catch(Exception& ex){   // unsuported type !
  //         delete ch ;
  //         ch =  0 ;
  //       }
  // 
  //     }
  //     // else { // handler already exists
  //     if( ch != 0 )
  //       ch->setEvent( &_evt ) ; 
  //     //      }
  //   }
  // }
  // 
  //----------------------------------------------------------------------------

  LCEvent* SIOReader::readNextEvent()  {
    return readNextEvent( LCIO::READ_ONLY ) ;
  }
  
  //----------------------------------------------------------------------------

  LCEvent* SIOReader::readNextEvent(int accessMode)  {
    sio::record_read_result readResult ;
    auto headerRecord = _lcioRecords.createEventHeaderRecord( &_evt, _readCollectionNames ) ;
    try {
      readRecord( {{headerRecord->name(), headerRecord}} , readResult ) ;
    }
    catch(EndOfDataException) {
      return 0 ;
    }
    // Create the record to be read. This also setup the collections to be read
    auto eventRecord = _lcioRecords.createEventRecord( &_evt ) ;
    try {
      readRecord( {{eventRecord->name(), eventRecord}} , readResult ) ;
    }
    catch(EndOfDataException) {
      return 0 ;
    }
    // Set the proper acces mode before returning the event
    _evt->setAccessMode( accessMode ) ;
    postProcessEvent() ;   
    return _evt ;
  }
  
  //----------------------------------------------------------------------------
  
  void SIOReader::skipNEvents( int n ) {     
    if( n < 1 ) { // nothing to skip
      return ;
    }
    int eventsSkipped = 0 ;
    sio::record_read_result readResult;
    auto headerRecord = _lcioRecords.createEventHeaderRecord( &_evt, _readCollectionNames ) ;
    while( eventsSkipped++ < n ) {
      try {
        readRecord( {{headerRecord->name(), headerRecord}} , readResult ) ;
      }
      catch(EndOfDataException) {
        return ;
      }
    }
    // Create the record to be read. This also setup the collections to be read
    auto eventRecord = _lcioRecords.createEventRecord( &_evt ) ;
    try {
      readRecord( {{eventRecord->name(), eventRecord}} , readResult ) ;
    }
    catch(EndOfDataException) {
      return ;
    }
  }
  
  //----------------------------------------------------------------------------

  EVENT::LCRunHeader * SIOReader::readRunHeader(int runNumber) {
    return readRunHeader( runNumber, EVENT::LCIO::READ_ONLY ) ;
  }
  
  //----------------------------------------------------------------------------

  EVENT::LCRunHeader * SIOReader::readRunHeader(int runNumber, int accessMode) {
    if( _readEventMap ) {
      EVENT::long64 pos = _raMgr.getPosition(  RunEvent( runNumber, -1 ) ) ; 
      if( pos != RunEventMap::npos ) {
	      int status = _stream->seek( pos ) ;
        if( status != SIO_STREAM_SUCCESS ) { 
          throw IOException( std::string( "[SIOReader::readRunHeader()] Can't seek stream to"
				  " requested position" ) ) ;
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
	      int status = _stream->seek( pos ) ;
	      if( status != SIO_STREAM_SUCCESS ) { 
	        throw IOException( std::string( "[SIOReader::readEvent()] Can't seek stream to"
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
		            << "       LCFactory::getInstance()->createLCReader( IO::LCReader::directAccess ) ; " << std::endl ;
      // ---- OLD code with fast skip -----------
      bool runFound = false ;
      bool evtFound = false ;
      // check current run - if any
      if( _run != 0 ) {
	       if( _run->getRunNumber() == runNumber ) runFound = true ;
      }
      // skip through run headers until run found or EOF
      while ( ! runFound ) {
	      if( readNextRunHeader() == 0 ) break ; 
	      runFound = ( _run->getRunNumber() == runNumber ) ;
      }
      if( !runFound ) {
	      return 0 ;
      }
      // Read event header until we find the requested event number
      sio::record_read_result readResult;
      auto headerRecord = _lcioRecords.createEventHeaderRecord( &_evt, _readCollectionNames ) ;
      while( ! evtFound ) {
        try {
          readRecord( {{headerRecord->name(), headerRecord}} , readResult ) ;
        }
        catch(EndOfDataException) {
          return 0 ;
        }
        evtFound = ( _evt->getEventNumber() == evtNumber ) ;
      }
      if( !evtFound ) {
        return 0 ;
      }
      // Create the record to be read. This also setup the collections to be read
      auto eventRecord = _lcioRecords.createEventRecord( &_evt ) ;
      try {
        readRecord( {{eventRecord->name(), eventRecord}} , readResult ) ;
      }
      catch(EndOfDataException) {
        return 0 ;
      }
      _evt->setAccessMode( LCIO::READ_ONLY ) ;
      postProcessEvent() ;
      return _evt ;  
    } //-- end fast skip
  }
  
  //----------------------------------------------------------------------------
  
  void SIOReader::close() {
    _raMgr.clear() ;
    _readEventMap = false ;
  }

  //----------------------------------------------------------------------------

  void SIOReader::registerLCEventListener(LCEventListener * ls) { 
    _evtListeners.insert( _evtListeners.end() , ls );
  }
  
  //----------------------------------------------------------------------------
  
  void SIOReader::removeLCEventListener(LCEventListener * ls) { 
    _evtListeners.erase( _evtListeners.find( ls )  );
  }
  
  //----------------------------------------------------------------------------
  
  void SIOReader::registerLCRunListener(LCRunListener * ls) { 
    _runListeners.insert( _runListeners.end() , ls );
  }
  
  //----------------------------------------------------------------------------

  void SIOReader::removeLCRunListener(LCRunListener * ls) {
    _runListeners.erase( _runListeners.find( ls ) );
  }
  
  //----------------------------------------------------------------------------

  void SIOReader::readStream() {
    readStream( INT_MAX ) ;
  }
  
  //----------------------------------------------------------------------------
  void SIOReader::readStream(int maxRecord) {
    const bool readUntilEOF = (maxRecord == INT_MAX) ;
    // here we need to read all the records on the stream
    // and then notify the listeners depending on the type ....
    // set all known records to unpack
    auto runRecord = _lcioRecords.createRunRecord( &_run ) ;
    auto headerRecord = _lcioRecords.createEventHeaderRecord( &_evt, _readCollectionNames ) ;
    sio::record_map records = {
      {runRecord->name(), runRecord} , 
      {headerRecord->name(), headerRecord}
    };
    // read records and notify listeners
    int recordsRead = 0 ;
    sio::record_read_result readResult;
    while( recordsRead < maxRecord ) {
      try {
        readRecord( records , readResult ) ;
      }
      catch(EndOfDataException) {  
        // only throw exception if a 'finite' number of records was 
        // specified that couldn't be read from the file
        if( readUntilEOF ) {
          return ;
        }
        else {
          std::stringstream message ;
          message << "SIOReader::readStream(int maxRecord) : EOF before " 
            << maxRecord << " records read from file" << std::ends ;
          throw EndOfDataException( message.str())  ;
        }
      }
      // Next record is an event. Setup the record and read it
      if( readResult._record->name() == LCSIO_HEADERRECORDNAME ) {
        auto eventRecord = _lcioRecords.createEventRecord( &_evt ) ;
        try {
          readRecord( {{eventRecord->name(), eventRecord}} , readResult ) ;
        }
        catch(EndOfDataException) {  
          // only throw exception if a 'finite' number of records was 
          // specified that couldn't be read from the file
          if( readUntilEOF ) {
            return ;
          }
          else {
            std::stringstream message ;
            message << "SIOReader::readStream(int maxRecord) : EOF before " 
              << maxRecord << " records read from file" << std::ends ;
            throw EndOfDataException( message.str())  ;
          }
        }
      }
      // notify LCRunListeners
      if( readResult._record->name() == LCSIO_RUNRECORDNAME ) {
        recordsRead++ ;
        std::set<IO::LCRunListener*>::iterator iter = _runListeners.begin() ;
        while( iter != _runListeners.end() ){
          _run->setReadOnly( false ) ;
          (*iter)->modifyRunHeader( _run ) ;
          _run->setReadOnly( true ) ;
          (*iter)->processRunHeader( _run ) ;          
          iter++ ;
        }
      }
      // notify LCEventListeners 
      if( readResult._record->name() == LCSIO_EVENTRECORDNAME ) {
        recordsRead++ ;
        std::set<IO::LCEventListener*>::iterator iter = _evtListeners.begin() ;
        while( iter != _evtListeners.end() ) {
          postProcessEvent() ;
          // fg20070813 changed order of update and process (needed for 
          // Marlin modifying processors )
          _evt->setAccessMode( LCIO::UPDATE ) ;
          (*iter)->modifyEvent( _evt ) ;
          _evt->setAccessMode( LCIO::READ_ONLY ) ; // set the proper acces mode
          (*iter)->processEvent( _evt ) ;
          iter++ ;    
        }
      }
    }
  }
  
  //----------------------------------------------------------------------------
  
  void  SIOReader::postProcessEvent() {
    // restore the daughter relations from the parent relations
    SIOParticleHandler::restoreParentDaughterRelations( _evt ) ;
    // check subset collection's pointers
    char* rColChar = getenv ("LCIO_IGNORE_NULL_IN_SUBSET_COLLECTIONS") ;
    if( nullptr != rColChar ) {
      return;
    }
    const std::vector< std::string >* strVec = _evt->getCollectionNames() ;
    for( auto name = strVec->begin() ; name != strVec->end() ; name++) {
    	LCCollection* col = _evt->getCollection( *name ) ;
    	if( col->isSubset() ) {
    	  for( int i=0,N=col->getNumberOfElements() ; i<N ; ++i ) {
    	    if( col->getElementAt( i ) == 0 ) {
    	      std::stringstream sts ;
    	      sts << " SIOReader::postProcessEvent: null pointer in subset collection " 
    		        << *name << " at position: " << i  << std::endl ;
    	      throw Exception( sts.str()  ) ;
    	    }
    	  }
    	}
    }
  }

} // namespace
