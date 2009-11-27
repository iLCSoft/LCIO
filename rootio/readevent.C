#include "TTree.h"
#include "TFile.h"
#include "TBranch.h"
#ifndef __CINT__ 
#include "include/EVENT/LCEvent.h"
#include "include/EVENT/ReconstructedParticle.h"
#include "include/EVENT/MCParticle.h"
#include "include/UTIL/LCTOOLS.h"
#endif
#include <vector>

//---------  loop over particles in the events  ------------
  
#define FILEN "DST01-06_ppr004_bbcsdu.rlcio"


void readevent() {

  //just in case this script is executed multiple times
  delete gROOT->GetListOfFiles()->FindObject( FILEN );
  delete gROOT->GetListOfCanvases()->FindObject("c1");
  

  if (!TClassTable::GetDict("IMPL::LCEventImpl")) {
    unsigned res ;
    
    res = gSystem->Load("./lib/liblcio.so"); 
    res = gSystem->Load("./lib/liblcioDict.so"); 
    
    //  if (res != 0 ) {
    //       std::cout << " can't load ./liblcioDict.so  !  err = " <<res<<std::endl;
    //   } 
  }
    
  
  
  TFile *f = TFile::Open( FILEN ,"READ");
   
  if (!f) { return; }
  
  TTree *t; f->GetObject("LCIO",t);

  

  //   // ---- create a few histograms ---------

  TH1F *htot = new TH1F("hetot","total energy from MCPs",100,0.,1000.);
  htot->SetFillColor(48);
  
  TH1F *hepfo = new TH1F("hepfo","total energy from PFOs",100,0.,1000.);
  hepfo->SetFillColor(32);

  TH2F *herm = new TH2F("herm","mcp energy vs PFO energy",100,0.,100.,100,0.,100.);
  herm->SetFillColor(40);


  //   TH1F *htphi = new TH1F("htphi","phi of tracks ",48,-1.6,+3.2);
  //   htphi->SetFillColor(32);
  
  //   TH1F *htphipo = new TH1F("htphipo","phi of tracks - PFOs",48,-1.6,+3.2);
  //   htphipo->SetFillColor(30);
  
  //   TH1F *hntrk = new TH1F("hntrk","number of tracks PFOs",11,0.,10.);
  //   hntrk->SetFillColor(41);
  
  
  
  
  std::vector<TBranch*> branches ;

  //---------  loop over particles in the events  ------------
  
  IMPL::LCEventImpl* evt =0 ; // new IMPL::LCEventImpl ; // 0 ;
  //  EVENT::LCEventImpl* evt = 0 ; 
  TBranch* bevt = t->GetBranch("LCEvent") ; 
  bevt->SetAddress( &evt ) ;
  branches.push_back( bevt ) ;

  evt->setCurrentEvent( evt ) ;

  IMPL::LCCollectionVec* col = new IMPL::LCCollectionVec ;
  branches.push_back( t->GetBranch("MCParticlesSkimmed") ) ;  
  branches.back()->SetAddress( &col ) ;
  
  IMPL::LCCollectionVec* cpfo = new IMPL::LCCollectionVec ;
  branches.push_back( t->GetBranch("PandoraPFOs") ) ;  
  branches.back()->SetAddress( &cpfo ) ;
  
  
  IMPL::LCCollectionVec* crml = new IMPL::LCCollectionVec ;
  branches.push_back( t->GetBranch("RecoMCTruthLink") ) ;  
  branches.back()->SetAddress( &crml ) ;
  
  //evt->setCurrentEvent( evt ) ;
  evt->addCollection( col ,  "MCParticlesSkimmed" ) ; 
  evt->addCollection( cpfo , "PandoraPFOs" ) ; 
  evt->addCollection( crml , "RecoMCTruthLink" ) ; 


  int nBranches = branches.size() ;
  
  
  
  
  //***************************************************************************************************
  
  
  int nevt = t->GetEntries();
 
  for (Int_t i = 0; i < nevt ; i++) {
    //for (Int_t i = 0; i < 1 ; i++) {
   
    
    Long64_t tentry = t->LoadTree(i);
   

    for (Int_t k = 0; k < nBranches ; k++) {
     
      int nbyte  = branches[k]->GetEntry(tentry);
     
      //       cout << " read " << nbyte << " bytes for branch " << branches[k]->GetName()  
      // 	   << std::endl ;
    }

//     EVENT::LCCollection* col  = evt->getCollection("MCParticlesSkimmed") ;
//     EVENT::LCCollection* cpfo = evt->getCollection("PandoraPFOs") ;
//     EVENT::LCCollection* crml = evt->getCollection("RecoMCTruthLink") ;
    
    
    
    int nMCP = col->getNumberOfElements() ;
    double eMCP = 0.0 ;
    for(int j=0;j<nMCP ;++j){
      
      EVENT::MCParticle* mcp = dynamic_cast<EVENT::MCParticle*>( col->getElementAt(j) ) ;
      
      //      cout << " -- " << mcp << endl ;
      
      if( mcp->getGeneratorStatus() == 1 )
	eMCP += mcp->getEnergy() ;
    }
    htot->Fill(eMCP) ;
    
   
    int nPFO = cpfo->getNumberOfElements() ;
    double ePFO = 0.0 ;
    for(int j=0;j<nPFO ;++j){
     
      EVENT::ReconstructedParticle* pfo = dynamic_cast<EVENT::ReconstructedParticle*>( cpfo->getElementAt(j) ) ;
     
      if( pfo ) 
	ePFO += pfo->getEnergy() ;
    }
    hepfo->Fill(ePFO) ;
   

    // check reco mc link:

    int nrml = crml->getNumberOfElements() ;

    for(int j=0;j<nrml ;++j){
      
      EVENT::LCRelation* rel = dynamic_cast<EVENT::LCRelation*>( crml->getElementAt(j) ) ;
      
      EVENT::ReconstructedParticle* pfo =  dynamic_cast<EVENT::ReconstructedParticle*>( rel->getFrom() ) ;
      EVENT::MCParticle* mcp =  dynamic_cast<EVENT::MCParticle*>( rel->getTo() ) ;
      
      //      cout << " ++ " << mcp << endl ;

      if( pfo && mcp ) 
	herm->Fill( pfo->getEnergy() ,   mcp->getEnergy()   ) ;

    }
    
    //    UTIL::LCTOOLS::dumpEvent( evt ) ;


  }
  //***************************************************************************************************
 
 
  // Since we passed the address of a local variable we need
  // to remove it.
  t->ResetBranchAddresses();
 
  
  // -------------------------
  TCanvas *c1 = new TCanvas("c1","Dynamic Filling Example",200,10,700,500);
  c1->Divide(2,2);
  c1->cd(1)  ;
  htot->Draw();
  c1->cd(2)  ;
  hepfo->Draw();
  c1->cd(3)  ;
  herm->Draw();
  
  
  
}

