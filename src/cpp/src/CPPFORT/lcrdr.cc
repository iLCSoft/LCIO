#include "CPPFORT/lcrdr.h"

#include "lcio.h" 
#include "Exceptions.h"
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/LCTOOLS.h"
#include <iostream>

using namespace lcio ;


PTRTYPE lcrdrcreate(){
  LCReader* lcReader = IOIMPL::LCFactory::getInstance()->createLCReader() ;
  return reinterpret_cast<PTRTYPE>(lcReader) ;
}

int lcrdrdelete(PTRTYPE reader){
  LCReader* lcReader = reinterpret_cast<LCReader*>( (reader) ) ;
  delete lcReader ;
  return LCIO::SUCCESS ;
}

int lcrdropen(PTRTYPE reader, const char* filename ){
  try{
    LCReader* lcReader = reinterpret_cast<LCReader*>(reader) ;
    lcReader->open( filename ) ; 
  }catch(...){ return LCIO::ERROR ; }
  return LCIO::SUCCESS ;
}

int lcrdrclose(PTRTYPE reader){
  try{
    LCReader* lcReader = reinterpret_cast<LCReader*>(reader) ;
    lcReader->close() ;
  }catch(...){ return LCIO::ERROR ; }
  return LCIO::SUCCESS ;
}

int lcrdrreadstream(PTRTYPE reader, int maxRecord){
  try{
    LCReader* lcReader = reinterpret_cast<LCReader*>(reader) ;
    lcReader->readStream( maxRecord ) ;
  }catch(...){ return LCIO::ERROR ; }
  return LCIO::SUCCESS ;
}

PTRTYPE lcrdrreadnextrunheader(PTRTYPE reader, int accessMode){
  LCReader* rdr = reinterpret_cast<LCReader*>(reader) ;
  return C2F_POINTER( LCRunHeader*,  rdr->readNextRunHeader( accessMode ) ) ;
}

PTRTYPE lcrdrreadnextevent(PTRTYPE reader, int accessMode){
  LCReader* rdr = reinterpret_cast<LCReader*>(reader) ;
  return C2F_POINTER( LCEvent*,  rdr->readNextEvent( accessMode ) );
}

PTRTYPE lcrdrreadevent(PTRTYPE reader, int runNumber, int evtNumber ){
  LCReader* rdr = reinterpret_cast<LCReader*>(reader) ;
  return C2F_POINTER( LCEvent*,  rdr->readEvent( runNumber, evtNumber ) );
}

int  lcrdrskipnevents( PTRTYPE reader, int n ) {
  LCReader* rdr = reinterpret_cast<LCReader*>(reader) ;
  rdr->skipNEvents( n ) ;
  return LCIO::SUCCESS ;
}
