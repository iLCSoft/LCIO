#include "SIO/SIOParticleHandler.h"
#include "SIO/LCSIO.h"

#include "EVENT/MCParticle.h"
#include "IOIMPL/MCParticleIOImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"


using namespace EVENT ;
//using namespace IMPL ;
using namespace IOIMPL ;

namespace SIO {
  
  unsigned int SIOParticleHandler::read(SIO_stream* stream, 
					LCObject** objP, 
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
    SIO_PNTR( stream , &(particle->_mother0) ) ;
    SIO_PNTR( stream , &(particle->_mother1) ) ;
    // daughters
    int nD ; 
    SIO_DATA( stream ,  &nD , 1  ) ;
    const MCParticle* daughter ;

    for(int i=0;i<nD;i++){
      SIO_PNTR( stream , &daughter ) ;
      particle->_daughters.push_back( daughter ) ;
    }
    SIO_DATA( stream ,  &(particle->_pdg) , 1  ) ;
    SIO_DATA( stream ,  &(particle->_status) , 1  ) ;
    SIO_DATA( stream ,  particle->_vertex  , 3 ) ;
    SIO_DATA( stream ,  particle->_p  , 3 ) ;
    SIO_DATA( stream ,  &(particle->_energy) , 1  ) ;
    SIO_DATA( stream ,  &(particle->_charge) , 1  ) ;
    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
  unsigned int SIOParticleHandler::write(SIO_stream* stream, 
					 const LCObject* obj, 
					 unsigned int flag ){
    
    unsigned int status ; 
    
    const MCParticle* particle  = dynamic_cast<const MCParticle*>(obj)  ;
    SIO_PTAG( stream , particle ) ;
    
    const MCParticle* myMom0 = particle->getParent() ;
    const MCParticle* myMom1 = particle->getSecondParent() ;
    SIO_PNTR( stream , &myMom0 ) ;
    SIO_PNTR( stream , &myMom1 ) ;

    const MCParticleVec* dVec =  particle->getDaughters() ;
    int nD = dVec->size() ;

    SIO_DATA( stream , &nD  , 1 ) ;

    MCParticleVec::const_iterator iter = dVec->begin() ;
    while( iter != dVec->end() ) {
      const MCParticle* part = *iter++ ;
      SIO_PNTR( stream ,  &part  ); 
    }
    LCSIO_WRITE( stream, particle->getPDG() ) ;
    LCSIO_WRITE( stream, particle->getHepEvtStatus() ) ;
    SIO_DATA( stream, const_cast<double*>( particle->getVertex() ) , 3 ) ;
    SIO_DATA( stream, const_cast<float*>( particle->getMomentum()), 3 ) ;
    LCSIO_WRITE( stream, particle->getEnergy() ) ;
    LCSIO_WRITE( stream, particle->getCharge() ) ;

    return ( SIO_BLOCK_SUCCESS ) ;

  }
}; // namespace
