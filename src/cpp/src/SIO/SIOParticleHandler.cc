#include "SIO/SIOParticleHandler.h"
#include "SIO/LCSIO.h"

#include "DATA/MCParticleData.h"
#include "IOIMPL/MCParticleIOImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"
#include <iostream>

using namespace EVENT ;
using namespace DATA ;
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
    int numberOfDaughters ; 
    SIO_DATA( stream ,  &numberOfDaughters , 1  ) ;
    const MCParticle* daughter ;

    for(int i=0;i<numberOfDaughters;i++){
      SIO_PNTR( stream , &daughter ) ;
      particle->_daughters.push_back( daughter ) ;
    }
    SIO_DATA( stream ,  &(particle->_pdg) , 1  ) ;
    SIO_DATA( stream ,  &(particle->_status) , 1  ) ;
    SIO_DATA( stream ,  particle->_vertex  , 3 ) ;
    SIO_DATA( stream ,  particle->_p  , 3 ) ;
    SIO_DATA( stream ,  &(particle->_energy) , 1  ) ;
    SIO_DATA( stream ,  &(particle->_charge) , 1  ) ;

    // if the particles doesn't have daughters we read its endpoint
    if( numberOfDaughters == 0 )
      SIO_DATA( stream ,  particle->_endpoint  , 3 ) ;
    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
  unsigned int SIOParticleHandler::write(SIO_stream* stream, 
					 const LCObject* obj, 
					 unsigned int flag ){

    //fg 20030609 changed to use MCParticleData    
    unsigned int status ; 
    
    const MCParticleData* particle  = dynamic_cast<const MCParticleData*>(obj)  ;
    SIO_PTAG( stream , particle ) ;
    
    const MCParticleData* myMom0 = particle->getParentData() ;
    const MCParticleData* myMom1 = particle->getSecondParentData() ;
    SIO_PNTR( stream , &myMom0 ) ;
    SIO_PNTR( stream , &myMom1 ) ;

    //const EVENT::MCParticleVec* dVec =  particle->getDaughters() ;
    int numberOfDaughters = particle->getNumberOfDaughters() ;

    SIO_DATA( stream , &numberOfDaughters  , 1 ) ;

    //EVENT::MCParticleVec::const_iterator iter = dVec->begin() ;
    //    while( iter != dVec->end() ) {
    //  const MCParticle* part = *iter++ ;
    for(int i=0;i<numberOfDaughters;i++){
      const MCParticleData* part = particle->getDaughterData(i) ;
      SIO_PNTR( stream ,  &part  ); 
    }
    LCSIO_WRITE( stream, particle->getPDG() ) ;
    LCSIO_WRITE( stream, particle->getHepEvtStatus() ) ;
    SIO_DATA( stream, const_cast<double*>( particle->getVertex() ) , 3 ) ;
    SIO_DATA( stream, const_cast<float*>( particle->getMomentum()), 3 ) ;
    LCSIO_WRITE( stream, particle->getEnergy() ) ;
    LCSIO_WRITE( stream, particle->getCharge() ) ;

    // only if the particles doesn't have daughters we write its endpoint
    if( numberOfDaughters == 0 )
      SIO_DATA( stream, const_cast<double*>( particle->getEndpoint() ) , 3 ) ;


    return ( SIO_BLOCK_SUCCESS ) ;

  }
}; // namespace
