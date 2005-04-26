#include "SIO/SIOTPCRawDataHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/TPCRawData.h"
#include "EVENT/LCIO.h"
#include "IMPL/LCFlagImpl.h"
#include "IOIMPL/TPCRawDataIOImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"


using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOTPCRawDataHandler::read(SIO_stream* stream, 
				      LCObject** objP){
    unsigned int status ; 
	
    // create a new object :
    TPCRawDataIOImpl* hit  = new TPCRawDataIOImpl ;
    *objP = hit ;
	
    SIO_DATA( stream ,  &(hit->_channelID) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_time0) , 1  ) ;

    int chargeSize ;
    SIO_DATA( stream ,  &chargeSize  , 1 ) ;

    hit->_charge.resize( chargeSize  ) ;

    SIO_DATA( stream ,  &(hit->_charge[0])  , chargeSize  ) ;

    
    SIO_PTAG( stream , dynamic_cast<const TPCRawData*>(hit) ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
  }
    
    
  unsigned int SIOTPCRawDataHandler::write(SIO_stream* stream, 
				       const LCObject* obj){
    
    unsigned int status ; 

    const TPCRawData* hit = dynamic_cast<const TPCRawData*>(obj)  ;

    LCSIO_WRITE( stream, hit->getChannelID()  ) ;
    LCSIO_WRITE( stream, hit->getTime0()  ) ;

    const ShortVec& v =  hit->getCharge() ;
    LCSIO_WRITE( stream, v.size()  ) ;
    
    short* v0 =  const_cast<short*> ( & v[0]  ) ; 
    SIO_DATA( stream ,  v0 , v.size() ) ;

    SIO_PTAG( stream , hit ) ;
    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
} // namespace
