#include "SIO/SIOTPCPulseHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/TPCPulse.h"
#include "EVENT/LCIO.h"
#include "IMPL/LCFlagImpl.h"
#include "IOIMPL/TPCPulseIOImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"


using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOTPCPulseHandler::read(SIO_stream* stream, 
				      LCObject** objP){
    unsigned int status ; 
	
    // create a new object :
    TPCPulseIOImpl* hit  = new TPCPulseIOImpl ;
    *objP = hit ;
	
    SIO_DATA( stream ,  &(hit->_channelID) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_time) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_charge )  , 1  ) ;
    SIO_DATA( stream ,  &(hit->_quality )  , 1  ) ;
    
    SIO_PNTR( stream , &(hit->_corrData) ) ;
    
    SIO_PTAG( stream , dynamic_cast<const TPCPulse*>(hit) ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
  }
    
    
  unsigned int SIOTPCPulseHandler::write(SIO_stream* stream, 
				       const LCObject* obj){
    
    unsigned int status ; 

    const TPCPulse* hit = dynamic_cast<const TPCPulse*>(obj)  ;

    LCSIO_WRITE( stream, hit->getChannelID()  ) ;
    LCSIO_WRITE( stream, hit->getTime()  ) ;
    LCSIO_WRITE( stream, hit->getCharge()  ) ;
    LCSIO_WRITE( stream, hit->getQuality()  ) ;

    TPCCorrectedData* corr = hit->getTPCCorrectedData() ;
    SIO_PNTR( stream ,  & corr ); 

    
    SIO_PTAG( stream , hit ) ;
    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
} // namespace
