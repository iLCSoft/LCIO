#include "SIO/SIOCalHitHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/LCIO.h"
#include "EVENT/MCParticle.h"
#include "EVENT/CalorimeterHit.h"
#include "IOIMPL/CalorimeterHitIOImpl.h"
#include "IMPL/LCFlagImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"

using namespace DATA ;
using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOCalHitHandler::read(SIO_stream* stream, 
				      LCObject** objP,
				      unsigned int flag,
				      unsigned int vers ){
    unsigned int status ; 
	
    // create a new object :
    CalorimeterHitIOImpl* hit  = new CalorimeterHitIOImpl ;
    *objP = hit ;
	
    SIO_DATA( stream ,  &(hit->_cellID0) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_cellID1) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_energy) , 1  ) ;

    if( LCFlagImpl(flag).bitSet( LCIO::CHBIT_LONG ) ){
      SIO_DATA( stream ,  hit->_position  , 3 ) ;
    }

//     // read a pointer tag for future reference to calorimeter hits
//     SIO_PTAG( stream , dynamic_cast<const CalorimeterHit*>(hit) ) ;
	
    return ( SIO_BLOCK_SUCCESS ) ;
	
  }
    
    
  unsigned int SIOCalHitHandler::write(SIO_stream* stream, 
				       const LCObject* obj,
				       unsigned int flag ){
    
    unsigned int status ; 

    const CalorimeterHitData* hit = dynamic_cast<const CalorimeterHitData*>(obj)  ;
    
    LCSIO_WRITE( stream, hit->getCellID0()  ) ;
    LCSIO_WRITE( stream, hit->getCellID1()  ) ;
    LCSIO_WRITE( stream, hit->getEnergy()  ) ;
    // as SIO doesn't provide a write function with const arguments
    // we have to cast away the constness 

    if( LCFlagImpl(flag).bitSet( LCIO::CHBIT_LONG ) ){
      float* pos = const_cast<float*> ( hit->getPosition() ) ; 
      SIO_DATA( stream,  pos , 3 ) ;
    }

//     // add a pointer tag for future reference to calorimeter hits
//     SIO_PTAG( stream , hit ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
    
  }
  
}; // namespace
