#include "SIO/SIOWriter.h" 

#include "DATA/LCEventData.h"
#include "DATA/LCRunHeaderData.h"
#include "EVENT/LCIO.h"
#include "DATA/LCCollectionData.h"

#include "SIO/LCSIO.h"
#include "SIO/SIOEventHandler.h" 
#include "SIO/SIOCollectionHandler.h" 
#include "SIO/SIORunHeaderHandler.h" 

#include "SIO_streamManager.h" 
#include "SIO_recordManager.h" 
#include "SIO_blockManager.h" 
#include "SIO_stream.h" 
#include "SIO_record.h" 

//#include <iostream>

//#define DEBUG 1
#include "IMPL/LCTOOLS.h"

using namespace DATA ;
//using namespace EVENT ;
using namespace IO ;

namespace SIO {

  SIOWriter::SIOWriter() : _hdrHandler(0), _runHandler(0) {
  
#ifdef DEBUG
    SIO_streamManager::setVerbosity( SIO_ALL ) ;
    SIO_recordManager::setVerbosity( SIO_ALL ) ;
    SIO_blockManager::setVerbosity( SIO_ALL ) ;
#else
    SIO_streamManager::setVerbosity( SIO_SILENT ) ;
    SIO_recordManager::setVerbosity( SIO_SILENT ) ;
    SIO_blockManager::setVerbosity(  SIO_SILENT ) ;
#endif
  
    //    evtP = new LCEvent* ;

  }

  SIOWriter::~SIOWriter(){

    delete _hdrHandler ;
    delete _runHandler ;
    //delete evtP ;

  }



  void SIOWriter::open(const std::string & filename) throw(IOException ){

    std::string sioFilename ;  
    getSIOFileName( filename, sioFilename ) ;

    // if the file exists we throw an exception

    FILE* f = fopen( sioFilename.c_str() , "r") ;
    if( f != 0 ){
      fclose(f) ;
      throw IOException( std::string( "[SIOWriter::open()] File already exists: " 
    				      + sioFilename
				      + " \n              open it in append or new mode !\n"
				      )) ;
    }
    // open new file for writing
    return open( filename, EVENT::LCIO::WRITE_NEW ) ;
  }

  void SIOWriter::getSIOFileName(const std::string& filename, 
				 std::string& sioFilename ) {

    if( !( filename.rfind(LCSIO::FILE_EXTENSION) 
	   + strlen( LCSIO::FILE_EXTENSION ) == filename.length() ))
      sioFilename = filename + LCSIO::FILE_EXTENSION ;
    else 
      sioFilename = filename ;    
  } 

  void SIOWriter::open(const std::string& filename, int writeMode) throw(IOException ) {

    
    // make sure filename has the proper extension (.slcio) 
    std::string sioFilename ;  
    getSIOFileName( filename, sioFilename ) ;

    // SIO has some rules about valid names for streams, records, etc ...
    const char* stream_name = LCSIO::getValidSIOName(sioFilename) ;
    _stream = SIO_streamManager::add(  stream_name , 64 * SIO_KBYTE ) ;
    
    if( _stream == 0 )
      throw IOException( std::string( "[SIOWriter::open()] Bad or duplicate stream name: " 
				      + std::string(stream_name)  )) ;
    delete stream_name ;
    

    unsigned int  status = 0  ;
    switch( writeMode ) 
      {
      case EVENT::LCIO::WRITE_NEW : 
	status  = _stream->open( sioFilename.c_str() , SIO_MODE_WRITE_NEW ) ; 
	break ;
      case EVENT::LCIO::WRITE_APPEND : 
	status  = _stream->open( sioFilename.c_str() , SIO_MODE_WRITE_APPEND ) ; 
	break ;
      }

    if( !(status &1) )
      throw IOException( std::string( "[SIOWriter::open()] Couldn't open file: " 
				      +  sioFilename ) ) ;
      
  
    // tell SIO the record names if not yet known 
    if( (_runRecord = SIO_recordManager::get( LCSIO::RUNRECORDNAME )) == 0 )
      _runRecord = SIO_recordManager::add( LCSIO::RUNRECORDNAME ) ;
    if( (_hdrRecord = SIO_recordManager::get( LCSIO::HEADERRECORDNAME )) == 0 )
      _hdrRecord = SIO_recordManager::add( LCSIO::HEADERRECORDNAME ) ;
    if( (_evtRecord = SIO_recordManager::get( LCSIO::EVENTRECORDNAME )) ==0 ) 
      _evtRecord = SIO_recordManager::add( LCSIO::EVENTRECORDNAME ) ;

    

    _hdrRecord->setCompress( LCSIO::COMPRESSION ) ;
    _evtRecord->setCompress( LCSIO::COMPRESSION ) ;
    _runRecord->setCompress( LCSIO::COMPRESSION ) ;
    
  }


  void SIOWriter::writeRunHeader(const DATA::LCRunHeaderData * hdr)  throw(IOException ) {

    // create a new handler for every new run 
    
    if( !_runHandler){
      _runHandler = new SIORunHeaderHandler( LCSIO::RUNBLOCKNAME  ) ;
      _runRecord->connect( _runHandler );
    }
    _runHandler->setRunHeader(  hdr ) ;

    if( _stream->getState()== SIO_STATE_OPEN ){
      
      // write LCRunHeader record
      unsigned int status =  _stream->write( LCSIO::RUNRECORDNAME    ) ;
   
      if( !(status & 1)  )
	throw IOException( std::string( "[SIOWriter::writeRunHeader] couldn't write run header to stream: "
					+  *_stream->getName() ) ) ;
    } else {
      
      throw IOException( std::string( "[SIOWriter::writeRunHeader] stream not opened: "
				      +  *_stream->getName() ) ) ;
      
    }
  }



  /** Creates Handlers needed for writing the event on this stream.
   * Needs to be called for every event.
   */
  void SIOWriter::setUpHandlers(const LCEventData * evt){
  
    // create SIOHandler for event 
    // and connect it to the sio record

    if( ! _hdrHandler ){
      _hdrHandler = new SIOEventHandler( LCSIO::HEADERBLOCKNAME ) ;
      _hdrRecord->connect( _hdrHandler ) ;
    }

    // now create handlers for all collections in the event
    // and connect them to the record
    // disconnect and delete old block handlers first 
    typedef std::vector< SIOCollectionHandler* >::iterator CHI ;
    for( CHI ch = _colVector.begin() ; ch !=  _colVector.end() ; ch++){
      _evtRecord->disconnect( *ch );
      delete *ch ;
    }
    _colVector.clear() ;
    
    
    const std::vector<std::string>* strVec = evt->getCollectionNames() ;
    
    for( std::vector<std::string>::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){

      const LCCollectionData* col = evt->getCollectionData( *name ) ;

      SIOCollectionHandler* ch = new SIOCollectionHandler( *name, col->getTypeName() ) ;
      
      ch->setCollection( col ) ; 

      _colVector.push_back( ch ) ;  
      _evtRecord->connect( ch ) ;
    } 
    
  }

  void SIOWriter::writeEvent(const LCEventData* evt)  throw(IOException ) {
  

    //here we set up the collection handlers 

    try{   setUpHandlers( evt) ;
    
    }catch(...){
      throw IOException(  "[SIOWriter::writeEvent] could not set up handlers " ) ;
    }

    if( _stream->getState()== SIO_STATE_OPEN ){
   
      // need to set the event in event header handler
      _hdrHandler->setEvent( evt ) ;

      unsigned int status ;

      // write LCEventHeader record
      status =  _stream->write( LCSIO::HEADERRECORDNAME    ) ;

      if( ! (status & 1) )
	throw IOException(  std::string("[SIOWriter::writeEvent] couldn't write event header to stream: "
					+  *_stream->getName() )) ;
      
      
      // write the event record
      status =  _stream->write( LCSIO::EVENTRECORDNAME    ) ;

      if( ! (status & 1) )
	throw IOException(  std::string("[SIOWriter::writeEvent] couldn't write event header to stream: "
					+  *_stream->getName() )) ;
    }
    else      

      throw IOException(  std::string("[SIOWriter::writeEvent] stream not opened : "
				      +  *_stream->getName()  )) ;
    
  }


  void SIOWriter::close() throw (IOException) {
  
    const std::string* streamName  = _stream->getName() ;

    int status  =  SIO_streamManager::remove( _stream ) ;
    
    if(! (status &1) ) 
      throw IOException(  std::string("[SIOWriter::close] couldn't close stream  : "
				      + *streamName  )) ;

  }

}; // namespace

