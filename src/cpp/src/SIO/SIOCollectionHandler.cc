#include "SIO/SIOCollectionHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/LCIO.h"
#include "SIO/SIOLCParameters.h"

#include "IOIMPL/LCCollectionIOVec.h"

#include "SIO/SIOHandlerMgr.h"
#include "SIO/SIOObjectHandler.h"

#include "SIO_functions.h"

//#include <iostream>

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
      // throw an exception that is caught in the SIOReader 
      throw Exception("SIOCollectionHandler: unsuported type") ;
    }
    //    std::cout << " creating SIOCollectionHandler " << _myType << std::endl ;
  }

  SIOCollectionHandler::~SIOCollectionHandler(){
//     std::cout << " deleting SIOCollectionHandler " << _myType << std::endl ;
  }
  
  void SIOCollectionHandler::setCollection(const LCCollection *col){
    _col = col ;
  } 
  void SIOCollectionHandler::setEvent(LCEventIOImpl**  anEvtP){
    _evtP = anEvtP ;
  }  
  
  unsigned int SIOCollectionHandler::xfer( SIO_stream* stream, SIO_operation op, 
					   unsigned int versionID){
    
    // if we don't have a handler we don't do anything ...
    if( !_myHandler) {
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
      
      // read parameters
      if( versionID > SIO_VERSION_ENCODE( 1, 1)   ) 
	SIOLCParameters::read( stream ,  ioCol->parameters() , versionID) ;
      

      // LCGenericObjects are different in that they might have the fixed length
      // of the attributes once in the block before the loop starts...
      if( _myType == LCIO::LCGENERICOBJECT ){
	//	SIOLCGenericObjectHandler.....( _ioCol->_flag ,
      }

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
      
//       const LCCollection* vec = _col ;
      
      if( _col  != 0 ){
	
	LCSIO_WRITE( stream, _col->getFlag()  ) ;

	// write parameters
	if( version() > SIO_VERSION_ENCODE( 1, 1) ) 
	  SIOLCParameters::write( stream ,  _col->getParameters() ) ;

	int nObj = _col->getNumberOfElements() ;

	SIO_DATA( stream,  &nObj , 1  ) ;
	
	//  now write all the objects :
	for( int i=0 ; i< nObj ; i ++ ){
	  
	  const LCObject* obj = _col->getElementAt(i)  ;
	  
	  status  =  _myHandler->write( stream , obj , _col->getFlag() ) ; 
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
