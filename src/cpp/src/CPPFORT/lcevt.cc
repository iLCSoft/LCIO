#include "CPPFORT/lcevt.h"

#include "lcio.h" 
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/LCTOOLS.h"
#include <iostream>


using namespace lcio ;


PTRTYPE lcevtcreate(){
  LCEventImpl*  event = new LCEventImpl() ;
  return reinterpret_cast<PTRTYPE>( event ) ;
}

int lcevtdelete( PTRTYPE event ){
  LCEventImpl* evt = reinterpret_cast<LCEventImpl*>(event) ; 
  delete evt ;
  return LCIO::SUCCESS ;
}

int lcevtgetrunnumber( PTRTYPE event ){
  LCEventImpl* evt = reinterpret_cast<LCEventImpl*>(event) ; 
  return evt->getRunNumber()  ;
}

int lcevtgeteventnumber( PTRTYPE event ){
  LCEventImpl* evt = reinterpret_cast<LCEventImpl*>(event) ; 
  return evt->getEventNumber()  ;
}

char* lcevtgetdetectorname( PTRTYPE event ){
  LCEventImpl* evt = reinterpret_cast<LCEventImpl*>(event) ; 
  return const_cast<char*> (evt->getDetectorName().c_str()  ) ;
}

PTRTYPE lcevtgetcollectionnames( PTRTYPE event ){
  LCEventImpl* evt = reinterpret_cast<LCEventImpl*>(event) ; 
  return reinterpret_cast<PTRTYPE> ( evt->getCollectionNames() ) ;
}

// PTRTYPE lcevtgettrelationnames( PTRTYPE event ){
//   LCEventImpl* evt = reinterpret_cast<LCEventImpl*>(event) ; 
//   return reinterpret_cast<PTRTYPE> ( evt->getRelationNames() ) ;
// }

long lcevtgettimestamp( PTRTYPE event )
{
  LCEventImpl* evt = reinterpret_cast<LCEventImpl*>(event) ; 
  return evt->getTimeStamp()  ;
}

PTRTYPE lcevtgetcollection(PTRTYPE event, const char* colname){
  try{
    LCEventImpl* evt = reinterpret_cast<LCEventImpl*>(event) ; 
    return reinterpret_cast<PTRTYPE>( evt->getCollection( colname ) ) ;
  }catch(...){ return 0 ;}
}

// PTRTYPE lcevtgetrelation(PTRTYPE event, const char* colname){
//   try{
//     LCEventImpl* evt = reinterpret_cast<LCEventImpl*>(event) ; 
//     return reinterpret_cast<PTRTYPE>( evt->getRelation( colname ) ) ;
//   }catch(...){ return 0 ;}
// }

int lcevtaddcollection( PTRTYPE event, PTRTYPE collection, char* colname ){
  try{
    LCEventImpl* evt = reinterpret_cast<LCEventImpl*>(event) ; 
    LCCollectionVec* col = reinterpret_cast<LCCollectionVec*>(collection) ; 
    evt->addCollection( col , colname ) ;
  }catch(...){ return LCIO::ERROR ; }
  return LCIO::SUCCESS ;
}

int lcevtremovecollection( PTRTYPE event, char* colname ){
  try{
    LCEventImpl* evt = reinterpret_cast<LCEventImpl*>(event) ; 
    evt->removeCollection( colname ) ;
  }catch(...){ return LCIO::ERROR ; }
  return LCIO::SUCCESS ;
}

// int lcevtaddrelation( PTRTYPE event, PTRTYPE relation, char* colname ){
//   try{
//     LCEventImpl* evt = reinterpret_cast<LCEventImpl*>(event) ; 
//     LCRelation* rel = reinterpret_cast<LCRelation*>(relation) ;
//     evt->addRelation( rel , colname ) ;
//   }catch(...){ return LCIO::ERROR ; }
//   return LCIO::SUCCESS ;
// }

// int lcevtremoverelation( PTRTYPE event, const char* colname ){
//   try{
//     LCEventImpl* evt = reinterpret_cast<LCEventImpl*>(event) ; 
//     evt->removeRelation( colname ) ;
//   }catch(...){ return LCIO::ERROR ; }
//   return LCIO::SUCCESS ;
// }

int lcevtsetrunnumber( PTRTYPE event, int rn ){
  LCEventImpl* evt = reinterpret_cast<LCEventImpl*>(event) ; 
  evt->setRunNumber( rn ) ;
  return LCIO::SUCCESS ;
}

int lcevtseteventnumber( PTRTYPE event, int en ){
  LCEventImpl* evt = reinterpret_cast<LCEventImpl*>(event) ; 
  evt->setEventNumber( en ) ;
  return LCIO::SUCCESS ;
}

int lcevtsetdetectorname( PTRTYPE event,  char* dn ){
  LCEventImpl* evt = reinterpret_cast<LCEventImpl*>(event) ; 
  evt->setDetectorName( dn ) ;
  return LCIO::SUCCESS ;
}

int lcevtsettimestamp( PTRTYPE event,  long ts ){
  LCEventImpl* evt = reinterpret_cast<LCEventImpl*>(event) ; 
  evt->setTimeStamp( ts ) ;
  return LCIO::SUCCESS ;
}
