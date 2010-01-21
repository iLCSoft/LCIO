#ifndef __CINT__ 
#include "IO/LCReader.h"
#include "IOIMPL/LCFactory.h"
#include "EVENT/MCParticle.h"
#include "EVENT/LCCollection.h"
#include "EVENT/LCEvent.h"
#include "UTIL/LCTOOLS.h"
#endif


/** Example script for testing the ROOT LCIO dictionary.
 * 
 *  anajob: reads *.slcio file and dumps events to the screen 
 *  in a ROOT macro.
 *
 */

void anajob(const char* FILEN) {
  
  
  //just in case this script is executed multiple times
  delete gROOT->GetListOfFiles()->FindObject( FILEN );
  delete gROOT->GetListOfCanvases()->FindObject("c1");
  

  if (!TClassTable::GetDict("IMPL::ReconstructedParticleImpl")) {
    unsigned res ;
    
    res = gSystem->Load("${LCIO}/lib/liblcio.so"); 
    res = gSystem->Load("${LCIO}/lib/liblcioDict.so"); 
  }

  std::cout << " loaded LCIO library and dictionary ... " << std::endl ;


  int nEvents = 0  ;
  IO::LCReader* lcReader = IOIMPL::LCFactory::getInstance()->createLCReader() ;
  lcReader->open( FILEN ) ;


  //----------- the event loop -----------
  while( (evt = lcReader->readNextEvent()) != 0 ) {

    UTIL::LCTOOLS::dumpEvent( evt ) ;
    nEvents ++ ;
  }
  // -------- end of event loop -----------

  std::cout << std::endl 
	    <<  "  " <<  nEvents 
	    << " events read from file: " 
	    << FILEN << std::endl  ;
  

  lcReader->close() ;

  delete lcReader ;
}
