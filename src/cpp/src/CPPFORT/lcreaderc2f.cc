
#include "CPPFORT/lcreaderc2f.h"

#include "lcio.h" 
#include "Exceptions.h"
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/LCTOOLS.h"
#include <iostream>

using namespace lcio ;


#define LCREADER_PNTR( reader ) if(! (reader) ) return LCIO::ERROR ; \
LCReader* lcReader = reinterpret_cast<LCReader*>( (reader) ) ;


// factory methods

PTRTYPE LCCreateReader(){
  LCReader* lcReader = IOIMPL::LCFactory::getInstance()->createLCReader() ;
  return reinterpret_cast<PTRTYPE>(lcReader) ;
}

PTRTYPE LCDeleteReader(PTRTYPE reader){
  LCREADER_PNTR( reader ) ;
  delete lcReader ;
  return LCIO::SUCCESS ;
}


// LCReader interface

int LCReaderOpen(PTRTYPE reader, const char* filename ){

  LCREADER_PNTR( reader ) ;
  try{
    lcReader->open( filename ) ; 
    return LCIO::SUCCESS ;
  }catch(Exception& e){
    std::cerr << "Exception in LCReaderOpen: " << e.what() << std::endl ;
    return LCIO::ERROR ;
  }
}

int LCReaderClose(PTRTYPE reader){
  try{
    LCREADER_PNTR( reader )  ;
    lcReader->close() ;
    }catch(Exception& e){
       std::cerr << "Exception in LCReaderClose: " << e.what() << std::endl ;
       return LCIO::ERROR ;
    }
   return LCIO::SUCCESS ;
}

