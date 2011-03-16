#include "lcio.h"

#include "IO/LCReader.h"
//#include "IMPL/LCTOOLS.h"
//#include "EVENT/LCRunHeader.h" 

#include <cstdlib>

static std::vector<std::string> FILEN ; 

using namespace std ;
using namespace lcio ;

/** Small utility to count the number of events in lcio files
 */

int main(int argc, char** argv ){

  // read file names from command line (only argument) 
  if( argc < 2) {
    cout << " count the number of events in the given input files" << endl << endl;
    cout << " usage:  lcio_event_counter <input-file1> [[input-file2],...]" << endl ;
    exit(1) ;
  }

  for(int i=1 ; i < argc ; i++){
      FILEN.push_back( argv[i] )  ;
  }

  int nFiles = argc-1 ;
  
  int total_events = 0 ;

  LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
  
   for(int i=0 ; i < nFiles ; i++){

    lcReader->open( FILEN[i] ) ;

    total_events += lcReader->getNumberOfEvents() ;

    lcReader->close() ;
  }  

  cout <<  total_events << endl ;

  delete lcReader ;
  return 0 ;
}

  
