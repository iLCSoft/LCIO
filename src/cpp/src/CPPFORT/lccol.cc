#include "CPPFORT/lccol.h"

#include "lcio.h" 
#include "Exceptions.h"
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/LCTOOLS.h"
#include "EVENT/MCParticle.h"
#include "EVENT/SimCalorimeterHit.h"
#include "EVENT/CalorimeterHit.h"
#include "EVENT/SimTrackerHit.h"
#include "EVENT/TPCHit.h"
#include "EVENT/LCIO.h"

#include <iostream>

using namespace lcio ;
using namespace std ;

PTRTYPE lccolcreate( const char* colname ){
  LCCollectionVec* col = new LCCollectionVec( colname ) ;
  return reinterpret_cast<PTRTYPE>( col ) ;
}
int lccoldelete( PTRTYPE collection ){
  LCCollectionVec* col = reinterpret_cast<LCCollectionVec*>(collection) ;
  delete col ;
  return LCIO::SUCCESS ;
}

int lccolgetnumberofelements( PTRTYPE collection ){
  LCCollectionVec* col = reinterpret_cast<LCCollectionVec*>(collection) ;
  return col->getNumberOfElements() ;
}

char* lccolgettypename( PTRTYPE collection ){
  const LCCollectionVec* col = reinterpret_cast<LCCollectionVec*>( (collection) ) ;
  return const_cast<char*>( col->getTypeName().c_str() ) ;
}

PTRTYPE lccolgetelementat( PTRTYPE collection, int index ){
  LCCollectionVec* col = reinterpret_cast<LCCollectionVec*>(collection) ;
//  return  reinterpret_cast<PTRTYPE>( col->getElementAt( index - 1 )  ) ;
  return C2F_POINTER( LCObject*, col->getElementAt( index - 1 )  ) ;
}

int lccolgetflag(PTRTYPE collection){
  LCCollectionVec* col = reinterpret_cast<LCCollectionVec*>(collection) ;
  return col->getFlag() ;
}

bool lccolistransient(PTRTYPE collection){
  LCCollectionVec* col = reinterpret_cast<LCCollectionVec*>(collection) ;
  return col->isTransient() ;
}

int lccolsettransient(PTRTYPE collection, bool value){
  LCCollectionVec* col = reinterpret_cast<LCCollectionVec*>(collection) ;
  col->setTransient( value) ;
  return LCIO::SUCCESS ;
}
bool lccolisdefault(PTRTYPE collection){
  LCCollectionVec* col = reinterpret_cast<LCCollectionVec*>(collection) ;
  return col->isDefault() ;
}

int lccolsetdefault(PTRTYPE collection, bool value){
  LCCollectionVec* col = reinterpret_cast<LCCollectionVec*>(collection) ;
  col->setDefault( value) ;
  return LCIO::SUCCESS ;
}
 
int lccolsetflag(PTRTYPE collection, int flag){
  LCCollectionVec* col = reinterpret_cast<LCCollectionVec*>(collection) ;
  col->setFlag( flag) ;
  return LCIO::SUCCESS ;
}
int lccoladdelement(PTRTYPE collection, PTRTYPE object){
  try{
    LCCollectionVec* col = reinterpret_cast<LCCollectionVec*>(collection) ;
    col->addElement( reinterpret_cast<LCObject*>(object) ) ;

  }catch(...){ return LCIO::ERROR ; }
  return LCIO::SUCCESS ;
}

int lccolremoveelementat(PTRTYPE collection, int i){
  try{
    LCCollectionVec* col = reinterpret_cast<LCCollectionVec*>(collection) ;
    col->removeElementAt( i ) ;
  }catch(...){ return LCIO::ERROR ; }
  return LCIO::SUCCESS ;
}

