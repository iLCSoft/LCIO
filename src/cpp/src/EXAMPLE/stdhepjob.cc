
#include "lcio.h"

#include "IO/LCWriter.h"
#include "EVENT/LCIO.h"
#include "IMPL/LCEventImpl.h" 
#include "IMPL/LCRunHeaderImpl.h" 

#include "UTIL/LCStdHepRdr.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <memory>


// using namespace std ;
using namespace lcio ;


/** Simple test program to demonstrate reading of binary .stdhep generator files.
 *  Writes MCParticle collections to the outputfile stdhepjob.slcio.
 */

int main(int argc, char** argv ){
  
  if(argc < 4) {
    
     std::cout << " usage: stdhepjob infile.stdhep outfile.slcio maxEvt " <<  std::endl 
	       << "   infile.stdhep    - input file name "  <<  std::endl 
	       << "   outfile.slcio    - ouput file name "  <<  std::endl 
	       << "   maxEvt           - max number of events to read [-1: all]"  <<  std::endl  ;
     
    return 1;
  }
    
  std::string inFile  = argv[1] ;
  std::string outFile = argv[2] ;
  int maxEvt = std::atoi( argv[3] ) ; 


  std::cout << "==================================================== " << std::endl
	    << " stdhepjob : " << std::endl  ;

  // Open an instance of the StdHep Reader with the given filename
  LCStdHepRdr rdr( inFile.c_str()  ) ;
  
  std::cout << " opened file : " << inFile << std::endl ;
  
  rdr.printHeader() ;

  // create sio writer
  std::auto_ptr<LCWriter> lcWrt( LCFactory::getInstance()->createLCWriter() ) ;
  
  lcWrt->open( outFile )  ;
  
  std::auto_ptr<LCRunHeaderImpl> runHdr(  new LCRunHeaderImpl ) ; 
  
  runHdr->setRunNumber( 0 ) ;
  
  std::string detName("Unknown")  ;
  runHdr->setDetectorName( detName ) ;
      
  std::stringstream description ; 

  description << " file generated with LCIO stdhepjob from "  << inFile  ;

  runHdr->setDescription( description.str()  ) ;

  lcWrt->writeRunHeader( runHdr.get()  ) ;

  int count = 0; 

  try {

    while( maxEvt < 0  || count < maxEvt ){
	
      std::auto_ptr<LCEventImpl> evt( new LCEventImpl() ) ;
	
      evt->setRunNumber(  0   ) ;
      evt->setEventNumber( count ) ;
      evt->setDetectorName( detName ) ;
	
      // read the next stdhep event and add an MCParticle collection to the event
      rdr.updateNextEvent( evt.get()  , "MCParticle" ) ;
	
      lcWrt->writeEvent( evt.get()  ) ;
	
      ++count ;
	
    } // evt loop
  }
    
  catch( IO::EndOfDataException& e ) {  
  }
    
  std::cout << "  converted " << count << " events - written to LCIO " << outFile  << std::endl ;
    
  std::cout << "==================================================== " 
	    << std::endl << std::endl ;
    
  lcWrt->close() ;
  
  return 0 ;

}

