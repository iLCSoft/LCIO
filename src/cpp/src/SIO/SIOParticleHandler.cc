#include "SIO/SIOParticleHandler.h"
#include "SIO/LCSIO.h"

#include "EVENT/MCParticle.h"
#include "IOIMPL/MCParticleIOImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"


namespace SIO {

  unsigned int SIOParticleHandler::read(SIO_stream* stream, 
					LCIOObject** objP, 
					unsigned int flag,  
					unsigned int vers ){
    unsigned int status ; 
  
    // create a new object :
    MCParticleIOImpl* particle  = new MCParticleIOImpl ;
    *objP = particle ;

    // tell SIO the address of particle as an abstract  MCParticle ...
    // this is important, as SIO takes the bare address 
    // (int)(MCParticle*) particle != (int)particle !!!!
    SIO_PTAG( stream , dynamic_cast<const MCParticle*>(particle) ) ;
    SIO_PNTR( stream , &(particle->_mom) ) ;

    SIO_DATA( stream ,  &(particle->_pdg) , 1  ) ;
    SIO_DATA( stream ,  particle->_p  , 3 ) ;
  
    return ( SIO_BLOCK_SUCCESS ) ;

  }

  unsigned int SIOParticleHandler::write(SIO_stream* stream, 
					 const LCIOObject* obj, 
					 unsigned int flag ){

    unsigned int status ; 
  
    // this is where we gave up type safety in order to
    // simplify the API and the implementation
    // by having a common collection of objects

    const MCParticle* particle  = dynamic_cast<const MCParticle*>(obj)  ;

    SIO_PTAG( stream , particle ) ;

    const MCParticle* myMom = particle->getMother() ;
    // how long is myMom's address needed by SIO 
    // - hopefully not after the next call - actually looks like  that ...
    SIO_PNTR( stream , &myMom ) ;

    LCSIO_WRITE( stream, particle->getPDG() ) ;
    // as SIO doesn't provide a write function with const arguments
    // we have to cast away the constness hoping for the best 
    float* pos = const_cast<float*> ( particle->getMomentum() ) ; 
    SIO_DATA( stream,  pos , 3 ) ;
  
    return ( SIO_BLOCK_SUCCESS ) ;

  }
}; // namespace
