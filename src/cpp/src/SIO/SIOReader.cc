#include "SIO/SIOReader.h" 

#include "SIO/LCSIO.h"
#include "SIO/SIOEventHandler.h" 
#include "SIO/SIOCollectionHandler.h"
#include "SIO/SIORunHeaderHandler.h"

#include "IMPL/LCRunHeaderImpl.h"

#include "IOIMPL/LCEventIOImpl.h"
#include "EVENT/LCIO.h"

#include "SIO_streamManager.h" 
#include "SIO_recordManager.h" 
#include "SIO_blockManager.h" 
#include "SIO_stream.h" 
#include "SIO_record.h" 

#include <iostream>

using namespace EVENT ;
using namespace IO ;
using namespace IOIMPL ;
using namespace IMPL ;

namespace SIO {

  // small helper class to activate the unpack mode of the
  // the SIO record for lifetime of this object (the current scope)
  class SIORecordUnpack{
  protected:
    SIORecordUnpack() ;
    SIO_record* _rec ;
  public:
    SIORecordUnpack(SIO_record* rec):_rec(rec){
      _rec->setUnpack( true ) ;
    }
    ~SIORecordUnpack(){
      _rec->setUnpack( false ) ;
    }
  };
  


  //#define DEBUG 1

  SIOReader::SIOReader() {
  
    _evtP = new LCEventIOImpl* ;
    *_evtP = 0 ;

    _runP = new LCRunHeaderImpl* ;
    *_runP = 0 ;

    // this is our default event 
    // collections are attached to this event when blocks are read

    // FIXME : default event no longer needed ....
    //    _defaultEvt = new LCEventIOImpl ;

#ifdef DEBUG
    SIO_streamManager::setVerbosity( SIO_ALL ) ;
    SIO_recordManager::setVerbosity( SIO_ALL ) ;
    SIO_blockManager::setVerbosity( SIO_ALL ) ;
#else
    SIO_streamManager::setVerbosity( SIO_SILENT ) ;
    SIO_recordManager::setVerbosity( SIO_SILENT ) ;
    SIO_blockManager::setVerbosity( SIO_SILENT ) ;
#endif  

  }

  SIOReader::~SIOReader(){
    delete _evtP ;
    delete _runP ;    
  }



  void SIOReader::open(const std::string& filename) throw( IOException )  {


    std::string sioFilename ;  
    // ---- we don't require the standard file extension for reading any more
    //if( !( filename.rfind(".") filename.length() ))
    //  sioFilename = filename + LCSIO::FILE_EXTENSION ;
    //else 
    sioFilename = filename ;
    
    const char* stream_name = LCSIO::getValidSIOName(sioFilename) ;
    _stream = SIO_streamManager::add(  stream_name , 64 * SIO_KBYTE ) ;

    if( _stream == 0 )
      throw IOException( std::string( "[SIOReader::open()] Bad stream name: " 
    				      + std::string(stream_name)  )) ;
    delete stream_name ;


    int status = _stream->open( sioFilename.c_str() , SIO_MODE_READ ) ; 
    
    if( status != SIO_STREAM_SUCCESS ) 
      throw IOException( std::string( "[SIOReader::open()] Can't open stream: "
				      + sioFilename ) ) ;


    if( (_hdrRecord = SIO_recordManager::get( LCSIO::HEADERRECORDNAME )) == 0 )
      _hdrRecord = SIO_recordManager::add( LCSIO::HEADERRECORDNAME ) ;
    
    if( (_evtRecord  = SIO_recordManager::get( LCSIO::EVENTRECORDNAME )) ==0 ) 
      _evtRecord = SIO_recordManager::add( LCSIO::EVENTRECORDNAME ) ;
    
    if( (_runRecord  = SIO_recordManager::get( LCSIO::RUNRECORDNAME )) ==0 )
      _runRecord = SIO_recordManager::add( LCSIO::RUNRECORDNAME ) ;   

    // create SIOHandlers for event and header and tell SIO about it
    //    SIO_blockManager::add( new SIOEventHandler( LCSIO::EVENTBLOCKNAME, _evtP ) ) ;
    SIO_blockManager::add( new SIOEventHandler( LCSIO::HEADERBLOCKNAME, _evtP ) ) ;
    SIO_blockManager::add( new SIORunHeaderHandler( LCSIO::RUNBLOCKNAME, _runP ) ) ;



  }


  int SIOReader::readRecord(){
    // read the next record from the stream
    if( _stream->getState()== SIO_STATE_OPEN ){
      

      if( SIO_blockManager::get( LCSIO::RUNBLOCKNAME ) == 0 ){ 
	std::cout << LCSIO::RUNBLOCKNAME  << " not in  blockManager ! " << std::endl ;
	return LCIO::ERROR ; }

      unsigned int status =  _stream->read( &_dummyRecord ) ;
      if( ! (status & 1)  ){
	return LCIO::ERROR ;
      }

      // if the record was an event header, we need to set up the collection handlers
      // for the next event record.
      if( ! strcmp( _dummyRecord->getName()->c_str() , LCSIO::HEADERRECORDNAME )){
	setUpHandlers() ;
      }

      // if the record was an LCEvent record, we need to move the collections
      // from the default event to the current event
//       if( ! strcmp( _dummyRecord->getName()->c_str() , LCSIO::EVENTRECORDNAME )){

// 	for ( LCCollectionMap::const_iterator iter= _defaultEvt->_map.begin() ; 
// 	      iter != _defaultEvt->_map.end() ; iter++ ){
// 	  (*_evtP)->_map[ iter->first ] = iter->second ;  
// 	  _defaultEvt->_map.erase( _defaultEvt->_map.find( iter->first ) ) ;
// 	}

//       }
      return LCIO::SUCCESS ;

    }    
    return LCIO::ERROR ;
  }
  

  LCRunHeader* SIOReader::readNextRunHeader(){

    // _runRecord->setUnpack( true ) ;  
    // this sets the _runRecord to unpack for this scope
    SIORecordUnpack runUnp( _runRecord ) ;
    
    if( readRecord() != LCIO::SUCCESS )   return 0 ;
    
    return *_runP ;
    
    //     if( _stream->getState()== SIO_STATE_OPEN ){
    // //       if (*_runP != 0 )  delete *_runP ;
    // //       *_runP = new LCRunHeaderImpl ;
    //       // read header record
    //       unsigned int status =  _stream->read( &_dummyRecord ) ;
    //       if( ! (status & 1)  ){
    // 	delete *_runP ;
    // 	//	_runRecord->setUnpack( false ) ;  
    // 	return 0 ;
    //       }
    //       //      _runRecord->setUnpack( false ) ;  
    //       return *_runP ;
    //     }
    //     //    _runRecord->setUnpack( false ) ;  
    //     return 0 ;
  }
  
  void SIOReader::setUpHandlers(){

    // use event *_evtP to setup the block readers from header information ....
    const std::vector<std::string>* strVec = (*_evtP)->getCollectionNames() ;
    for( std::vector<std::string>::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
      
      // remove any old handler of the same name  
      // these handlers are static - so if we write at the same time (e.g. in a recojob)
      // we remove the hanlders needed there ....
      // this needs more thought ...
      //SIO_blockManager::remove( name->c_str()  ) ;

      const LCCollection* col = (*_evtP)->getCollection( *name ) ;

      // create a collection handler, using the default event to attach the data
      // as the real event might not exist at the time the corresponding block is read
      // (order of blocks in the SIO record is undefined) 
      // collections have to be moved from the default event to the current event 
      // after the LCEvent record has been read in total (see readRecord() )
    //      SIOCollectionHandler* ch =  new SIOCollectionHandler( *name, col->getTypeName() , &_defaultEvt   )  ;

    //SIO_blockManager::add( ch  )  ; 
      
      // check if block handler exists in manager
      SIO_block* oldCH = SIO_blockManager::get( name->c_str() ) ;
      if( oldCH != NULL) {
	// remove and then delete old collection handler
	//SIO_blockManager::remove( name->c_str()  ) ;
	// the d'tor of SIo_block calls remove ....
	delete oldCH ; 
      }

      // create collection handler for event
      SIOCollectionHandler* ch = 0 ;
      try{
	ch =  new SIOCollectionHandler( *name, col->getTypeName() , _evtP )  ;
	SIO_blockManager::add( ch  )  ; 
      }
      catch(Exception& ex){
	// unsuported type
	delete ch ;
      }
    }
  }


  LCEvent* SIOReader::readNextEvent() {

    return readNextEvent( LCIO::READ_ONLY ) ;

  }

  LCEvent* SIOReader::readNextEvent(int accessMode) {
    

    // first, we need to read the event header 
    // to know what collections are in the event
    { // -- scope for unpacking evt header --------
      
      SIORecordUnpack hdrUnp( _hdrRecord ) ;
      if( readRecord() != LCIO::SUCCESS )   return 0 ;
      
    }// -- end of scope for unpacking evt header --
    
    { // now read the event record
      SIORecordUnpack evtUnp( _evtRecord ) ;
      
      if( readRecord() != LCIO::SUCCESS )   return 0 ;
      
      // set the proper acces mode before returnning the event
      (*_evtP)->setAccessMode( accessMode ) ;
      
      return *_evtP ;      
    }

  }


  EVENT::LCEvent * SIOReader::readEvent(int runNumber, int evtNumber) {
    
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
    if( !runFound ) return 0 ; 
    
    { // -- scope for unpacking evt header --------
      SIORecordUnpack hdrUnp( _hdrRecord ) ;
      while( !evtFound ){
	if( readRecord() != LCIO::SUCCESS ) return 0 ;
	evtFound = ( (*_evtP)->getEventNumber() == evtNumber ) ;
      }
    }// -- end of scope for unpacking evt header --
    
    if( !evtFound ) return 0 ;

    { // now read the event record
      SIORecordUnpack evtUnp( _evtRecord ) ;
      
      if( readRecord() != LCIO::SUCCESS )   return 0 ;
      
      // set the proper acces mode before returning the event
      // FIXME : check access mode ...
      // (*_evtP)->setAccessMode( accessMode ) ;
      
      return *_evtP ;      
    }

  }


  int SIOReader::close(){
  
    int status  =  SIO_streamManager::remove( _stream ) ;
    
    if(! (status &1) )  return LCIO::ERROR ;
    //throw IOException( std::string("couldn't remove stream") ) ;
    return LCIO::SUCCESS ; 
  }




  void SIOReader::registerLCEventListener(LCEventListener * ls){ 
    _evtListeners.insert( ls );
  }
  void SIOReader::removeLCEventListener(LCEventListener * ls){ 
    _evtListeners.erase( _evtListeners.find( ls )  );
  }
  
  void SIOReader::registerLCRunListener(LCRunListener * ls){ 
    _runListeners.insert( ls );
  }

  void SIOReader::removeLCRunListener(LCRunListener * ls){
    _runListeners.erase( _runListeners.find( ls ) );
 }

  int SIOReader::readStream() {
    
    // here we need to read all the records on the stream
    // and then notify the listeners depending on the type ....
    
    // set all known records to unpack 
    SIORecordUnpack runUnp( _runRecord ) ;
    SIORecordUnpack hdrUnp( _hdrRecord ) ;
    SIORecordUnpack evtUnp( _evtRecord ) ;

    while( readRecord() == LCIO::SUCCESS  ){

      // notify LCRunListeners 
      if( ! strcmp( _dummyRecord->getName()->c_str() , LCSIO::RUNRECORDNAME )){
	
	std::set<IO::LCRunListener*>::iterator iter = _runListeners.begin() ;
	while( iter != _runListeners.end() ){
	  (*iter)->analyze( *_runP ) ;
	  (*iter)->update( *_runP ) ;
	  iter++ ;
	}
      }
      // notify LCEventListeners 
      if( ! strcmp( _dummyRecord->getName()->c_str() , LCSIO::EVENTRECORDNAME )){

	std::set<IO::LCEventListener*>::iterator iter = _evtListeners.begin() ;
	while( iter != _evtListeners.end() ){
	  // set the proper acces mode for the event
	  (*_evtP)->setAccessMode( LCIO::READ_ONLY ) ;
	  (*iter)->analyze( *_evtP ) ;

	  (*_evtP)->setAccessMode( LCIO::UPDATE ) ;
	  (*iter)->update( *_evtP ) ;
	  iter++ ;

	}
      }
    } 

    return LCIO::SUCCESS ;

 }




}; // namespace
