/***********************************************************
 load LCIO libraries before calling this macro in root:

   gSystem->Load("liblcio");  gSystem->Load("liblcioDict");

 ***********************************************************/

/** Example script for testing the ROOT LCIO dictionary.
 * 
 *  treeDraw: 
 *  use the TTree::Draw to create some plots from the 
 *  MCParticle tree created with writeTree.C  
 *
 */

void treeDraw(const char* FILEN){



TFile* f = new TFile( FILEN ,"READ") ;

TTree *t; f->GetObject("LCIO",t);


// -------------------------
TCanvas *c1 = new TCanvas("c1","LCIO root example",200,10,700,500);
c1->Divide(2,2);
c1->cd(1)  ;




// define some cuts
TCut sizeCut("abs(MCParticlesSkimmed._endpoint[][0])<1000.&&abs(MCParticlesSkimmed._endpoint[][1])<1000." ) ;

TCut isPhoton("MCParticlesSkimmed.getPDG()==22" ) ;
TCut isElectron("abs(MCParticlesSkimmed.getPDG())==11" ) ;


// simple drawing of scalar variable (using API !) 
//t->Draw("MCParticlesSkimmed.getPDG()") ;

t->Draw("MCParticlesSkimmed.getPDG()", isPhoton || isElectron ) ;



c1->cd(2)  ;
t->Draw("MCParticlesSkimmed._endpoint[][0]:MCParticlesSkimmed._endpoint[][1]", sizeCut && isPhoton ) ;

c1->cd(3)  ;
t->Draw("MCParticlesSkimmed._endpoint[][0]:MCParticlesSkimmed._endpoint[][1]", sizeCut && isElectron ) ;


TH2F h_trkStop("h_trkStop","particles stopped vs in/out of tracker region" , 2 , -.5 , 1.5 , 2 , -.5 , 1.5 ) ; 

c1->cd(4)  ;
t->Draw("MCParticlesSkimmed.isDecayedInTracker():MCParticlesSkimmed.isStopped()>>h_trkStop" ) ;

}
