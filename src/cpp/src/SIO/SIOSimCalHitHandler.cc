#include "SIO/SIOSimCalHitHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/LCIO.h"
#include "EVENT/MCParticle.h"
#include "EVENT/SimCalorimeterHit.h"
#include "IOIMPL/SimCalorimeterHitIOImpl.h"
#include "IMPL/LCFlagImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"


using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOSimCalHitHandler::read(SIO_stream* stream, 
				      LCObject** objP){
    unsigned int status ; 

    LCFlagImpl flag(_flag) ;

    // create a new object :
    SimCalorimeterHitIOImpl* hit  = new SimCalorimeterHitIOImpl ;
    *objP = hit ;
	
    SIO_DATA( stream ,  &(hit->_cellID0) , 1  ) ;

    // in v00-08 cellid1 has been stored by default
    if( flag.bitSet( LCIO::CHBIT_ID1 ) || 

	( SIO_VERSION_MAJOR(_vers)==0 && SIO_VERSION_MINOR(_vers)==8) ){

      SIO_DATA( stream ,  &(hit->_cellID1) , 1  ) ;

    }
    SIO_DATA( stream ,  &(hit->_energy) , 1  ) ;

    if( flag.bitSet( LCIO::CHBIT_LONG ) ){
      SIO_DATA( stream ,  hit->_position  , 3 ) ;
    }

    // read MCContributions
    int nCon ;
    SIO_DATA( stream ,  &nCon , 1  ) ;

    hit->_vec.reserve(nCon) ;

    for(int i=0; i< nCon ; i++){

      MCParticleCont* mcCon = new MCParticleCont ;
      SIO_PNTR( stream , &(mcCon->Particle)  ) ;
      SIO_DATA( stream , &(mcCon->Energy) , 1 ) ;
      SIO_DATA( stream , &(mcCon->Time)   , 1 ) ;

      if( flag.bitSet( LCIO::CHBIT_STEP )){ 

	if( _vers > SIO_VERSION_ENCODE( 2, 10 ) ){
	  SIO_DATA( stream , &(mcCon->Length) , 1 ) ;
	}
	SIO_DATA( stream , &(mcCon->PDG)  , 1 ) ;

	if( _vers > SIO_VERSION_ENCODE( 1, 51 ) ){
	  SIO_DATA( stream , &(mcCon->StepPosition[0])    , 1 ) ;
	  SIO_DATA( stream , &(mcCon->StepPosition[1])    , 1 ) ;
	  SIO_DATA( stream , &(mcCon->StepPosition[2])    , 1 ) ;
	}
      }

      
      hit->_vec.push_back(  mcCon  );
    }

    // read a pointer tag for  reference to calorimeter hits

    if( _vers > SIO_VERSION_ENCODE( 1, 0) ){ 
      SIO_PTAG( stream , dynamic_cast<const SimCalorimeterHit*>(hit) ) ;
    }
    
    return ( SIO_BLOCK_SUCCESS ) ;
	
  }
    
    
  unsigned int SIOSimCalHitHandler::write(SIO_stream* stream, 
				       const LCObject* obj){
    
    unsigned int status ; 
	
    const SimCalorimeterHit* hit = dynamic_cast<const SimCalorimeterHit*>(obj)  ;
    
    LCSIO_WRITE( stream, hit->getCellID0()  ) ;

    LCFlagImpl flag(_flag) ;

    if( flag.bitSet( LCIO::CHBIT_ID1 ) ){
      LCSIO_WRITE( stream, hit->getCellID1()  ) ;
    }
    LCSIO_WRITE( stream, hit->getEnergy()  ) ;
    // as SIO doesn't provide a write function with const arguments
    // we have to cast away the constness 

    if( flag.bitSet( LCIO::CHBIT_LONG ) ){
      float* pos = const_cast<float*> ( hit->getPosition() ) ; 
      SIO_DATA( stream,  pos , 3 ) ;
    }
    // now the MCParticle contributions
    int nMC = hit->getNMCContributions() ;
    SIO_DATA( stream,  &nMC , 1 ) ;

    for(int i=0; i<nMC ;i++){

      const MCParticle* part = hit->getParticleCont(i)  ;
      SIO_PNTR( stream , &part ) ;
      
      LCSIO_WRITE( stream, hit->getEnergyCont(i)  ) ;
      LCSIO_WRITE( stream, hit->getTimeCont(i)  ) ;

      if( flag.bitSet( LCIO::CHBIT_STEP ) ){

	LCSIO_WRITE( stream, hit->getLengthCont(i)  ) ;

	LCSIO_WRITE( stream, hit->getPDGCont(i)  ) ;

	float* sp = const_cast<float*> ( hit->getStepPosition(i) ) ; 
	SIO_DATA( stream,  sp , 3 ) ;
      }
    }
    
    // add a pointer tag for reference to sim. calorimeter hits - added in v1.1
    SIO_PTAG( stream , hit ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
    
  }
  
} // namespace
