#include "CPPFORT/lcioc2f.h"

#include "lcio.h" 
#include "Exceptions.h"
#include "IOIMPL/LCFactory.h"
#include <iostream>

using namespace lcio ;


// factory methods
PTRTYPE lfactCreateLCWriter(){
  LCWriter* lcWriter = LCFactory::getInstance()->createLCWriter() ;
  return reinterpret_cast<PTRTYPE>(lcWriter) ;
}
PTRTYPE lfactCreateLCReader(){
  LCReader* lcReader = IOIMPL::LCFactory::getInstance()->createLCReader() ;
  return reinterpret_cast<PTRTYPE>(lcReader) ;
}


// LCReader interface
#define LCREADER_PNTR( reader ) if(! (reader) ) return LCIO::ERROR ; LCReader* lcReader = reinterpret_cast<LCReader*>( (reader) ) ;

int lrdrOpen(PTRTYPE reader, const char* filename ){

  LCREADER_PNTR( reader )  ;
  try{
    lcReader->open( filename ) ; 
    return LCIO::SUCCESS ;
  }catch(IOException& e){
    std::cerr << "Exception in lrdrOpen: " << e.what() << std::endl ;
    return LCIO::ERROR ;
  }
}

PTRTYPE lrdrReadNextEvent(PTRTYPE reader){
  LCREADER_PNTR( reader )  ;
  LCEvent* evt = lcReader->readNextEvent() ;
  return reinterpret_cast<PTRTYPE>( evt ) ;
}
int lrdrClose(PTRTYPE reader){
  LCREADER_PNTR( reader )  ;
  return  lcReader->close() ;
}


// LCEvent interface
#define LCEVENT_PNTR( event ) if(! (event) ) return LCIO::ERROR ; LCEvent* lcEvent = reinterpret_cast<LCEvent*>( (event) ) ;

int levtGetRunNumber( PTRTYPE event ){
  LCEVENT_PNTR( event ) ;
  return lcEvent->getRunNumber() ;
}
int levtGetEventNumber( PTRTYPE event ){
  LCEVENT_PNTR( event ) ;
  return lcEvent->getEventNumber() ;

}
char* levtGetDetectorName( PTRTYPE event ){
  LCEVENT_PNTR( event ) ;
  return const_cast<char*> ( lcEvent->getDetectorName().c_str() ) ;
}
