
#include "CPPFORT/lceventc2f.h"

#include "lcio.h" 
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/LCTOOLS.h"
#include <iostream>

using namespace lcio ;


// LCEvent interface
#define LCREADER_PNTR( reader ) if(! (reader) ) return LCIO::ERROR ; \
LCReader* lcReader = reinterpret_cast<LCReader*>( (reader) ) ;

#define LCWRITER_PNTR( writer ) if(! (writer) ) return LCIO::ERROR ; \
LCWriter* lcWriter = reinterpret_cast<LCWriter*>( (writer) ) ;


#define LCEVENTIMPL_PNTR( event ) if(! (event) ) return LCIO::ERROR ; \
LCEventImpl* lcEventImpl = reinterpret_cast<LCEventImpl*>( (event) ) ;

#define LCCOLLECTION_PNTR( collection ) if(! (collection) ) return LCIO::ERROR ; \
LCCollectionVec* lcCollection = reinterpret_cast<LCCollectionVec*>( (collection) ) ;

#define LCEVENT_PNTR( event ) if(! (event) ) return LCIO::ERROR ; \
LCEvent* lcEvent = reinterpret_cast<LCEvent*>( (event) ) ;


PTRTYPE LCCreateEvent(){
  LCEventImpl*  event = new LCEventImpl() ;
  return reinterpret_cast<PTRTYPE>( event ) ;
}

PTRTYPE LCDeleteEvent( PTRTYPE event ){
  LCEVENT_PNTR( event ) ;
  delete lcEvent ;
  return LCIO::SUCCESS ;
}

int LCSetEventHeader( PTRTYPE event, const int irun, const int ievent, const int timestamp, const char* detname ){
  LCEVENTIMPL_PNTR( event ) ;
  lcEventImpl->setRunNumber( irun ) ;
  lcEventImpl->setEventNumber( ievent ) ;
  lcEventImpl->setTimeStamp( timestamp ) ;
  lcEventImpl->setDetectorName( detname ) ;
  return LCIO::SUCCESS ;
}

PTRTYPE LCAddCollection( PTRTYPE event, PTRTYPE collection, char* colname ){
  LCEVENT_PNTR( event ) ;
  LCCOLLECTION_PNTR( collection ) ;
  lcEvent->addCollection( (LCCollection*) lcCollection , colname ) ;
  return LCIO::SUCCESS ;
}

PTRTYPE LCWriteEvent( PTRTYPE writer, PTRTYPE evtout ){
  LCWRITER_PNTR( writer ) ;
  LCEVENTIMPL_PNTR( evtout ) ;
  lcWriter->writeEvent( lcEventImpl ) ;
  return LCIO::SUCCESS ;
}


PTRTYPE LCReadNextEvent( PTRTYPE reader ){
  LCREADER_PNTR( reader )  ;
  LCEvent* evt = lcReader->readNextEvent() ;
  return reinterpret_cast<PTRTYPE>( evt ) ;
}

int LCGetEventHeader( PTRTYPE event, int* irun, int* ievent, int* timestamp, void* detname ){
  LCEVENT_PNTR( event ) ;
  *irun = lcEvent->getRunNumber() ;
  *ievent = lcEvent->getEventNumber() ;
  *timestamp = lcEvent->getTimeStamp() ;
  int stringpos = reinterpret_cast<int>( detname ) ;
  char* detnam = reinterpret_cast<char*>( stringpos ) ;
  const char* detectorname = lcEvent->getDetectorName().c_str() ;
  strcpy(detnam,detectorname) ;
  return LCIO::SUCCESS ;
}

PTRTYPE LCGetCollection(PTRTYPE event, const char* colname){
  LCEVENT_PNTR( event ) ;
  // loop over all collections:
  const std::vector< std::string >* strVec = lcEvent->getCollectionNames() ;
  std::vector< std::string >::const_iterator name ;
  for( name = strVec->begin() ; name != strVec->end() ; name++)
  {
    if( *name == colname )
    {
       const LCCollection* lcCollection = lcEvent->getCollection( *name ) ;
       return reinterpret_cast<PTRTYPE>(lcCollection) ;
    }
  }
  return LCIO::ERROR ;
}


PTRTYPE LCDumpEvent( PTRTYPE event ){
  LCEVENT_PNTR( event ) ;
  LCTOOLS::dumpEvent(  lcEvent ) ;
  return LCIO::SUCCESS ;
}

PTRTYPE LCDumpEventDetailed ( PTRTYPE event ){
  LCEVENT_PNTR( event ) ;
  LCTOOLS::dumpEventDetailed(  lcEvent ) ;
  return LCIO::SUCCESS ;
}

