
/** Example script for testing the ROOT LCIO dictionary.
 * 
 *  treeDraw: 
 *  use the TTree::Draw to create some plots from the 
 *  MCParticle tree created with writeTree.C  
 *
 */

{


if (!TClassTable::GetDict("IMPL::ReconstructedParticleImpl")) {
  unsigned res ;
  
  res = gSystem->Load("$LCIO/lib/liblcio.so"); 
  res = gSystem->Load("$LCIO/lib/liblcioDict.so"); 
}


TFile* f = new TFile("lcioTree.root","READ") ;


// -------------------------
TCanvas *c1 = new TCanvas("c1","LCIO root example",200,10,700,500);
c1->Divide(2,2);
c1->cd(1)  ;




// define some cuts
TCut sizeCut("abs(MCParticlesSkimmed._endpoint[][0])<1000.&&abs(MCParticlesSkimmed._endpoint[][1])<1000." ) ;

TCut isPhoton("MCParticlesSkimmed.getPDG()==22" ) ;
TCut isElectron("abs(MCParticlesSkimmed.getPDG())==11" ) ;


// simple drawing of scalar variable (using API !) 
//LCIO->Draw("MCParticlesSkimmed.getPDG()") ;

LCIO->Draw("MCParticlesSkimmed.getPDG()", isPhoton || isElectron ) ;



c1->cd(2)  ;
LCIO->Draw("MCParticlesSkimmed._endpoint[][0]:MCParticlesSkimmed._endpoint[][1]", sizeCut && isPhoton ) ;

c1->cd(3)  ;
LCIO->Draw("MCParticlesSkimmed._endpoint[][0]:MCParticlesSkimmed._endpoint[][1]", sizeCut && isElectron ) ;


// this syntax does not work !!!!
// 
//  in order to access array like data,
//  you need to use the member variable names as shown above 
//
//LCIO->Draw("MCParticlesSkimmed.getEndpoint()[][0]:MCParticlesSkimmed.getEndpoint()[][1]", sizeCut) ;

//LCIO->Draw("MCParticlesSkimmed.getEndpoint()[0]:MCParticlesSkimmed.getEndpoint()[1]" ) ;



TH2F h_trkStop("h_trkStop","particles stopped vs in/out of tracker region" , 2 , -.5 , 1.5 , 2 , -.5 , 1.5 ) ; 

c1->cd(4)  ;
LCIO->Draw("MCParticlesSkimmed.isDecayedInTracker():MCParticlesSkimmed.isStopped()>>h_trkStop" ) ;

}
