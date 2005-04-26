#include "SIO/SIOTPCCorrectedDataHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/TPCCorrectedData.h"
#include "EVENT/LCIO.h"
#include "IMPL/LCFlagImpl.h"
#include "IOIMPL/TPCCorrectedDataIOImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"


using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOTPCCorrectedDataHandler::read(SIO_stream* stream, 
				      LCObject** objP){
    unsigned int status ; 
	
    // create a new object :
    TPCCorrectedDataIOImpl* hit  = new TPCCorrectedDataIOImpl ;
    *objP = hit ;
	
    SIO_DATA( stream ,  &(hit->_channelID) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_time0) , 1  ) ;

    int chargeSize ;
    SIO_DATA( stream ,  &chargeSize  , 1 ) ;

    hit->_charge.resize( chargeSize  ) ;

    SIO_DATA( stream ,  &(hit->_charge[0])  , chargeSize  ) ;

    
    SIO_PTAG( stream , dynamic_cast<const TPCCorrectedData*>(hit) ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
  }
    
    
  unsigned int SIOTPCCorrectedDataHandler::write(SIO_stream* stream, 
				       const LCObject* obj){
    
    unsigned int status ; 

    const TPCCorrectedData* hit = dynamic_cast<const TPCCorrectedData*>(obj)  ;

    LCSIO_WRITE( stream, hit->getChannelID()  ) ;
    LCSIO_WRITE( stream, hit->getTime0()  ) ;

    const FloatVec& v =  hit->getCharge() ;
    LCSIO_WRITE( stream, v.size()  ) ;
    
    float* v0 =  const_cast<float*> ( & v[0]  ) ; 
    SIO_DATA( stream ,  v0 , v.size() ) ;

    SIO_PTAG( stream , hit ) ;
    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
} // namespace
