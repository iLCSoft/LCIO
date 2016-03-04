
/***********************************************************
 load LCIO libraries before calling this macro in root:

   gSystem->Load("liblcio");  gSystem->Load("liblcioDict");

 ***********************************************************/

/** Example script for testing the ROOT LCIO dictionary.
 * 
 *  writeEventTree: 
 *  reads *.slcio file and creates a ROOT tree with one branch
 *  that hold the complete event 
 *  
 */

void writeEventTree(const char* FILEN) {
    
  //--- create a ROOT file, a tree and a branch ...
  
  TFile* file = new TFile( "lcioEventTree.root" , "RECREATE");    
  
  TTree* tree = new TTree( "LCIO" , "lcio event data tree");
  
  std::string treeName("LCEvent") ;
  
  
  IMPL::LCEventImpl* treeEvt=0 ;

  std::string type("IMPL::LCEventImpl") ;
  
  TBranch* mcpBranch = tree->Branch( treeName.c_str(), 
				     type.c_str(), 
				     (void*) &treeEvt, 
				     1024, // record size 
				     199    // split level 
				     );
  

  std::cout << " loaded LCIO library and dictionary ... " << std::endl ;
  
  int nEvents = 0  ;

  int maxEvt = 10000 ; 

  IO::LCReader* lcReader = IOIMPL::LCFactory::getInstance()->createLCReader() ;

  lcReader->open( FILEN ) ;
  

  //----------- the event loop -----------
  EVENT::LCEvent* evt = 0 ;
  while( (evt = lcReader->readNextEvent()) != 0  && nEvents < maxEvt ) {
    
    if( nEvents < 3 )  // only dump first 3 events
      UTIL::LCTOOLS::dumpEvent( evt ) ;

    nEvents ++ ;


    treeEvt = (IMPL::LCEventImpl*) evt ;
    

    tree->Fill() ;

  }
  // -------- end of event loop -----------

  file->Write() ;
  file->Close() ;
  
  delete file ;


  std::cout << std::endl 
	    <<  "  " <<  nEvents 
	    << " events read from file: " 
	    << FILEN << std::endl  ;
  
  
  lcReader->close() ;
  
  delete lcReader ;
}
