#include "CPPFORT/lccah.h"

#include "lcio.h" 
#include "IMPL/CalorimeterHitImpl.h"

using namespace lcio ;


PTRTYPE lccahcreate(){
  CalorimeterHitImpl* hit = new CalorimeterHitImpl ;
  return C2F_POINTER( LCObject*, hit ) ; 
}
int lccahdelete( PTRTYPE calhit ) {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( calhit ) ;
  delete hit ;
  return LCIO::SUCCESS ;
}
int lccahid( PTRTYPE calhit )  {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( calhit ) ;
  return hit->id() ;
}

int lccahgetcellid0( PTRTYPE calhit )  {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( calhit ) ;
  return hit->getCellID0() ;
}
int lccahgetcellid1( PTRTYPE calhit )  {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( calhit ) ;
  return hit->getCellID1() ;
}
float lccahgetenergy( PTRTYPE calhit )  {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( calhit ) ;
  return hit->getEnergy() ;
}
float lccahgetenergyerr( PTRTYPE calhit )  {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( calhit ) ;
  return hit->getEnergyError() ;
}
float lccahgettime( PTRTYPE calhit )  {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( calhit ) ;
  return hit->getTime() ;
}
int lccahgetposition( PTRTYPE calhit, float *pos)  {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( calhit ) ;
  for(int i=0;i<3; *pos++ = hit->getPosition()[i++] ) ;
  return LCIO::SUCCESS ;
}
int lccahgettype( PTRTYPE calhit )  {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( calhit ) ;
  return hit->getType() ;
}

PTRTYPE lccahgetrawhit( PTRTYPE calhit )  {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( calhit ) ;
  return C2F_POINTER( LCObject*, hit->getRawHit() ) ;
}

int lccahsetcellid0( PTRTYPE calhit, int id0) {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( calhit ) ;
  hit->setCellID0( id0 ) ;
  return  LCIO::SUCCESS ;
}
int lccahsetcellid1( PTRTYPE calhit, int id1) {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( calhit ) ;
  hit->setCellID1( id1 ) ;
  return  LCIO::SUCCESS ;
}
int lccahsetenergy( PTRTYPE calhit, float en) {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( calhit ) ;
  hit->setEnergy( en ) ;
  return  LCIO::SUCCESS ;
}
int lccahsetenergyerr( PTRTYPE calhit, float enerr) {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( calhit ) ;
  hit->setEnergyError( enerr ) ;
  return  LCIO::SUCCESS ;
}
int lccahsettime( PTRTYPE calhit, float time) {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( calhit ) ;
  hit->setTime( time ) ;
  return  LCIO::SUCCESS ;
}
int lccahsetposition( PTRTYPE calhit, float pos[3])  {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( calhit ) ;
  hit->setPosition( pos ) ;
  return  LCIO::SUCCESS ;
}
int lccahsettype( PTRTYPE calhit, int type) {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( calhit ) ;
  hit->setType( type ) ;
  return LCIO::SUCCESS ;
}
int lccahsetrawhit(PTRTYPE calhit, PTRTYPE rawHit) {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( calhit ) ;
  hit->setRawHit( f2c_pointer<LCObject,LCObject>( rawHit ) ) ; 
  return LCIO::SUCCESS ;
}

