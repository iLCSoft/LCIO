
#include "CPPFORT/lcwriterc2f.h"

#include "lcio.h" 
#include "Exceptions.h"
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/LCTOOLS.h"
#include <iostream>

using namespace lcio ;


#define LCWRITER_PNTR( writer ) if(! (writer) ) return LCIO::ERROR ; \
LCWriter* lcWriter = reinterpret_cast<LCWriter*>( (writer) ) ;


// factory methods

PTRTYPE LCCreateWriter(){
  LCWriter* lcWriter = LCFactory::getInstance()->createLCWriter() ;
  return reinterpret_cast<PTRTYPE>(lcWriter) ;
}

PTRTYPE LCDeleteWriter(PTRTYPE writer){
  LCWRITER_PNTR( writer )  ;
  delete lcWriter ;
  return LCIO::SUCCESS ;
}


// LCWriter interface

int LCWriterOpen(PTRTYPE writer, const char* filename ){

  LCWRITER_PNTR( writer )  ;
  try{
    lcWriter->open( filename ) ;
    return LCIO::SUCCESS ;
  }catch(Exception& e){
    std::cerr << "Exception in LCWriterOpen: " << e.what() << std::endl ;
    return LCIO::ERROR ;
  }
}

int LCWriterClose(PTRTYPE writer){
  try{
    LCWRITER_PNTR( writer )  ;
    lcWriter->close() ;
    }catch(Exception& e){
      std::cerr << "Exception in LCWriterClose: " << e.what() << std::endl ;
      return LCIO::ERROR ;
    }
  return LCIO::SUCCESS ;
}

