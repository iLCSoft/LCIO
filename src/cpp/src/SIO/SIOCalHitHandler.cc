#include "SIO/SIOCalHitHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/LCIO.h"
#include "EVENT/MCParticle.h"
#include "EVENT/CalorimeterHit.h"
#include "IOIMPL/CalorimeterHitIOImpl.h"
#include "IMPL/LCFlagImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"


using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOCalHitHandler::read(SIO_stream* stream, 
				      LCObject** objP ){
    unsigned int status ; 
	
    // create a new object :
    CalorimeterHitIOImpl* hit  = new CalorimeterHitIOImpl ;
    *objP = hit ;
	
    SIO_DATA( stream ,  &(hit->_cellID0) , 1  ) ;

    LCFlagImpl lcFlag(_flag) ;


    // in v00-08 cellid1 has been stored by default
    if( lcFlag.bitSet( LCIO::RCHBIT_ID1 ) || 

	( SIO_VERSION_MAJOR(_vers)==0 && SIO_VERSION_MINOR(_vers)==8) ){

      SIO_DATA( stream ,  &(hit->_cellID1) , 1  ) ;

    }
    SIO_DATA( stream ,  &(hit->_energy) , 1  ) ;

    if( _vers > SIO_VERSION_ENCODE( 1, 9 ) && lcFlag.bitSet( LCIO::RCHBIT_ENERGY_ERROR )){
        SIO_DATA( stream ,  &(hit->_energyError) , 1  ) ;
    } 


    if( _vers > SIO_VERSION_ENCODE( 1, 2) && lcFlag.bitSet( LCIO::RCHBIT_TIME ) ){
      SIO_DATA( stream ,  &(hit->_time) , 1  ) ;
    }

    if( lcFlag.bitSet( LCIO::RCHBIT_LONG ) ){
      SIO_DATA( stream ,  hit->_position  , 3 ) ;
    }

    if( _vers > SIO_VERSION_ENCODE( 1, 2)   ){
      SIO_DATA( stream ,  &(hit->_type) , 1  ) ;
      SIO_PNTR( stream , &(hit->_rawHit)  ) ;
  }
  
    // read a pointer tag for future reference to calorimeter hits
    
    if( _vers > SIO_VERSION_ENCODE( 1, 2) ){    // the logic of the pointer bit has been inverted in v1.3
      
      if( ! lcFlag.bitSet( LCIO::RCHBIT_NO_PTR )  )
	SIO_PTAG( stream , dynamic_cast<const CalorimeterHit*>(hit) ) ;
      
    }else{
      
      if( lcFlag.bitSet( LCIO::RCHBIT_NO_PTR )  )
	SIO_PTAG( stream , dynamic_cast<const CalorimeterHit*>(hit) ) ;
    }
    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
    
  unsigned int SIOCalHitHandler::write(SIO_stream* stream, 
				       const LCObject* obj ){
    
    unsigned int status ; 

    const CalorimeterHit* hit = dynamic_cast<const CalorimeterHit*>(obj)  ;
    
    LCFlagImpl lcFlag(_flag) ;


    LCSIO_WRITE( stream, hit->getCellID0()  ) ;

    if( lcFlag.bitSet( LCIO::RCHBIT_ID1 ) ){
      LCSIO_WRITE( stream, hit->getCellID1()  ) ;
    }
    LCSIO_WRITE( stream, hit->getEnergy()  ) ;
    if( lcFlag.bitSet( LCIO::RCHBIT_ENERGY_ERROR ) ){
        LCSIO_WRITE( stream, hit->getEnergyError()  ) ;
    }

    if( lcFlag.bitSet( LCIO::RCHBIT_TIME ) ){
      LCSIO_WRITE( stream ,  hit->getTime() ) ;
    }

    // as SIO doesn't provide a write function with const arguments
    // we have to cast away the constness 
    if( lcFlag.bitSet( LCIO::RCHBIT_LONG ) ){
      float* pos = const_cast<float*> ( hit->getPosition() ) ; 
      SIO_DATA( stream,  pos , 3 ) ;
    }


    LCSIO_WRITE( stream ,  hit->getType() ) ;

    EVENT::LCObject *rawHit = hit->getRawHit() ;
    SIO_PNTR( stream , &rawHit ) ;
    
    if( !lcFlag.bitSet( LCIO::RCHBIT_NO_PTR ) ){
      SIO_PTAG( stream , hit  ) ;
    }

    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
} // namespace
