#ifndef __CINT__ 
#include "include/IMPL/MCParticleImpl.h"
#include "include/IMPL/LCCollectionVec.h"
#include "include/IMPL/LCEventImpl.h"
#endif

  //---------  loop over particles in the events  ------------
  

void instantLCIO() {


  if (!TClassTable::GetDict("IMPL::ReconstructedParticleImpl")) {
    unsigned res ;
    
    res = gSystem->Load("./lib/liblcio.so"); 
    res = gSystem->Load("./lib/liblcioDict.so"); 
    
    //  if (res != 0 ) {
    //       std::cout << " can't load ./liblcioDict.so  !  err = " <<res<<std::endl;
    //   } 
  }


  IMPL::MCParticleImpl* mcp = new IMPL::MCParticleImpl ;

  IMPL::LCCollectionVec* col = new IMPL::LCCollectionVec ;

  IMPL::LCEventImpl* evt = new IMPL::LCEventImpl ;
  evt->setEventNumber( 42 )  ;


  std::cout << " mcp pdg: " << mcp->getPDG() << std::endl ;
  //  std::cout << " col #entries: " << col->getNumberOfEntries()  << std::endl ;
  std::cout << " evt : " << evt->getEventNumber()  << std::endl ;


}
