
#include "CPPFORT/lcrunheaderc2f.h"

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

#define LCWRITER_PNTR( writer ) if(! (writer) ) return LCIO::ERROR ; \
LCWriter* lcWriter = reinterpret_cast<LCWriter*>( (writer) ) ;

PTRTYPE LCWriteRunHeader( PTRTYPE writer , const int irun , const char* detname , const char* description ,
                          void* sdnamevec , const int nsubd , const int nchsd){
  try {
    LCWRITER_PNTR( writer ) ;
    LCRunHeaderImpl* runHdr =  new LCRunHeaderImpl ;
    runHdr->setRunNumber( irun ) ;
    runHdr->setDetectorName( detname ) ;
    runHdr->setDescription( description ) ;

    int elemlen = nchsd + 1 ;
    int stringpos = reinterpret_cast<int>( sdnamevec ) ;
    for (int j=0;j < nsubd;j++)
    {
        char* subdetectorname = reinterpret_cast<char*>( stringpos ) ;
        runHdr->addActiveSubdetector( subdetectorname ) ;
        stringpos = stringpos + elemlen ;
    }
    lcWriter->writeRunHeader( runHdr ) ;
    delete runHdr ;
    return LCIO::SUCCESS ;
  }catch( Exception& e1) {
    std::cerr << "Exception in LCWriteRunHeader: " << e1.what() << std::endl ;
    return LCIO::ERROR ;
  }
}

PTRTYPE LCReadNextRunHeader(PTRTYPE reader , int* irun , void* detname , void* description ,
                            void* sdnamevec , int* nsubd , const int nchsubd){
  LCREADER_PNTR( reader ) ;
  try {
    LCRunHeader* runHdr = lcReader->readNextRunHeader() ;
    *irun = runHdr->getRunNumber() ;

    int stringpos ;
    stringpos = reinterpret_cast<int>( detname ) ;
    char* detname = reinterpret_cast<char*>( stringpos ) ;
    const char* detectorname = runHdr->getDetectorName().c_str() ;
    strcpy(detname,detectorname) ;
    stringpos = reinterpret_cast<int>( description ) ;
    char* descr = reinterpret_cast<char*>( stringpos ) ;
    const char* rundescription = runHdr->getDescription().c_str() ;
    strcpy(descr,rundescription) ;

    const std::vector<std::string>* strVec = runHdr->getActiveSubdetectors() ;

    int elemlen = nchsubd + 1;
    *nsubd  = strVec->size() ;
    stringpos = reinterpret_cast<int>( sdnamevec ) ;
    for( std::vector<std::string>::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
      std::string out = *name ;
      char* tmpname = reinterpret_cast<char*>( stringpos ) ;
      const char* subdname = const_cast<char*>( out.c_str() ) ;
      strcpy(tmpname,subdname) ;
      stringpos = stringpos + elemlen ;
    }

    return reinterpret_cast<PTRTYPE>(runHdr) ;
  }catch( Exception& e1) {
    std::cerr << "Exception in LCReadNextRunHeader: " << e1.what() << std::endl ;
    return LCIO::ERROR ;
  }
}



