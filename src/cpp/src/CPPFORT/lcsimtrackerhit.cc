
#include "CPPFORT/lcsimtrackerhit.h"

#include "lcio.h" 
#include "Exceptions.h"
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/SimTrackerHitImpl.h"
#include "IMPL/LCTOOLS.h"
#include <iostream>

using namespace lcio ;
using namespace std ;


#define LCCOLLECTIONVEC_PNTR( collectionvec ) if(! (collectionvec) ) return LCIO::ERROR ; \
LCCollectionVec* lcCollectionVec = reinterpret_cast<LCCollectionVec*>( (collectionvec) ) ;

#define LCCOLLECTION_PNTR( collection ) if(! (collection) ) return LCIO::ERROR ; \
const LCCollection* lcCollection = reinterpret_cast<LCCollection*>( (collection) ) ;


PTRTYPE LCSetSimTrackerHit( PTRTYPE collectionvec, int cellID, double* pos, float dEdx, float time, PTRTYPE mcp ){
  LCCOLLECTIONVEC_PNTR( collectionvec ) ;
  SimTrackerHitImpl* hit = new SimTrackerHitImpl ;
  MCParticle* mmcp = reinterpret_cast<MCParticle*>( mcp ) ;

  hit->setCellID( cellID ) ;
  hit->setPosition( pos ) ;
  hit->setdEdx( dEdx ) ;
  hit->setTime( time ) ;
  hit->setMCParticle( mmcp ) ;

  lcCollectionVec->push_back( hit ) ;
  return LCIO::SUCCESS ;
}

PTRTYPE LCGetSimTrackerHit( PTRTYPE collection, int i, int* cellID, double* pos, float* dEdx, float* time, PTRTYPE* mcp ){
  LCCOLLECTION_PNTR( collection ) ;
  SimTrackerHit* hit =  dynamic_cast<SimTrackerHit*>( lcCollection->getElementAt( i-1 ) ) ;

  *cellID = hit->getCellID() ;
  const double* tmp = hit->getPosition() ;
  for(int k=0;k<3;k++)  *pos++ = tmp[k] ;
  *dEdx   = hit->getdEdx() ;
  *time   = hit->getTime() ;
  *mcp    = reinterpret_cast<PTRTYPE>( hit->getMCParticle() ) ;
  return LCIO::SUCCESS ;
}

