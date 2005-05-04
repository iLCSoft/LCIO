#include "SIO/SIOSiliconRawHitHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/SiliconRawHit.h"
#include "EVENT/LCIO.h"
#include "IMPL/LCFlagImpl.h"
#include "IOIMPL/SiliconRawHitIOImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"


using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOSiliconRawHitHandler::read(SIO_stream* stream, 
				      LCObject** objP){
    unsigned int status ; 
	
    // create a new object :
    SiliconRawHitIOImpl* hit  = new SiliconRawHitIOImpl ;
    *objP = hit ;
	
    SIO_DATA( stream ,  &(hit->_cellID0) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_cellID1) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_timeStamp) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_adcCounts) , 1  ) ;

    SIO_PTAG( stream , dynamic_cast<const SiliconRawHit*>(hit) ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
  }
    
    
  unsigned int SIOSiliconRawHitHandler::write(SIO_stream* stream, 
				       const LCObject* obj){
    
    unsigned int status ; 

    const SiliconRawHit* hit = dynamic_cast<const SiliconRawHit*>(obj)  ;

    LCSIO_WRITE( stream, hit->getCellID0()  ) ;
    LCSIO_WRITE( stream, hit->getCellID1()  ) ;
    LCSIO_WRITE( stream, hit->getTimeStamp()  ) ;
    LCSIO_WRITE( stream, hit->getADCCounts()  ) ;

    SIO_PTAG( stream , hit ) ;
    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
} // namespace
