#include "CPPFORT/lcwrt.h"

#include "lcio.h" 
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCEventImpl.h"
#include <iostream>

using namespace lcio ;

#define LCWRITER_PNTR( writer ) if(! (writer) ) return 0  ; \
LCWriter* lcWriter = reinterpret_cast<LCWriter*>( (writer) ) ;


PTRTYPE lcwrtcreate(){
  LCWriter* lcWriter = LCFactory::getInstance()->createLCWriter() ;
  return reinterpret_cast<PTRTYPE>(lcWriter) ;
}

int lcwrtdelete(PTRTYPE writer){
  LCWRITER_PNTR( writer )  ;
  delete lcWriter ;
  return LCIO::SUCCESS ;
}

int lcwrtopen(PTRTYPE writer, const char* filename, int writeMode ){
  try{ 
    LCWriter* wrt = reinterpret_cast<LCWriter*>(  writer ) ; 
    wrt->open( filename , writeMode ) ;
    
  }catch(...){ return LCIO::ERROR ; }
  return LCIO::SUCCESS ;
  
}

int lcwrtclose(PTRTYPE writer){
  try{
    LCWriter* wrt = reinterpret_cast<LCWriter*>(  writer ) ; 
    wrt->close() ;
    }catch(...){ return LCIO::ERROR ; }

  return LCIO::SUCCESS ;
}

int lcwrtwriterunheader( PTRTYPE writer, PTRTYPE header){
  try{

    LCWriter* wrt = reinterpret_cast<LCWriter*>(  writer ) ; 
    LCRunHeader* hdr = reinterpret_cast<LCRunHeader*>(  header ) ; 
    
    wrt->writeRunHeader( hdr ) ;

  }catch(...){ return LCIO::ERROR ; }
  
  return LCIO::SUCCESS ;
}
int   lcwrtwriteevent( PTRTYPE writer, PTRTYPE event){
  
  try{
    
    LCWriter* wrt = reinterpret_cast<LCWriter*>(  writer ) ; 
    LCEvent* evt = reinterpret_cast<LCEvent*>( event ) ; 
    
    wrt->writeEvent( evt ) ;
    
  }catch(...){ return LCIO::ERROR ; }
  
  return LCIO::SUCCESS ;
}
