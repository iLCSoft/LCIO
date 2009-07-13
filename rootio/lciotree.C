#include "TTree.h"
#include "TFile.h"
#include "TBranch.h"
#ifndef __CINT__ 
#include "include/IMPL/ReconstructedParticleImpl.h"
#include "include/IMPL/TrackImpl.h"
#endif

  //---------  loop over particles in the events  ------------
  

void lciotree() {


  if (!TClassTable::GetDict("IMPL::ReconstructedParticleImpl")) {
    unsigned res ;
    
    res = gSystem->Load("./lib/liblcio.so"); 
    res = gSystem->Load("./lib/liblcioDict.so"); 
    
    //  if (res != 0 ) {
    //       std::cout << " can't load ./liblcioDict.so  !  err = " <<res<<std::endl;
    //   } 
  }
    
  
  
  TFile *f = TFile::Open("dst.root","READ");
   
  if (!f) { return; }
  
  TTree *t; f->GetObject("lciotree",t);

  //  t->Draw("PandoraPFOs.getEnergy()" ) ;
  //   t->Draw("PandoraPFOs.getTracks().size()" ) ;
  

  // ---- create a few histograms ---------

  TH1F *htot = new TH1F("hetot","total energy from PFOs",100,0.,1000.);
  htot->SetFillColor(48);
  
  TH1F *htphi = new TH1F("htphi","phi of tracks ",48,-1.6,+3.2);
  htphi->SetFillColor(32);
  
  TH1F *htphipo = new TH1F("htphipo","phi of tracks - PFOs",48,-1.6,+3.2);
  htphipo->SetFillColor(30);
  
  TH1F *hntrk = new TH1F("hntrk","number of tracks PFOs",11,0.,10.);
  hntrk->SetFillColor(41);
  
  
  
  
  //---------  loop over particles in the events  ------------
  

  std::vector<EVENT::ReconstructedParticle*> * vpfo = 0;
  std::vector<EVENT::Track*>* vtrk = 0; 
  
//   std::vector<IMPL::ReconstructedParticleImpl*> * vpfo = 0;
//   std::vector<IMPL::TrackImpl*>* vtrk = 0; 

  TBranch* bpfo = 0;
  t->SetBranchAddress("PandoraPFOs",&vpfo,&bpfo);
  
  TBranch* btrk = 0;
  t->SetBranchAddress("LDCTracks",&vtrk,&btrk);
  

  int nevt = t->GetEntries();

  for (Int_t i = 0; i < nevt ; i++) {

    Long64_t tentry = t->LoadTree(i);

    bpfo->GetEntry(tentry);
    btrk->GetEntry(tentry);



    double etot = 0. ;

    for (UInt_t j = 0; j < vtrk->size(); ++j) {
      EVENT::Track* trk =  vtrk->at(j) ;
      //    cout << "vtrk: " << trk << endl ;
      htphi->Fill(  trk->getPhi()  ) ;
    }
    

    for (UInt_t j = 0; j < vpfo->size(); ++j) {
      
      
      EVENT::ReconstructedParticle* part =  vpfo->at(j) ;

      //etot += (*vpfo)[j]->getEnergy() ; // does not work in CINT !!?
      //etot += vpfo->at(j)->getEnergy() ;

      etot += part->getEnergy() ;

      
      const std::vector< Track * >& trks = part->getTracks() ;

      hntrk->Fill(  trks.size() ) ;

      for (UInt_t k = 0; k < trks.size(); ++k) {
	//	cout << " trks: " << trks[k] << endl ;
       htphipo->Fill(  trks[k]->getPhi()  ) ;
      }

    }

     htot->Fill( etot ) ;
     //     cout << " etot : " << etot << endl ;

  }
  
  // Since we passed the address of a local variable we need
  // to remove it.
  t->ResetBranchAddresses();
  


  // -------------------------
  TCanvas *c1 = new TCanvas("c1","Dynamic Filling Example",200,10,700,500);
  c1->Divide(2,2);
  c1->cd(1)  ;
  htot->Draw();
  c1->cd(2)  ;
  htphi->Draw();
  c1->cd(3)  ;
  hntrk->Draw();
  c1->cd(4)  ;
  htphipo->Draw();
  
  
}
