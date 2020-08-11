#ifndef __CINT__ 
#include "lcio.h"
#include "IO/LCReader.h"
#include "IOIMPL/LCFactory.h"
#include "EVENT/LCCollection.h"
#include "EVENT/MCParticle.h"
#include "EVENT/ReconstructedParticle.h"
#include "EVENT/LCEvent.h"
#include "UTIL/LCTOOLS.h"
#include "UTIL/LCIterator.h"
#endif

#include "TH1F.h"

/*
put this into your .rootlogon.C file

{
 gInterpreter->AddIncludePath("$LCIO");
 gSystem->Load("${LCIO}/lib/liblcio.so");
 gSystem->Load("${LCIO}/lib/liblcioDict.so");
}
*/


using namespace lcio ;

/** Example script for using the LCIO root dictionary.
 *  Fill some simple histograms.
 *
 */
 
void fill_histos_lcio(const char* FILEN) {



//---- create root output file ----
 std::string rootFileBaseName( FILEN , (strlen(FILEN) ) - strlen(".slcio") ) ;
 std::string rootFileName = rootFileBaseName + std::string(".root") ;
 TFile* file = new TFile( rootFileName.c_str()  , "RECREATE");    

 TH1F* henmcp = new TH1F("henmcp","MCParticle energy", 100, 0. , 30. ) ;
 TH1F* henpfo = new TH1F("henpfo","Reco particle energy", 100, 0. , 30. ) ;

 TH1F* hetotmcp = new TH1F("hetotmcp","MCParticle total energy", 100, 0. , 300. ) ;
 TH1F* hetotpfo = new TH1F("hetotpfo","Reco particle total energy", 100, 0. , 300. ) ;


//---------------------------------
 
 int nEvents = 0  ;
 int maxEvt = 10000 ;  // change as needed

 IO::LCReader* lcReader = IOIMPL::LCFactory::getInstance()->createLCReader() ;

 // read only these collections from the file (faster):
 lcReader->setReadCollectionNames( {"MCParticles", "PFOs", "RecoMCTruthLink" ,"MCTruthRecoLink" } ) ;
 lcReader->open( FILEN ) ;
  

 EVENT::LCEvent* evt ;
  
 //==================== the event loop ============================================================
 while( ( evt = lcReader->readNextEvent()) != 0  && nEvents++ < maxEvt ) {

   
   LCIterator<MCParticle> mcparticles( evt, "MCParticles" ) ;

   double emcp = 0., epfo=0. ;
   while( auto mcp = mcparticles.next()  ){

     if( mcp->getGeneratorStatus() == 1 ){ 
       henmcp->Fill(  mcp->getEnergy() ) ;

       int pdg = abs( mcp->getPDG() ) ;
       if( pdg != 12 && pdg != 14 && pdg != 16 )
	 emcp +=  mcp->getEnergy() ;
     }
   }
   hetotmcp->Fill( emcp ) ;





   LCIterator<ReconstructedParticle> recoparticles( evt, "PFOs" ) ;

   while( auto pfo = recoparticles.next()  ){
     henpfo->Fill(  pfo->getEnergy() ) ;
     epfo +=  pfo->getEnergy() ;
   }
   hetotpfo->Fill( epfo ) ;

 }

 //===================================================================================================
#if 0  // draw directly
 TCanvas* c1 = new TCanvas("LCIO histos");

 c1->Divide(1,2);
 c1->cd(1) ;
 henmcp->Draw() ;
 henpfo->Draw("E;same") ;

 c1->cd(2) ;
 hetotmcp->Draw() ;
 hetotpfo->Draw("E;same") ;

 //===================================================================================================

#else   // safe to file

 file->Write() ;
 file->Close() ;
 delete file ;

#endif

 std::cout << std::endl
	   <<  "  " <<  nEvents
	   << " events read from file: "
	   << FILEN << std::endl  ;


 lcReader->close() ;
 delete lcReader ;
}
