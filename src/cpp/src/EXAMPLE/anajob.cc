#include "lcio.h"

#include "IO/LCReader.h"
#include "IMPL/LCTOOLS.h"
#include "EVENT/LCRunHeader.h" 
//#include "CPPFORT/lcioc2f.h"

static const char* FILEN = "recjob.slcio" ; // default file name 


using namespace std ;
using namespace lcio ;

/** Example for an analysis job. No concrete implementations are needed
 *  for accessing the data - just the 'read only' interface. 
 *  See dumpEvent(const LCEvent* evt) in lcio_tools.cc) for details.  
 */

int main(int argc, char** argv ){

  // read file name from command line (only argument) 
  if( argc > 1 ) {
    FILEN = argv[1] ;
  }


  LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
  // test the wrapper function
  //LCReader*  lcReader = reinterpret_cast<LCReader*>( lfactCreateLCReader() ) ;
 
  // read all run headers first :
  //  if( lcReader->open( FILEN )  != LCIO::SUCCESS ) {
  //  cout << " can't open file: " << FILEN     << endl ;
  //  exit(1) ;
  //} 
  try{

    lcReader->open( FILEN ) ;

  } catch( exception& e) {
    cout << e.what() << endl ;
    exit(1) ;
  }
  cout << " opened " << FILEN << " for reading " << endl ; 
  
  LCRunHeader *runHdr ;
  while( (runHdr = lcReader->readNextRunHeader()) != 0 ){ 
    cout << "  Run : " << runHdr->getRunNumber() 
     	 << " - "  <<  runHdr->getDetectorName() 
	 << ":  "  << runHdr->getDescription()  << endl ;
  }
  cout << endl ;
  // close and reopen the file
  lcReader->close() ;

//   if( lcReader->open( FILEN )  != LCIO::SUCCESS ) {
//     cout << " can't open file: " << FILEN     << endl ;
//     exit(1) ;
//   } 

  try{   lcReader->open( FILEN ) ; } 

//   catch( IOException& ioe) { 
//     cout << ioe.what() << endl ; 
//     exit(1) ;
//   }
//   catch( Exception& e) { 
//     cout << e.what() << endl ; 
//     exit(1) ;
//   }
  catch( exception& e1) { 
    cout << e1.what() << endl ; 
    exit(1) ;
  }


  cout << " reopened " << FILEN << " for reading " << endl ; 

  LCEvent* evt ;
  
  // dump all events in the file
  int nEvents = 0 ;
  while( (evt = lcReader->readNextEvent()) != 0 ){ 
    
    // the following code will fail at runtime - event is read only !
    // if we use " (const LCEvent*) evt " it won't even compile 
    // so we have a twofold protection against data corruption
    // int status = evt->addCollection( new LCCollectionVec(LCIO::CALORIMETERHIT ),"NewCol" ) ; 
    //  if( status != LCIO::SUCCESS ){
    //    cout << " error - couldn't add new collection to the event ! " << endl ;
    //    exit(1) ;
    //  }

    LCTOOLS::dumpEvent( evt ) ;

    nEvents ++ ;
  }
  
  cout << endl <<  "  " <<  nEvents << " events read from file : " << FILEN << endl << endl ;


  lcReader->close() ;
  
  return 0 ;
}

