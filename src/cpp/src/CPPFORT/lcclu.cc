#include "CPPFORT/lcclu.h"

#include "lcio.h" 
#include "IMPL/ClusterImpl.h"

using namespace lcio ;

#include <iostream>

// create delete Cluster

PTRTYPE lcclucreate(){
  ClusterImpl* clu = new ClusterImpl ;
  return C2F_POINTER( LCObject*, clu ) ;
}
int lccludelete( PTRTYPE cluster ){
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  delete clu ;
  return LCIO::SUCCESS ;
}


// get Methods

int lcclugetid( PTRTYPE cluster )  {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  return clu->id() ;
}

int lcclugettype( PTRTYPE cluster )  {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  return clu->getType() ;
}

float lcclugetenergy( PTRTYPE cluster ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  return clu->getEnergy() ;
}

int lcclugetposition( PTRTYPE cluster, float* pos )  {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  for( int i=0 ; i<3 ;  *pos++ = clu->getPosition()[i++]  ) ;
  return LCIO::SUCCESS ;
}

int lcclugetpositionerror( PTRTYPE cluster, float poserr[6] )  {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  for( int i=0 ; i<6 ;  *poserr++ = clu->getPositionError()[i++]  ) ;
  return LCIO::SUCCESS ;
}

float lcclugettheta( PTRTYPE cluster ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  return clu->getTheta() ;
}

float lcclugetphi( PTRTYPE cluster ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  return clu->getPhi() ;
}

int lcclugetdirectionerror( PTRTYPE cluster, float direrr[3] )  {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  for( int i=0 ; i<3 ;  *direrr++ = clu->getDirectionError()[i++]  ) ;
  return LCIO::SUCCESS ;
}

int lcclugetparticletype( PTRTYPE cluster, float weights[3] )  {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  for( int i=0 ; i<3 ;  *weights++ = clu->getParticleType()[i++]  ) ;
  return LCIO::SUCCESS ;
}

int lcclugetshape( PTRTYPE cluster, float shape[6] )  {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  for( int i=0 ; i<6 ;  *shape++ = clu->getShape()[i++]  ) ;
  return LCIO::SUCCESS ;
}

PTRTYPE lcclugetclusters( PTRTYPE cluster ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  const ClusterVec& idvect = clu->getClusters();
  return reinterpret_cast<PTRTYPE>( &idvect );
}

PTRTYPE lcclugetcalorimeterhits( PTRTYPE cluster ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  const CalorimeterHitVec& idvect = clu->getCalorimeterHits();
  return reinterpret_cast<PTRTYPE>( &idvect );
}

int lcclugethitcontributions( PTRTYPE cluster, float* weights, int* nweights ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  int ntot = *nweights -1 ;
  for(unsigned int l=0;l<clu->getCalorimeterHits().size();l++){
    if ( l > (unsigned)ntot ) {
      std::cout << "LCCluster: in getHitContributions more than " << ntot << "weights to store" << std::endl ;
      return LCIO::ERROR ;
    }
    *weights++ = clu->getHitContributions()[l] ;
  }
  *nweights = (int)(clu->getCalorimeterHits().size() + 1);
  return LCIO::SUCCESS ;
}


// set,add Methods

// int lcclusettype( PTRTYPE cluster, int type ) {
//   ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
//   clu->setType( type ) ;
//   return LCIO::SUCCESS ;
// }
int lcclusettypebit( PTRTYPE cluster, int bit ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  clu->setTypeBit( bit ) ;
  return LCIO::SUCCESS ;
}

int lcclusetenergy( PTRTYPE cluster, float energy ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  clu->setEnergy( energy ) ;
  return LCIO::SUCCESS ;
}

int lcclusetposition( PTRTYPE cluster, float pos[3] ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  clu->setPosition( pos ) ;
  return LCIO::SUCCESS ;
}


int lcclusetpositionerror( PTRTYPE cluster, float errpos[3] ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  clu->setPositionError( errpos ) ;
  return LCIO::SUCCESS ;
}

int lcclusettheta( PTRTYPE cluster, float theta) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  clu->setTheta( theta ) ;
  return LCIO::SUCCESS ;
}

int lcclusetphi( PTRTYPE cluster, float phi ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  clu->setPhi( phi ) ;
  return LCIO::SUCCESS ;
}

int lcclusetdirectionerror( PTRTYPE cluster, float errdir[3] ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  clu->setDirectionError( errdir ) ;
  return LCIO::SUCCESS ;
}

int lcclusetshape( PTRTYPE cluster, float shape[6] ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  clu->setShape( shape ) ;
  return LCIO::SUCCESS ;
}

int lcclusetemweight( PTRTYPE cluster, float weight) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  clu->setEMWeight( weight ) ;
  return LCIO::SUCCESS ;
}

int lcclusethadweight( PTRTYPE cluster, float weight) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  clu->setHADWeight( weight ) ;
  return LCIO::SUCCESS ;
}

int lcclusetmuonweight( PTRTYPE cluster, float weight) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  clu->setMuonWeight( weight ) ;
  return LCIO::SUCCESS ;
}

int lccluaddcluster( PTRTYPE cluster, PTRTYPE clus) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  Cluster* cclus = f2c_pointer<Cluster,LCObject>( clus ) ;
  clu->addCluster( cclus ) ;
  return LCIO::SUCCESS ;
}

int lccluaddhit( PTRTYPE cluster, PTRTYPE calohit, float weight ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  CalorimeterHit* ccalh = f2c_pointer<CalorimeterHit,LCObject>( calohit ) ;
  clu->addHit( ccalh, weight ) ;
  return LCIO::SUCCESS ;
}



