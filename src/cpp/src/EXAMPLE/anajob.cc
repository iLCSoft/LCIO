#include "lcio.h"

#include "IO/LCReader.h"
#include "IMPL/LCTOOLS.h"
#include "EVENT/LCRunHeader.h" 

static const char* FILEN = "recjob.slcio" ; // default file name 

using namespace std ;
using namespace lcio ;

/** Example for an analysis job. No concrete implementations are needed
 *  for reading the data - just the EVENT interfaces. 
 *  In a first loop we read the run information and then reopen the file for
 *  event loop.
 *  See LCTOOLS::dumpEvent(const LCEvent* evt) ) for details on how to access 
 *  the data in the LCEvent.  
 */

int main(int argc, char** argv ){

  // read file name from command line (only argument) 
  if( argc > 1 ) {
    FILEN = argv[1] ;
  }
  
  LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;

  try{
    
    lcReader->open( FILEN ) ;
    
    cout << " opened " << FILEN << " for reading " << endl ; 
    
    LCRunHeader *runHdr ;
    
    // loop over all run headers
    while( ( runHdr = lcReader->readNextRunHeader() ) != 0 ){
      
      cout << "  Run : " << runHdr->getRunNumber() 
	   << " - "      << runHdr->getDetectorName() 
	   << ":  "      << runHdr->getDescription()  << endl ;
    }
    
    cout << endl ;
    
    lcReader->close() ;
  }
  catch(IOException& e){
    cout << " io error : " << e.what() << endl ;
  }
  
  // now loop over the file again and dump event data
  try {
    
    lcReader->open( FILEN ) ;
    
    
    cout << " reopened " << FILEN << " for reading " << endl ; 
    
    LCEvent* evt ;
    
    // dump all events in the file
    int nEvents = 0 ;
    
    //----------- the event loop -----------
    while( (evt = lcReader->readNextEvent()) != 0 ) {
      
      LCTOOLS::dumpEvent( evt ) ;
      
      nEvents ++ ;
      
    } 
    // -------- end of event loop -----------
    
    cout << endl <<  "  " <<  nEvents << " events read from file : " << FILEN << endl << endl ;
    
    
    lcReader->close() ;
    
  }  
  catch(IOException& e){
    cout << " io error : " << e.what() << endl ;
  }
  catch(Exception& e){
    cout << " exception caught : " << e.what() << endl ;
  }
  
  
  return 0 ;
}
  
  
