#include "SIO/SIOEventHandler.h"
#include "SIO/LCSIO.h"

#include "EVENT/LCEvent.h"
#include "EVENT/LCIO.h"
#include "IOIMPL/LCEventIOImpl.h"
#include "IOIMPL/LCCollectionIOVec.h"
// #include "IOIMPL/LCRelationIOImpl.h"
#include "LCIOSTLTypes.h"
#include "SIO/SIOLCParameters.h"

#include "SIO_functions.h"
#include <sstream> 


using namespace EVENT ;
using namespace IOIMPL ;

namespace SIO  {


  SIOEventHandler::SIOEventHandler(const std::string& name) : 
    SIO_block( name.c_str() ),
    _evtP(0) {
  }

  SIOEventHandler::SIOEventHandler(const std::string& name, LCEventIOImpl** anEvtP) : 
    SIO_block( name.c_str() ),
    _evtP( anEvtP ) {
 
    *_evtP = 0 ;
 }


  SIOEventHandler::~SIOEventHandler(){
//     if (*_evtP )  delete *_evtP ; 
  }

  
  void SIOEventHandler::setEvent(const LCEvent* evt ){
    _evt = evt ;
  }
  void SIOEventHandler::setEventPtr(IOIMPL::LCEventIOImpl** evtP ){
    _evtP = evtP ;
  } 


  unsigned int SIOEventHandler::xfer( SIO_stream* stream, SIO_operation op, 
				      unsigned int versionID){
  
    unsigned int status ; // needed by the SIO_DATA macro
  

    if( op == SIO_OP_READ ){ 

      LCSIO::checkVersion(versionID) ;

      // delete the old event object 
      // -> for every handler there will only be one event object at any given time      
       if (*_evtP )  delete *_evtP ;
       *_evtP = new LCEventIOImpl ;
      
      SIO_DATA( stream ,  &(*_evtP)->_runNumber  , 1  ) ;
      SIO_DATA( stream ,  &(*_evtP)->_eventNumber  , 1  ) ;
      SIO_DATA( stream ,  &(*_evtP)->_timeStamp  , 1  ) ;
    
      char* detName ; 
      LCSIO_READ( stream,  &detName ) ; 
      (*_evtP)->setDetectorName( detName  )  ;

      // read collection types and names
      // not needed for the event record 
      // but SIO crashes if block is not read completely ...
      int nCol ;
      SIO_DATA( stream ,  &nCol , 1 ) ;
      for( int i=0; i<nCol ; i++ ){
	char* dummy ; 
	LCSIO_READ( stream,  &dummy ) ; 
	std::string colName( dummy ) ;
	char* type ;
	// read type 
	LCSIO_READ( stream,  &type ) ; 
	 ;

	//  we have to attach a new collection or relation object to the event for every type in the header
// 	 std::string typeStr( type ) ;
// 	 if( typeStr.find( LCIO::LCRELATION) == 0  ){ // typeStr starts with LCRelation
// 	   try{ (*_evtP)->addRelation( new LCRelationIOImpl() , colName) ; 
// 	   }
// 	   catch( EventException ){  return LCIO::ERROR ; }
// 	 }else{

	   try{ (*_evtP)->addCollection( new LCCollectionIOVec( type ) , colName) ; 
	   }
	   catch( EventException ){  return LCIO::ERROR ; }
// 	 }
      }

      // read parameters
      if( versionID > SIO_VERSION_ENCODE( 1, 1)   ) 
	SIOLCParameters::read( stream ,  (*_evtP)->parameters() , versionID) ;
      

    }  else if( op == SIO_OP_WRITE ){ 
    
      if( _evt ){
	LCSIO_WRITE( stream, _evt->getRunNumber() ) ; 
	LCSIO_WRITE( stream, _evt->getEventNumber() ) ;
	LCSIO_WRITE( stream, _evt->getTimeStamp()  ) ;
	LCSIO_WRITE( stream, _evt->getDetectorName() ) ;
      
	// now write a list of colection types and names
      
// 	const std::vector<std::string>* strVec = _evt->getCollectionNames() ;
// 	int nCol = strVec->size() ;
// 	SIO_DATA( stream, &nCol, 1 ) ;
      
// 	for( std::vector<std::string>::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
	
// 	  const LCCollection* col = _evt->getCollection( *name ) ;
// 	  LCSIO_WRITE( stream, *name ) ;
// 	  LCSIO_WRITE( stream, col->getTypeName() ) ;
		
// 	} 

//	const std::vector<std::string>* strVec = _evt->getCollectionNames() ;
	const StringVec* colNames =   _evt->getCollectionNames() ;
// 	const StringVec* relNames =   _evt->getRelationNames() ;
	
	int nCol = colNames->size()  ; //+ relNames->size()  ;

	for(unsigned int i=0 ; i < colNames->size() ; i++ ) {   
	  if( _evt->getCollection( (*colNames)[i] )->isTransient() ) nCol-- ;
	}

	SIO_DATA( stream, &nCol, 1 ) ;
      
	//	for( std::vector<std::string>::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
	for(unsigned int i=0 ; i < colNames->size() ; i++ ) {   
	  const LCCollection* col = _evt->getCollection( (*colNames)[i] ) ;
	  if( ! col->isTransient() ){
	    LCSIO_WRITE( stream, (*colNames)[i] ) ;
	    LCSIO_WRITE( stream, col->getTypeName() ) ;
	  }
	} 
// 	for(unsigned int i=0 ; i < relNames->size() ; i++ ) {   
// 	  const LCRelation* rel = _evt->getRelation( (*relNames)[i] ) ;
// 	  std::stringstream  relTypeName  ;
// 	  relTypeName <<  LCIO::LCRELATION ; 
// 	  // << "_" <<   rel->getFromType() << "_"  << rel->getToType() << std::ends ;
// 	  LCSIO_WRITE( stream, (*relNames)[i] ) ;
// 	  LCSIO_WRITE( stream, relTypeName.str() ) ;
// 	} 
	
	// write parameters
	if( version() > SIO_VERSION_ENCODE( 1, 1) ) 
	  SIOLCParameters::write( stream ,  _evt->getParameters() ) ;

      } else {
	return 0 ;
      }
    }
  
  
    return ( SIO_BLOCK_SUCCESS ) ;
  }

  unsigned int   SIOEventHandler::version(){

    int version = SIO_VERSION_ENCODE( LCIO::MAJORVERSION, LCIO::MINORVERSION ) ;
    return version ;

  }

};
