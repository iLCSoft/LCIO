
#include "CPPFORT/lcrnv.h"
#include "UTIL/LCRelationNavigator.h"

#include "lcio.h"
#include "IMPL/LCRelationImpl.h"
#include "EVENT/LCCollection.h"
#include "EVENT/LCObject.h"
#include "LCIOSTLTypes.h"

#include <iostream>

using namespace lcio ;

 PTRTYPE lcrnvcreate( const char* fromType, const char* toType ){
   LCRelationNavigator*  relation = new LCRelationNavigator( fromType, toType ) ;
   return reinterpret_cast<PTRTYPE>( relation ) ;

 }

 int lcrnvdelete( PTRTYPE relation ){
   LCRelationNavigator* rel = reinterpret_cast<LCRelationNavigator*>( relation ) ;
   delete rel ;
   return LCIO::SUCCESS ;

 }

 PTRTYPE lcrnvcreatefromcollection( PTRTYPE collection ){
   LCCollection*  col = reinterpret_cast<LCCollection*>( collection ) ;
   LCRelationNavigator*  relation = new LCRelationNavigator( col ) ;
   return reinterpret_cast<PTRTYPE>( relation ) ;

 }

 char* lcrnvgetfromtype( PTRTYPE relation ){
   LCRelationNavigator* rel = reinterpret_cast<LCRelationNavigator*>( relation ) ;
   return const_cast<char*>(rel->getFromType().c_str() ) ;
 }

 char* lcrnvgettotype( PTRTYPE relation ){
   LCRelationNavigator* rel = reinterpret_cast<LCRelationNavigator*>( relation ) ;
   return const_cast<char*>(rel->getToType().c_str() ) ;
 }

 PTRTYPE lcrnvgetrelatedtoobjects( PTRTYPE relation, PTRTYPE object ){
   LCRelationNavigator* rel = reinterpret_cast<LCRelationNavigator*>( relation ) ;
   LCObject* obj       = f2c_pointer<LCObject,LCObject>( object ) ;
   const LCObjectVec& vec    = rel->getRelatedToObjects( obj ) ;
   return reinterpret_cast<PTRTYPE>( &vec ) ;
 }

 PTRTYPE lcrnvgetrelatedfromobjects( PTRTYPE relation, PTRTYPE object ){
   LCRelationNavigator* rel = reinterpret_cast<LCRelationNavigator*>( relation ) ;
   LCObject* obj       = f2c_pointer<LCObject,LCObject>( object ) ;
   const LCObjectVec& vec    = rel->getRelatedFromObjects( obj ) ;
   return reinterpret_cast<PTRTYPE>( &vec ) ;
 }

 PTRTYPE lcrnvgetrelatedtoweights ( PTRTYPE relation, PTRTYPE object ){
   LCRelationNavigator* rel = reinterpret_cast<LCRelationNavigator*>( relation ) ;
   LCObject* obj       = f2c_pointer<LCObject,LCObject>( object ) ;
   const FloatVec &  vec     = rel->getRelatedToWeights( obj ) ;
   return reinterpret_cast<PTRTYPE>( &vec ) ;
 }

 PTRTYPE lcrnvgetrelatedfromweights ( PTRTYPE relation, PTRTYPE object ){
   LCRelationNavigator* rel = reinterpret_cast<LCRelationNavigator*>( relation ) ;
   LCObject* obj       = f2c_pointer<LCObject,LCObject>( object ) ;
   const FloatVec &  vec     = rel->getRelatedFromWeights( obj ) ;
   return reinterpret_cast<PTRTYPE>( &vec ) ;
 }

 int lcrnvgaddrelation(PTRTYPE relation, PTRTYPE objectfrom, PTRTYPE objectto, float weight ){
   LCRelationNavigator* rel = reinterpret_cast<LCRelationNavigator*>( relation ) ;
   LCObject* objf      = f2c_pointer<LCObject,LCObject>( objectfrom ) ;
   LCObject* objt      = f2c_pointer<LCObject,LCObject>( objectto ) ;
   rel->addRelation( objf, objt, weight ) ;
   return LCIO::SUCCESS ;
 }

 int lcrnvgremoverelation(PTRTYPE relation, PTRTYPE objectfrom, PTRTYPE objectto ){
   LCRelationNavigator* rel = reinterpret_cast<LCRelationNavigator*>( relation ) ;
   LCObject* objf      = f2c_pointer<LCObject,LCObject>( objectfrom ) ;
   LCObject* objt      = f2c_pointer<LCObject,LCObject>( objectto ) ;
   rel->removeRelation( objf, objt ) ;
   return LCIO::SUCCESS ;
 }

 PTRTYPE lcrnvcreatecollection(PTRTYPE relation ){
   LCRelationNavigator* rel = reinterpret_cast<LCRelationNavigator*>( relation ) ;
   LCCollection*  col = rel->createLCCollection() ;
   return reinterpret_cast<PTRTYPE>( col ) ;

 }


