#include "SIO/SIORelationHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/LCIO.h"

#include "IOIMPL/LCRelationIOImpl.h"

//#include "SIO/SIOHandlerMgr.h"
//#include "SIO/SIOObjectHandler.h"

#include "SIO_functions.h"
#include "IMPL/LCFlagImpl.h"
#include "LCIOSTLTypes.h"

#include <set>

using namespace EVENT ;
using namespace IOIMPL ;
using namespace IMPL ;

namespace SIO {


  SIORelationHandler::SIORelationHandler(const std::string& name, 
					 //					     const std::string& type,
					     LCEventIOImpl**  anEvtP)
    throw (Exception) : 
    SIO_block( name.c_str() ), 
    _evtP( anEvtP ) , _rel(0)  
    //  ,_myType( type ) 
  {
    
//     // here we need to get the handler for our type
//     _myHandler = SIOHandlerMgr::instance()->getHandler( _myType  ) ;
//     if( ! _myHandler ){
//       // throw an exception that is caught in the SIOReader 
//       throw Exception("SIORelationHandler: unsuported type") ;
//     }
//     //    std::cout << " creating SIORelationHandler " << _myType << std::endl ;
//   }
  }
  SIORelationHandler::~SIORelationHandler(){
//     std::cout << " deleting SIORelationHandler " << _myType << std::endl ;
  }
  
  void SIORelationHandler::setRelation(LCRelation *rel){
    _rel = rel ;
  } 
  void SIORelationHandler::setEvent(LCEventIOImpl**  anEvtP){
    _evtP = anEvtP ;
  }  
  
  unsigned int SIORelationHandler::xfer( SIO_stream* stream, SIO_operation op, 
					   unsigned int versionID){
    
    
    unsigned int status ; // needed by the SIO_DATA macro
    
    if( op == SIO_OP_READ ){ 
      
//       // get address of this  handlers relation in the event
      
//       LCRelationIOImpl* ioRel ;


//       // get the relation from event that has been attached by SIOEventHandler
//       try{   // can safely cast - we know we have an LCEventImpl that has LCRelationIOImpls
// 	ioRel = dynamic_cast<LCRelationIOImpl*>( (*_evtP)->getRelation( getName()->c_str() )) ;
//       }
//       catch(DataNotAvailableException){   return LCIO::ERROR ; }

//       int flag ;
//       SIO_DATA( stream ,  &flag , 1  ) ;

//       //      bool isOneToMany = LCFlagImpl(flag).bitSet( LCIO::LCREL_ONE2MANY ) ;
//       bool isWeighted =  LCFlagImpl(flag).bitSet( LCIO::LCREL_WEIGHTED ) ;
      

//       int nRel ;
//       SIO_DATA( stream ,  &nRel , 1  ) ;
      
//       // initialize the helper vector
//       ioRel->_relVec = new RelVec( nRel )  ;

//       // now read all the relations into the helper vector
//       for( int i=0 ; i< nRel ; i ++ ){

// 	SIO_PNTR( stream ,   &(  (*ioRel->_relVec)[i].from ) ) ;
// 	SIO_PNTR( stream ,   &(  (*ioRel->_relVec)[i].to ) ) ;
// 	if( isWeighted) {
// 	  SIO_PNTR( stream ,   &(  (*ioRel->_relVec)[i].wgt ) ) ;
// 	}
//       }
//       // -> after pointer reallocation the vector is filled into the multimap

    } else if( op == SIO_OP_WRITE ){ 
      

//       if( _rel  != 0 ){

// 	// check if we have weights not equal to one
// 	LCFlagImpl flag(0) ;
// 	bool isWeighted = false ; 
	
// 	//	std::cout << " number of relations : "  <<  _rel->numberOfRelations() << std::endl ;

// 	for(int i=0 ; i< _rel->numberOfRelations()  ; i++){
	  

// 	  LCObject* from =  _rel->getRelation( i ) ;
	  
// 	  for(int j=0 ; j< _rel->numberOfRelations( from ) ; j++ ){
	    
// 	    if( _rel->getWeight( from , j  ) != 1.0 ){
// 	      isWeighted = true ;
// 	      break ;
// 	    }
	    
// 	  }
// 	  if( isWeighted ) {
// 	    flag.setBit(  LCIO::LCREL_WEIGHTED ) ;
// 	    break ;
// 	  }
// 	}

// 	int flagWord = flag.getFlag() ; 

// 	SIO_DATA( stream ,  &flagWord , 1  ) ;


// 	int nRel = _rel->numberOfRelations() ;
// 	SIO_DATA( stream,  &nRel , 1  ) ;
	
// 	int count(0) ;
// 	// first we need a set of all the 'from' objects in the relations map
// 	std::set< LCObject* > fromObjects ;

// 	for( int i=0 ; i< nRel ; i ++ ){

// 	  LCObject* from =  _rel->getRelation( i ) ;
// 	  fromObjects.insert( from ) ;
// 	}

// 	for( std::set< LCObject* >::const_iterator iter = fromObjects.begin() ; iter != fromObjects.end() ; iter++ ){

// 	  LCObject* from = *iter ;

// 	  for(int j=0 ; j< _rel->numberOfRelations( from )  ; j++ ){
	    
// 	    LCObject* to = _rel->getRelation( from , j  ) ;

// 	    SIO_PNTR( stream , &from ) ;
// 	    SIO_PNTR( stream , &to ) ;
	    
// 	    if( isWeighted ){ 
// 	      float weight = _rel->getWeight( from , j  ) ;
// 	      SIO_DATA( stream ,  &weight , 1  ) ;
// 	    }
	    
// 	  }
// 	}

//       }else{ 
// 	return 0 ;
//       }
    }

    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
  unsigned int   SIORelationHandler::version(){
    
    int version = SIO_VERSION_ENCODE( LCIO::MAJORVERSION, LCIO::MINORVERSION ) ;
    
    return version ;
  }


  void SIORelationHandler::fillRelationMap( IOIMPL::LCEventIOImpl* evt ) {

    const StringVec* relNames =  evt->getRelationNames() ;
    for( StringVec::const_iterator relName = relNames->begin() ; relName != relNames->end() ; relName++ ) {
      
      LCRelationIOImpl* rel = dynamic_cast<LCRelationIOImpl*> ( evt->getRelation( *relName ) ) ;
      
      rel->fillMap() ;
    }
    
    


  }
  
}; // namespace
