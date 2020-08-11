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
#include "TLorentzVector.h"

/*
put this into your .rootlogon.C file

{
 gInterpreter->AddIncludePath("$LCIO");
 gSystem->Load("${LCIO}/lib/liblcio.so");
 gSystem->Load("${LCIO}/lib/liblcioDict.so");
}

for the LCIO API documentation see:
http://lcio.desy.de/v02-09/doc/doxygen_api/html/index.html
*/


using namespace lcio ;



template<class T>
double inv_mass(T* p1, T* p2){
  double e = p1->getEnergy()+p2->getEnergy() ;
  double px = p1->getMomentum()[0]+p2->getMomentum()[0];
  double py = p1->getMomentum()[1]+p2->getMomentum()[1];
  double pz = p1->getMomentum()[2]+p2->getMomentum()[2];
  return( sqrt( e*e - px*px - py*py - pz*pz  ) );
}

template<class T>
TLorentzVector v4(T* p){
  return TLorentzVector( p->getMomentum()[0],p->getMomentum()[1], p->getMomentum()[2],p->getEnergy());
}

/** Example script for creating invariant mass plots from a ee -> HZ-> X mu mu sample
 *
 */
 
void higgs_recoil_plots(const char* FILEN) {

 int nEvents = 0  ;
 int maxEvt = 100000 ;  // change as needed

//---- create some histograms ----

 TH1F* hmuonmass = new TH1F("hmuonmass","inv. mass - muons", 100,  60. , 120. ) ;
 TH1F* hjetmass  = new TH1F("hjetmass","inv. mass - jets", 100, 0. , 150. ) ;
 TH1F* hrecoilm  = new TH1F("hrecoilm","recoil mass", 100, 110. , 170. ) ;

//---------------------------------
 
 IO::LCReader* lcReader = IOIMPL::LCFactory::getInstance()->createLCReader() ;
 lcReader->setReadCollectionNames( { "PFOs","Jets","IsolatedMuons" } );
 //lcReader->setReadCollectionNames( {"MCParticle", "PFOs", "RecoMCTruthLink" ,"MCTruthRecoLink","Jets","IsolatedMuons" } ) ;
 lcReader->open( FILEN ) ;
  

 EVENT::LCEvent* evt ;
  
 //==================== the event loop ============================================================
 while( ( evt = lcReader->readNextEvent()) != 0  && nEvents++ < maxEvt ) {
   
   
   LCIterator<ReconstructedParticle> jets( evt, "Jets" ) ;
   LCIterator<ReconstructedParticle> muons( evt, "IsolatedMuons" ) ;

   if( jets.size() != 2)
     continue;

   if( muons.size() != 2)
     continue;

   auto mu1 = muons.next(); 
   auto mu2 = muons.next(); 
   hmuonmass->Fill( inv_mass( mu1, mu2) ) ;

   auto j1 = jets.next(); 
   auto j2 = jets.next(); 
   hjetmass->Fill( inv_mass( j1, j2) ) ;
   

   // the recoil mass
   const auto& vm1 = v4(mu1) ;
   const auto& vm2 = v4(mu2) ;
   TLorentzVector ecms(0.,0.,0.,250.) ;
   TLorentzVector recoil = ecms - ( vm1 + vm2 ) ;
   hrecoilm->Fill( recoil.M() ) ;
 }

 //===================================================================================================
 TCanvas* c1 = new TCanvas("recoil plots");

 c1->Divide(2,2);
 c1->cd(1) ;
 hmuonmass->Draw() ;
 c1->cd(2) ;
 hjetmass->Draw();
 c1->cd(3) ;
 hrecoilm->Draw();

  c1->Print("recoil_plots.pdf") ;
 //===================================================================================================

  std::cout << std::endl 
	    <<  "  " <<  nEvents 
	    << " events read from file: " 
	    << FILEN << std::endl  ;
  
  
  lcReader->close() ;
  delete lcReader ;
}
