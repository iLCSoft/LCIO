#include "lcio.h"
#include <stdio.h>

#include "IO/LCReader.h"
#include "IMPL/LCTOOLS.h"
#include "EVENT/LCRunHeader.h" 

#include "EVENT/SimCalorimeterHit.h" 
#include "EVENT/CalorimeterHit.h" 
#include "EVENT/RawCalorimeterHit.h" 
// #include "EVENT/SimTrackerHit.h" 

#include "UTIL/CellIDDecoder.h"

#include <cstdlib>

using namespace std ;
using namespace lcio ;

/** dump the given event to screen
 */

int main(int argc, char** argv ){


  char* FILEN ;
  int runNumber=0 ;
  int evtNumber=0 ;
  int nthEvent=1 ;

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
  

  // set the default encoding for cellid's according to the old Mokka convention
  CellIDDecoder<SimCalorimeterHit>::setDefaultEncoding("M:3,S-1:3,I:9,J:9,K-1:6") ;
  CellIDDecoder<CalorimeterHit>::setDefaultEncoding("M:3,S-1:3,I:9,J:9,K-1:6") ;
  CellIDDecoder<RawCalorimeterHit>::setDefaultEncoding("M:3,S-1:3,I:9,J:9,K-1:6") ;

  LCReader* lcReader ;
  if( dumpNthEvent ) 
    lcReader = LCFactory::getInstance()->createLCReader() ;
  else
    lcReader = LCFactory::getInstance()->createLCReader(LCReader::directAccess) ;
  
  LCEvent* evt(0) ;

  try{
    
     lcReader->open( FILEN ) ;
     
     if( dumpNthEvent ) {
       
       if( nthEvent > 1 )
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

