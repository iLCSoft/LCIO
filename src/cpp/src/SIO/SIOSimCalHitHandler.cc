#include "SIO/SIOSimCalHitHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/LCIO.h"
#include "EVENT/MCParticle.h"
#include "EVENT/SimCalorimeterHit.h"
#include "IOIMPL/SimCalorimeterHitIOImpl.h"
#include "IMPL/LCFlagImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"

using namespace DATA ;
using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOSimCalHitHandler::read(SIO_stream* stream, 
				      LCObject** objP,
				      unsigned int flag,
				      unsigned int vers ){
    unsigned int status ; 
	
    // create a new object :
    SimCalorimeterHitIOImpl* hit  = new SimCalorimeterHitIOImpl ;
    *objP = hit ;
	
    SIO_DATA( stream ,  &(hit->_cellID0) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_cellID1) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_energy) , 1  ) ;

    if( LCFlagImpl(flag).bitSet( LCIO::CHBIT_LONG ) ){
      SIO_DATA( stream ,  hit->_position  , 3 ) ;
    }

    // read MCContributions
    int nCon ;
    SIO_DATA( stream ,  &nCon , 1  ) ;

    for(int i=0; i< nCon ; i++){

      MCParticleCont* mcCon = new MCParticleCont ;
      SIO_PNTR( stream , &(mcCon->Particle)  ) ;
      SIO_DATA( stream , &(mcCon->Energy) , 1 ) ;
      SIO_DATA( stream , &(mcCon->Time)   , 1 ) ;
      if( LCFlagImpl(flag).bitSet( LCIO::CHBIT_PDG ) )
	SIO_DATA( stream , &(mcCon->PDG)    , 1 ) ;

      hit->_vec.push_back(  mcCon  );
    }

//     // read a pointer tag for future reference to calorimeter hits
//     SIO_PTAG( stream , dynamic_cast<const SimCalorimeterHit*>(hit) ) ;
	
    return ( SIO_BLOCK_SUCCESS ) ;
	
  }
    
    
  unsigned int SIOSimCalHitHandler::write(SIO_stream* stream, 
				       const LCObject* obj,
				       unsigned int flag ){
    
    unsigned int status ; 
	
    // this is where we gave up type safety in order to
    // simplify the API and the implementation
    // by having a common collection of objects
    
    //fg 20030609 changed to use SimCalorimeterHitData
    const SimCalorimeterHitData* hit = dynamic_cast<const SimCalorimeterHitData*>(obj)  ;
    
    LCSIO_WRITE( stream, hit->getCellID0()  ) ;
    LCSIO_WRITE( stream, hit->getCellID1()  ) ;
    LCSIO_WRITE( stream, hit->getEnergy()  ) ;
    // as SIO doesn't provide a write function with const arguments
    // we have to cast away the constness 

    if( LCFlagImpl(flag).bitSet( LCIO::CHBIT_LONG ) ){
      float* pos = const_cast<float*> ( hit->getPosition() ) ; 
      SIO_DATA( stream,  pos , 3 ) ;
    }
    // now the MCParticle contributions
    int nMC = hit->getNMCParticles() ;
    SIO_DATA( stream,  &nMC , 1 ) ;

    for(int i=0; i<nMC ;i++){

      const MCParticleData* part = hit->getParticleContData(i)  ;
      SIO_PNTR( stream , &part ) ;
      
      LCSIO_WRITE( stream, hit->getEnergyCont(i)  ) ;
      LCSIO_WRITE( stream, hit->getTimeCont(i)  ) ;
      if( LCFlagImpl(flag).bitSet( LCIO::CHBIT_PDG ) )
	LCSIO_WRITE( stream, hit->getPDGCont(i)  ) ;
      
    }
    
//     // add a pointer tag for future reference to calorimeter hits
//     SIO_PTAG( stream , hit ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
    
  }
  
}; // namespace
