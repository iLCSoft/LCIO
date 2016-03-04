
/***********************************************************
 load LCIO libraries before calling this macro in root:

   gSystem->Load("liblcio");  gSystem->Load("liblcioDict");

 ***********************************************************/


/** Example script for testing the ROOT LCIO dictionary.
 * 
 *  readEventTree: 
 *  read LCEvents from tree with branch holding complete event  e.g.
 *  created with writeEventTree.C
 *  and fill a histogram from MCParticle::getEnergy()
 */

void readEventTree(const char* FILEN) {
  

  //======== open file and get tree and branch =====================
  
  TFile* f = new TFile( FILEN, "READ") ;
  if (!f) { return; }
  
  TTree *t; f->GetObject("LCIO",t);
  
  IMPL::LCEventImpl* evt=0 ;
  TBranch* bevt = t->GetBranch("LCEvent") ;
  
  if( bevt ){
    
    bevt->SetAddress( &evt  ) ; 
    
  } else {

    std::cout << " --- branch 'LCEvent' not found ;-( "  <<  std::endl ;
  }


  // define a simple histogram

  TH1F* h_mcpEner = new TH1F("h_mcpEner","MCParticles E(GeV) ", 100, 0.  , 100.  ) ;

 std::string mcpName("MCParticlesSkimmed") ;


  int nEvents = 0  ;
  
  int nevt = t->GetEntries();

  for (Int_t i = 0; i < nevt ; i++) {

    Long64_t tentry = t->LoadTree(i);

    int nbyte  = bevt->GetEntry(tentry);

    
    //========  LCIO event loop below this line ============================


    //    UTIL::LCTOOLS::dumpEventDetailed( evt ) ;
    UTIL::LCTOOLS::dumpEvent( evt ) ;


    EVENT::LCCollection* col =  evt->getCollection( mcpName  ) ;

    int nMcp = col->getNumberOfElements() ;
    for( int j = 0 ; j < nMcp ; ++j ) { 

      EVENT::MCParticle* mcp = (EVENT::MCParticle*) col->getElementAt(j) ;

      h_mcpEner->Fill( mcp->getEnergy() ) ; 

    }

    nEvents ++ ;

  }

  // -------- end of event loop -----------
  
  std::cout << std::endl 
	    <<  "  " <<  nEvents 
	    << " events read from file: " 
	    << FILEN << std::endl  ;
  
  


  //========   draw histogram(s)  ============================

  // -------------------------
  TCanvas *c1 = new TCanvas("c1","LCIO root example",200,10,700,500);
  //  c1->Divide(2,2);
  c1->cd(1)  ;
  h_mcpEner->Draw();
  
}
