
#include "CPPFORT/lcrdrrep.h"

using namespace std ;
using namespace lcio ;

#include "lcrdrrep.icc"
int lcrdreventprocessor(const char* argv ){
  
    // create reader and writer for input and output streams 
    LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
    
    lcReader->open( argv ) ;

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

