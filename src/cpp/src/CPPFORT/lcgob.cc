
#include "lcio.h"
#include "CPPFORT/lcgob.h"
#include "IMPL/LCGenericObjectImpl.h"

#include <iostream>

using namespace lcio ;

 PTRTYPE lcgobcreate(){
   LCGenericObjectImpl* object = new LCGenericObjectImpl() ;
   return reinterpret_cast<PTRTYPE>( object ) ;
 }

 PTRTYPE lcgobcreatefixed( int nint, int nfloat, int ndouble ){
   LCGenericObjectImpl*  object = new LCGenericObjectImpl( nint, nfloat, ndouble ) ;
   return reinterpret_cast<PTRTYPE>( object ) ;

 }

 int lcgobdelete( PTRTYPE genericobject ){
   LCGenericObjectImpl* object = reinterpret_cast<LCGenericObjectImpl*>( genericobject ) ;
   delete object ;
   return LCIO::SUCCESS ;

 }

 int lcgobid( PTRTYPE genericobject ){
   LCGenericObjectImpl* object = reinterpret_cast<LCGenericObjectImpl*>( genericobject ) ;
   return object->id() ;
 }

 int lcgobgetnint( PTRTYPE genericobject ){
   LCGenericObjectImpl* object = reinterpret_cast<LCGenericObjectImpl*>( genericobject ) ;
   return object->getNInt() ;
 }

 int lcgobgetnfloat( PTRTYPE genericobject ){
   LCGenericObjectImpl* object = reinterpret_cast<LCGenericObjectImpl*>( genericobject ) ;
   return object->getNFloat() ;
 }

 int lcgobgetndouble( PTRTYPE genericobject ){
   LCGenericObjectImpl* object = reinterpret_cast<LCGenericObjectImpl*>( genericobject ) ;
   return object->getNDouble() ;
 }

 int lcgobgetintval( PTRTYPE genericobject, int index ){
   LCGenericObjectImpl* object = reinterpret_cast<LCGenericObjectImpl*>( genericobject ) ;
   return object->getIntVal(index-1) ;
 }

 float lcgobgetfloatval( PTRTYPE genericobject, int index ){
   LCGenericObjectImpl* object = reinterpret_cast<LCGenericObjectImpl*>( genericobject ) ;
   return object->getFloatVal(index-1) ;
 }

 double lcgobgetdoubleval( PTRTYPE genericobject, int index ){
   LCGenericObjectImpl* object = reinterpret_cast<LCGenericObjectImpl*>( genericobject ) ;
   return object->getDoubleVal(index-1) ;
 }


 int lcgobsetintval( PTRTYPE genericobject, int index, int value ){
   LCGenericObjectImpl* object = reinterpret_cast<LCGenericObjectImpl*>( genericobject ) ;
   object->setIntVal(index-1, value) ;
   return LCIO::SUCCESS ;
 }

 int lcgobsetfloatval( PTRTYPE genericobject, int index, float value ){
   LCGenericObjectImpl* object = reinterpret_cast<LCGenericObjectImpl*>( genericobject ) ;
   object->setFloatVal(index-1, value) ;
   return LCIO::SUCCESS ;
 }

 int lcgobsetdoubleval( PTRTYPE genericobject, int index, double value ){
   LCGenericObjectImpl* object = reinterpret_cast<LCGenericObjectImpl*>( genericobject ) ;
   object->setDoubleVal(index-1, value) ;
   return LCIO::SUCCESS ;
 }


 bool lcgobisfixedsize( PTRTYPE genericobject ){
   LCGenericObjectImpl* object = reinterpret_cast<LCGenericObjectImpl*>( genericobject ) ;
   return object->isFixedSize() ;
 }

 char* lcgobgettypename( PTRTYPE genericobject ){
   LCGenericObjectImpl* object = reinterpret_cast<LCGenericObjectImpl*>( genericobject ) ;
   return const_cast<char*>( object->getTypeName().c_str() ) ;
 }

 char* lcgobgetdatadescription( PTRTYPE genericobject ){
   LCGenericObjectImpl* object = reinterpret_cast<LCGenericObjectImpl*>( genericobject ) ;
   return const_cast<char*>( object->getDataDescription().c_str() ) ;
 }


