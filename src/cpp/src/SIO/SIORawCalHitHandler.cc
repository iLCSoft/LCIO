#include "SIO/SIORawCalHitHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/LCIO.h"
#include "EVENT/MCParticle.h"
#include "EVENT/RawCalorimeterHit.h"
#include "IOIMPL/RawCalorimeterHitIOImpl.h"
#include "IMPL/LCFlagImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"


using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIORawCalHitHandler::read(SIO_stream* stream, 
				      LCObject** objP ){
    unsigned int status ; 
	
    // create a new object :
    RawCalorimeterHitIOImpl* hit  = new RawCalorimeterHitIOImpl ;
    *objP = hit ;
	
    LCFlagImpl lcFlag(_flag) ;

    SIO_DATA( stream ,  &(hit->_cellID0) , 1  ) ;

    
    if( lcFlag.bitSet( LCIO::RCHBIT_ID1 )
	|| _vers == SIO_VERSION_ENCODE( 0, 8 )  // in v00-08 cellid1 has been stored by default
	){
      SIO_DATA( stream ,  &(hit->_cellID1) , 1  ) ;
    }

    SIO_DATA( stream ,  &(hit->_amplitude) , 1  ) ;

    if( lcFlag.bitSet( LCIO::RCHBIT_TIME ) ){
      SIO_DATA( stream ,  &(hit->_timeStamp)  , 1 ) ;
    }

    if( ! lcFlag.bitSet( LCIO::RCHBIT_NO_PTR )  ) {
      SIO_PTAG( stream , dynamic_cast<const RawCalorimeterHit*>(hit) ) ;
    }
    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
    
    
  unsigned int SIORawCalHitHandler::write(SIO_stream* stream, 
				       const LCObject* obj ){
    
    unsigned int status ; 

    const RawCalorimeterHit* hit = dynamic_cast<const RawCalorimeterHit*>(obj)  ;
    
    LCSIO_WRITE( stream, hit->getCellID0()  ) ;

    LCFlagImpl lcFlag(_flag) ;

    if( lcFlag.bitSet( LCIO::RCHBIT_ID1 ) ){
      LCSIO_WRITE( stream, hit->getCellID1()  ) ;
    }

    LCSIO_WRITE( stream, hit->getAmplitude()  ) ;

    if( lcFlag.bitSet( LCIO::RCHBIT_TIME ) ){
      LCSIO_WRITE( stream, hit->getTimeStamp()  ) ;
    }

    if( ! lcFlag.bitSet( LCIO::RCHBIT_NO_PTR ) ){
      SIO_PTAG( stream , hit  ) ;
    }

    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
} // namespace
