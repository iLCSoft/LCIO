#include "CPPFORT/lctrh.h"

#include "lcio.h" 
#include "IMPL/TrackerHitImpl.h"

using namespace lcio ;

#include <iostream>

// create delete TrackerHit

PTRTYPE lctrhcreate(){
  TrackerHitImpl* trh = new TrackerHitImpl ;
  return C2F_POINTER( LCObject*, trh ) ;
}
int lctrhdelete( PTRTYPE trhit ){
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  delete trh ;
  return LCIO::SUCCESS ;
}


// get Methods

int lctrhid( PTRTYPE trhit )  {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  return trh->id() ;
}

int lctrhgetposition( PTRTYPE trhit, double dpos[3] ) {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  for( int i=0 ; i<3 ;  i++ ) dpos[i] = trh->getPosition()[i] ;
  return LCIO::SUCCESS ;
}

int lctrhgetcovmatrix( PTRTYPE trhit, float cvmtx[TRKHITNCOVMATRIX] ) {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  for( int i=0 ; i<TRKHITNCOVMATRIX ;  i++ ) cvmtx[i] = trh->getCovMatrix()[i] ;
  return LCIO::SUCCESS ;
}

// DEPRECATED. use lctrhgetedep()
float lctrhgetdedx( PTRTYPE trhit ) {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  return trh->getdEdx() ;
}

float lctrhgetedep( PTRTYPE trhit ) {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  return trh->getEDep() ;
}

float lctrhgetedeperr( PTRTYPE trhit ) {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  return trh->getEDepError() ;
}

float lctrhgettime( PTRTYPE trhit ) {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  return trh->getTime() ;
}

// char* lctrhgettype( PTRTYPE trhit )  {
//   TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
//   return const_cast<char*>( trh->getType().c_str() ) ;
// }

int lctrhgettype( PTRTYPE trhit )  {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  return trh->getType() ;
}

int lctrhgetquality( PTRTYPE trhit )  {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  return trh->getQuality() ;
}

int lctrhgetcellid0( PTRTYPE trhit )  {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  return trh->getCellID0() ;
}
int lctrhgetcellid1( PTRTYPE trhit )  {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  return trh->getCellID1() ;
}


PTRTYPE lctrhgetrawhits( PTRTYPE trhit ) {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
//  const TPCHit& hit = trh->getRawHits();
//  return reinterpret_cast<PTRTYPE>( &hit );
  return reinterpret_cast<PTRTYPE>( &trh->getRawHits() );
}


// set,add Methods

int lctrhsetposition( PTRTYPE trhit, double dpos[3] ) {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  trh->setPosition( dpos ) ;
  return LCIO::SUCCESS ;
}

int lctrhsetcovmatrix( PTRTYPE trhit, float cvmtx[TRKHITNCOVMATRIX] ) {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  trh->setCovMatrix( cvmtx ) ;
  return LCIO::SUCCESS ;
}

// DEPRECATED. use lctrhsetedep()
int lctrhsetdedx( PTRTYPE trhit, float dedx) {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  trh->setdEdx( dedx ) ;
  return LCIO::SUCCESS ;
}

int lctrhsetedep( PTRTYPE trhit, float e ) {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  trh->setEDep( e ) ;
  return LCIO::SUCCESS ;
}

int lctrhsetedeperr( PTRTYPE trhit, float e ) {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  trh->setEDepError( e ) ;
  return LCIO::SUCCESS ;
}

int lctrhsettime( PTRTYPE trhit, float time) {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  trh->setTime( time ) ;
  return LCIO::SUCCESS ;
}

int lctrhsettype( PTRTYPE trhit, int type) {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  trh->setType( type ) ;
  return LCIO::SUCCESS ;
}

int lctrhsetquality( PTRTYPE trhit, int q) {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  trh->setQuality( q ) ;
  return LCIO::SUCCESS ;
}

int lctrhsetcellid0( PTRTYPE trhit, int id0) {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  trh->setCellID0( id0 ) ;
  return  LCIO::SUCCESS ;
}
int lctrhsetcellid1( PTRTYPE trhit, int id1) {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  trh->setCellID1( id1 ) ;
  return  LCIO::SUCCESS ;
}

int lctrhaddrawhit( PTRTYPE trhit, PTRTYPE rawhit ) {
  TrackerHitImpl* trh = f2c_pointer<TrackerHitImpl,LCObject>( trhit ) ;
  LCObject* hit = f2c_pointer<LCObject,LCObject>( rawhit ) ;
  trh->rawHits().push_back( hit ) ;
  return LCIO::SUCCESS ;
}


