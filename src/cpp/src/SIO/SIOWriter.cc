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

  SIOWriter::SIOWriter() :
    _compressionLevel(-1) {
    /* nop */
  }
  
  //----------------------------------------------------------------------------

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
    open( filename, EVENT::LCIO::WRITE_NEW ) ;
  }
  
  //----------------------------------------------------------------------------

  void SIOWriter::getSIOFileName(const std::string& filename, std::string& sioFilename ) {
    if( filename.rfind(LCSIO::FILE_EXTENSION) == std::string::npos ||  // .slcio not found at all
	   !( filename.rfind(LCSIO::FILE_EXTENSION) + strlen( LCSIO::FILE_EXTENSION ) == filename.length() ) ) {  // found, but not at end 
      sioFilename = filename + LCSIO::FILE_EXTENSION ;
    } 
    else {
      sioFilename = filename ;          
    }
  }
  
  //----------------------------------------------------------------------------

  void SIOWriter::open(const std::string& filename, int writeMode)  {
    // make sure filename has the proper extension (.slcio) 
    std::string sioFilename ;  
    getSIOFileName( filename, sioFilename ) ;

    _stream = std::make_shared<sio::stream>();

    // SIO_stream takes any value and maps it to [-1,0,1...,9]
    _stream->set_compression_level( _compressionLevel ) ;
    
    // needed by SIO macros
    unsigned int status = 0  ;
    
    switch( writeMode ) {
      case EVENT::LCIO::WRITE_NEW : 
      	status  = _stream->open( sioFilename.c_str() , SIO_MODE_WRITE_NEW ) ; 
      	break ;
      case EVENT::LCIO::WRITE_APPEND :
      	// try to read the last LCIORandomAccess record at the end --------------------
        SIO_DEBUG( "SIOWriter::open: Opening in write append mode" );
      	status  = _stream->open( sioFilename.c_str() , SIO_MODE_READ ) ;         
      	if( status != SIO_STREAM_SUCCESS ) {
      	  throw IOException( std::string( "[SIOWriter::open()] Can't open stream for reading TOC: " + sioFilename ) ) ;
        }
        SIO_DEBUG( "SIOWriter::open: Opening in write append mode 1" );        
      	bool hasRandomAccess = _raMgr.initAppend( _stream.get() ) ;
       	_stream->close() ;
        SIO_DEBUG( "SIOWriter::open: Opening in write append mode 2" );
      	if( hasRandomAccess ) {
      	  status  = _stream->open( sioFilename.c_str() , SIO_MODE_READ_WRITE ) ;
      	  // position at the beginnning of the file record which will then be overwritten with the next record ...
      	  _raMgr.seekStream( _stream.get(), -LCSIO_RANDOMACCESS_SIZE ) ; 
      	} 
        else {
      	  // --- old files: ll simjopen the file in append mode 	
      	  status  = _stream->open( sioFilename.c_str() , SIO_MODE_WRITE_APPEND ) ; 
      	}
        SIO_DEBUG( "SIOWriter::open: Opening in write append mode ... OK" );
      	break ;
    }
    if( !(status &1) ) {
      throw IOException( std::string( "[SIOWriter::open()] Couldn't open file: " +  sioFilename ) ) ;
    }
  }
  
  //----------------------------------------------------------------------------

  void SIOWriter::setCompressionLevel(int level) {
    _compressionLevel = level ;
  }

  //----------------------------------------------------------------------------

  void SIOWriter::writeRunHeader(const EVENT::LCRunHeader * hdr)   {

    auto runRecord = _lcioRecords.createRunRecord( hdr );
    
    if( _stream->state() == SIO_STATE_OPEN ) {
      runRecord->set_compression( _compressionLevel != 0 );
      auto result = _stream->write_record( runRecord );
      _raMgr.add( RunEvent(  hdr->getRunNumber(), -1 ) , result._record_begin ) ;
      
      if( !(result._status & 1) ) {
        throw IOException( "[SIOWriter::writeRunHeader] couldn't write run header to stream") ;
      }
    }
    else {
      throw IOException( "[SIOWriter::writeRunHeader] stream not opened") ;
    }
  }
  
  //----------------------------------------------------------------------------

  void SIOWriter::writeEvent(const LCEvent* evt) {
    // create event header and event records
    auto headerRecord = _lcioRecords.createEventHeaderRecord( evt ) ;
    headerRecord->set_compression( _compressionLevel != 0 ) ;
    auto eventRecord = _lcioRecords.createEventRecord( evt ) ;
    eventRecord->set_compression( _compressionLevel != 0 ) ;
    if( _stream->state() == SIO_STATE_OPEN ) {
      // write event header to file
      auto headerResult = _stream->write_record( headerRecord ) ;
      // store event record position in random access manager 
      _raMgr.add( RunEvent(  evt->getRunNumber(), evt->getEventNumber()  ) , headerResult._record_begin ) ;
      if( ! (headerResult._status & 1) ) {
        throw IOException( "[SIOWriter::writeEvent] couldn't write event header to stream" ) ;        
      }
      // write the event record
      auto eventResult = _stream->write_record( eventRecord ) ;
      if( ! (eventResult._status & 1) ) {
        throw IOException( "[SIOWriter::writeEvent] couldn't write event to stream" ) ;        
      }
    }
    else {
      throw IOException( "[SIOWriter::writeEvent] stream not opened" ) ;      
    }
  }

  //----------------------------------------------------------------------------

  void SIOWriter::close() {
    _raMgr.writeRandomAccessRecords( _stream.get() ) ;
    SIO_DEBUG( "SIOWriter::close: last position on close: " << _stream->file_tell() );
    _raMgr.clear() ;
    _stream->close();
    _stream = nullptr;
  }
  
  //----------------------------------------------------------------------------

  void SIOWriter::flush()  {
    int status =  _stream->flush() ;
    if(! (status &1) ) {
      throw IOException( "[SIOWriter::flush] couldn't flush stream" ) ;
    }
  }

} // namespace

