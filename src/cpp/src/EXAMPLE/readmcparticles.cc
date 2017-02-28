#include "lcio.h"

#include "IO/LCReader.h"
#include "EVENT/LCCollection.h"
#include "EVENT/MCParticle.h"

#include <cstdlib>
#include <sstream>

using namespace std ;
using namespace lcio ;

/** Example for reading the MCParticle collection from a genrator file. 
 */

int main(int argc, char** argv ){

  // read file names from command line (only argument) 
  if( argc < 2) {
    cout << " usage:  readmcparticles input-file" << endl << endl ;
    exit(1) ;
  }
  std::string FILEN = argv[1] ;

  LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
  lcReader->open( FILEN ) ;
  
  
  LCEvent* evt ;
  int nEvents = 0 ;
  
  double e_evt = 0. ;

 //----------- the event loop -----------
  while( (evt = lcReader->readNextEvent()) != 0 ) {
    
    
    LCCollection* col = evt->getCollection("MCParticle") ;

    double e_mcp = 0. ;

    for( unsigned i=0,N=col->getNumberOfElements() ; i< N ; ++i){
      
      MCParticle* p = (MCParticle*) col->getElementAt(i) ;

      if( p->getGeneratorStatus() == 1) 
	e_mcp += p->getEnergy() ;

    }

    e_evt += e_mcp ;
 
    nEvents ++ ;
  } 
  // -------- end of event loop -----------
  
  std::cout << " read " << nEvents << "  <Ecms> = " << e_evt/nEvents << std::endl ;
  
  lcReader->close() ;
  delete lcReader ;
  return 0 ;
}

  
