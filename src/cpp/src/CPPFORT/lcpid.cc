#include "CPPFORT/lcpid.h"

#include "lcio.h" 
#include "IMPL/ParticleIDImpl.h"
#include "EVENT/LCFloatVec.h"

using namespace lcio ;

#include <iostream>

// create delete ParticleID

PTRTYPE lcpidcreate(){
  ParticleIDImpl* pid = new ParticleIDImpl ;
  return C2F_POINTER( LCObject*, pid ) ;
}
int lcpiddelete( PTRTYPE pidit ){
  ParticleIDImpl* pid = f2c_pointer<ParticleIDImpl,LCObject>( pidit ) ;
  delete pid ;
  return LCIO::SUCCESS ;
}


// get Methods

int lcpidid( PTRTYPE pidit )  {
  ParticleIDImpl* pid = f2c_pointer<ParticleIDImpl,LCObject>( pidit ) ;
  return pid->id() ;
}

int lcpidgettype( PTRTYPE pidit )  {
  ParticleIDImpl* pid = f2c_pointer<ParticleIDImpl,LCObject>( pidit ) ;
  return pid->getType() ;
}

int lcpidgetpdg( PTRTYPE pidit )  {
  ParticleIDImpl* pid = f2c_pointer<ParticleIDImpl,LCObject>( pidit ) ;
  return pid->getPDG() ;
}

float lcpidgetlikelihood( PTRTYPE pidit ) {
  ParticleIDImpl* pid = f2c_pointer<ParticleIDImpl,LCObject>( pidit ) ;
  return pid->getLikelihood() ;
}

int lcpidgetalgorithmtype( PTRTYPE pidit )  {
  ParticleIDImpl* pid = f2c_pointer<ParticleIDImpl,LCObject>( pidit ) ;
  return pid->getAlgorithmType() ;
}

int lcpidgetparameters( PTRTYPE pidit, float* vec, int* nvec ) {
  ParticleIDImpl* pid = f2c_pointer<ParticleIDImpl,LCObject>( pidit ) ;
  int ntot = *nvec - 1 ;
  const FloatVec* floatVec = &pid->getParameters() ;
  for(unsigned int l=0;l<floatVec->size();l++){
    if ( l > (unsigned)(ntot) ) {
      std::cout << "LCParticleID: in getParameters more than " << ntot << "weights to store" << std::endl ;
      return LCIO::ERROR ;
    }
    *vec++ = (*floatVec)[l] ;
  }
  *nvec = (int)(floatVec->size() + 1);
  return LCIO::SUCCESS ;
}


// set,add Methods

int lcpidsettype( PTRTYPE pidit, int type ) {
  ParticleIDImpl* pid = f2c_pointer<ParticleIDImpl,LCObject>( pidit ) ;
  pid->setType( type ) ;
  return LCIO::SUCCESS ;
}

int lcpidsetpdg( PTRTYPE pidit, int pdg ) {
  ParticleIDImpl* pid = f2c_pointer<ParticleIDImpl,LCObject>( pidit ) ;
  pid->setPDG( pdg ) ;
  return LCIO::SUCCESS ;
}

int lcpidsetlikelihood( PTRTYPE pidit, float logl) {
  ParticleIDImpl* pid = f2c_pointer<ParticleIDImpl,LCObject>( pidit ) ;
  pid->setLikelihood( logl ) ;
  return LCIO::SUCCESS ;
}

int lcpidsetalgorithmtype( PTRTYPE pidit, int ident ) {
  ParticleIDImpl* pid = f2c_pointer<ParticleIDImpl,LCObject>( pidit ) ;
  pid->setAlgorithmType( ident ) ;
  return LCIO::SUCCESS ;
}

int lcpidaddparameter( PTRTYPE pidit, float param) {
  ParticleIDImpl* pid = f2c_pointer<ParticleIDImpl,LCObject>( pidit ) ;
  pid->addParameter( param ) ;
  return LCIO::SUCCESS ;
}


