#include "lcio.h"

#include "IO/LCReader.h"
#include "IMPL/LCTOOLS.h"
#include "EVENT/LCRunHeader.h" 

#include <cstdlib>

//static const char* FILEN = "recjob.slcio" ; // default file name 
static std::vector<std::string> FILEN ; 

using namespace std ;
using namespace lcio ;

/** Example for an analysis job. No concrete implementations are needed
 *  for reading the data - just the EVENT interfaces. 
 *  In a first loop we read the run information and then reopen the file(s) for
 *  event loop.
 *  See LCTOOLS::dumpEvent(const LCEvent* evt) ) for details on how to access 
 *  the data in the LCEvent.  
 */

int main(int argc, char** argv ){

  // read file names from command line (only argument) 
  if( argc < 2) {
    cout << " usage:  anajob <input-file1> [[input-file2],...]" << endl ;
    exit(1) ;
  }
  for(int i=1 ; i < argc ; i++){
      FILEN.push_back( argv[i] )  ;
  }
  int nFiles = argc-1 ;
  
  LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
  
  // first we read the run information
  
  
  // for reading from one file only use sth. like:
  //  const char* FILEN = "recjob.slcio" ;
  lcReader->open( FILEN ) ;
  

  cout << " will open and read from files: " << endl ;  
  for(int i=0 ; i < nFiles ; i++){
    cout  << "     "  << FILEN[i] << endl ; 
  }  

  LCRunHeader *runHdr ;
  
  // use a try catch block here: if sth. went wrong with reading the run data we 
  // still can try and read the event data - see below
  try{  

    // loop over all run headers
    while( ( runHdr = lcReader->readNextRunHeader() ) != 0 ){
      
      LCTOOLS::dumpRunHeader( runHdr ) ;
//       cout << "  Run : " << runHdr->getRunNumber() 
// 	   << " - "      << runHdr->getDetectorName() 
// 	   << ":  "      << runHdr->getDescription()  << endl ;
    }
    
  }catch(IOException& e){
    cout << " io error when reading run data : " << e.what() << endl ;
  }
  cout << endl ;
  
  lcReader->close() ;
  
  
  // now loop over the file again and dump event data

  lcReader->open( FILEN ) ;

  //  cout << " reopened " << FILEN << " for reading " << endl ; 
  cout << " will reopen and read from files: " << endl ;  
  for(int i=0 ; i < nFiles ; i++){
    cout  << "     "  << FILEN[i] << endl ; 
  }  
  
  LCEvent* evt ;
  int nEvents = 0 ;
  
  //----------- the event loop -----------
  while( (evt = lcReader->readNextEvent()) != 0 ) {
    
    LCTOOLS::dumpEvent( evt ) ;
    nEvents ++ ;
  } 
  // -------- end of event loop -----------
  
  cout << endl <<  "  " <<  nEvents << " events read from files: " << endl  ;
  for(int i=0 ; i < nFiles ; i++){
    cout  << "     "  << FILEN[i] << endl ; 
  }  

  
  lcReader->close() ;
  delete lcReader ;
  return 0 ;
}

  
