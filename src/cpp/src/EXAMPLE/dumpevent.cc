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
  int nthEvent ;

  // read file name from command line (only argument) 
  if( argc < 3 ) {

    cout << " usage: dumpevent filename runNum evtNum " << endl ;
    cout << "    or: dumpevent filename n      " << endl ;
    cout << "  where the first dumps the event with the specified run and event number" << endl ;
    cout << "  and the second simply dumps the n-th event in the file" << endl ;

    exit(1) ;
  }
  
  FILEN = argv[1] ;

  bool dumpNthEvent( argc == 3 ) ;
 


  if( dumpNthEvent ) {

    nthEvent  = atoi( argv[2] ) ;

    if( nthEvent < 1 ) {

      cout << " usage: dumpevent filename n   -   whith  n > 0 !  " << endl ;
      
      exit(1) ;
    }

  }else{

    runNumber = atoi( argv[2] ) ;
    evtNumber = atoi( argv[3] ) ;
  }
  
  LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
  
  LCEvent* evt(0) ;

  try{
    
     lcReader->open( FILEN ) ;
     
     if( dumpNthEvent ) {
       
       lcReader->skipNEvents(  nthEvent - 1 ) ;

       evt = lcReader->readNextEvent() ; 
       
     }else{
       
       evt = lcReader->readEvent(runNumber,  evtNumber) ; 
     }
  
     
     //   } catch( EndOfDataException& e) {
     //     cout << " couldn't find event " << evtNumber << " - run " << runNumber 
     // 	 << " in file " << FILEN << endl ;    
     //     exit(1) ;
   
     if( !evt  ){

       if(dumpNthEvent){

	 cout << " less than " << nthEvent << "  events in  file " << FILEN << endl ;    
	 
       }else{

	 cout << " couldn't find event " << evtNumber << " - run " << runNumber 
	      << " in file " << FILEN << endl ;    
       } 
       
       exit(1) ;
     }

     LCTOOLS::dumpEventDetailed( evt ) ;
     
     
     lcReader->close() ;
     
   }
   catch( IOException& e) {
     cout << e.what() << endl ;
     exit(1) ;
   }

//    catch( Exception& e) {
//      cout << e.what() << endl ;
//      exit(1) ;
//    }
   return 0 ;
}

