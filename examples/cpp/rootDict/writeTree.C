#ifndef __CINT__ 
#include "IO/LCReader.h"
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/MCParticleImpl.h"
#include "EVENT/LCEvent.h"
#include "UTIL/LCTOOLS.h"
#include <vector>
#endif


/** Example script for testing the ROOT LCIO dictionary.
 * 
 *  writeTree: 
 *  reads *.slcio file and writes the MCParticle collection to 
 *  a branch of type   std::vector<IMPL::MCParticleImpl*> 
 *  in file lcioTree.root
 *
 */
 
void writeTree(const char* FILEN) {
  
  std::string mcpName("MCParticlesSkimmed") ;


  //just in case this script is executed multiple times
  delete gROOT->GetListOfFiles()->FindObject( FILEN );
  delete gROOT->GetListOfCanvases()->FindObject("c1");
  
  if (!TClassTable::GetDict("IMPL::ReconstructedParticleImpl")) {
    unsigned res ;
    
    res = gSystem->Load("${LCIO}/lib/liblcio.so"); 
    res = gSystem->Load("${LCIO}/lib/liblcioDict.so"); 
  }
  
  
  //--- create a ROOT file, a tree and a branch ...

  TFile* file = new TFile( "lcioTree.root" , "RECREATE");    

  TTree* tree = new TTree( "LCIO" , "lcio event data tree");
  

  
  //  IMPL::LCCollectionVec* treeCol = 0 ;
  //  std::string mcpType("IMPL::LCCollectionVec") ;

  std::vector<IMPL::MCParticleImpl*>* treeCol = new std::vector<IMPL::MCParticleImpl*> ;
  std::string mcpType("std::vector<IMPL::MCParticleImpl*>") ;
  

  TBranch* mcpBranch = tree->Branch( mcpName.c_str(), 
				     mcpType.c_str(), 
				     (void*) &treeCol, 
				     1024, // record size 
				     99    // split level 
				     );
  

  std::cout << " loaded LCIO library and dictionary ... " << std::endl ;
  
  int nEvents = 0  ;
  int maxEvt = 10000 ;  // change as needed

  IO::LCReader* lcReader = IOIMPL::LCFactory::getInstance()->createLCReader() ;

  lcReader->open( FILEN ) ;
  

  IMPL::LCCollectionVec emptyCol ;
  
  //----------- the event loop -----------
  while( (evt = lcReader->readNextEvent()) != 0  && nEvents < maxEvt ) {
    
    UTIL::LCTOOLS::dumpEvent( evt ) ;
    nEvents ++ ;


    // ------- fill the MCParticle collection to the branch...

    IMPL::LCCollectionVec* col = (IMPL::LCCollectionVec*) evt->getCollection( mcpName  ) ;

//     treeCol = col ? col : emptyCol ;
    
    treeCol->clear() ;

    int nMCP = col->getNumberOfElements() ;

    for(int i=0 ; i<nMCP ; ++i){

      treeCol->push_back( (IMPL::MCParticleImpl*) col->getElementAt(i)  ) ;
    }


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
