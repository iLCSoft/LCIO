#include "lcio.h"

#include "IO/LCReader.h"
#include "IMPL/LCTOOLS.h"
#include "EVENT/LCRunHeader.h" 




using namespace std ;
using namespace lcio ;

/** dump the given event to screen
 */

int main(int argc, char** argv ){


  char* FILEN ;
  int runNumber ;
  int evtNumber ;

  // read file name from command line (only argument) 
  if( argc < 4 ) {
    cout << " usage: dumpevent filename runNum evtNum " << endl ;
    exit(1) ;
  }
  
  FILEN = argv[1] ;
  runNumber = atoi( argv[2] ) ;
  evtNumber = atoi( argv[3] ) ;
  
  
  LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
  try{
    
    lcReader->open( FILEN ) ;

  } catch( Exception& e) {
    cout << e.what() << endl ;
    exit(1) ;
  }
  LCEvent* evt = lcReader->readEvent(runNumber,  evtNumber) ; 
    
  if( evt == 0 ){
    cout << " couldn't fine event " << evtNumber << " - run " << runNumber 
	 << " in file " << FILEN << endl ;    
    exit(1) ;
  }

  LCTOOLS::dumpEvent( evt ) ;


  lcReader->close() ;
  
  return 0 ;
}

