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
    _defaultEvt = new LCEventIOImpl ;

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

    std::string stream_name( filename.data() ,  filename.find(".") ) ;
    _stream = SIO_streamManager::add( stream_name.c_str() , 64 * SIO_KBYTE ) ;
    
    int status = _stream->open( filename.c_str() , SIO_MODE_READ ) ; 
    
    if( status != SIO_STREAM_SUCCESS ) 
      throw IOException( std::string( "[SIOReader::open()] Couldn't open stream" 
				      + filename ) ) ;


    if( (_hdrRecord = SIO_recordManager::get( LCSIO::HEADERRECORDNAME )) == 0 )
      _hdrRecord = SIO_recordManager::add( LCSIO::HEADERRECORDNAME ) ;
    
    if( (_evtRecord  = SIO_recordManager::get( LCSIO::EVENTRECORDNAME )) ==0 ) 
      _evtRecord = SIO_recordManager::add( LCSIO::EVENTRECORDNAME ) ;
    
    if( (_runRecord  = SIO_recordManager::get( LCSIO::RUNRECORDNAME )) ==0 )
      _runRecord = SIO_recordManager::add( LCSIO::RUNRECORDNAME ) ;   

    // create SIOHandlers for event and header and tell SIO about it
    SIO_blockManager::add( new SIOEventHandler( LCSIO::EVENTBLOCKNAME, _evtP ) ) ;
    SIO_blockManager::add( new SIOEventHandler( LCSIO::HEADERBLOCKNAME, _evtP ) ) ;
    SIO_blockManager::add( new SIORunHeaderHandler( LCSIO::RUNBLOCKNAME, _runP ) ) ;


  }


  int SIOReader::readRecord(){
    // read the next record from the stream
    if( _stream->getState()== SIO_STATE_OPEN ){
      
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
      if( ! strcmp( _dummyRecord->getName()->c_str() , LCSIO::EVENTRECORDNAME )){

	for ( LCCollectionMap::const_iterator iter= _defaultEvt->_map.begin() ; 
	      iter != _defaultEvt->_map.end() ; iter++ ){
	  (*_evtP)->_map[ iter->first ] = iter->second ;  
	  _defaultEvt->_map.erase( _defaultEvt->_map.find( iter->first ) ) ;
	}

      }
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
    const StringVec* strVec = (*_evtP)->getCollectionNames() ;
    for( StringVec::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
      
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
      SIOCollectionHandler* ch =  new SIOCollectionHandler( *name, col->getTypeName() , &_defaultEvt   )  ;
      SIO_blockManager::add( ch  )  ; 

    }
  }


  LCEvent* SIOReader::readNextEvent() {

    return readNextEvent( LCIO::READ_ONLY ) ;

  }

  LCEvent* SIOReader::readNextEvent(int accessMode) {
    

    // first, we need to read the event header 
    // to know what collections are in the event
    { // -- scope for unpacking run header --------
      
      SIORecordUnpack hdrUnp( _hdrRecord ) ;
      if( readRecord() != LCIO::SUCCESS )   return 0 ;
      
    }// -- end of scope for unpacking run header --
    
    setUpHandlers() ;
    
    { // now read the event record
      SIORecordUnpack evtUnp( _evtRecord ) ;
      
      if( readRecord() != LCIO::SUCCESS )   return 0 ;
      
      // set the proper acces mode before returnning the event
      (*_evtP)->setAccessMode( accessMode ) ;
      
      return *_evtP ;      
    }



//       _hdrRecord->setUnpack( true ) ;
//       // create new event with pointer at the known address **_evtP
//       // delete the old event first
//       // event will be created read only
// //        if (*_evtP != 0 )  
// //  	delete *_evtP ;
// //        *_evtP = new LCEventIOImpl()  ;
    
//       // read header record first 
//       unsigned int status =  _stream->read( &_dummyRecord ) ;
//       if( ! (status & 1)  ){
// 	delete *_evtP ;
// 	_hdrRecord->setUnpack( false ) ;
// 	return 0 ;
//       }
      
//       if( strcmp( _dummyRecord->getName()->c_str() , LCSIO::HEADERRECORDNAME )  ){
// 	std::cout << " wrong event header record : " <<  *_dummyRecord->getName() << std::endl ;
// 	_hdrRecord->setUnpack( false ) ;
// 	return 0 ;
//       }
//       _hdrRecord->setUnpack( false ) ;

//       setUpHandlers() ;

//       _evtRecord->setUnpack( true ) ;
// //        delete *_evtP ;
// //        *_evtP = new LCEventIOImpl()  ;
      
//       status =  _stream->read( &_dummyRecord ) ;
//       if( ! (status & 1)  ){
// 	delete *_evtP ;
// 	_evtRecord->setUnpack( false ) ;
// 	return 0 ;
//       }
    
//       if( strcmp( _dummyRecord->getName()->c_str() , LCSIO::EVENTRECORDNAME )){
// 	std::cout << " wrong event record : "
// 		  <<  *_dummyRecord->getName() 
// 		  << std::endl ;
// 	_evtRecord->setUnpack( false ) ;
// 	return 0 ;
//       }
//       // set the proper acces mode before returnning the event
//       dynamic_cast<LCEventIOImpl*>(*_evtP)->setAccessMode( accessMode ) ;


//       // move the collections from the default event to our current event
//       typedef LCCollectionMap::const_iterator LCI ;
//       for ( LCI iter= _defaultEvt->_map.begin() ; iter != _defaultEvt->_map.end() ; iter++ ){

// 	(*_evtP)->_map[ iter->first ] = iter->second ;  

// 	_defaultEvt->_map.erase( _defaultEvt->_map.find( iter->first ) ) ;
//       }

      

//       _evtRecord->setUnpack( false ) ;
//       return *_evtP ;
//     } 
//     return  0 ;
  }


  int SIOReader::close(){
  
    int status  =  SIO_streamManager::remove( _stream ) ;
    
    if(! (status &1) ) return LCIO::ERROR ;

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
