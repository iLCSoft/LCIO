#include "SIO/SIOCollectionHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/LCIO.h"

#include "IOIMPL/LCCollectionIOVec.h"
#include "IOIMPL/LCEventIOImpl.h" 

#include "SIO/SIOHandlerMgr.h"
#include "SIO/SIOObjectHandler.h"

#include "SIO_functions.h"

//#include <iostream>

using namespace DATA ;
using namespace EVENT ;
using namespace IOIMPL ;

namespace SIO {


  SIOCollectionHandler::SIOCollectionHandler(const std::string& name, 
					     const std::string& type,
					     LCEventIOImpl**  anEvtP)
    throw (Exception) : 
    SIO_block( name.c_str() ), 
    _evtP( anEvtP ) , _col(0) , 
    _myType( type )   {
    
    // here we need to get the handler for our type
    _myHandler = SIOHandlerMgr::instance()->getHandler( _myType  ) ;
    if( ! _myHandler ){
      // FIXME - this is inconsistent - is this an exception or not ???
      // don't print a warning - just ignore unknown types ....
      //      std::cerr << "WARNING:  SIOCollectionHandler no handler for type : " 
      //		<< _myType << std::endl ;
      throw Exception("SIOCollectionHandler: unsuported type") ;
    }

  }

  SIOCollectionHandler::~SIOCollectionHandler(){
  }
  
  void SIOCollectionHandler::setCollection(const LCCollectionData *col){
    _col = col ;
  } 
  
  
  unsigned int SIOCollectionHandler::xfer( SIO_stream* stream, SIO_operation op, 
					   unsigned int versionID){
    
    // if we don't have a handler we don't do anything ...
    if( !_myHandler) {
      //      std::cout << "WARNING:  SIOCollectionHandler no handler for type : " 
      //		<< _myType << std::endl ;
      return LCIO::SUCCESS ;
    }
    
    unsigned int status ; // needed by the SIO_DATA macro
    
    if( op == SIO_OP_READ ){ 
      
      // get address of this  handlers collection in the event
      
      LCCollectionIOVec* ioCol ;


      // get the collection from event that has been attached by SIOEventHandler
      try{   // can safely cast - we know we have an LCEventImpl that has LCCollectionIOVecs
	ioCol = dynamic_cast<LCCollectionIOVec*>( (*_evtP)->getCollection( getName()->c_str() )) ;
      }
      catch(DataNotAvailableException){   return LCIO::ERROR ; }

      SIO_DATA( stream ,  &(ioCol->_flag) , 1  ) ;
      int nObj ;
      SIO_DATA( stream ,  &nObj , 1  ) ;
      
      // now read all the objects :
      for( int i=0 ; i< nObj ; i ++ ){
	
	LCObject* obj ;
	
	status  = _myHandler->read( stream , &obj , ioCol->_flag , versionID ) ;
	if( !( status & 1 ) ) return status ;
	
	ioCol->push_back( obj ) ;
      }
      
    } else if( op == SIO_OP_WRITE ){ 
      
      const LCCollectionData* vec = _col ;
      
      if( vec  != 0 ){
	
	LCSIO_WRITE( stream, vec->getFlag()  ) ;
	int nObj = vec->getNumberOfElements() ;
	SIO_DATA( stream,  &nObj , 1  ) ;
	
	
	//  now write all the objects :
	for( int i=0 ; i< nObj ; i ++ ){
	  
	  const LCObject* obj = vec->getElementAt(i)  ;
	  
	  status  =  _myHandler->write( stream , obj , vec->getFlag() ) ; 
	  if( !( status & 1 ) ) return status ;
	  
	}
	
	
      }else{ 
	return 0 ;
      }
    }

    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
  unsigned int   SIOCollectionHandler::version(){
    
    int version = SIO_VERSION_ENCODE( LCIO::MAJORVERSION, LCIO::MINORVERSION ) ;
    
    return version ;
  }
  
}; // namespace
