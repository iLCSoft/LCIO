#include "CPPFORT/lcvtx.h"

#include "lcio.h" 
#include "IMPL/VertexImpl.h"
#include "IMPL/ReconstructedParticleImpl.h"

using namespace lcio ;

#include <iostream>

// create delete Vertex

PTRTYPE lcvtxcreate(){
  VertexImpl* vtx = new VertexImpl ;
  return C2F_POINTER( LCObject*, vtx ) ;
}
int lcvtxdelete( PTRTYPE vertex ){
  VertexImpl* vtx = f2c_pointer<VertexImpl,LCObject>( vertex ) ;
  delete vtx ;
  return LCIO::SUCCESS ;
}


// get Methods

int lcvtxid( PTRTYPE vertex ) {
  VertexImpl* vtx = f2c_pointer<VertexImpl,LCObject>( vertex ) ;
  return vtx->id() ;
}

bool lcvtxisprimary ( PTRTYPE vertex ) {
  VertexImpl* vtx = f2c_pointer<VertexImpl,LCObject>( vertex ) ;
  return vtx->isPrimary() ;
}

char* lcvtxgetalgorithmtype ( PTRTYPE vertex ) {
  VertexImpl* vtx = f2c_pointer<VertexImpl,LCObject>( vertex ) ;
  return const_cast<char*>( vtx->getAlgorithmType().c_str() );
}

float lcvtxgetchi2( PTRTYPE vertex ) {
  VertexImpl* vtx = f2c_pointer<VertexImpl,LCObject>( vertex ) ;
  return vtx->getChi2() ;
}

float lcvtxgetprobability( PTRTYPE vertex ) {
  VertexImpl* vtx = f2c_pointer<VertexImpl,LCObject>( vertex ) ;
  return vtx->getProbability() ;
}

int lcvtxgetposition( PTRTYPE vertex, float pos[3] ) {
  VertexImpl* vtx = f2c_pointer<VertexImpl,LCObject>( vertex ) ;
  for( int i=0 ; i<3 ;  *pos++ = vtx->getPosition()[i++]  ) ;
  return LCIO::SUCCESS ;
}

int lcvtxgetcovmatrix( PTRTYPE vertex, float cvmtx[VTXCOVMATRIX] )  {
  VertexImpl* vtx = f2c_pointer<VertexImpl,LCObject>( vertex ) ;
  for( int i=0 ; i<VTXCOVMATRIX ;  i++ ) cvmtx[i] = vtx->getCovMatrix()[i] ;
  return LCIO::SUCCESS ;
}

int lcvtxgetparameters( PTRTYPE vertex, float* vec, int* nvec ) {
  VertexImpl* vtx = f2c_pointer<VertexImpl,LCObject>( vertex ) ;
  int ntot = *nvec - 1 ;
  const FloatVec* floatVec = &vtx->getParameters() ;
  for(unsigned int l=0; l < floatVec->size(); l++){
    if ( l > (unsigned)(ntot) ) {
      std::cout << "LCVertex: in getParameters more than " << ntot << "parameters to store" << std::endl ;
      return LCIO::ERROR ;
    }
    *vec++ = (*floatVec)[l] ;
  }
  *nvec = (int)(floatVec->size() + 1);
  return LCIO::SUCCESS ;
}

PTRTYPE lcvtxgetassociatedparticle( PTRTYPE vertex ) {
  Vertex* vtx = f2c_pointer<Vertex,LCObject>( vertex ) ;
  ReconstructedParticle* recP = vtx->getAssociatedParticle();
  return reinterpret_cast<PTRTYPE>( recP );
}


// set,add Methods

int lcvtxsetprimary( PTRTYPE vertex, bool pri ) {
  VertexImpl* vtx = f2c_pointer<VertexImpl,LCObject>( vertex ) ;
  vtx->setPrimary( pri ) ;
  return LCIO::SUCCESS ;
}

int lcvtxsetalgorithmtype( PTRTYPE vertex, char* type ) {
  VertexImpl* vtx = f2c_pointer<VertexImpl,LCObject>( vertex ) ;
  vtx->setAlgorithmType( type ) ;
  return LCIO::SUCCESS ;
}

int lcvtxsetchi2( PTRTYPE vertex, float chi2 ) {
  VertexImpl* vtx = f2c_pointer<VertexImpl,LCObject>( vertex ) ;
  vtx->setChi2( chi2 ) ;
  return LCIO::SUCCESS ;
}

int lcvtxsetprobability( PTRTYPE vertex, float probability ) {
  VertexImpl* vtx = f2c_pointer<VertexImpl,LCObject>( vertex ) ;
  vtx->setProbability( probability ) ;
  return LCIO::SUCCESS ;
}

int lcvtxsetposition( PTRTYPE vertex, float pos[3] ) {
  VertexImpl* vtx = f2c_pointer<VertexImpl,LCObject>( vertex ) ;
  vtx->setPosition( pos ) ;
  return LCIO::SUCCESS ;
}

int lcvtxsetcovmatrix( PTRTYPE vertex, float cvmtx[VTXCOVMATRIX] ) {
  VertexImpl* vtx = f2c_pointer<VertexImpl,LCObject>( vertex ) ;
  vtx->setCovMatrix( cvmtx ) ;
  return LCIO::SUCCESS ;
}

int lcvtxaddparameter( PTRTYPE vertex, float param) {
  VertexImpl* vtx = f2c_pointer<VertexImpl,LCObject>( vertex ) ;
  vtx->addParameter( param ) ;
  return LCIO::SUCCESS ;
}

int lcvtxsetassociatedparticle( PTRTYPE vertex, PTRTYPE particle ) {
  VertexImpl* vtx = f2c_pointer<VertexImpl,LCObject>( vertex ) ;
  ReconstructedParticle* rcp = f2c_pointer<ReconstructedParticle,LCObject>( particle ) ;
  vtx->setAssociatedParticle( rcp ) ;
  return LCIO::SUCCESS ;
}

