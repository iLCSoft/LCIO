#include "CPPFORT/lccah.h"

#include "lcio.h" 
#include "IMPL/CalorimeterHitImpl.h"

using namespace lcio ;


PTRTYPE lccahcreate(){
  CalorimeterHitImpl* hit = new CalorimeterHitImpl ;
  return C2F_POINTER( LCObject*, hit ) ; 
}
int lccahdelete( PTRTYPE simcalhit ) {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( simcalhit ) ;
  delete hit ;
  return LCIO::SUCCESS ;
}
int lccahid( PTRTYPE simcalhit )  {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( simcalhit ) ;
  return hit->id() ;
}

int lccahgetcellid0( PTRTYPE simcalhit )  {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( simcalhit ) ;
  return hit->getCellID0() ;
}
int lccahgetcellid1( PTRTYPE simcalhit )  {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( simcalhit ) ;
  return hit->getCellID1() ;
}
float lccahgetenergy( PTRTYPE simcalhit )  {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( simcalhit ) ;
  return hit->getEnergy() ;
}
float lccahgettime( PTRTYPE simcalhit )  {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( simcalhit ) ;
  return hit->getTime() ;
}
int lccahgetposition( PTRTYPE simcalhit, float *pos)  {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( simcalhit ) ;
  for(int i=0;i<3; *pos++ = hit->getPosition()[i++] ) ;
  return LCIO::SUCCESS ;
}
int lccahgettype( PTRTYPE simcalhit )  {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( simcalhit ) ;
  return hit->getType() ;
}

PTRTYPE lccahgetrawhit( PTRTYPE simcalhit )  {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( simcalhit ) ;
  return C2F_POINTER( LCObject*, hit->getRawHit() ) ;
}

int lccahsetcellid0( PTRTYPE simcalhit, int id0) {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( simcalhit ) ;
  hit->setCellID0( id0 ) ;
  return  LCIO::SUCCESS ;
}
int lccahsetcellid1( PTRTYPE simcalhit, int id1) {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( simcalhit ) ;
  hit->setCellID1( id1 ) ;
  return  LCIO::SUCCESS ;
}
int lccahsetenergy( PTRTYPE simcalhit, float en) {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( simcalhit ) ;
  hit->setEnergy( en ) ;
  return  LCIO::SUCCESS ;
}
int lccahsettime( PTRTYPE simcalhit, float time) {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( simcalhit ) ;
  hit->setTime( time ) ;
  return  LCIO::SUCCESS ;
}
int lccahsetposition( PTRTYPE simcalhit, float pos[3])  {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( simcalhit ) ;
  hit->setPosition( pos ) ;
  return  LCIO::SUCCESS ;
}
int lccahsettype( PTRTYPE simcalhit, int type) {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( simcalhit ) ;
  hit->setType( type ) ;
  return LCIO::SUCCESS ;
}
int lccahsetrawhit(PTRTYPE simcalhit, PTRTYPE rawHit) {
  CalorimeterHitImpl* hit = f2c_pointer<CalorimeterHitImpl,LCObject>( simcalhit ) ;
  hit->setRawHit( f2c_pointer<LCObject,LCObject>( rawHit ) ) ; 
  return LCIO::SUCCESS ;
}

