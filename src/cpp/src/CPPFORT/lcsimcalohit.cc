
#include "CPPFORT/lcsimcalohit.h"

#include "lcio.h" 
#include "Exceptions.h"
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/SimCalorimeterHitImpl.h"
#include "IMPL/LCTOOLS.h"
#include <iostream>

using namespace lcio ;
using namespace std ;


#define LCCOLLECTION_PNTR( collection ) if(! (collection) ) return LCIO::ERROR ; \
const LCCollection* lcCollection = reinterpret_cast<LCCollection*>( (collection) ) ;

#define LCHITIMPL_PNTR( hit ) if(! (hit) ) return LCIO::ERROR ; \
SimCalorimeterHitImpl* lcHit = reinterpret_cast<SimCalorimeterHitImpl*>( (hit) ) ;

#define LCHIT_PNTR( hit ) if(! (hit) ) return LCIO::ERROR ; \
SimCalorimeterHit* lcHit = reinterpret_cast<SimCalorimeterHit*>( (hit) ) ;


PTRTYPE LCSetSimCaloHit( int cellID0, int cellID1, float energy, float* pos ) {
  SimCalorimeterHitImpl* lcHit = new SimCalorimeterHitImpl ;
  lcHit->setCellID0( cellID0 ) ;
  lcHit->setCellID1( cellID1 ) ;
  lcHit->setEnergy( energy ) ;
  lcHit->setPosition( pos ) ;
  return reinterpret_cast<PTRTYPE>( lcHit ) ;
}

PTRTYPE LCSetSimCaloHitCmc(PTRTYPE hit, PTRTYPE mcp, float energy, float time, int pdg ) {
  LCHITIMPL_PNTR ( hit ) ;
  MCParticle* mmcp = reinterpret_cast<MCParticle*>( mcp ) ;
  lcHit->addMCParticleContribution ( mmcp, energy, time, pdg ) ;
  return LCIO::SUCCESS ;
}

PTRTYPE LCGetSimCaloHit( PTRTYPE collection, int i, int* cellID0, int* cellID1, float* energy, float* pos ) {
  LCCOLLECTION_PNTR( collection ) ;

// checking on i !
  const int Nelements = lcCollection->getNumberOfElements() ;
  if (i < 0 && i > Nelements-1) {
    std::cerr << "Exception in LCGetSimCaloHit: index " << i << " out of range" << std::endl ;
    return LCIO::ERROR ;
  }

  SimCalorimeterHit* lcHit = dynamic_cast<SimCalorimeterHit*>( lcCollection->getElementAt( i-1 ) ) ;
  *cellID0 = lcHit->getCellID0() ;
  *cellID1 = lcHit->getCellID1() ;
  *energy  = lcHit->getEnergy() ;
  const float* tmp = lcHit->getPosition() ;
  for(int k=0;k<3;k++)  *pos++ = tmp[k] ;
  return reinterpret_cast<PTRTYPE>( lcHit ) ;
}

int LCGetSimCaloHitNmcp(PTRTYPE hit ) {
  LCHIT_PNTR ( hit ) ;
  return lcHit->getNMCParticles() ;
}

int LCGetSimCaloHitCmcp( PTRTYPE hit, int i, PTRTYPE* mcp, float* energy, float* time, int* pdg ) {
  LCHIT_PNTR ( hit ) ;

// checking on i !
  const int Nelements = lcHit->getNMCParticles() ;
  if (i < 0 && i > Nelements-1) {
    std::cerr << "Exception in LCGetSimCaloHitCmcp: index " << i << " out of range" << std::endl ;
    return LCIO::ERROR ;
  }
  *mcp    = reinterpret_cast<PTRTYPE>( lcHit->getParticleCont(i-1) ) ;
  *energy = lcHit->getEnergyCont( i-1 ) ;
  *time   = lcHit->getTimeCont( i-1 ) ;
  *pdg    = lcHit->getPDGCont( i-1 ) ;
  return LCIO::SUCCESS ;
}

