#include "lcio.h"

#include "IO/LCReader.h"
#include "UTIL/LCTOOLS.h"
#include "EVENT/LCRunHeader.h" 
#include "EVENT/LCCollection.h" 

#include "CalibrationConstant.h"

static const char* FILEN ;

//using namespace std ;
using namespace lcio ;

/** Example program that reads an LCIO file and print all calibration constants
 */

int main(int argc, char** argv ){

  // read file names from command line (only argument) 
  if( argc != 2) {
    std::cout << " usage:  readcalibration input-file1" << std::endl ;
    exit(1) ;
  }

  FILEN = argv[1] ;

  LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
  
  lcReader->open( FILEN ) ;
  
  LCEvent* evt ;
  int nEvents = 0 ;
  
  //----------- the event loop -----------
  while( (evt = lcReader->readNextEvent()) != 0 ) {

    // loop over collections  and check if we have objects of user defined 
    // type "CalibrationConstant" 
    // NB: this is only needed if you don't know the name of the collection 
    // that holds the calibration constants

    const StringVec* colNames = evt->getCollectionNames() ;

    for(unsigned int i=0 ; i < colNames->size() ; i++ ){

      LCCollection* col = evt->getCollection( (*colNames)[i] ) ;

      if( col->getParameters().getStringVal("TypeName")  ==  "CalibrationConstant"  ) {


	// now print the calibration constants
	for(int j=0;j<col->getNumberOfElements();j++){

	  CalibrationConstant cal( col->getElementAt( j )  ) ;

	  std::cout << " calibration for cellid: " << cal.getCellID() 
		    << " offset: "                 << cal.getOffset() 
		    << " gain: "                   << cal.getGain() 
		    << std::endl ;

	}

      }
    }
    
    nEvents ++ ;
  } 
  // -------- end of event loop -----------
  
  std::cout << std::endl <<  "  " <<  nEvents << " events read from file: " 
	    << FILEN << std::endl ; 

  
  lcReader->close() ;
  delete lcReader ;
  return 0 ;
}

  
