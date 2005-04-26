#include "SIO/SIOVTXRawHitHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/VTXRawHit.h"
#include "EVENT/LCIO.h"
#include "IMPL/LCFlagImpl.h"
#include "IOIMPL/VTXRawHitIOImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"


using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOVTXRawHitHandler::read(SIO_stream* stream, 
				      LCObject** objP){
    unsigned int status ; 
	
    // create a new object :
    VTXRawHitIOImpl* hit  = new VTXRawHitIOImpl ;
    *objP = hit ;
	
    SIO_DATA( stream ,  &(hit->_moduleID) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_row) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_column) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_adcCounts) , 1  ) ;

    SIO_PTAG( stream , dynamic_cast<const VTXRawHit*>(hit) ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
  }
    
    
  unsigned int SIOVTXRawHitHandler::write(SIO_stream* stream, 
				       const LCObject* obj){
    
    unsigned int status ; 

    const VTXRawHit* hit = dynamic_cast<const VTXRawHit*>(obj)  ;

    LCSIO_WRITE( stream, hit->getModuleID()  ) ;
    LCSIO_WRITE( stream, hit->getRow()  ) ;
    LCSIO_WRITE( stream, hit->getColumn()  ) ;
    LCSIO_WRITE( stream, hit->getADCCounts()  ) ;

    SIO_PTAG( stream , hit ) ;
    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
} // namespace
