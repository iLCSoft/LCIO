#include "CPPFORT/lcvec.h"
#include "DATA/LCIntVec.h"
#include "DATA/LCFloatVec.h"

using namespace DATA ;

// define an interface to read a string/int/float vector

int lcsvcgetlength(PTRTYPE strvec){
  LCStrVec* strVec = reinterpret_cast<LCStrVec*>(strvec) ;
  return strVec->size() ;
}

char* lcsvcgetstringat(PTRTYPE strvec, int index){
  LCStrVec* strVec = reinterpret_cast<LCStrVec*>(strvec) ;
  return const_cast<char*>( (*strVec)[index-1].c_str() );
}

int lcivcgetlength(PTRTYPE intvec){
  LCIntVec* intVec =  reinterpret_cast<LCIntVec*>(intvec) ;
  return intVec->size() ;
}

int lcivcgetintat(PTRTYPE intvec, int index){
  LCIntVec* intVec =  reinterpret_cast<LCIntVec*>(intvec) ;
  return (*intVec)[index-1] ;
}

int lcfvcgetlength(PTRTYPE floatvec){
  LCFloatVec* floatVec =  reinterpret_cast<LCFloatVec*>(floatvec) ;
  return floatVec->size() ;
}

float lcfvcgetfloatat(PTRTYPE floatvec, int index){
  LCFloatVec* floatVec =  reinterpret_cast<LCFloatVec*>(floatvec) ;
  return (*floatVec)[index-1] ;
}

