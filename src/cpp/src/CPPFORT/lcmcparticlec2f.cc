
#include "CPPFORT/lcmcparticlec2f.h"
#include "CPPFORT/HEPEVT.h"

#include "lcio.h" 
#include "Exceptions.h"
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/LCTOOLS.h"

#include <iostream>

using namespace lcio ;

#define LCEVENT_PNTR( event ) if(! (event) ) return LCIO::ERROR ; \
LCEvent* lcEvent = reinterpret_cast<LCEvent*>( (event) ) ;

#define LCMCPARTICLE_PNTR( mcparticle ) if(! (mcparticle) ) return LCIO::ERROR ; \
EVENT::MCParticle* lcMCParticle = reinterpret_cast<MCParticle*>( (mcparticle) ) ;

int LCGetMCParticleCont( PTRTYPE mcparticle, int* pdg, int* hepevtstatus, double* prodvtx,
                         float* momentum, float* mass, int* ndaughters ){
  LCMCPARTICLE_PNTR( mcparticle ) ;
  *pdg               = lcMCParticle->getPDG() ;
  *hepevtstatus      = lcMCParticle->getHepEvtStatus() ;
  const double* dtmp = lcMCParticle->getVertex() ;
  for(int k=0;k<3;k++)  *prodvtx++  = dtmp[k] ;
  const float*  tmp  = lcMCParticle->getMomentum() ;
  for(int k=0;k<3;k++)  *momentum++ = tmp[k] ;
  *mass              = lcMCParticle->getMass() ;
  *ndaughters        = lcMCParticle->getNumberOfDaughters() ;
  return LCIO::SUCCESS ;
}

