#include "SIO/SIOReader.h" 

#include "SIO/LCSIO.h"
#include "SIO/SIOEventHandler.h" 
#include "SIO/SIOCollectionHandler.h"

#include "IOIMPL/LCEventIOImpl.h"
#include "EVENT/LCIO.h"

#include "SIO_streamManager.h" 
#include "SIO_recordManager.h" 
#include "SIO_blockManager.h" 
#include "SIO_stream.h" 
#include "SIO_record.h" 


using namespace EVENT ;
using namespace IOIMPL ;

namespace SIO {

  //#define DEBUG 1

  SIOReader::SIOReader() {
  
    _evtP = new LCEventIOImpl* ;
    *_evtP = 0 ;

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
  }


  int SIOReader::open(std::string filename){
  

    string stream_name( filename.begin() ,  filename.find(".") ) ;
    _stream = SIO_streamManager::add( stream_name.c_str() , 64 * SIO_KBYTE ) ;
    _stream->open( filename.c_str() , SIO_MODE_READ ) ; 
  
    if( (_hdrRecord = SIO_recordManager::get( LCSIO::HEADERRECORDNAME )) == 0 )
      _hdrRecord = SIO_recordManager::add( LCSIO::HEADERRECORDNAME ) ;

    if( (_evtRecord  = SIO_recordManager::get( LCSIO::EVENTRECORDNAME )) ==0 ) 
      _evtRecord = SIO_recordManager::add( LCSIO::EVENTRECORDNAME ) ;
   
    _hdrRecord->setUnpack( true ) ;
    _evtRecord->setUnpack( true ) ;

    // create SIOHandlers for event and header and tell SIO about it
    SIO_blockManager::add( new SIOEventHandler( LCSIO::EVENTBLOCKNAME, _evtP ) ) ;
    SIO_blockManager::add( new SIOEventHandler( LCSIO::HEADERBLOCKNAME, _evtP ) ) ;
  

    // no error handling so far ...
    return LCIO::SUCCESS ; 

  }

  void SIOReader::setUpHandlers(){

    // use event *_evtP to setup the block readers from header information ....
    StringVec* strVec = (*_evtP)->getCollectionNames() ;
    for( StringVec::iterator name = strVec->begin() ; name != strVec->end() ; name++){
    
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
    
      // create new event with pointer at the known address **_evtP
      // delete the old event first
      // event will be created read only
      if (*_evtP != 0 )  delete *_evtP ;
      *_evtP = new LCEventIOImpl()  ;
    
      // read header record first 
      unsigned int status =  _stream->read( &_hdrRecord ) ;
      if( ! (status & 1)  ){
	delete *_evtP ;
	return 0 ;
      }
    
      if( strcmp( _hdrRecord->getName()->c_str() , LCSIO::HEADERRECORDNAME )  ){
	std::cout << " wrong event header record : " <<  *_hdrRecord->getName() << std::endl ;
	return 0 ;
      }
    
      setUpHandlers() ;

      delete *_evtP ;
      *_evtP = new LCEventIOImpl()  ;
    
      status =  _stream->read( &_hdrRecord ) ;
      if( ! (status & 1)  ){
	delete *_evtP ;
	return 0 ;
      }
    
      if( strcmp( _hdrRecord->getName()->c_str() , LCSIO::EVENTRECORDNAME )){
	std::cout << " wrong event record : "
		  <<  *_hdrRecord->getName() 
		  << std::endl ;
	return 0 ;
      }
      // set the proper acces mode before returnning the event
      dynamic_cast<LCEventIOImpl*>(*_evtP)->setAccessMode( accessMode ) ;
      return *_evtP ;
    } 
    return  0 ;
  }


  int SIOReader::close(){
  
    _stream->close() ;
    // no error handling so far ...
    return LCIO::SUCCESS ; 
  }

}; // namespace
