#include "SIO/SIOWriter.h" 

#include "EVENT/LCEvent.h"
#include "EVENT/LCIO.h"
#include "EVENT/LCCollection.h"

#include "SIO/SIOEventHandler.h" 
#include "SIO/SIOEventHeaderHandler.h" 
#include "SIO/SIOCollectionHandler.h" 

#include "SIO_streamManager.h" 
#include "SIO_recordManager.h" 
#include "SIO_blockManager.h" 
#include "SIO_stream.h" 
#include "SIO_record.h" 

#include <iostream>

//#define DEBUG 1

using namespace EVENT ;


namespace SIO {

SIOWriter::SIOWriter() : isFirstEvent(true)   {
  
#ifdef DEBUG
  SIO_streamManager::setVerbosity( SIO_ALL ) ;
  SIO_recordManager::setVerbosity( SIO_ALL ) ;
  SIO_blockManager::setVerbosity( SIO_ALL ) ;
#else
  SIO_streamManager::setVerbosity( SIO_SILENT ) ;
  SIO_recordManager::setVerbosity( SIO_SILENT ) ;
  SIO_blockManager::setVerbosity(  SIO_SILENT ) ;
#endif
  
  evtP = new LCEvent* ;

}

SIOWriter::~SIOWriter(){

  delete evtP ;

}


int SIOWriter::open(std::string filename){
  
  unsigned int status ;
  string stream_name( filename.begin() ,  filename.find(".") ) ;

 
  _stream = SIO_streamManager::add( stream_name.c_str() , 64 * SIO_KBYTE ) ;
  status  = _stream->open( filename.c_str() , SIO_MODE_WRITE_NEW ) ; 

  if( !(status &1) ) return LCIO::ERROR ;
  
  if( (_hdrRecord = SIO_recordManager::get( SIOWriter::HEADERRECORDNAME )) == 0 )
    _hdrRecord = SIO_recordManager::add( SIOWriter::HEADERRECORDNAME ) ;
  if( (_evtRecord = SIO_recordManager::get( SIOWriter::EVENTRECORDNAME )) ==0 ) 
    _evtRecord = SIO_recordManager::add( SIOWriter::EVENTRECORDNAME ) ;


  _hdrRecord->setCompress( false ) ;
  _evtRecord->setCompress( false ) ;

  return LCIO::SUCCESS ;
}


/** Creates Handlers needed for writing the events on this stream.
 */
void SIOWriter::setUpHandlers(const LCEvent * evt){
  
  // create SIOHandler for event 
  // and connect it to the sio record

  _evtHandler = new SIOEventHandler() ;
  _hdrHandler = new SIOEventHeaderHandler() ;
  _evtRecord->connect( _evtHandler ) ;
  _hdrRecord->connect( _hdrHandler ) ;


  // now create handlers for all collections in the event
  // and connect them to the record
  
  StringVec* strVec = evt->getCollectionNames() ;

  for( StringVec::iterator name = strVec->begin() ; name != strVec->end() ; name++){
    
    const LCCollection* col = evt->getCollection( *name ) ;

    SIOCollectionHandler* ch = new SIOCollectionHandler( *name, col->getTypeName() ) ;

    _colVector.push_back( ch ) ;  
    _evtRecord->connect( ch ) ;
  } 
    
}

int SIOWriter::writeEvent(const LCEvent* evt){
  
  if( isFirstEvent ){
    setUpHandlers( evt) ;
    isFirstEvent = false ;
  }
  
  if( _stream->getState()== SIO_STATE_OPEN ){
    
    // need to set the event in event (header) handlers
    
    _evtHandler->setEvent( evt ) ;
    _hdrHandler->setEvent( evt ) ;

    // set the collections in the corresponding handlers
    typedef std::vector< SIOCollectionHandler* >::iterator CHI ;
    for( CHI ch = _colVector.begin() ; ch !=  _colVector.end() ; ch++){
      (*ch)->setCollection(  evt->getCollection(*(*ch)->getName()) ) ;
    }

    // write LCEventHeader record
    unsigned int status =  _stream->write( SIOWriter::HEADERRECORDNAME    ) ;
    if( ! status & 1 ){
      std::cout << "ERROR: couldn't write event header to stream : " 
		<< _stream->getName()  
		<<  "  status  : " << status << std::endl ;
    }

    // write the event record
    status =  _stream->write( SIOWriter::EVENTRECORDNAME    ) ;
    if( ! status & 1 ){
      std::cout << "ERROR: couldn't write event to stream : " 
		<< _stream->getName()
		<<  "  status  : " << status << std::endl ;
      return LCIO::ERROR ;
    }
    
  } else {
    
    std::cout << "ERROR: can't write to stream : " << _stream << std::endl ;
    return LCIO::ERROR ;
   
  }
  
  // no error handling, really ...
  return LCIO::SUCCESS ;
}


int SIOWriter::close(){
  
  _stream->close() ;
  
  return LCIO::SUCCESS ; 
}

}; // namespace

