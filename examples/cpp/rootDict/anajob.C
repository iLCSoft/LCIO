
// R__LOAD_LIBRARY("liblcio" )
// R__LOAD_LIBRARY("liblcioDict" )
// namespace {
//   int loadMyLibrariesTriggerFunc() {
//     gSystem->Load("liblcio");
//     gSystem->Load("liblcioDict");
//     return 0 ;
//   }
//   static int loadMyLibraryTrigger = loadMyLibrariesTriggerFunc();
// }

/************************************************************
fg: none of the above seem to work on MacOS w/ ROOT 6.04 
  - so one needs to call 

 gSystem->Load("liblcio");  gSystem->Load("liblcioDict");

at the root prompt before calling

.x ./anajob("myFile.slcio")
************************************************************/


/** Example script for testing the ROOT LCIO dictionary.
 * 
 *  anajob: reads *.slcio file and dumps events to the screen 
 *  in a ROOT macro.
 *
 */

void anajob(const char* FILEN) {
  
  
  int nEvents = 0  ;
  IO::LCReader* lcReader = IOIMPL::LCFactory::getInstance()->createLCReader() ;
  lcReader->open( FILEN ) ;

  EVENT::LCEvent* evt = 0 ;

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
