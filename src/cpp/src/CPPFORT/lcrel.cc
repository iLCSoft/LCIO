
#include "CPPFORT/lcrel.h"

#include "lcio.h"
#include "IMPL/LCRelationImpl.h"
#include <iostream>

using namespace lcio ;

PTRTYPE lcrelcreate( const char* fromType, const char* toType ){
  LCRelationImpl* relation = new LCRelationImpl( fromType, toType ) ;
  return reinterpret_cast<PTRTYPE>( relation ) ;

}

int lcreldelete( PTRTYPE relation ){
  LCRelationImpl* rel = reinterpret_cast<LCRelationImpl*>( relation ) ;
  delete rel ;
  return LCIO::SUCCESS ;

}

int lcrelnumberofrelations( PTRTYPE relation,  PTRTYPE object ){
  LCRelationImpl* rel = reinterpret_cast<LCRelationImpl*>( relation ) ;
  LCObject* obj       = f2c_pointer<LCObject,LCObject>( object ) ;
  return rel->numberOfRelations( obj )  ;

}

PTRTYPE lcrelgetrelation( PTRTYPE relation, PTRTYPE object, int i ){
  LCRelationImpl* rel = reinterpret_cast<LCRelationImpl*>( relation ) ;
  LCObject* obj       = f2c_pointer<LCObject,LCObject>( object ) ;
  LCObject* objget    = rel->getRelation( obj, i - 1 ) ;
  return reinterpret_cast<PTRTYPE>( objget ) ;
}

float lcrelgetweight( PTRTYPE relation, PTRTYPE object, int i ){
  LCRelationImpl* rel = reinterpret_cast<LCRelationImpl*>( relation ) ;
  LCObject* obj       = f2c_pointer<LCObject,LCObject>( object ) ;
  return rel->getWeight( obj, i - 1 ) ;

}

int lcreladdrelation(PTRTYPE relation, PTRTYPE objectfrom, PTRTYPE objectto, float weight ){
  LCRelationImpl* rel = reinterpret_cast<LCRelationImpl*>( relation ) ;
  LCObject* objf      = f2c_pointer<LCObject,LCObject>( objectfrom ) ;
  LCObject* objt      = f2c_pointer<LCObject,LCObject>( objectto ) ;
  rel->addRelation( objf, objt, weight ) ;
  return LCIO::SUCCESS ;

}

int lcrelusecaching(PTRTYPE relation, bool val ){
  LCRelationImpl* rel = reinterpret_cast<LCRelationImpl*>( relation ) ;
  rel->useCaching( val ) ;
  return LCIO::SUCCESS ;

}

int lcrelsettypes(PTRTYPE relation, const char* fromType, const char* toType ){
  LCRelationImpl* rel = reinterpret_cast<LCRelationImpl*>( relation ) ;
  rel->setTypes( fromType, toType ) ;
  return LCIO::SUCCESS ;

}


