
#include "CPPFORT/lcrdrrep.h"

using namespace std ;
using namespace lcio ;

#include "lcrdrrep.icc"
int lcrdreventprocessor( PTRTYPE filenamevec ){
  
    StringVec* filenameVec = reinterpret_cast<StringVec*>(filenamevec);

    // create reader and writer for input and output streams 
    LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
    
    lcReader->open( *filenameVec ) ;

    {
      RunEventProcessor evtProc ;
      
      lcReader->registerLCRunListener( &evtProc ) ; 
      lcReader->registerLCEventListener( &evtProc ) ; 
      
      lcReader->readStream() ;

    } 
    
    lcReader->close() ;
    delete lcReader ;
    return 0 ;
}

