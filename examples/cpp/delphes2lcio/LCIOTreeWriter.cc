
/** \class LCIOTreeWriter
 *
 *  Class handling output of ROOT tree in LCIO
 *
 *  \author F.Gaede, DESY
 *  \date June 2020
 *
 */

#include "LCIOTreeWriter.h"

#include "ExRootAnalysis/ExRootTreeBranch.h"
#include "classes/DelphesClasses.h"

#include "TClonesArray.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"

#include "lcio.h"
#include "IMPL/LCEventImpl.h" 
#include "IMPL/LCRunHeaderImpl.h" 
#include "IMPL/LCCollectionVec.h"
#include "IMPL/MCParticleImpl.h" 
#include "IMPL/LCTOOLS.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;
using namespace lcio;


LCIOTreeWriter::LCIOTreeWriter(IO::LCWriter* writer, const char *treeName) :
  fWriter(writer), fTree(0), fTreeName(treeName)
{
}

//------------------------------------------------------------------------------

LCIOTreeWriter::~LCIOTreeWriter()
{


  set<ExRootTreeBranch *>::iterator itBranches;
  for(itBranches = fBranches.begin(); itBranches != fBranches.end(); ++itBranches)
  {
    delete(*itBranches);
  }

  if(fTree) delete fTree;
}

//------------------------------------------------------------------------------

ExRootTreeBranch *LCIOTreeWriter::NewBranch(const char *name, TClass *cl)
{
  if(!fTree) fTree = NewTree();
  ExRootTreeBranch *branch = new ExRootTreeBranch(name, cl, fTree);
  fBranches.insert(branch);
  return branch;
}

//------------------------------------------------------------------------------

void LCIOTreeWriter::Fill()
{
  fTree->Fill();
  int nEntry =  fTree->GetEntriesFast() ;
  
  //--- create LCIO event -----------------------------------------------
  auto* evt = new LCEventImpl ;
  auto* mcps = new LCCollectionVec( LCIO::MCPARTICLE )  ;

  // --------------------------

  size_t n = fTree->GetListOfBranches()->GetEntries();
  for( size_t i = 0; i < n; ++ i ) {
    TBranch *br = dynamic_cast<TBranch*>(fTree->GetListOfBranches()->At(i));
//    cout << " *************** branch \"" << br->GetName() << " --- " << br->GetClassName() << std::endl;

    
    if( !strcmp( br->GetName(), "Particle" ) ){
      TClonesArray* col = *(TClonesArray**) br->GetAddress()  ;

      // std::cout << " particle col size " << col->GetEntriesFast()  << std::endl ;
      int ngp = col->GetEntriesFast();
      
      for(int j = 0; j < ngp ; ++j){
       	GenParticle* p = (GenParticle*) col->At(j);

	auto* mcp = new MCParticleImpl ;
	mcps->addElement( mcp ) ;

	mcp->setPDG(p->PID) ;
	double m[3] = { p->Px , p->Py , p->Pz }  ;
	mcp->setMomentum( m ) ;
	mcp->setGeneratorStatus( p->Status );
	mcp->setCharge( p->Charge );
	mcp->setMass( p->Mass );
	mcp->setTime( p->T ) ;
	double v[3] = { p->X , p->Y , p->Z }  ;
	mcp->setVertex( v ) ;
	// std::cout << " found particle with pdg = " << p->PID
	// 	  << " m1:" << p->M1 
	// 	  << " m2:" << p->M2 
	// 	  << " d1:" << p->D1 
	// 	  << " d2:" << p->D2 
	// 	  << std::endl ;
      }
      for(int j = 0; j < ngp ; ++j){
	GenParticle* p = (GenParticle*) col->At(j);
	auto* mcp = (MCParticleImpl*) mcps->getElementAt(j) ;

	if( p->M1 > -1 )
	  mcp->addParent( (MCParticle*) mcps->getElementAt( p->M1 ) ) ;
	if( p->M2 > -1 )
	  mcp->addParent( (MCParticle*) mcps->getElementAt( p->M2 ) ) ;

	// for(int k=p->D1 ; k<=p->D2 ; k++){
	//   mcp->addDaughter(  (MCParticle*) mcps->getElementAt( k ) ) ; 
	// }

      }
    }

  }


 

  evt->addCollection( mcps, "MCParticle" ) ;

  fWriter->writeEvent( evt ) ;

  LCTOOLS::dumpEvent( evt ) ;


  delete evt;


}

//------------------------------------------------------------------------------

void LCIOTreeWriter::Write()
{

  fWriter->close() ;
}

//------------------------------------------------------------------------------

void LCIOTreeWriter::Clear()
{
  set<ExRootTreeBranch *>::iterator itBranches;
  for(itBranches = fBranches.begin(); itBranches != fBranches.end(); ++itBranches)
  {
    (*itBranches)->Clear();
  }
}

//------------------------------------------------------------------------------

TTree *LCIOTreeWriter::NewTree()
{

  // if(!fFile) return 0;

  TDirectory *dir = gDirectory;

  // fFile->cd();
  fTree = new TTree(fTreeName, "Analysis tree");
  dir->cd();

  if(!fTree)
  {
    throw runtime_error("can't create output ROOT tree");
  }

  // tree->SetDirectory(fFile);
  // tree->SetAutoSave(10000000); // autosave when 10 MB written

  return fTree;
}
