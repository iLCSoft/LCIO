#include "SIO/SIOObjectHandler.h"
#include "SIO/LCSIO.h"

#include "EVENT/LCIO.h"
#include "SIO_functions.h"
#include "SIO/SIOLCParameters.h"


using namespace EVENT ;
// using namespace IMPL ;
// using namespace IOIMPL ;

namespace SIO{


  unsigned int SIOObjectHandler::init( SIO_stream* stream,  
				       SIO_operation op, 
				       EVENT::LCCollection* col , 
				       unsigned int vers ) {

    unsigned int status ; 
    
    
    if( op == SIO_OP_READ ) {


      SIO_DATA( stream ,  &_flag , 1  ) ;
      
      if( vers > SIO_VERSION_ENCODE( 1, 1)   ) 
	SIOLCParameters::read( stream ,  col->parameters() , vers ) ;
      
      col->setFlag( _flag ) ;
      _vers = vers ;

    } 

    else if( op == SIO_OP_WRITE ) {
      

      _flag = col->getFlag() ;

      LCSIO_WRITE( stream, _flag  ) ;
      SIOLCParameters::write( stream , col->getParameters() ) ;

      _vers = vers ;  // not really needed !?

    }
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
  unsigned int SIOObjectHandler::readBase( SIO_stream* stream, EVENT::LCObject** objP ) {

    if( _flag & ( 1 << LCCollection::BITSubset ) ){

      int status ;
      // we only read the pointer 
      SIO_PNTR( stream , objP  ) ;
      
      return status ;

    } else {

      return  read( stream, objP ) ;
    }

  }
  
  unsigned int SIOObjectHandler::writeBase(SIO_stream* stream, const EVENT::LCObject* obj ) {

    if( _flag & ( 1 << LCCollection::BITSubset ) ){
  
      int status ;

      // we only write the pointer
      const EVENT::LCObject* objLocal = obj ; 

      SIO_PNTR( stream , &objLocal ) ;
      return status ;
 
    } else {

      return  write( stream, obj ) ;
    }
  }
  
}
