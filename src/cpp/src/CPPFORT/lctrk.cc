#include "CPPFORT/lctrk.h"

#include "lcio.h" 
#include "IMPL/TrackImpl.h"
#include "LCIOSTLTypes.h"

using namespace lcio ;

#include <iostream>

#define NCOVARIANCE 15

// create delete ReconstructedParticle

PTRTYPE lctrkcreate(){
  TrackImpl* trk = new TrackImpl ;
  return C2F_POINTER( LCObject*, trk ) ;
}
int lctrkdelete( PTRTYPE track ){
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  delete trk ;
  return LCIO::SUCCESS ;
}


// get Methods

int lctrkid( PTRTYPE track )  {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  return trk->id() ;
}

int lctrkgettype( PTRTYPE track )  {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  return trk->getType() ;
}

// int lctrktesttype( PTRTYPE track , int bit )  {
//   TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
//   return trk->testType( bit ) ;
// }

// float lctrkgetmomentum( PTRTYPE track )  {
//   TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
//   return trk->getMomentum() ;
// }

float lctrkgetomega( PTRTYPE track )  {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  return trk->getOmega() ;
}

// float lctrkgettheta( PTRTYPE track ) {
//   TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
//   return trk->getTheta() ;
// }

float lctrkgettanlambda( PTRTYPE track ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  return trk->getTanLambda() ;
}

float lctrkgetphi( PTRTYPE track ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  return trk->getPhi() ;
}

float lctrkgetd0( PTRTYPE track ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  return trk->getD0() ;
}

float lctrkgetz0( PTRTYPE track ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  return trk->getZ0() ;
}

int lctrkgetcovmatrix( PTRTYPE track, float cvmtx[NCOVARIANCE] ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  for( int i=0 ; i<NCOVARIANCE ;  *cvmtx++ = trk->getCovMatrix()[i++]  ) ;
  return LCIO::SUCCESS ;
}

int lctrkgetreferencepoint( PTRTYPE track, float* refpoint ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  for( int i=0 ; i<3 ;  *refpoint++ = trk->getReferencePoint()[i++]  ) ;
  return LCIO::SUCCESS ;
}

int lctrkisreferencepointpca( PTRTYPE track ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  return trk->isReferencePointPCA() ;
}

float lctrkgetchi2( PTRTYPE track ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  return trk->getChi2() ;
}

int lctrkgetndf( PTRTYPE track ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  return trk->getNdf() ;
}

float lctrkgetdedx( PTRTYPE track ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  return trk->getdEdx() ;
}

float lctrkgetdedxerror( PTRTYPE track ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  return trk->getdEdxError() ;
}

float lctrkgetradiusofinnermosthit( PTRTYPE track ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  return trk->getRadiusOfInnermostHit() ;
}

int     lctrkgetsubdetectorhitnumbers( PTRTYPE track, int* intv, int* nintv ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  IntVec& intVec =  trk->subdetectorHitNumbers() ;
  int n = intVec.size() ;
  if (n > *nintv) {
    std::cerr << "Warning in lctrkgetsubdetectorhitnumbers: vector size " <<  n
              << " larger then target array size " << *nintv << std::endl ;
      n = *nintv ;
  }
  for(int j=0;j<n;j++) {
    intv[j] = intVec[j] ;
  }
  *nintv = n ;
  return LCIO::SUCCESS ;
}

PTRTYPE lctrkgettracks( PTRTYPE track ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  const TrackVec& idvect = trk->getTracks();
  return reinterpret_cast<PTRTYPE>( &idvect );
}

PTRTYPE lctrkgettrackerhits( PTRTYPE track ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  const TrackerHitVec& idvect = trk->getTrackerHits();
  return reinterpret_cast<PTRTYPE>( &idvect );
}


// set,add Methods

// int lctrksettype( PTRTYPE track, int type ) {
//   TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
//   trk->setType( type ) ;
//   return LCIO::SUCCESS ;
// }

int lctrksettypebit( PTRTYPE track, int index , int val) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  trk->setTypeBit( index , val ) ;
  return LCIO::SUCCESS ;
}

int lctrksetomega( PTRTYPE track, float omega ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
   trk->setOmega( omega ) ;
   return LCIO::SUCCESS ;
}

int lctrksettanlambda( PTRTYPE track, float tanLambda ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  trk->setTanLambda( tanLambda ) ;
  return LCIO::SUCCESS ;
}

int lctrksetphi( PTRTYPE track, float phi ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  trk->setPhi( phi ) ;
  return LCIO::SUCCESS ;
}

int lctrksetd0( PTRTYPE track, float d0 ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  trk->setD0( d0 ) ;
  return LCIO::SUCCESS ;
}

int lctrksetz0( PTRTYPE track, float z0 ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  trk->setZ0( z0 ) ;
  return LCIO::SUCCESS ;
}

int lctrksetcovmatrix( PTRTYPE track, float cvmtx[NCOVARIANCE] ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  trk->setCovMatrix( cvmtx ) ;
  return LCIO::SUCCESS ;
}

int lctrksetreferencepoint( PTRTYPE track, float refpoint[3] ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  trk->setReferencePoint( refpoint ) ;
  return LCIO::SUCCESS ;
}

int lctrksetisreferencepointpca( PTRTYPE track , int val) { 
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  trk->setIsReferencePointPCA( val ) ;
  return LCIO::SUCCESS ;
}

int lctrksetchi2( PTRTYPE track, float chi2) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  trk->setChi2( chi2 ) ;
  return LCIO::SUCCESS ;
}

int lctrksetndf( PTRTYPE track, int ndf) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  trk->setNdf( ndf ) ;
  return LCIO::SUCCESS ;
}

int lctrksetdedx( PTRTYPE track, float dedx) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  trk->setdEdx( dedx ) ;
  return LCIO::SUCCESS ;
}

int lctrksetdedxerror( PTRTYPE track, float dedxerr) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  trk->setdEdxError( dedxerr ) ;
  return LCIO::SUCCESS ;
}

int lctrksetradiusofinnermosthit( PTRTYPE track, float r) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  trk->setRadiusOfInnermostHit( r ) ;
  return LCIO::SUCCESS ;
}

int lctrkaddtrack( PTRTYPE track, PTRTYPE addtrack ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  Track* addtrk = f2c_pointer<Track,LCObject>( addtrack ) ;
  trk->addTrack( addtrk ) ;
  return LCIO::SUCCESS ;
}

int lctrkaddhit( PTRTYPE track, PTRTYPE hit ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  TrackerHit* addhit = f2c_pointer<TrackerHit,LCObject>( hit ) ;
  trk->addHit( addhit ) ;
  return LCIO::SUCCESS ;
}


int     lctrksetsubdetectorhitnumbers( PTRTYPE track, int* intv, const int nintv ) {
  TrackImpl* trk = f2c_pointer<TrackImpl,LCObject>( track ) ;
  IntVec& intVec =  trk->subdetectorHitNumbers() ;
  intVec.resize( nintv ) ;
  for(int j=0;j<nintv;j++) {
    intVec[j] =  intv[j]  ;
  }
  return LCIO::SUCCESS ;
}

