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

namespace SIO {

  //#define DEBUG 1

  SIOReader::SIOReader() {
  
    _evtP = new LCEventIOImpl* ;
    *_evtP = 0 ;

    _runP = new IMPL::LCRunHeaderImpl* ;
    *_runP = 0 ;


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



  int SIOReader::open(const std::string& filename){
  

    std::string stream_name( filename.data() ,  filename.find(".") ) ;
    _stream = SIO_streamManager::add( stream_name.c_str() , 64 * SIO_KBYTE ) ;
    _stream->open( filename.c_str() , SIO_MODE_READ ) ; 
    
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
    
    
    // no error handling so far ...
    return LCIO::SUCCESS ; 

  }

  LCRunHeader * SIOReader::readNextRunHeader(){
    
    _runRecord->setUnpack( true ) ;  

    if( _stream->getState()== SIO_STATE_OPEN ){
      
      if (*_runP != 0 )  delete *_runP ;
      *_runP = new IMPL::LCRunHeaderImpl  ;
      
      // read header record
      unsigned int status =  _stream->read( &_dummy ) ;
      if( ! (status & 1)  ){
	delete *_runP ;
	_runRecord->setUnpack( false ) ;  
	return 0 ;
      }
      _runRecord->setUnpack( false ) ;  
      return *_runP ;
    }
    _runRecord->setUnpack( false ) ;  
    return 0 ;
  }

  void SIOReader::setUpHandlers(){

    // use event *_evtP to setup the block readers from header information ....
    const StringVec* strVec = (*_evtP)->getCollectionNames() ;
    for( StringVec::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
    
      // remove an old handler of the same name   ??? 
      //   SIO_blockManager::remove( name->c_str()  ) ;

      const LCCollection* col = (*_evtP)->getCollection( *name ) ;

      SIOCollectionHandler* ch =  new SIOCollectionHandler( *name, col->getTypeName() , _evtP   )  ;
      SIO_blockManager::add( ch  )  ; 

    }
  }


  LCEvent* SIOReader::readNextEvent() {

    return readNextEvent( LCIO::READ_ONLY ) ;

  }

  LCEvent* SIOReader::readNextEvent(int accessMode) {
    

    if( _stream->getState()== SIO_STATE_OPEN ){
    
      _hdrRecord->setUnpack( true ) ;
      // create new event with pointer at the known address **_evtP
      // delete the old event first
      // event will be created read only
      if (*_evtP != 0 )  
	delete *_evtP ;
      *_evtP = new LCEventIOImpl()  ;
    
      // read header record first 
      unsigned int status =  _stream->read( &_dummy ) ;
      if( ! (status & 1)  ){
	delete *_evtP ;
	_hdrRecord->setUnpack( false ) ;
	return 0 ;
      }
    
      if( strcmp( _dummy->getName()->c_str() , LCSIO::HEADERRECORDNAME )  ){
	std::cout << " wrong event header record : " <<  *_dummy->getName() << std::endl ;
	_hdrRecord->setUnpack( false ) ;
	return 0 ;
      }
    
      _hdrRecord->setUnpack( false ) ;
      setUpHandlers() ;

      _evtRecord->setUnpack( true ) ;
      delete *_evtP ;
      *_evtP = new LCEventIOImpl()  ;
      
      status =  _stream->read( &_dummy ) ;
      if( ! (status & 1)  ){
	delete *_evtP ;
	_evtRecord->setUnpack( false ) ;
	return 0 ;
      }
    
      if( strcmp( _dummy->getName()->c_str() , LCSIO::EVENTRECORDNAME )){
	std::cout << " wrong event record : "
		  <<  *_dummy->getName() 
		  << std::endl ;
	_evtRecord->setUnpack( false ) ;
	return 0 ;
      }
      // set the proper acces mode before returnning the event
      dynamic_cast<LCEventIOImpl*>(*_evtP)->setAccessMode( accessMode ) ;

      _evtRecord->setUnpack( false ) ;
      return *_evtP ;
    } 
    return  0 ;
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

    // .... 
    
    return LCIO::SUCCESS ;

 }




}; // namespace
