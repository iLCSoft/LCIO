#include "SIO/SIOTrackerRawDataHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/TrackerRawData.h"
#include "EVENT/LCIO.h"
#include "IMPL/LCFlagImpl.h"
#include "IOIMPL/TrackerRawDataIOImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"


using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOTrackerRawDataHandler::read(SIO_stream* stream, 
				      LCObject** objP){
    unsigned int status ; 
	
    // create a new object :
    TrackerRawDataIOImpl* hit  = new TrackerRawDataIOImpl ;
    *objP = hit ;
	
    SIO_DATA( stream ,  &(hit->_cellID0) , 1  ) ;
    
    LCFlagImpl lcFlag(_flag) ;
    
    if( lcFlag.bitSet( LCIO::TRAWBIT_ID1 ) )
      SIO_DATA( stream ,  &(hit->_cellID1) , 1  ) ;

    SIO_DATA( stream ,  &(hit->_time) , 1  ) ;

    int adcSize ;
    SIO_DATA( stream ,  &adcSize  , 1 ) ;

    hit->_adc.resize( adcSize  ) ;

    SIO_DATA( stream ,  &(hit->_adc[0])  , adcSize  ) ;

    
    SIO_PTAG( stream , dynamic_cast<const TrackerRawData*>(hit) ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
  }
    
    
  unsigned int SIOTrackerRawDataHandler::write(SIO_stream* stream, 
				       const LCObject* obj){
    
    unsigned int status ; 

    const TrackerRawData* hit = dynamic_cast<const TrackerRawData*>(obj)  ;

    LCSIO_WRITE( stream, hit->getCellID0()  ) ;
      
    LCFlagImpl lcFlag(_flag) ;
    if( lcFlag.bitSet( LCIO::TRAWBIT_ID1 ) )
      LCSIO_WRITE( stream, hit->getCellID1()  ) ;
    

    LCSIO_WRITE( stream, hit->getTime()  ) ;

    const ShortVec& v =  hit->getADCValues() ;
    LCSIO_WRITE( stream, v.size()  ) ;
    
    short* v0 =  const_cast<short*> ( & v[0]  ) ; 
    SIO_DATA( stream ,  v0 , v.size() ) ;

    SIO_PTAG( stream , hit ) ;
    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
} // namespace
