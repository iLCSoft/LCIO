#include "SIO/SIOTrackerDataHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/TrackerData.h"
#include "EVENT/LCIO.h"
#include "IMPL/LCFlagImpl.h"
#include "IOIMPL/TrackerDataIOImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"


using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOTrackerDataHandler::read(SIO_stream* stream, 
				      LCObject** objP){
    unsigned int status ; 
	
    // create a new object :
    TrackerDataIOImpl* hit  = new TrackerDataIOImpl ;
    *objP = hit ;
    
    SIO_DATA( stream ,  &(hit->_cellID0) , 1  ) ;
    
    LCFlagImpl lcFlag(_flag) ;
    
    if( lcFlag.bitSet( LCIO::TRAWBIT_ID1 ) )
      SIO_DATA( stream ,  &(hit->_cellID1) , 1  ) ;
    
    SIO_DATA( stream ,  &(hit->_time) , 1  ) ;
    
    int chargeSize ;
    SIO_DATA( stream ,  &chargeSize  , 1 ) ;
    
    hit->_charge.resize( chargeSize  ) ;

    SIO_DATA( stream ,  &(hit->_charge[0])  , chargeSize  ) ;

    
    SIO_PTAG( stream , dynamic_cast<const TrackerData*>(hit) ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
  }
    
    
  unsigned int SIOTrackerDataHandler::write(SIO_stream* stream, 
				       const LCObject* obj){
    
    unsigned int status ; 

    const TrackerData* hit = dynamic_cast<const TrackerData*>(obj)  ;

    
    LCSIO_WRITE( stream, hit->getCellID0()  ) ;
      
    LCFlagImpl lcFlag(_flag) ;
    if( lcFlag.bitSet( LCIO::TRAWBIT_ID1 ) )
      LCSIO_WRITE( stream, hit->getCellID1()  ) ;
    

    LCSIO_WRITE( stream, hit->getTime()  ) ;

    const FloatVec& v =  hit->getChargeValues() ;
    LCSIO_WRITE( stream, v.size()  ) ;
    
    float* v0 =  const_cast<float*> ( & v[0]  ) ; 
    SIO_DATA( stream ,  v0 , v.size() ) ;

    SIO_PTAG( stream , hit ) ;
    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
} // namespace
