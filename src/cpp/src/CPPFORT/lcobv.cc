
#include "CPPFORT/lcobv.h"
#include "UTIL/LCRelationNavigator.h"

#include "lcio.h"
#include "IMPL/LCRelationImpl.h"
#include "EVENT/LCCollection.h"
#include "EVENT/LCObject.h"
#include "LCIOSTLTypes.h"

#include <iostream>

using namespace lcio ;

typedef std::map< EVENT::LCObject* , std::pair< EVENT::LCObjectVec , EVENT::FloatVec > > RelMap ;

 int lcobvgetlength( PTRTYPE vector ){
   const LCObjectVec* vec    = reinterpret_cast<LCObjectVec*>( vector ) ;
   return  vec->size() ;
 }

 int lcobvgetobjectid( PTRTYPE vector, int index){
   const LCObjectVec* vec    = reinterpret_cast<LCObjectVec*>( vector ) ;
   LCObject* obj = vec->at(index-1) ;
   return obj->id() ;
 }

 float lcobvgetweight( PTRTYPE vector, int index){
   const FloatVec* vec    = reinterpret_cast<FloatVec*>( vector ) ;
   return vec->at(index-1) ;
 }


