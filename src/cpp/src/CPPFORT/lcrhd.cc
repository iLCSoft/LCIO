#include "CPPFORT/lcrhd.h"

#include "lcio.h" 
#include "Exceptions.h"
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/LCTOOLS.h"
#include <iostream>

using namespace lcio ;


PTRTYPE lcrhdcreate(){
  LCRunHeaderImpl* rhd = new LCRunHeaderImpl ;
  return reinterpret_cast<PTRTYPE>(rhd) ;
}

int lcrhddelete( PTRTYPE runHeader ){
  LCRunHeaderImpl* rhd =  reinterpret_cast<LCRunHeaderImpl*>(runHeader) ;
  delete rhd ;
  return LCIO::SUCCESS ;
}
int lcrhdgetrunnumber( PTRTYPE runHeader ){
  LCRunHeaderImpl* rhd =  reinterpret_cast<LCRunHeaderImpl*>(runHeader) ;
  return rhd->getRunNumber() ;
}
char* lcrhdgetdetectorname( PTRTYPE runHeader  ){
  LCRunHeaderImpl* rhd =  reinterpret_cast<LCRunHeaderImpl*>(runHeader) ;
  return const_cast<char*>( rhd->getDetectorName().c_str() );
}
char* lcrhdgetdescription( PTRTYPE runHeader ){
  LCRunHeaderImpl* rhd =  reinterpret_cast<LCRunHeaderImpl*>(runHeader) ;
  return  const_cast<char*>( rhd->getDescription().c_str() );
}


PTRTYPE lcrhdgetactivesubdetectors(PTRTYPE runHeader){
  LCRunHeaderImpl* rhd =  reinterpret_cast<LCRunHeaderImpl*>(runHeader) ;
  return reinterpret_cast<PTRTYPE>( rhd->getActiveSubdetectors()  ) ;
}

int lcrhdsetrunnumber( PTRTYPE runHeader, int rn){
  LCRunHeaderImpl* rhd =  reinterpret_cast<LCRunHeaderImpl*>(runHeader) ;
  rhd->setRunNumber( rn ) ;
  return LCIO::SUCCESS ;
}
int lcrhdsetdetectorname( PTRTYPE runHeader, const char* dn){
  LCRunHeaderImpl* rhd =  reinterpret_cast<LCRunHeaderImpl*>(runHeader) ;
  rhd->setDetectorName( dn ) ;
  return LCIO::SUCCESS ;
}
int lcrhdsetdescription( PTRTYPE runHeader, const char* dsc){
  LCRunHeaderImpl* rhd =  reinterpret_cast<LCRunHeaderImpl*>(runHeader) ;
  rhd->setDescription( dsc ) ;
  return LCIO::SUCCESS ;
}
int lcrhdaddactivesubdetector( PTRTYPE runHeader, const char* adn){
  LCRunHeaderImpl* rhd =  reinterpret_cast<LCRunHeaderImpl*>(runHeader) ;
  rhd->addActiveSubdetector( adn ) ;
  return LCIO::SUCCESS ;
}




