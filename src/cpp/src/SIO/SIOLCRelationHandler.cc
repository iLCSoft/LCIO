#include "SIO/SIOLCRelationHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/LCIO.h"
#include "EVENT/MCParticle.h"
#include "EVENT/LCRelation.h"
#include "IOIMPL/LCRelationIOImpl.h"
#include "IMPL/LCFlagImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"


using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOLCRelationHandler::read(SIO_stream* stream, 
				      LCObject** objP){
    unsigned int status ; 
	
    // create a new object :
    LCRelationIOImpl* rel  = new LCRelationIOImpl ;
    *objP = rel ;
	

    SIO_PNTR( stream , &(rel->_from ) );
    SIO_PNTR( stream , &(rel->_to ) ) ;
    
    if( LCFlagImpl(_flag).bitSet( LCIO::LCREL_WEIGHTED ) ){
      
      SIO_DATA( stream ,  &(rel->_weight) , 1  ) ;
      
    } 
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
    
  unsigned int SIOLCRelationHandler::write(SIO_stream* stream, 
				       const LCObject* obj){
    
    unsigned int status ; 

    const LCRelation* rel = dynamic_cast<const LCRelation*>(obj)  ;
    
    LCObject* from = rel->getFrom() ;
    SIO_PNTR( stream,  &from ) ;

    LCObject* to = rel->getTo() ;
    SIO_PNTR( stream,  &to ) ;

    if( LCFlagImpl(_flag).bitSet( LCIO::LCREL_WEIGHTED ) ){

      LCSIO_WRITE( stream ,  rel->getWeight() ) ;

    } 


    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
} // namespace
