
#include "UTIL/ProcessFlag.h"

#include "EVENT/LCCollection.h"
#include "EVENT/MCParticle.h" 
#include "UTIL/LCTOOLS.h"
#include "UTIL/Operators.h"


namespace UTIL{

  ProcessFlag decodeMCTruthProcess(const EVENT::LCCollection* col, int maxParticles){

    // this is WIP and needs iteration:
    //  - make more general for different Whizrd versions
    //  - deal with BSM and other exotic cases
    //  - decide on whether individual fermions from W's are used to set the corresponging flag
    //  - ...


//    LCTOOLS::printMCParticles( col ) ;

    ProcessFlag flag ;
    std::map< int, int> pdgFSCount ;
    EVENT::MCParticle* higgs = nullptr ;
    int hDpdg0 =0 , hDpdg1 =0;
    
    int np = col->getNumberOfElements() > maxParticles ? maxParticles :  col->getNumberOfElements() ;
    

    for( int i=0 ; i<np ; ++i){

      EVENT::MCParticle* mcp = static_cast<EVENT::MCParticle*>( col->getElementAt(i) ) ;

      int pdg = std::abs( mcp->getPDG() ) ;

      /// for DBD stdhep files these seem to be the particles of the hard sub-process
      ///      -> needs generalization to other versions of Whizard ....

      if( mcp->getParents().empty() ){ 
	pdgFSCount[ pdg ]++ ;
      }

      if( pdg == 25 && mcp->getDaughters().size()==2 ){ // higgs decay

	hDpdg0 = mcp->getDaughters()[0]->getPDG() ;
	hDpdg1 = mcp->getDaughters()[1]->getPDG() ;
	higgs = mcp ;
      }

    }
    pdgFSCount[22] -= 2 ; // remove the FSR photons


    // fill final state particles into flag:

    for( auto m : pdgFSCount ){
      if( m.second  > 1 )
	flag.addFSParticles( m.first ) ;
    }

    // fill Higgs decay
    
    if(higgs != nullptr){

      if( std::abs( hDpdg0 ) == std::abs( hDpdg1 ) ) // particle anti-particle pair
	flag.addHiggsDecay( std::abs( hDpdg0 ) ) ;

      
      //FIXME: else deal with other more exotic cases ....
    }

    
    return flag ;
  }

}
