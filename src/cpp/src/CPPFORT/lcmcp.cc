#include "CPPFORT/lcmcp.h"

#include "lcio.h" 
#include "IMPL/MCParticleImpl.h"

using namespace lcio ;

#include <iostream>


PTRTYPE lcmcpcreate(){
  MCParticleImpl* mcp = new MCParticleImpl ;
  
  // uncomment to demonstrate that the addresses are different depending on the
  // type of the pointer....
  //   cout << "lcmcpcreate: " <<  C2F_POINTER( LCObject*, mcp ) << ", "  
  //        << C2F_POINTER( MCParticleData*, mcp ) << ", " 
  //        << C2F_POINTER( MCParticle*, mcp ) << ", " 
  //        << C2F_POINTER( MCParticleImpl*, mcp ) << endl ;
  return C2F_POINTER( LCObject*, mcp ) ;
}
int lcmcpdelete( PTRTYPE mcparticle ){
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  delete mcp ;
  return LCIO::SUCCESS ;
}
PTRTYPE lcmcpgetparent( PTRTYPE mcparticle ) {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  return C2F_POINTER(LCObject*,  mcp->getParent()  ) ;  
}
PTRTYPE lcmcpgetsecondparent( PTRTYPE mcparticle ) {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  return C2F_POINTER(LCObject*,  mcp->getSecondParent()  ) ;  
}
PTRTYPE lcmcpgetdaughter( PTRTYPE mcparticle, int i ) {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  return C2F_POINTER(LCObject*,  mcp->getDaughter( i )  ) ;  
}
int lcmcpgetendpoint(  PTRTYPE mcparticle, double* ep) {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  for( int i=0 ; i<3 ;  *ep++ =  mcp->getEndpoint()[i++]  ) ;
  return LCIO::SUCCESS ;
}
int lcmcpgetnumberofdaughters( PTRTYPE mcparticle )  {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  return mcp->getNumberOfDaughters() ;  
}
int lcmcpgetpdg( PTRTYPE mcparticle )  {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  return mcp->getPDG() ;  
}
int lcmcpgethepevtstatus( PTRTYPE mcparticle )  {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  return mcp->getHepEvtStatus() ;  
}
int lcmcpgetvertex( PTRTYPE mcparticle, double* vtx )  {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  for( int i=0 ; i<3 ;  *vtx++ =  mcp->getVertex()[i++]  ) ;
  return LCIO::SUCCESS ;
}
int lcmcpgetmomentum( PTRTYPE mcparticle, float* p)  {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  for( int i=0 ; i<3 ;  *p++ =  mcp->getMomentum()[i++]  ) ;
  return LCIO::SUCCESS ;
}
float lcmcpgetmass( PTRTYPE mcparticle )  {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  return mcp->getMass() ;  
}
float lcmcpgetcharge( PTRTYPE mcparticle )  {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  return mcp->getCharge() ;  
}
float lcmcpgetenergy( PTRTYPE mcparticle )  {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  return mcp->getEnergy() ;  
}

int lcmcpsetparent(  PTRTYPE mcparticle, PTRTYPE parent ) {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  MCParticle* mom = f2c_pointer<MCParticle,LCObject>( parent ) ;
  mcp->setParent( mom ) ;
  return LCIO::SUCCESS ;
}
int lcmcpsetsecondparent(  PTRTYPE mcparticle, PTRTYPE parent ) {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  MCParticle* mom = f2c_pointer<MCParticle,LCObject>( parent ) ;
  mcp->setSecondParent( mom ) ;
  return LCIO::SUCCESS ;
}
int lcmcpadddaughter(  PTRTYPE mcparticle, PTRTYPE daughter ) {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  MCParticle* dgh = f2c_pointer<MCParticle,LCObject>( daughter ) ;
  mcp->addDaughter( dgh ) ;
  return LCIO::SUCCESS ;
}
int lcmcpsetpdg( PTRTYPE mcparticle, int pdg ) {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  mcp->setPDG( pdg ) ;
  return LCIO::SUCCESS ;
}
int lcmcpsethepevtstatus( PTRTYPE mcparticle, int status ) {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  mcp->setHepEvtStatus( status ) ;
  return LCIO::SUCCESS ;
}
int lcmcpsetvertex( PTRTYPE mcparticle, double vtx[3] ) {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  mcp->setVertex( vtx ) ;
  return LCIO::SUCCESS ;
}
int lcmcpsetendpoint( PTRTYPE mcparticle, double pnt[3] ) {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  mcp->setEndpoint( pnt ) ;
  return LCIO::SUCCESS ;
}
int lcmcpsetmomentum( PTRTYPE mcparticle,  float p[3] ){
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  mcp->setMomentum( p ) ;
  return LCIO::SUCCESS ;
}
int lcmcpsetmass( PTRTYPE mcparticle, float m) {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  mcp->setMass( m ) ;
  return LCIO::SUCCESS ;
}
int lcmcpsetcharge( PTRTYPE mcparticle, float c ) {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  mcp->setCharge( c ) ;
  return LCIO::SUCCESS ;
}
