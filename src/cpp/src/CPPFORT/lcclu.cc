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

int lccluid( PTRTYPE cluster )  {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  return clu->id() ;
}

int lcclugettype( PTRTYPE cluster )  {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  return clu->getType() ;
}

// int lcclutesttype( PTRTYPE cluster , int bit )  {
//   ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
//   return clu->testType( bit ) ;
// }

float lcclugetenergy( PTRTYPE cluster ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  return clu->getEnergy() ;
}
float lcclugetenergyerr( PTRTYPE cluster )  {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  return clu->getEnergyError() ;
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

float lcclugetitheta( PTRTYPE cluster ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  return clu->getITheta() ;
}

float lcclugetiphi( PTRTYPE cluster ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  return clu->getIPhi() ;
}

int lcclugetdirectionerror( PTRTYPE cluster, float direrr[3] )  {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  for( int i=0 ; i<3 ;  *direrr++ = clu->getDirectionError()[i++]  ) ;
  return LCIO::SUCCESS ;
}

// int lcclugetparticletype( PTRTYPE cluster, float weights[3] )  {
//   ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
//   for( int i=0 ; i<3 ;  *weights++ = clu->getParticleType()[i++]  ) ;
//   return LCIO::SUCCESS ;
// }

// int lcclugetshape( PTRTYPE cluster, float shape[6] )  {
//   ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
//   for( int i=0 ; i<6 ;  *shape++ = clu->getShape()[i++]  ) ;
//   return LCIO::SUCCESS ;
// }

PTRTYPE lcclugetshape( PTRTYPE cluster )  {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  const FloatVec& shape = clu->getShape();
  return reinterpret_cast<PTRTYPE>( &shape );
}

PTRTYPE lcclugetparticleids( PTRTYPE cluster ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  const ParticleIDVec& idvect = clu->getParticleIDs();
  return reinterpret_cast<PTRTYPE>( &idvect );
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

PTRTYPE lcclugetsubdetectorenergies( PTRTYPE cluster ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  const FloatVec& idvect = clu->getSubdetectorEnergies();
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
int lcclusettypebit( PTRTYPE cluster, int bit , int val) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  clu->setTypeBit( bit , val ) ;
  return LCIO::SUCCESS ;
}

int lcclusetenergy( PTRTYPE cluster, float energy ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  clu->setEnergy( energy ) ;
  return LCIO::SUCCESS ;
}

int lcclusetenergyerr( PTRTYPE cluster, float enerr ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  clu->setEnergyError( enerr ) ;
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

int lcclusetitheta( PTRTYPE cluster, float theta) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  clu->setITheta( theta ) ;
  return LCIO::SUCCESS ;
}

int lcclusetiphi( PTRTYPE cluster, float phi ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  clu->setIPhi( phi ) ;
  return LCIO::SUCCESS ;
}

int lcclusetdirectionerror( PTRTYPE cluster, float errdir[3] ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  clu->setDirectionError( errdir ) ;
  return LCIO::SUCCESS ;
}

int lcclusetshape( PTRTYPE cluster, PTRTYPE vector ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  FloatVec* shape =  reinterpret_cast<FloatVec*>(vector) ;
  clu->setShape( *shape ) ;
  return LCIO::SUCCESS ;
}

// int lcclusetemweight( PTRTYPE cluster, float weight) {
//   ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
//   clu->setEMWeight( weight ) ;
//   return LCIO::SUCCESS ;
// }

// int lcclusethadweight( PTRTYPE cluster, float weight) {
//   ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
//   clu->setHADWeight( weight ) ;
//   return LCIO::SUCCESS ;
// }

// int lcclusetmuonweight( PTRTYPE cluster, float weight) {
//   ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
//   clu->setMuonWeight( weight ) ;
//   return LCIO::SUCCESS ;
// }


int lccluaddparticleid( PTRTYPE cluster, PTRTYPE pid) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  ParticleID* cpid = f2c_pointer<ParticleID,LCObject>( pid ) ;
  clu->addParticleID( cpid ) ;
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

int lcclusetsubdetectorenergies( PTRTYPE cluster, float* floatv, const int nfloatv ) {
  ClusterImpl* clu = f2c_pointer<ClusterImpl,LCObject>( cluster ) ;
  FloatVec& floatvec =  clu->subdetectorEnergies() ;
  floatvec.resize( nfloatv ) ;
  for(int j=0;j<nfloatv;j++) {
    floatvec[j] =  floatv[j]  ;
  }
  return LCIO::SUCCESS ;

}


