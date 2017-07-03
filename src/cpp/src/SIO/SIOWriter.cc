#include "SIO/SIOWriter.h" 

#include "EVENT/LCEvent.h"
#include "EVENT/LCRunHeader.h"
#include "EVENT/LCIO.h"
#include "EVENT/LCCollection.h"

#include "SIO/LCSIO.h"
#include "SIO/SIOEventHandler.h" 
#include "SIO/SIOCollectionHandler.h" 
//#include "SIO/SIOLCRelationHandler.h" 
#include "SIO/SIORunHeaderHandler.h" 

#include "SIO/SIORandomAccessHandler.h"
#include "SIO/SIOIndexHandler.h"

#include "LCIOSTLTypes.h"

#include "SIO_streamManager.h" 
#include "SIO_recordManager.h" 
#include "SIO_blockManager.h" 
#include "SIO_stream.h" 
#include "SIO_record.h" 
#include "IMPL/LCIOExceptionHandler.h"

//#define DEBUG 1
#include "IMPL/LCTOOLS.h"
#include "IMPL/LCRelationImpl.h" 

#include <cstring>

using namespace EVENT ;
using namespace IO ;
using namespace IMPL ;

namespace SIO {

  SIOWriter::SIOWriter() :  _stream(0),
			    _compressionLevel(-1), 
			    _hdrHandler(0), 
			    _runHandler(0)  {
    
#ifdef DEBUG
    SIO_streamManager::setVerbosity( SIO_ALL ) ;
    SIO_recordManager::setVerbosity( SIO_ALL ) ;
    SIO_blockManager::setVerbosity( SIO_ALL ) ;
#else
    SIO_streamManager::setVerbosity( SIO_SILENT ) ;
    SIO_recordManager::setVerbosity( SIO_SILENT ) ;
    SIO_blockManager::setVerbosity(  SIO_SILENT ) ;
#endif
//     SIO_streamManager::setVerbosity( SIO_ERRORS ) ;
//     SIO_recordManager::setVerbosity( SIO_ERRORS ) ;
//     SIO_blockManager::setVerbosity(  SIO_ERRORS ) ;


    _runHandler = new SIORunHeaderHandler( LCSIO_RUNBLOCKNAME  ) ;
    _hdrHandler = new SIOEventHandler( LCSIO_HEADERBLOCKNAME ) ;
  
//     _evtRecord = LCSIO::records()[ SIORecords::Event ] ;
//     _hdrRecord = LCSIO::records()[ SIORecords::Header ] ;
//     _runRecord = LCSIO::records()[ SIORecords::Run ] ;
    
    LCSIO::records() ; // initialize global SIO records

    LCIOExceptionHandler::createInstance() ;

  }

  SIOWriter::~SIOWriter(){
    
    delete _hdrHandler ;
    delete _runHandler ;
    SIO_blockManager::clear();
    
  }



  void SIOWriter::open(const std::string & filename)  {

    std::string sioFilename ;  
    getSIOFileName( filename, sioFilename ) ;

    // if the file exists we throw an exception

    FILE* f = FOPEN( sioFilename.c_str() , "r") ;
    if( f != 0 ){
      FCLOSE(f) ;
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


    if( filename.rfind(LCSIO::FILE_EXTENSION) == std::string::npos ||  // .slcio not found at all
	!(  filename.rfind(LCSIO::FILE_EXTENSION)
	    + strlen( LCSIO::FILE_EXTENSION ) == filename.length() ) ) {  // found, but not at end 
      
      // find_last_of looks for characters and not substrings !!
//     if( filename.find_last_of(LCSIO::FILE_EXTENSION) != filename.length() - 1  ) {

      sioFilename = filename + LCSIO::FILE_EXTENSION ;
    } 
    else 
      sioFilename = filename ;    
  } 

  void SIOWriter::open(const std::string& filename, int writeMode)  {

    
    // make sure filename has the proper extension (.slcio) 
    std::string sioFilename ;  
    getSIOFileName( filename, sioFilename ) ;

    // SIO has some rules about valid names for streams, records, etc ...
    //    const char* stream_name = LCSIO::getValidSIOName(sioFilename) ;
    std::string stream_name = LCSIO::getValidSIOName(sioFilename) ;
    _stream = SIO_streamManager::add(  stream_name.c_str() , 32*SIO_KBYTE*SIO_KBYTE ) ;
    
    if( _stream == 0 )
      throw IOException( std::string( "[SIOWriter::open()] Bad or duplicate stream name: " 
 				      + stream_name  )) ;

    // SIO_stream takes any value and maps it to [-1,0,1...,9]
    _stream->setCompressionLevel( _compressionLevel ) ;
    

    unsigned int  status = 0  ;
    

    switch( writeMode ) 
      {
      case EVENT::LCIO::WRITE_NEW : 

	status  = _stream->open( sioFilename.c_str() , SIO_MODE_WRITE_NEW ) ; 

	break ;

      case EVENT::LCIO::WRITE_APPEND : 

	// try to read the last LCIORandomAccess record at the end --------------------
	status  = _stream->open( sioFilename.c_str() , SIO_MODE_READ ) ; 

	if( status != SIO_STREAM_SUCCESS ) 
	  throw IOException( std::string( "[SIOWriter::open()] Can't open stream for reading TOC: "
					  + sioFilename ) ) ;

	bool hasRandomAccess = _raMgr.initAppend( _stream ) ;
	
 	_stream->close() ;

	if( hasRandomAccess ) {
	  status  = _stream->open( sioFilename.c_str() , SIO_MODE_READ_WRITE ) ; // SIO_MODE_WRITE_APPEND ) ; 
	  // position at the beginnning of the file record which will then be overwritten with the next record ...
	  LCSIO::seekStream( _stream, -LCSIO_RANDOMACCESS_SIZE ) ; 
	} else {

	  // --- old files: ll simjopen the file in append mode 	
	  status  = _stream->open( sioFilename.c_str() , SIO_MODE_WRITE_APPEND ) ; 

	}

	break ;
      }

    if( !(status &1) )
      throw IOException( std::string( "[SIOWriter::open()] Couldn't open file: " 
				      +  sioFilename ) ) ;
    
    
    LCSIO::records().setCompress( _compressionLevel != 0 ) ;  
  }

  void SIOWriter::setCompressionLevel(int level) {
    _compressionLevel = level ;
  }


  void SIOWriter::writeRunHeader(const EVENT::LCRunHeader * hdr)   {


    SIO_record* runRecord = LCSIO::records()[ SIORecords::Run ] ;
    runRecord->connect( _runHandler );

    _runHandler->setRunHeader(  hdr ) ;
    
    if( _stream->getState()== SIO_STATE_OPEN ){
      
      LCSIO::records().setCompress( _compressionLevel != 0 ) ; 
      
      // write LCRunHeader record
      unsigned int status =  _stream->write( LCSIO_RUNRECORDNAME    ) ;
      

      // store position for random access records
      _raMgr.add( RunEvent(  hdr->getRunNumber(), -1 ) , _stream->lastRecordStart() ) ;

//       std::cout << " writeRunHeader " << hdr->getRunNumber()   << " at position : " << _stream->lastRecordStart()  
// 		<< std::endl ;

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
  void SIOWriter::setUpHandlers(const LCEvent * evt){
  
    
    //    _hdrRecord->disconnect( LCSIO_HEADERBLOCKNAME ) ;

    SIO_record* hdrRecord = LCSIO::records()[ SIORecords::Header ] ;
    hdrRecord->connect( _hdrHandler ) ;
    
    // need to disconnect all blocks for multiple I/O streams
    SIO_record* evtRecord = LCSIO::records()[ SIORecords::Event ] ;
    evtRecord->disconnectAll() ;
    
    const std::vector<std::string>* strVec = evt->getCollectionNames() ;
    
    for( std::vector<std::string>::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
      
      
      SIOCollectionHandler* ch = dynamic_cast<SIOCollectionHandler*> 
	( SIO_blockManager::get( name->c_str() )  ) ;
      
      LCCollection* col = evt->getCollection( *name ) ;
      
      if(! col->isTransient() ){ // if a collection is transient we simply ignore it

	try{

	  if( ch == 0 ) {
	    ch =  new SIOCollectionHandler( *name, col->getTypeName())  ;
	  }

	  evtRecord->connect( ch ) ;

	  ch->setCollection( col ) ; 
	  
	} 
	catch(Exception& ex){   // unsuported type !
	  delete ch ;
	  ch =  0 ;
	}
	
      }
    } 
  }

  void SIOWriter::writeEvent(const LCEvent* evt)   {

    
    //here we set up the collection handlers 
    
    try{   setUpHandlers( evt) ;
    
    }catch(...){
      throw IOException(  "[SIOWriter::writeEvent] could not set up handlers " ) ;
    }

    LCSIO::records().setCompress( _compressionLevel != 0 ) ; 

    if( _stream->getState()== SIO_STATE_OPEN ){
   
      // need to set the event in event header handler
      _hdrHandler->setEvent( evt ) ;

      unsigned int  status = 0  ;


      // write LCEventHeader record
      status =  _stream->write( LCSIO_HEADERRECORDNAME    ) ;

      _raMgr.add( RunEvent(  evt->getRunNumber(), evt->getEventNumber()  ) , _stream->lastRecordStart() ) ;

//       std::cout << " writeEventHeader " << evt->getRunNumber() << " - " << evt->getEventNumber()    
//  		<< " at position : " << _stream->lastRecordStart()  
//  		<< std::endl ;      

      if( ! (status & 1) )
	throw IOException(  std::string("[SIOWriter::writeEvent] couldn't write event header to stream: "
					+  *_stream->getName() )) ;
      
      
      // write the event record
      status =  _stream->write( LCSIO_EVENTRECORDNAME    ) ;

      if( ! (status & 1) )
	throw IOException(  std::string("[SIOWriter::writeEvent] couldn't write event header to stream: "
					+  *_stream->getName() )) ;
    }
    else      

      throw IOException(  std::string("[SIOWriter::writeEvent] stream not opened : "
				      +  *_stream->getName()  )) ;
    
  }


  void SIOWriter::close()  {
  
    
    _raMgr.writeRandomAccessRecords( _stream ) ;


    const std::string* streamName  = _stream->getName() ;

    int status  =  SIO_streamManager::remove( _stream ) ;
    
    if(! (status &1) ) 
      throw IOException(  std::string("[SIOWriter::close] couldn't close stream  : "
				      + *streamName  )) ;

    _raMgr.clear() ;

  }

  void SIOWriter::flush()  {
  
    const std::string* streamName  = _stream->getName() ;

    int status =  _stream->flush() ;
    
    if(! (status &1) ) 
      throw IOException(  std::string("[SIOWriter::flush] couldn't flush stream  : "
				      + *streamName  )) ;

  }

} // namespace

