#include "SIO/SIOTPCHitHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/TPCHit.h"
#include "EVENT/LCIO.h"
#include "IMPL/LCFlagImpl.h"
#include "IOIMPL/TPCHitIOImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"

using namespace DATA ;
using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOTPCHitHandler::read(SIO_stream* stream, 
				      LCObject** objP,
				      unsigned int flag,
				      unsigned int vers ){
    unsigned int status ; 
	
    // create a new object :
    TPCHitIOImpl* hit  = new TPCHitIOImpl ;
    *objP = hit ;
	
    SIO_DATA( stream ,  &(hit->_cellID) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_time) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_charge) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_quality) , 1  ) ;

    if( LCFlagImpl(flag).bitSet( LCIO::TPCBIT_RAW ) ){

      SIO_DATA( stream ,  &(hit->_rawDataSize)  , 1 ) ;
      SIO_DATA( stream ,  hit->_rawDataArray  , hit->_rawDataSize  ) ;

    }
    return ( SIO_BLOCK_SUCCESS ) ;
  }
    
    
  unsigned int SIOTPCHitHandler::write(SIO_stream* stream, 
				       const LCObject* obj,
				       unsigned int flag ){
    
    unsigned int status ; 

    const TPCHitData* hit = dynamic_cast<const TPCHitData*>(obj)  ;

    LCSIO_WRITE( stream, hit->getCellID()  ) ;
    LCSIO_WRITE( stream, hit->getTime()  ) ;
    LCSIO_WRITE( stream, hit->getCharge()  ) ;
    LCSIO_WRITE( stream, hit->getQuality()  ) ;

    if( LCFlagImpl(flag).bitSet( LCIO::TPCBIT_RAW ) ){
      
      LCSIO_WRITE( stream, hit->getNRawDataWords()  ) ;

      for(int i=0;i<hit->getNRawDataWords();i++){
	LCSIO_WRITE( stream, hit->getRawDataWord(i)  ) ;
      }
    }

    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
}; // namespace
