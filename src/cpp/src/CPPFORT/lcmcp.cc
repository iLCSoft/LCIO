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
int lcmcpgetnumberofparents( PTRTYPE mcparticle )  {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  return mcp->getParents().size() ;
}
PTRTYPE lcmcpgetparent( PTRTYPE mcparticle, int i ) {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  return C2F_POINTER(LCObject*,  mcp->getParents()[i] ) ;
}
// PTRTYPE lcmcpgetsecondparent( PTRTYPE mcparticle ) {
//   MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
//   return C2F_POINTER(LCObject*,  mcp->getSecondParent()  ) ;  
// }
PTRTYPE lcmcpgetdaughter( PTRTYPE mcparticle, int i ) {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  return C2F_POINTER(LCObject*,  mcp->getDaughters()[i-1] ) ;
}
int lcmcpgetendpoint(  PTRTYPE mcparticle, double* ep) {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  for( int i=0 ; i<3 ;  *ep++ =  mcp->getEndpoint()[i++]  ) ;
  return LCIO::SUCCESS ;
}
int lcmcpgetnumberofdaughters( PTRTYPE mcparticle )  {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  return mcp->getDaughters().size() ;
}
int lcmcpgetpdg( PTRTYPE mcparticle )  {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  return mcp->getPDG() ;  
}
int lcmcpgetgeneratorstatus( PTRTYPE mcparticle )  {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  return mcp->getGeneratorStatus() ;  
}
int lcmcpgetsimulatorstatus( PTRTYPE mcparticle )  {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  return mcp->getSimulatorStatus() ;  
}
int lcmcpgetvertex( PTRTYPE mcparticle, double* vtx )  {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  for( int i=0 ; i<3 ;  *vtx++ =  mcp->getVertex()[i++]  ) ;
  return LCIO::SUCCESS ;
}
float lcmcpgettime( PTRTYPE mcparticle )  {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  return mcp->getTime() ;  
}
int lcmcpgetmomentum( PTRTYPE mcparticle, double* p)  {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  for( int i=0 ; i<3 ;  *p++ =  mcp->getMomentum()[i++]  ) ;
  return LCIO::SUCCESS ;
}
double lcmcpgetmass( PTRTYPE mcparticle )  {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  return mcp->getMass() ;  
}
float lcmcpgetcharge( PTRTYPE mcparticle )  {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  return mcp->getCharge() ;  
}
double lcmcpgetenergy( PTRTYPE mcparticle )  {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  return mcp->getEnergy() ;  
}

int lcmcpgetspin( PTRTYPE mcparticle, float* spin )  {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  for( int i=0 ; i<3 ;  *spin++ = mcp->getSpin()[i++]  ) ;
  return LCIO::SUCCESS ;
}
int lcmcpgetcolorflow( PTRTYPE mcparticle, int* cflow )  {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  for( int i=0 ; i<2 ;  *cflow++ = mcp->getColorFlow()[i++]  ) ;
  return LCIO::SUCCESS ;
}


int lcmcpaddparent(  PTRTYPE mcparticle, PTRTYPE parent ) {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  MCParticle* mom = f2c_pointer<MCParticle,LCObject>( parent ) ;
  mcp->addParent( mom ) ;
  return LCIO::SUCCESS ;
}
// int lcmcpsetsecondparent(  PTRTYPE mcparticle, PTRTYPE parent ) {
//   MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
//   MCParticle* mom = f2c_pointer<MCParticle,LCObject>( parent ) ;
//   mcp->setSecondParent( mom ) ;
//   return LCIO::SUCCESS ;
// }
// int lcmcpadddaughter(  PTRTYPE mcparticle, PTRTYPE daughter ) {
//   MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
//   MCParticle* dgh = f2c_pointer<MCParticle,LCObject>( daughter ) ;
//   mcp->addDaughter( dgh ) ;
//   return LCIO::SUCCESS ;
// }
int lcmcpsetpdg( PTRTYPE mcparticle, int pdg ) {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  mcp->setPDG( pdg ) ;
  return LCIO::SUCCESS ;
}
int lcmcpsetgeneratorstatus( PTRTYPE mcparticle, int status ) {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  mcp->setGeneratorStatus( status ) ;
  return LCIO::SUCCESS ;
}
int lcmcpsetsimulatorstatus( PTRTYPE mcparticle, int status ) {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  mcp->setSimulatorStatus( status ) ;
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
int lcmcpsetspin( PTRTYPE mcparticle, float spin[3] ) {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  mcp->setSpin( spin ) ;
  return LCIO::SUCCESS ;
}
int lcmcpsetcolorflow( PTRTYPE mcparticle, int cflow[2] ) {
  MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
  mcp->setColorFlow( cflow ) ;
  return LCIO::SUCCESS ;
}

