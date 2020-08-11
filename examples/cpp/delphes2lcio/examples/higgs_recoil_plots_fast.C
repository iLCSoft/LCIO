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

#include "UTIL/EventSummary.h"

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

/** Example script for creating invariant mass plots from a ee -> HZ-> X mu mu sample using
 *  the EventSummaries collection in the last event to read only events of interest.
 *
 */
 
void higgs_recoil_plots_fast(const char* FILEN) {

 int nEvents = 0  ;
 int maxEvt = 100000 ;  // change as needed

//---- create some histograms ----

 TH1F* hmuonmass = new TH1F("hmuonmass","inv. mass - muons", 100,  60. , 120. ) ;
 TH1F* hjetmass  = new TH1F("hjetmass","inv. mass - jets", 100, 0. , 150. ) ;
 TH1F* hrecoilm  = new TH1F("hrecoilm","recoil mass", 100, 110. , 170. ) ;

 TH1F* hetotmcp = new TH1F("hetotmcp","total visible energy (truth/rec)", 100, 150. , 300. ) ;
 TH1F* hetotpfo = new TH1F("hetotpfo","Reco particle total energy", 100, 150. , 300. ) ;
//---------------------------------
 
 IO::LCReader* lcReader = IOIMPL::LCFactory::getInstance()->createLCReader() ;

 // read only these collections from the file (faster):
 lcReader->setReadCollectionNames( { "PFOs","Jets","IsolatedMuons" , "EventSummaries"} );

 lcReader->open( FILEN ) ;
  

 EVENT::LCEvent* evt ;
  
 //==================== read event summary  ============================================================
 int nEventTotal = lcReader->getNumberOfEvents() ;

 evt = lcReader->readEvent( -99, -99 ) ; // dummy evt with summaries

 if( ! evt){
   std::cout << " no event -99 run -99 with EventSummaries collection found !" ;
   exit(1) ;
 }

 // take the collection from the event and keep it for the job
 auto escol = evt->takeCollection("EventSummaries") ;
 
 if( nEventTotal != escol->getNumberOfElements() ){

   std::cout << "ERROR file contains  " << nEventTotal << " events "
	     << "EventSummaries collection : " << escol->getNumberOfElements() << std::endl;
   exit(1) ;
 }

 //==================== the event loop ============================================================

 for(int i=0 ; i< nEventTotal ; ++i){

   EventSummary es( escol->getElementAt(i)  ) ;

   bool myCut = ( es.getI( ESI::munum )== 2   &&
		  es.getI( ESI::jetnum ) == 2 ) ;


   // read only events fulfilling the pre-cut 
   if( myCut) {

     evt = lcReader->readEvent( es.getI( ESI::runnum ), es.getI( ESI::evtnum ) ) ;

     if( evt == nullptr )
       break ;

     ++nEvents ;
   

     //----------  the actual event processing
   
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


     hetotpfo->Fill( es.getF( ESF::epfotot ) ) ;
     hetotmcp->Fill( es.getF( ESF::emcptot ) ) ;
     
   }
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

 c1->cd(4) ;
 hetotmcp->Draw() ;
 hetotpfo->Draw("E;same") ;

  c1->Print("recoil_plots.pdf") ;
 //===================================================================================================

  std::cout << std::endl 
	    <<  "  " <<  nEvents 
	    << " events from a total of " << nEventTotal
	    << " read from file: " 
	    << FILEN << std::endl  ;
  
  
  lcReader->close() ;
  delete lcReader ;
}
