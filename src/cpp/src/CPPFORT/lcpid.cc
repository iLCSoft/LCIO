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

int lcpidgettypeid( PTRTYPE pidit )  {
  ParticleIDImpl* pid = f2c_pointer<ParticleIDImpl,LCObject>( pidit ) ;
  return pid->getTypeID() ;
}

float lcpidgetprobability( PTRTYPE pidit ) {
  ParticleIDImpl* pid = f2c_pointer<ParticleIDImpl,LCObject>( pidit ) ;
  return pid->getProbability() ;
}

char* lcpidgetidentifier( PTRTYPE pidit )  {
  ParticleIDImpl* pid = f2c_pointer<ParticleIDImpl,LCObject>( pidit ) ;
  return const_cast<char*>( pid->getIdentifier().c_str() ) ;
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

int lcpidsettypeid( PTRTYPE pidit, int type ) {
  ParticleIDImpl* pid = f2c_pointer<ParticleIDImpl,LCObject>( pidit ) ;
  pid->setTypeID( type ) ;
  return LCIO::SUCCESS ;
}

int lcpidsetprobability( PTRTYPE pidit, float prob) {
  ParticleIDImpl* pid = f2c_pointer<ParticleIDImpl,LCObject>( pidit ) ;
  pid->setProbability( prob ) ;
  return LCIO::SUCCESS ;
}

int lcpidsetidentifier( PTRTYPE pidit, char* ident ) {
  ParticleIDImpl* pid = f2c_pointer<ParticleIDImpl,LCObject>( pidit ) ;
  pid->setIdentifier( ident ) ;
  return LCIO::SUCCESS ;
}

int lcpidaddparameter( PTRTYPE pidit, float param) {
  ParticleIDImpl* pid = f2c_pointer<ParticleIDImpl,LCObject>( pidit ) ;
  pid->addParameter( param ) ;
  return LCIO::SUCCESS ;
}


