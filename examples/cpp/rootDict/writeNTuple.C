#ifndef __CINT__ 
#include "IO/LCReader.h"
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/MCParticleImpl.h"
#include "EVENT/MCParticle.h"
#include "EVENT/LCEvent.h"
#include "UTIL/LCTOOLS.h"
#include <vector>
#endif


/** Example script for using the LCIO root dictionary.
 *  writeNTuple: 
 *  reads *.slcio file and creates a simple NTuple from the MCParticle collection.
 *
 */
 
void writeNTuple(const char* FILEN) {
  

  std::string rootFileBaseName( FILEN , (strlen(FILEN) ) - strlen(".slcio") ) ;
  std::string rootFileName = rootFileBaseName + std::string(".root") ;

  //  std::cout << " rootFileBaseName: " << rootFileBaseName << std::endl ;

  std::string mcpName("MCParticle") ;

  const char* tupleNames = "px:py:pz:vx:vy:vz:ex:ey:ez:pdg:np:nd:gs:ss" ;
  double px,py,pz,vx,vy,vz,ex,ey,ez ;
  int pdg,np,nd,gs,ss ;


  //just in case this script is executed multiple times
  delete gROOT->GetListOfFiles()->FindObject( FILEN );
  delete gROOT->GetListOfCanvases()->FindObject("c1");
  
  if (!TClassTable::GetDict("IMPL::ReconstructedParticleImpl")) {
    unsigned res ;
    
    res = gSystem->Load("${LCIO}/lib/liblcio.so"); 
    res = gSystem->Load("${LCIO}/lib/liblcioDict.so"); 
  }
  
  
  //--- create a ROOT file and an NTuple

  TFile* file = new TFile( rootFileName.c_str()  , "RECREATE");    

  TNtupleD *tuple= new TNtupleD("MCP", "", tupleNames );

  std::cout << " loaded LCIO library and dictionary ... " << std::endl ;
  
  int nEvents = 0  ;
  int maxEvt = 10000 ;  // change as needed

  IO::LCReader* lcReader = IOIMPL::LCFactory::getInstance()->createLCReader() ;

  lcReader->open( FILEN ) ;
  

  IMPL::LCCollectionVec emptyCol ;
  
  //----------- the event loop -----------
  while( (evt = lcReader->readNextEvent()) != 0  && nEvents < maxEvt ) {
    
    //    UTIL::LCTOOLS::dumpEvent( evt ) ;
    nEvents ++ ;

    // ------- fill the MCParticle collection to tuple

    IMPL::LCCollectionVec* col = (IMPL::LCCollectionVec*) evt->getCollection( mcpName  ) ;

    int nMCP = col->getNumberOfElements() ;

    for(int i=0 ; i<nMCP ; ++i){

      EVENT::MCParticle* mcp =  (EVENT::MCParticle*) col->getElementAt(i) ;
      
      px = mcp->getMomentum()[0] ;
      py = mcp->getMomentum()[1] ;
      pz = mcp->getMomentum()[2] ;

      ex = mcp->getEndpoint()[0] ;
      ey = mcp->getEndpoint()[1] ;
      ez = mcp->getEndpoint()[2] ;

      vx = mcp->getVertex()[0] ;
      vy = mcp->getVertex()[1] ;
      vz = mcp->getVertex()[2] ;

      pdg = mcp->getPDG() ;
      np = mcp->getParents().size() ;
      nd = mcp->getDaughters().size() ;
      gs = mcp->getGeneratorStatus() ;
      ss = mcp->getSimulatorStatus() ;

      tuple->Fill(px,py,pz,vx,vy,vz,ex,ey,ez,pdg,np,nd,gs,ss) ;

    }



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
