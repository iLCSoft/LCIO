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
   LCEvent* evt(0) ;
   try{
    
    lcReader->open( FILEN ) ;

    evt = lcReader->readEvent(runNumber,  evtNumber) ; 

  } catch( EndOfDataException& e) {
    cout << " couldn't fine event " << evtNumber << " - run " << runNumber 
	 << " in file " << FILEN << endl ;    
    exit(1) ;
  }
  catch( Exception& e) {
    cout << e.what() << endl ;
    exit(1) ;
  }
   
  LCTOOLS::dumpEventDetailed( evt ) ;


  lcReader->close() ;
  
  return 0 ;
}

