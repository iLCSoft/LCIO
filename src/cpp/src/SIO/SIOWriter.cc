#include "SIO/SIOWriter.h" 

#include "EVENT/LCEvent.h"
#include "EVENT/LCIO.h"
#include "EVENT/LCCollection.h"

#include "SIO/LCSIO.h"
#include "SIO/SIOEventHandler.h" 
#include "SIO/SIOCollectionHandler.h" 
#include "SIO/SIORunHeaderHandler.h" 

#include "SIO_streamManager.h" 
#include "SIO_recordManager.h" 
#include "SIO_blockManager.h" 
#include "SIO_stream.h" 
#include "SIO_record.h" 

#include <iostream>

//#define DEBUG 1
#include "IMPL/LCTOOLS.h"

using namespace EVENT ;
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



  int SIOWriter::open(const std::string & filename) throw(IOException ){

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


    // default is append, i.e. append to existing file or create new one
    return open( filename, LCIO::WRITE_APPEND ) ;


  }

  void SIOWriter::getSIOFileName(const std::string& filename, 
				 std::string& sioFilename ){

    if( !( filename.rfind(LCSIO::FILE_EXTENSION) 
	   + strlen( LCSIO::FILE_EXTENSION ) == filename.length() ))
      sioFilename = filename + LCSIO::FILE_EXTENSION ;
    else 
      sioFilename = filename ;    
  } 

  int SIOWriter::open(const std::string& filename, int writeMode){

    unsigned int status ;
    
    // make sure filename has the proper extension (.slcio) 
    std::string sioFilename ;  
    getSIOFileName( filename, sioFilename ) ;

    const char* stream_name = LCSIO::getValidSIOName(sioFilename) ;
    _stream = SIO_streamManager::add(  stream_name , 64 * SIO_KBYTE ) ;
    
    if( _stream == 0 ) return LCIO::ERROR ;
    //      throw IOException( std::string( "[SIOReader::open()] Bad stream name: " 
    //				      + std::string(stream_name)  )) ;
    delete stream_name ;
    

    switch( writeMode ) 
      {
      case LCIO::WRITE_NEW : 
	status  = _stream->open( sioFilename.c_str() , SIO_MODE_WRITE_NEW ) ; 
	break ;
      case LCIO::WRITE_APPEND : 
	status  = _stream->open( sioFilename.c_str() , SIO_MODE_WRITE_APPEND ) ; 
	break ;
      default: 
	return LCIO::ERROR ;
      }

    if( !(status &1) ) return LCIO::ERROR ;
  
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
    
    return LCIO::SUCCESS ;
  }


  int SIOWriter::writeRunHeader(const EVENT::LCRunHeader * hdr){

    // create a new handler for every new run 
    
    if( !_runHandler){
      _runHandler = new SIORunHeaderHandler( LCSIO::RUNBLOCKNAME  ) ;
      _runRecord->connect( _runHandler );
    }
    _runHandler->setRunHeader(  hdr ) ;


    if( _stream->getState()== SIO_STATE_OPEN ){
      
      // write LCRunHeader record
      unsigned int status =  _stream->write( LCSIO::RUNRECORDNAME    ) ;
      if( ! status & 1 ){
	std::cout << "ERROR: couldn't write run header to stream : " 
		  << _stream->getName()  
		  <<  "  status  : " << status << std::endl ;
	return LCIO::ERROR ;
      }
    }

    //    _runRecord->disconnect( runHandler);
    //    delete runHandler ;
    return LCIO::SUCCESS ;
  }



  /** Creates Handlers needed for writing the event on this stream.
   * Needs to be called for every event.
   */
  void SIOWriter::setUpHandlers(const LCEvent * evt){
  
    // create SIOHandler for event 
    // and connect it to the sio record

    //    _evtHandler = new SIOEventHandler( LCSIO::EVENTBLOCKNAME ) ;
    //    _evtRecord->connect( _evtHandler) ;
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


    const StringVec* strVec = evt->getCollectionNames() ;

    for( StringVec::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){

      const LCCollection* col = evt->getCollection( *name ) ;

      SIOCollectionHandler* ch = new SIOCollectionHandler( *name, col->getTypeName() ) ;

      ch->setCollection( col ) ; // fg20030513

      _colVector.push_back( ch ) ;  
      _evtRecord->connect( ch ) ;
    } 
    
  }

  int SIOWriter::writeEvent(const LCEvent* evt){
  

    //here we set up the collection handlers 
    setUpHandlers( evt) ;
  
    if( _stream->getState()== SIO_STATE_OPEN ){
    
     //      IMPL::LCTOOLS::dumpEvent( evt ) ;

      // need to set the event in event (header) handlers
      //_evtHandler->setEvent( evt ) ;
      _hdrHandler->setEvent( evt ) ;

      // set the collections in the corresponding handlers
      //      typedef std::vector< SIOCollectionHandler* >::iterator CHI ;
      //for( CHI ch = _colVector.begin() ; ch !=  _colVector.end() ; ch++){
      //	(*ch)->setCollection(  evt->getCollection(*(*ch)->getName()) ) ;
      //}

      // write LCEventHeader record
      unsigned int status =  _stream->write( LCSIO::HEADERRECORDNAME    ) ;
      if( ! status & 1 ){
	std::cout << "ERROR: couldn't write event header to stream : " 
		  << _stream->getName()  
		  <<  "  status  : " << status << std::endl ;
      }

      // write the event record
      status =  _stream->write( LCSIO::EVENTRECORDNAME    ) ;
      if( ! status & 1 ){
	std::cout << "ERROR: couldn't write event to stream : " 
		  << _stream->getName()
		  <<  "  status  : " << status << std::endl ;
	return LCIO::ERROR ;
      }
    
    }else{
    
      std::cout << "ERROR: can't write to stream : " << _stream << std::endl ;
      return LCIO::ERROR ;
   
    }
  
    return LCIO::SUCCESS ;
  }


  int SIOWriter::close(){
  
    //    _stream->close() ;
    int status  =  SIO_streamManager::remove( _stream ) ;
    
    if(! (status &1) ) return LCIO::ERROR ;

    return LCIO::SUCCESS ; 
  }

}; // namespace

