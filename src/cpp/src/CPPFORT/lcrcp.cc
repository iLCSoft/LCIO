#include "CPPFORT/lcrcp.h"

#include "lcio.h" 
#include "IMPL/ReconstructedParticleImpl.h"

using namespace lcio ;

#include <iostream>

// create delete ReconstructedParticle

PTRTYPE lcrcpcreate(){
  ReconstructedParticleImpl* rcp = new ReconstructedParticleImpl ;
  return C2F_POINTER( LCObject*, rcp ) ;
}
int lcrcpdelete( PTRTYPE recopart ){
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  delete rcp ;
  return LCIO::SUCCESS ;
}


// get Methods

int lcrcpgettype( PTRTYPE recopart )  {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  return rcp->getType() ;
}

bool lcrcpisprimary ( PTRTYPE recopart ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  return rcp->isPrimary() ;
}

int lcrcpgetmomentum( PTRTYPE recopart, float* p )  {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  for( int i=0 ; i<3 ;  *p++ = rcp->getMomentum()[i++]  ) ;
  return LCIO::SUCCESS ;
}

float lcrcpgetenergy( PTRTYPE recopart ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  return rcp->getEnergy() ;
}

int lcrcpgetcovmatrix( PTRTYPE recopart, float cvmtx[NCOVARIANCE] )  {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  for( int i=0 ; i<NCOVARIANCE ;  cvmtx[i] = rcp->getCovMatrix()[i++]  ) ;
  return LCIO::SUCCESS ;
}

float lcrcpgetmass( PTRTYPE recopart ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  return rcp->getMass() ;
}

float lcrcpgetcharge( PTRTYPE recopart ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  return rcp->getCharge() ;
}

int lcrcpgetreferencepoint( PTRTYPE recopart, float refpoint[3] ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  for( int i=0 ; i<3 ;  *refpoint++ = rcp->getReferencePoint()[i++]  ) ;
  return LCIO::SUCCESS ;
}

PTRTYPE lcrcpgetparticleids( PTRTYPE recopart ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  const ParticleIDVec& idvect = rcp->getParticleIDs();
  return reinterpret_cast<PTRTYPE>( &idvect );
}

PTRTYPE lcrcpgetparticles( PTRTYPE recopart ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  const ReconstructedParticleVec& idvect = rcp->getParticles();
  return reinterpret_cast<PTRTYPE>( &idvect );
}


int lcrcpgetparticleweights( PTRTYPE recopart, float* weights, int* nweights ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  int ntot = *nweights - 1 ;
  for(unsigned int l=0;l<rcp->getParticles().size();l++){
    if ( l > (unsigned)ntot ) {
      std::cout << "LCReconstructedParticle: in getParticleWeights more than " 
                << ntot << "weights to store" << std::endl ;
      return LCIO::ERROR ;
    }
    *weights++ = rcp->getParticleWeights()[l] ;
  }
  *nweights = (int)(rcp->getParticles().size() + 1);
  return LCIO::SUCCESS ;
}

PTRTYPE lcrcpgetclusters( PTRTYPE recopart ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  const ClusterVec& idvect = rcp->getClusters();
  return reinterpret_cast<PTRTYPE>( &idvect );
}


int lcrcpgetclusterweights( PTRTYPE recopart, float* weights, int* nweights ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  int ntot = *nweights - 1;
  for(unsigned int l=0;l<rcp->getClusters().size();l++){
    if ( l > (unsigned)ntot ) {
      std::cout << "LCReconstructedParticle: in getClusterWeights more than " 
                << ntot << "weights to store" << std::endl ;
      return LCIO::ERROR ;
    }
    *weights++ = rcp->getClusterWeights()[l] ;
  }
  *nweights = (int)(rcp->getClusters().size() + 1) ;
  return LCIO::SUCCESS ;
}

PTRTYPE lcrcpgettracks( PTRTYPE recopart ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  const TrackVec& idvect = rcp->getTracks();
  return reinterpret_cast<PTRTYPE>( &idvect );
}

int lcrcpgettrackweights( PTRTYPE recopart, float* weights, int* nweights ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  int ntot = *nweights - 1;
  for(unsigned int l=0;l<rcp->getTracks().size();l++){
    if ( l > (unsigned)ntot ) {
      std::cout << "LCReconstructedParticle: in getTrackWeights more than "
                << ntot << "weights to store" << std::endl ;
      return LCIO::ERROR ;
    }
    *weights++ = rcp->getTrackWeights()[l] ;
  }
  *nweights = (int)(rcp->getTracks().size() + 1) ;
  return LCIO::SUCCESS ;
}

PTRTYPE lcrcpgetmcparticles( PTRTYPE recopart ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  const MCParticleVec& idvect = rcp->getMCParticles();
  return reinterpret_cast<PTRTYPE>( &idvect );
}

int lcrcpgetmcparticleweights( PTRTYPE recopart, float* weights, int* nweights ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  int ntot = *nweights - 1 ;
  for(unsigned int l=0;l<rcp->getMCParticles().size();l++){
    if ( l > (unsigned)ntot ) {
      std::cout << "LCReconstructedParticle: in getMCParticleWeights more than " 
                << ntot << "weights to store" << std::endl ;
      return LCIO::ERROR ;
    }
    *weights++ = rcp->getMCParticleWeights()[l] ;
  }
  *nweights = (int)(rcp->getMCParticles().size() + 1);
  return LCIO::SUCCESS ;
}


// set,add Methods

int lcrcpsettype( PTRTYPE recopart, int type ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  rcp->setType( type ) ;
  return LCIO::SUCCESS ;
}

int lcrcpsetprimary( PTRTYPE recopart, bool lprimary ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  rcp->setPrimary( lprimary ) ;
  return LCIO::SUCCESS ;
}

int lcrcpsetmomentum( PTRTYPE recopart, float p[3] ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
   rcp->setMomentum( p ) ;
   return LCIO::SUCCESS ;
}

int lcrcpsetenergy( PTRTYPE recopart, float energy ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  rcp->setEnergy( energy ) ;
  return LCIO::SUCCESS ;
}

int lcrcpsetcovmatrix( PTRTYPE recopart, float cvmtx[NCOVARIANCE] ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  rcp->setCovMatrix( cvmtx ) ;
  return LCIO::SUCCESS ;
}

int lcrcpsetmass( PTRTYPE recopart, float xmass) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  rcp->setMass( xmass ) ;
  return LCIO::SUCCESS ;
}

int lcrcpsetcharge( PTRTYPE recopart, float charge ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  rcp->setCharge( charge ) ;
  return LCIO::SUCCESS ;
}

int lcrcpsetreferencepoint( PTRTYPE recopart, float refpoint[3] ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  rcp->setReferencePoint( refpoint ) ;
  return LCIO::SUCCESS ;
}

int lcrcpaddparticleid( PTRTYPE recopart, PTRTYPE pid ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  ParticleID* ppid = f2c_pointer<ParticleID,LCObject>( pid ) ;
  rcp->addParticleID( ppid ) ;
  return LCIO::SUCCESS ;
}

int lcrcpaddparticle( PTRTYPE recopart, PTRTYPE particle, float weight ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  ReconstructedParticle* rparticle = f2c_pointer<ReconstructedParticle,LCObject>( particle ) ;
  rcp->addParticle( rparticle, weight ) ;
  return LCIO::SUCCESS ;
}

int lcrcpaddcluster( PTRTYPE recopart, PTRTYPE clus, float weight ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  Cluster* cclus = f2c_pointer<Cluster,LCObject>( clus ) ;
  rcp->addCluster( cclus, weight ) ;
  return LCIO::SUCCESS ;
}

int lcrcpaddtrack( PTRTYPE recopart, PTRTYPE track, float weight ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  Track* ctrack = f2c_pointer<Track,LCObject>( track ) ;
  rcp->addTrack( ctrack, weight ) ;
  return LCIO::SUCCESS ;
}

int lcrcpaddmcparticle( PTRTYPE recopart, PTRTYPE particle, float weight ) {
  ReconstructedParticleImpl* rcp = f2c_pointer<ReconstructedParticleImpl,LCObject>( recopart ) ;
  MCParticle* mmcp = f2c_pointer<MCParticle,LCObject>( particle ) ;
  rcp->addMCParticle( mmcp, weight ) ;
  return LCIO::SUCCESS ;
}



