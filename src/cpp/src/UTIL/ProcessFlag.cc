
#include "UTIL/ProcessFlag.h"

#include "EVENT/LCCollection.h"
#include "EVENT/MCParticle.h" 
#include "UTIL/LCTOOLS.h"
#include "UTIL/Operators.h"


namespace UTIL{

  ProcessFlag decodeMCTruthProcess(const EVENT::LCCollection* col, int maxParticles){

    /// first step: decide which type of file we have:
    ///  icase 1: Whizard 2.8.4: incoming particles of hard subprocess (i.e. after BS, ISR etc) have generatorStatus 3
    ///      => use their daughters as hard final state
    ///  icase 2: DBD files WITHOUT beam particles (e.g. 250 GeV DBD samples) have more than 2 PARENTLESS particles
    ///      => use these as hard final state
    ///  icase 3: DBD files WITH beam particles (e.g. non-Higgs 500 GeV DBD samples) have exactly 2 PARENTLESS particles
    ///      => their daughters are the incoming particles of hard subprocess and the ISR and/or outgoing beam particles
    ///      => select those daughters which have more than one daughter
    ///      => take the daughters of these daughters of the parentless particles as hard final state

    ProcessFlag flag ;

//    LCTOOLS::printMCParticles( col ) ;

    std::map< int, int> pdgFSCount ;
    EVENT::MCParticle* higgs = nullptr ;
    int hDpdg0 =0 , hDpdg1 =0;
    int icase = 0;
    int n_parentless = 0;
    EVENT::MCParticleVec hf_vec;
    
    int nP = maxParticles > col->getNumberOfElements() ? col->getNumberOfElements()  : maxParticles ;

    for( int i=0 ; i<nP ; ++i){
      auto* mcp = static_cast<EVENT::MCParticle*>( col->getElementAt(i) ) ;

      if( mcp->getGeneratorStatus() == 3 ){
	icase = 1 ;
        hf_vec = mcp->getDaughters();
        break; // in this case we're done!
      }

      if( mcp->getParents().empty() ){
	n_parentless++ ;
      }
    }

    /// if not Whizard 2, still need to decide which DBD case
    /// and find hard final state
    if (icase == 0) {

      if (n_parentless > 2) { // 2 ISRs and more parentless particles

        icase = 2;

	for( int i=0 ; i<n_parentless ; ++i){ // this assumes that all parentless particles are at the beginning of the list

	  auto* mcp = static_cast<EVENT::MCParticle*>( col->getElementAt(i) ) ;

	  hf_vec.push_back(mcp);
	}
      }
      else if (n_parentless == 2) {

        auto* mcp = static_cast<EVENT::MCParticle*>( col->getElementAt(0) ) ;
        auto& daughter_vec = mcp->getDaughters();

        for( unsigned i=0 ; i<daughter_vec.size() ; ++i){
          auto* dmcp = daughter_vec[i] ;

          if (dmcp->getDaughters().size() > 1) {   // found daughter with more than one daughter -> hard final state

	    icase = 3;   // set icase only here to keep icase == 0 in case there is no daughter with more than one daughter...
	    hf_vec = dmcp->getDaughters();
	    break;
          }
        }
      }
      else { // something is WRONG - cannot decode MC process

	icase = 0;
      }
    }

    if(icase == 0 || hf_vec.empty() ) {

      flag.add( PF::unknown ) ;
      return flag ;
    }

    // ------------------------------------------------------------------------------------------
    // NOW we safely have the pointers to the MCParticles forming the hard final state in hf_vec !

    for(unsigned i=0 ; i<hf_vec.size() ; ++i){

      auto* mcp = hf_vec[i] ;

      int pdg = std::abs( mcp->getPDG() ) ;
      pdgFSCount[ pdg ]++ ;

      /// the first higgs is usually not the one which decays
      if( pdg == 25 ){ // higgs

        while (mcp->getDaughters().size() == 1 && std::abs( mcp->getDaughters()[0]->getPDG() ) == 25) {

          // keep going while single daughter is still a (SM) Higgs
          mcp = mcp->getDaughters()[0] ;
        }

        hDpdg0 = mcp->getDaughters()[0]->getPDG() ;
	if( mcp->getDaughters().size() > 1 )
	  hDpdg1 = mcp->getDaughters()[1]->getPDG() ;

        higgs = mcp ;
      }
    }
    // for (icase == 2) we have falsly counted 2 ISR photons:
    if( icase == 2 )
      pdgFSCount[ 22 ] -= 2 ;


    //---------------------------------------------------------------------------------------------------

    // fill final state particles into flag:
    // set flag already for single occurrance of a given particle, so that eg muons and neutrinos from WW-> qqmunu will be visible

    for( auto m : pdgFSCount ){

      if( m.second  > 0 )
	flag.addFSParticles( m.first ) ;

      if( m.second > 1000000 )
	flag.add( PF::exotic ) ;
    }

    // fill Higgs decay
    
    if(higgs != nullptr){

      if( std::abs( hDpdg0 ) == std::abs( hDpdg1 ) ) // particle anti-particle pair
	flag.addHiggsDecay( std::abs( hDpdg0 ) ) ;

      if( hDpdg0 == 22 && hDpdg1 == 23 )
	flag.add( PF::higgsgaZ ) ;

      if( hDpdg0 == 23 && hDpdg1 == 22 )
	flag.add( PF::higgsgaZ ) ;

      if( (std::abs( hDpdg0 ) > 1000000 ) || (std::abs( hDpdg1 ) > 1000000 ) )
	flag.add( PF::exotic ) ;

    }

    return flag ;
  }

}
