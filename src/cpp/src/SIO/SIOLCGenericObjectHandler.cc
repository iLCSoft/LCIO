#include "SIO/SIOLCGenericObjectHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/LCIO.h"
#include "EVENT/MCParticle.h"
#include "EVENT/LCGenericObject.h"
#include "IOIMPL/LCGenericObjectIOImpl.h"
#include "IMPL/LCFlagImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"


using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOLCGenericObjectHandler::read(SIO_stream* stream, 
				      LCObject** objP,
				      unsigned int flag,
				      unsigned int vers ){
    unsigned int status ; 
	
    // create a new object :
    LCGenericObjectIOImpl* rel  = new LCGenericObjectIOImpl ;
    *objP = rel ;
	

//     SIO_PNTR( stream , &(rel->_from ) );
//     SIO_PNTR( stream , &(rel->_to ) ) ;
//     if( LCFlagImpl(flag).bitSet( LCIO::LCREL_WEIGHTED ) ){
//       SIO_DATA( stream ,  &(rel->_weight) , 1  ) ;
//     } 

    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
    
  unsigned int SIOLCGenericObjectHandler::write(SIO_stream* stream, 
				       const LCObject* obj,
				       unsigned int flag ){
    
    unsigned int status ; 

    const LCGenericObject* rel = dynamic_cast<const LCGenericObject*>(obj)  ;
    
//     LCObject* from = rel->getFrom() ;
//     SIO_PNTR( stream,  &from ) ;
//     LCObject* to = rel->getTo() ;
//     SIO_PNTR( stream,  &to ) ;
//     if( LCFlagImpl(flag).bitSet( LCIO::LCREL_WEIGHTED ) ){
//       LCSIO_WRITE( stream ,  rel->getWeight() ) ;
//     } 


    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
}; // namespace
