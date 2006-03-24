#include "CPPFORT/lcsth.h"

#include "lcio.h" 
#include "Exceptions.h"
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/SimTrackerHitImpl.h"
#include "IMPL/LCTOOLS.h"
#include <iostream>

using namespace lcio ;
using namespace std ;

PTRTYPE lcsthcreate(){
  SimTrackerHitImpl* hit = new SimTrackerHitImpl ;
  return C2F_POINTER( LCObject*, hit ) ;
}

int lcsthdelete( PTRTYPE hit ){
  SimTrackerHitImpl* sth = f2c_pointer<SimTrackerHitImpl,LCObject>( hit ) ;
  delete sth ;
  return LCIO::SUCCESS ;
}

int lcsthgetcellid( PTRTYPE hit ){
  SimTrackerHitImpl* sth = f2c_pointer<SimTrackerHitImpl,LCObject>( hit ) ;
  return sth->getCellID() ;
}

double lcsthgetposition( PTRTYPE hit, int index ){
  SimTrackerHitImpl* sth = f2c_pointer<SimTrackerHitImpl,LCObject>( hit ) ;
  return sth->getPosition()[index] ;
}

float lcsthgetmomentum( PTRTYPE hit, int index ){
  SimTrackerHitImpl* sth = f2c_pointer<SimTrackerHitImpl,LCObject>( hit ) ;
  return sth->getMomentum()[index] ;
}

float lcsthgetpathlength( PTRTYPE hit ){
  SimTrackerHitImpl* sth = f2c_pointer<SimTrackerHitImpl,LCObject>( hit ) ;
  return sth->getPathLength() ;
}

float lcsthgetdedx( PTRTYPE hit ){
  SimTrackerHitImpl* sth = f2c_pointer<SimTrackerHitImpl,LCObject>( hit ) ;
  return sth->getdEdx() ;
}

float lcsthgettime( PTRTYPE hit ){
  SimTrackerHitImpl* sth = f2c_pointer<SimTrackerHitImpl,LCObject>( hit ) ;
  return sth->getTime() ;
}

PTRTYPE lcsthgetmcparticle( PTRTYPE hit ){
  SimTrackerHitImpl* sth = f2c_pointer<SimTrackerHitImpl,LCObject>( hit ) ;
  return C2F_POINTER( LCObject*, sth->getMCParticle() );
}

int lcsthsetcellid( PTRTYPE hit, int id ){
  SimTrackerHitImpl* sth = f2c_pointer<SimTrackerHitImpl,LCObject>( hit ) ;
  sth->setCellID( id ) ;
  return LCIO::SUCCESS ;
}
int lcsthsetposition( PTRTYPE hit, double pos[3] ){
  SimTrackerHitImpl* sth = f2c_pointer<SimTrackerHitImpl,LCObject>( hit ) ;
  sth->setPosition( pos ) ;
  return LCIO::SUCCESS ;
}

int lcsthsetmomentum( PTRTYPE hit, float pos[3] ){
  SimTrackerHitImpl* sth = f2c_pointer<SimTrackerHitImpl,LCObject>( hit ) ;
  sth->setMomentum( pos ) ;
  return LCIO::SUCCESS ;
}

int lcsthsetmomentumxyz( PTRTYPE hit, float px, float py, float pz ){
  SimTrackerHitImpl* sth = f2c_pointer<SimTrackerHitImpl,LCObject>( hit ) ;
  sth->setMomentum( px, py, pz ) ;
  return LCIO::SUCCESS ;
}

int lcsthsetpathlength(PTRTYPE hit, float pathLength) {
  SimTrackerHitImpl* sth = f2c_pointer<SimTrackerHitImpl,LCObject>( hit ) ;
  sth->setPathLength( pathLength ) ;
  return LCIO::SUCCESS ;
}

int lcsthsetdedx( PTRTYPE hit, float dEdX ){
  SimTrackerHitImpl* sth = f2c_pointer<SimTrackerHitImpl,LCObject>( hit ) ;
  sth->setdEdx( dEdX ) ;
  return LCIO::SUCCESS ;
}

int lcsthsettime( PTRTYPE hit, float t ){
  SimTrackerHitImpl* sth = f2c_pointer<SimTrackerHitImpl,LCObject>( hit ) ;
  sth->setTime( t ) ;
  return LCIO::SUCCESS ;
}

int lcsthsetmcparticle( PTRTYPE hit,  PTRTYPE  particle ){
  SimTrackerHitImpl* sth = f2c_pointer<SimTrackerHitImpl,LCObject>( hit ) ;
  MCParticle* mcp = f2c_pointer<MCParticle,LCObject>( particle ) ;
  sth->setMCParticle( mcp ) ;
  return LCIO::SUCCESS ;
}
