#include "SIO/SIOReader.h" 


#include "SIO/LCSIO.h"
#include "SIO/SIOEventHandler.h" 
#include "SIO/SIOCollectionHandler.h"
//#include "SIO/SIOLCRelationHandler.h"
#include "SIO/SIORunHeaderHandler.h"
#include "SIO/SIOParticleHandler.h"
#include "SIO/SIOUnpack.h"
#include "SIO/SIOWriter.h"

#include "LCIOSTLTypes.h"

#include "EVENT/LCIO.h"

#include "SIO_streamManager.h" 
#include "SIO_recordManager.h" 
#include "SIO_blockManager.h" 
#include "SIO_stream.h" 
#include "SIO_record.h" 
#include "IMPL/LCIOExceptionHandler.h"


#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <climits>
//#include <limits>

//#include <sys/stat.h> 

using namespace EVENT ;
using namespace IO ;
using namespace IOIMPL ;
using namespace IMPL ;

#define EVENTKEY(RN,EN)  ( EVENT::long64( RN ) << 32 ) | EN 
 
namespace SIO {

  // small helper class to activate the unpack mode of the
  // the SIO record for lifetime of this object (the current scope)
//   class SIORecordUnpack{
//   protected:
//     SIORecordUnpack() ;
//     SIO_record* _rec ;
//   public:
//     SIORecordUnpack(SIO_record* rec):_rec(rec){
//       _rec->setUnpack( true ) ;
//     }
//     ~SIORecordUnpack(){
//       _rec->setUnpack( false ) ;
//     }
//   };

  
  //#define DEBUG 1
  
  SIOReader::SIOReader( int lcReaderFlag ) :
    _readEventMap( lcReaderFlag & LCReader::directAccess  )
    //     :     
    //     _myFilenames(0), 
    //     _currentFileIndex(0) 
  {
    _myFilenames = 0 ;
    _currentFileIndex = 0 ;

    _evtP = new LCEventIOImpl* ;
    *_evtP = 0 ;

    _runP = new LCRunHeaderIOImpl* ;
    *_runP = 0 ;


//     SIORunHeaderHandler* rh   = dynamic_cast<SIORunHeaderHandler*> 
//       ( SIO_blockManager::get( LCSIO::RUNBLOCKNAME  ) ) ;
//     if( rh == 0 ) 
//       rh = new SIORunHeaderHandler( LCSIO::RUNBLOCKNAME, _runP ) ;
//     else
//       rh->setRunHeaderPtr( _runP ) ;

//     SIOEventHandler* eh  = dynamic_cast<SIOEventHandler*> 
//       ( SIO_blockManager::get( LCSIO::HEADERBLOCKNAME  ) ) ;
//     if( eh == 0 ) 
//       eh = new SIOEventHandler( LCSIO::HEADERBLOCKNAME, _evtP ) ;
//     else
//       eh->setEventPtr( _evtP ) ;

    _runHandler = new SIORunHeaderHandler( LCSIO::RUNBLOCKNAME, _runP ) ;
    _evtHandler = new SIOEventHandler( LCSIO::HEADERBLOCKNAME, _evtP ) ;


    // debug
//     std::cout << " _runHandler created : " << _runHandler 
// 	      << " with _runP " << _runP
// 	      << std::endl ;
//     std::cout << " _evtHandler created : " << _evtHandler 
// 	      << " with _evtP " << _evtP
// 	      << std::endl ;



#ifdef DEBUG
    SIO_streamManager::setVerbosity( SIO_ALL ) ;
    SIO_recordManager::setVerbosity( SIO_ALL ) ;
    SIO_blockManager::setVerbosity( SIO_ALL ) ;
#else
    SIO_streamManager::setVerbosity( SIO_SILENT ) ;
    SIO_recordManager::setVerbosity( SIO_SILENT ) ;
    SIO_blockManager::setVerbosity( SIO_SILENT ) ;
#endif  


    LCIOExceptionHandler::createInstance() ;


  }

  SIOReader::~SIOReader(){
    
    delete *_evtP ;
    delete *_runP ;    
    delete _evtP ;
    delete _runP ;    
    delete  _runHandler ;
    delete  _evtHandler ;


    SIO_blockManager::clear() ;
  }

  void SIOReader::open(const std::vector<std::string>& filenames) 
    throw( IOException , std::exception){

    unsigned int i;
    struct stat fileinfo ;
    std::string missing_files;
    
    // JE: first we check if all files exist
    for(i=0; i < filenames.size(); i++){
      
      if ( FSTAT( filenames[i].c_str(), &fileinfo ) != 0 ){
	missing_files += filenames[i] ;
	missing_files += "  " ;
      }
    }
    
    // JE: if not raise IOException
    if( missing_files.size() != 0 ){
      throw IOException( std::string( "[SIOReader::open()] File(s) not found:  " + missing_files )) ;
    }
    
    _myFilenames = &filenames ;
    _currentFileIndex = 0 ;
    open( (*_myFilenames)[ _currentFileIndex ]  ) ;
  }

  void SIOReader::open(const std::string& filename) throw( IOException , std::exception)  {


    std::string sioFilename ;  
    // ---- we don't require the standard file extension for reading any more
    //if( !( filename.rfind(".") filename.length() ))
    //  sioFilename = filename + LCSIO::FILE_EXTENSION ;
    //else 
    sioFilename = filename ;
    
    std::string stream_name = LCSIO::getValidSIOName(sioFilename) ;
    _stream = SIO_streamManager::add(  stream_name.c_str() , 64 * SIO_KBYTE ) ;

    if( _stream == 0 )
      throw IOException( std::string( "[SIOReader::open()] Bad stream name: " 
    				      + stream_name  )) ;

    int status = _stream->open( sioFilename.c_str() , SIO_MODE_READ ) ; 
    
    if( status != SIO_STREAM_SUCCESS ) 
      throw IOException( std::string( "[SIOReader::open()] Can't open stream: "
				      + sioFilename ) ) ;


    if( (SIOWriter::_hdrRecord = SIO_recordManager::get( LCSIO::HEADERRECORDNAME )) == 0 )
      SIOWriter::_hdrRecord = SIO_recordManager::add( LCSIO::HEADERRECORDNAME ) ;
    
    if( (SIOWriter::_evtRecord  = SIO_recordManager::get( LCSIO::EVENTRECORDNAME )) ==0 ) 
      SIOWriter::_evtRecord = SIO_recordManager::add( LCSIO::EVENTRECORDNAME ) ;
    
    if( (SIOWriter::_runRecord  = SIO_recordManager::get( LCSIO::RUNRECORDNAME )) ==0 )
      SIOWriter::_runRecord = SIO_recordManager::add( LCSIO::RUNRECORDNAME ) ;   


//     SIORunHeaderHandler* rh   = dynamic_cast<SIORunHeaderHandler*> 
//       ( SIO_blockManager::get( LCSIO::RUNBLOCKNAME  ) ) ;
//     if( rh == 0 ) 
//       rh = new SIORunHeaderHandler( LCSIO::RUNBLOCKNAME, _runP ) ;
//     else
//       rh->setRunHeaderPtr( _runP ) ;

//     SIOEventHandler* eh  = dynamic_cast<SIOEventHandler*> 
//       ( SIO_blockManager::get( LCSIO::HEADERBLOCKNAME  ) ) ;
//     if( eh == 0 ) 
//       eh = new SIOEventHandler( LCSIO::HEADERBLOCKNAME, _evtP ) ;
//     else
//       eh->setEventPtr( _evtP ) ;

    if( _readEventMap ){
        getEventMap() ;
    }

  }
  
  void SIOReader::getEventMap() {


    int status = _stream->seek(0) ; // go to start - FIXME - should we store the current position ?

    if( status != SIO_STREAM_SUCCESS ) 
      throw IOException( std::string( "[SIOReader::getEventMap()] Can't seek stream to 0" ) ) ;
    
//    std::cout << " SIOReader::getEventMap() recreating event map for direct access ..." 
//	      << std::endl ;
    
    { // -- scope for unpacking evt header --------
      SIOUnpack hdrUnp( SIOUnpack::EVENTHDR ) ;
      
      while( true ){
	
	SIO_blockManager::remove(  LCSIO::HEADERBLOCKNAME ) ;
	SIO_blockManager::add( _evtHandler ) ;

	//----	  readRecord() ;
	// read the next record from the stream
	if( _stream->getState()== SIO_STATE_OPEN ){
      
	  unsigned int status =  _stream->read( &_dummyRecord ) ;
	  
	  if( ! (status & 1)  ){

	    if( status & SIO_STREAM_EOF ){
	      break ;
	    }
	    
	    throw IOException( std::string(" io error on stream: ") + *_stream->getName() ) ;
	  }
	} else {
	  throw IOException( std::string(" stream not open: ")+ *_stream->getName() ) ;
	}
	
	//--
	int runNum = (*_evtP)->getRunNumber() ;
	int evtNum = (*_evtP)->getEventNumber() ;
	
        _evtMap[  EVENTKEY( runNum , evtNum ) ] = _stream->lastRecordStart() ;
	
// 	EVENT::long64 key  = (EVENT::long64( runNum ) << 32 ) | evtNum ;
// 	std::cout << "  " <<  key << " - " << _stream->lastRecordStart()  
// 		  << " evt: " << evtNum << std::endl ;
	
      } // while

      _stream->seek(0) ; // go to start - FIXME - should we store the current 

      if( status != SIO_STREAM_SUCCESS ) 
	throw IOException( std::string( "[SIOReader::getEventMap()] Can't seek stream to 0" ) ) ;

    }// -- end of scope for unpacking evt header --

//    std::cout << " SIOReader::getEventMap() : done " << std::endl ;
  }

  void SIOReader::readRecord() throw (IOException , EndOfDataException , std::exception) {

    SIO_blockManager::remove(  LCSIO::RUNBLOCKNAME ) ;
    //_runHandler->setRunHeaderPtr( _runP ) ;
    SIO_blockManager::add( _runHandler ) ;
    
    SIO_blockManager::remove(  LCSIO::HEADERBLOCKNAME ) ;
    //_evtHandler->setEventPtr( _evtP ) ;
    SIO_blockManager::add( _evtHandler ) ;
    
//     // try to set the proper handlers every time before reading a record
//     SIORunHeaderHandler* rh   = dynamic_cast<SIORunHeaderHandler*> 
//       ( SIO_blockManager::get( LCSIO::RUNBLOCKNAME  ) ) ;
//     if( rh == 0 ) 
//       rh = new SIORunHeaderHandler( LCSIO::RUNBLOCKNAME, _runP ) ;
//     else
//       rh->setRunHeaderPtr( _runP ) ;
//     SIOEventHandler* eh  = dynamic_cast<SIOEventHandler*> 
//       ( SIO_blockManager::get( LCSIO::HEADERBLOCKNAME  ) ) ;
//     if( eh == 0 ) 
//       eh = new SIOEventHandler( LCSIO::HEADERBLOCKNAME, _evtP ) ;
//     else
//       eh->setEventPtr( _evtP ) ;


    // read the next record from the stream
    if( _stream->getState()== SIO_STATE_OPEN ){
      
      unsigned int status =  _stream->read( &_dummyRecord ) ;

      if( ! (status & 1)  ){

	if( status & SIO_STREAM_EOF ){

	  // if we have a list of filenames open the next file
	  if( _myFilenames != 0  && ++_currentFileIndex < _myFilenames->size()  ){
	    close() ;

	    open( (*_myFilenames)[ _currentFileIndex  ] ) ;

	    readRecord() ;
	    return ;
	  }

	  throw EndOfDataException("EOF") ;
	}

	throw IOException( std::string(" io error on stream: ") + *_stream->getName() ) ;

      }

      // if the record was an event header, we need to set up the collection handlers
      // for the next event record.
      if( ! strcmp( _dummyRecord->getName()->c_str() , LCSIO::HEADERRECORDNAME )){
	setUpHandlers() ;
      }


    }else{
      throw IOException( std::string(" stream not open: ")+ *_stream->getName() ) ;
    }
  }
  

  LCRunHeader* SIOReader::readNextRunHeader() throw (IOException , std::exception ) {
    return readNextRunHeader( LCIO::READ_ONLY ) ;
  }

  LCRunHeader* SIOReader::readNextRunHeader(int accessMode) throw (IOException , std::exception ) {

    // set the _runRecord to unpack for this scope
    //    SIORecordUnpack runUnp( SIOWriter::_runRecord ) ;
    SIOUnpack runUnp( SIOUnpack::RUN ) ;


    // this might throw the exceptions
    try{ 
      readRecord() ;
    }
    catch(EndOfDataException){
      return 0 ;
    }
    
    // set the proper acces mode before returning the event
    (*_runP)->setReadOnly(  accessMode == LCIO::READ_ONLY   ) ;
    return *_runP ;
  }
  
  void SIOReader::setUpHandlers(){

    // use event *_evtP to setup the block readers from header information ....
    const std::vector<std::string>* strVec = (*_evtP)->getCollectionNames() ;
    for( std::vector<std::string>::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
      
      const LCCollection* col = (*_evtP)->getCollection( *name ) ;


      // check if block handler exists in manager
      SIOCollectionHandler* ch = dynamic_cast<SIOCollectionHandler*> 
	( SIO_blockManager::get( name->c_str() )  ) ;
      
      // if not, create a new block handler
      if( ch == 0 ) {
	
	// create collection handler for event
	try{
	  ch =  new SIOCollectionHandler( *name, col->getTypeName() , _evtP )  ;
	  // calls   SIO_blockManager::add( ch )  in the c'tor !
	}
	catch(Exception& ex){   // unsuported type !
	  delete ch ;
	  ch =  0 ;
	}

      }
      // else { // handler already exists
      if( ch != 0 )
	ch->setEvent( _evtP ) ; 
      //      }
    }


//     //---- fg 20040504 add block handlers for the relations in the event
//     const StringVec* relNames =  (*_evtP)->getRelationNames() ;
//     for( StringVec::const_iterator relName = relNames->begin() ; relName != relNames->end() ; relName++ ) {

//       //      const LCRelation rel = (*_evtP)->getRelation( *relName )  ;

//       // check if block handler exists in manager
//       SIOLCRelationHandler* rh = dynamic_cast<SIORelationHandler*> 
// 	( SIO_blockManager::get( relName->c_str() )  ) ;
      
//       // if not, create a new block handler
//       if( rh == 0 ) {
	
// 	// create collection handler for event
// 	try{
// 	  rh =  new SIOLCRelationHandler( *relName , _evtP )  ;
// 	  // calls   SIO_blockManager::add( rh )  in the c'tor !
// 	}
// 	catch(Exception& ex){   // unsuported type !
// 	  delete rh ;
// 	  rh =  0 ;
// 	}

//       } else { // handler already exists
// 	if( rh != 0 )
// 	  rh->setEvent( _evtP ) ; 
//       }
//     }
//     // ---- fg 20040504 ----------

  }


  LCEvent* SIOReader::readNextEvent() throw (IOException , std::exception ) {

    return readNextEvent( LCIO::READ_ONLY ) ;

  }

  LCEvent* SIOReader::readNextEvent(int accessMode) throw (IOException, std::exception ) {
    

    // first, we need to read the event header 
    // to know what collections are in the event
    { // -- scope for unpacking evt header --------
      
      //      SIORecordUnpack hdrUnp( SIOWriter::_hdrRecord ) ;
      SIOUnpack hdrUnp( SIOUnpack::EVENTHDR ) ;
      
      try{ 
	readRecord() ;
      }
      catch(EndOfDataException){
	return 0 ;
      }
      
    }// -- end of scope for unpacking evt header --
    
    { // now read the event record
      //      SIORecordUnpack evtUnp( SIOWriter::_evtRecord ) ;
      SIOUnpack evtUnp( SIOUnpack::EVENT ) ;
      
      try{ 
	readRecord() ;
      }
      catch(EndOfDataException){
	return 0 ;
      }
      
//       //---debug------------------------
//       LCEventIOImpl* evt = *_evtP ; 
//       const StringVec* colNames = evt->getCollectionNames() ;
//       for( StringVec::const_iterator it = colNames->begin() ;
//       it != colNames->end() ; it++) {
// 	LCCollection* col = evt->getCollection( *it ) ; 
// 	if( col->getTypeName() == LCIO::MCPARTICLE ) {
// 	  for(int i=0;i < col->getNumberOfElements() ; i++){
// 	    std::cout <<  " -- " << i << ": " << col->getElementAt(i) << std::endl ;
// 	  }
// 	}
//       }
//       //---debug------------------------

      // set the proper acces mode before returning the event
       (*_evtP)->setAccessMode( accessMode ) ;
      
//       // restore the daughter relations from the parent relations
//       SIOParticleHandler::restoreParentDaughterRelations( *_evtP ) ;
       postProcessEvent() ;
     
      return *_evtP ;      
    }
  }
  
  void SIOReader::skipNEvents(int n) {
    
    int eventsSkipped = 0 ;
    
    SIOUnpack hdrUnp( SIOUnpack::EVENTHDR ) ;
    
    while( eventsSkipped++ < n ){
      
      try { 
	
	readRecord() ;

      }
      catch(EndOfDataException){

	return ;
      }
    }

    // now we need to also read the next  record which suposedly is an event record
    // in order to prevent readStream from reading this event (the last to be skipped)
    SIOUnpack evtUnp( SIOUnpack::EVENT ) ;
    
    try{ 
      readRecord() ;
    }
    catch(EndOfDataException){
      return ;
    }
    
  }

  EVENT::LCEvent * SIOReader::readEvent(int runNumber, int evtNumber) 
    throw (IOException , std::exception) {
    

    EventMap::iterator it = _evtMap.find( EVENTKEY( runNumber,evtNumber ) ) ;

    if( it != _evtMap.end() ) {
      
      int status = _stream->seek( it->second ) ;

      if( status != SIO_STREAM_SUCCESS ) 
	throw IOException( std::string( "[SIOReader::readEvent()] Can't seek stream to"
					" requested position" ) ) ;
      
      return readNextEvent() ;
    } 
    else 
 
      return 0 ;


    /* ---- OLD code with fast skip -----------

    bool runFound = false ;
    bool evtFound = false ;
    // check current run - if any
    if( *_runP != 0 ){
      if( (*_runP)->getRunNumber() == runNumber ) runFound = true ;
    }
    // skip through run headers until run found or EOF
    while (!runFound ) {
      if( readNextRunHeader() == 0 ) break ; 
      runFound = ( (*_runP)->getRunNumber() == runNumber ) ;
    }
    if( !runFound ){
//       std::stringstream message ;
//       message << " run not found: " << runNumber << std::ends ;
//       throw NotAvailableException( message.str()  ) ;
      return 0 ;
    }
    { // -- scope for unpacking evt header --------
      //      SIORecordUnpack hdrUnp( SIOWriter::_hdrRecord ) ;
      SIOUnpack hdrUnp( SIOUnpack::EVENTHDR ) ;

      while( !evtFound ){
	
	try{ 
	  readRecord() ;
	}
	catch(EndOfDataException){
	  return 0 ;
	}

	evtFound = ( (*_evtP)->getEventNumber() == evtNumber ) ;
      }
    }// -- end of scope for unpacking evt header --
    
    if( !evtFound ) return 0 ;

    { // now read the event record
      //      SIORecordUnpack evtUnp( SIOWriter::_evtRecord ) ;
      SIOUnpack evtUnp( SIOUnpack::EVENT ) ;
      
      try{ 
	readRecord() ;
      }
      catch(EndOfDataException){
	return 0 ;
      }
      
      // set the proper acces mode before returning the event
      // FIXME : need update mode as well
      // (*_evtP)->setAccessMode( accessMode ) ;
      (*_evtP)->setAccessMode( LCIO::READ_ONLY ) ;
      
//       // restore the daughter relations from the parent relations
//       SIOParticleHandler::restoreParentDaughterRelations( *_evtP ) ;
 	  postProcessEvent() ;

      return *_evtP ;      
    }

    */
  }


  void SIOReader::close() throw (IOException, std::exception ){
  
    int status  =  SIO_streamManager::remove( _stream ) ;
    
    if(! (status &1) ) //  return LCIO::ERROR ;
      throw IOException( std::string("couldn't remove stream") ) ;
    // return LCIO::SUCCESS ; 
  }




  void SIOReader::registerLCEventListener(LCEventListener * ls){ 
    _evtListeners.insert( _evtListeners.end() , ls );
  }
  void SIOReader::removeLCEventListener(LCEventListener * ls){ 
    _evtListeners.erase( _evtListeners.find( ls )  );
  }
  
  void SIOReader::registerLCRunListener(LCRunListener * ls){ 
    _runListeners.insert( _runListeners.end() , ls );
  }

  void SIOReader::removeLCRunListener(LCRunListener * ls){
    _runListeners.erase( _runListeners.find( ls ) );
 }

  void SIOReader::readStream() throw ( IO::IOException, std::exception ){

    int maxInt = INT_MAX ; // numeric_limits<int>::max() ;
    readStream( maxInt ) ;
  }
  void SIOReader::readStream(int maxRecord) throw (IOException, std::exception ){
    

    bool readUntilEOF = false ;
    if( maxRecord == INT_MAX ) readUntilEOF = true ;
    
    // here we need to read all the records on the stream
    // and then notify the listeners depending on the type ....
    
    // set all known records to unpack 
//     SIORecordUnpack runUnp( SIOWriter::_runRecord ) ;
//     SIORecordUnpack hdrUnp( SIOWriter::_hdrRecord ) ;
//     SIORecordUnpack evtUnp( SIOWriter::_evtRecord ) ;
    SIOUnpack allUnp( SIOUnpack::RUN + SIOUnpack::EVENT + SIOUnpack::EVENTHDR ) ;
    
    int recordsRead = 0 ;
    while( recordsRead < maxRecord ){ 
	
      try{ 
	readRecord() ;
      }
      catch(EndOfDataException){
	
	// only throw exception if a 'finite' number of records was 
	// specified that couldn't be read from the file
	if( readUntilEOF ){  
	  return ;
	}else{
	  std::stringstream message ;
	  message << "SIOReader::readStream(int maxRecord) : EOF before " 
		  << maxRecord << " records read from file" << std::ends ;
	  throw EndOfDataException( message.str())  ;
	}
      }
      
      // notify LCRunListeners 
      if( ! strcmp( _dummyRecord->getName()->c_str() , LCSIO::RUNRECORDNAME )){
	
	recordsRead++ ;

	std::set<IO::LCRunListener*>::iterator iter = _runListeners.begin() ;
	while( iter != _runListeners.end() ){

	  (*_runP)->setReadOnly( false ) ;
	  (*iter)->modifyRunHeader( *_runP ) ;

	  (*_runP)->setReadOnly( true ) ;
	  (*iter)->processRunHeader( *_runP ) ;
	  
	  iter++ ;
	}
      }
      // notify LCEventListeners 
      if( ! strcmp( _dummyRecord->getName()->c_str() , LCSIO::EVENTRECORDNAME )){
	
	recordsRead++ ;

	std::set<IO::LCEventListener*>::iterator iter = _evtListeners.begin() ;
	while( iter != _evtListeners.end() ){

// 	  // restore the daughter relations from the parent relations
// 	  SIOParticleHandler::restoreParentDaughterRelations( *_evtP ) ;
	  postProcessEvent() ;

	  // fg20070813 changed order of update and process (needed for 
	  // Marlin modifying processors )
	  (*_evtP)->setAccessMode( LCIO::UPDATE ) ;
	  (*iter)->modifyEvent( *_evtP ) ;

	  (*_evtP)->setAccessMode( LCIO::READ_ONLY ) ; // set the proper acces mode
	  (*iter)->processEvent( *_evtP ) ;


	  iter++ ;
	  
	}
      }
    }
  }
  
  void  SIOReader::postProcessEvent() {
    // restore the daughter relations from the parent relations
    SIOParticleHandler::restoreParentDaughterRelations( *_evtP ) ;
//     // fill the relation map from intermediate vector
//     SIOLCRelationHandler::fillRelationMap(  *_evtP ) ;
  }

} // namespace
