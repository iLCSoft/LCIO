#include "IMPL/LCTOOLS.h"
#include "EVENT/LCCollection.h"
#include "EVENT/SimCalorimeterHit.h"
#include "EVENT/SimTrackerHit.h"
#include "EVENT/LCIO.h"
#include "EVENT/MCParticle.h"
#include "DATA/LCFloatVec.h"

#include "IMPL/LCFlagImpl.h"

using namespace std ;
using namespace EVENT ;
using namespace DATA ;


namespace IMPL {

  static int MAX_HITS = 1000 ;


  void LCTOOLS::dumpEventDetailed(const LCEvent* evt){
    
    // the event:
    cout << endl 
	 << "==============================================================" << endl ;
    cout << "        Event  : " << evt->getEventNumber() 
	 << " - run:  "         << evt->getRunNumber()
         << " - timestamp "     << evt->getTimeStamp()  << endl ;
    cout << "==============================================================" << endl ;    

    cout << " detector : "      << evt->getDetectorName() << endl ;
    

    const std::vector< std::string >* strVec = evt->getCollectionNames() ;

    // loop over all collections:
    std::vector< std::string >::const_iterator name ;
    
    for( name = strVec->begin() ; name != strVec->end() ; name++){
    
      const LCCollection* col = evt->getCollection( *name ) ;
    
      cout << endl 
	   << " collection name : " << *name 
	   << endl ;

      // call the detailed print functions depending on type name
      if( evt->getCollection( *name )->getTypeName() == LCIO::MCPARTICLE ){
	  
	printMCParticles( col ) ;

      }
      else if( evt->getCollection( *name )->getTypeName() == LCIO::SIMTRACKERHIT ){
	  
	printSimTrackerHits( col ) ;

      }
      else if( evt->getCollection( *name )->getTypeName() == LCIO::SIMCALORIMETERHIT ){
	  
	printSimCalorimeterHits( col ) ;

      }


    }

  }

  void LCTOOLS::dumpEvent(const LCEvent* evt){
    
    // the event:
    cout << "event  : " 
	 << evt->getEventNumber() 
	 << " - run " << evt->getRunNumber()
	 << " detector : "  <<  evt->getDetectorName()
	 << " - collections  : "
	 << endl ;
    
    const std::vector< std::string >* strVec = evt->getCollectionNames() ;

    // loop over collections:
    for( std::vector< std::string >::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
    
      cout << "     " <<  *name <<   " " 
	   <<   evt->getCollection( *name )->getTypeName() << " : "  ; 
    
      const LCCollection* col = evt->getCollection( *name ) ;
    
    
      // print SimCalorimeterHit collections:
      if(evt->getCollection( *name )->getTypeName() == LCIO::SIMCALORIMETERHIT ){
      
	int nHits =  col->getNumberOfElements() ;
	cout << nHits << " hits - first hit: " ;
	int nPrint = nHits>0 ? 1 : 0 ;
	int flag = col->getFlag() ;

	if(!nPrint ) cout << endl ;
	for( int i=0 ; i< nPrint ; i++ ){
	
	  const SimCalorimeterHit* hit = 
	    dynamic_cast<const SimCalorimeterHit*>( col->getElementAt( i ) ) ;
	
	  cout << "    hit -  e: "  << hit->getEnergy() ;

	  if( LCFlagImpl(flag).bitSet( LCIO::CHBIT_LONG ) ){

	    const float* x =  hit->getPosition() ;
	    cout << "  pos: " << x[0] << ", " << x[1] << ", " << x[2] << endl ;   

	  } else{
	    cout << "  short hits: position not available! " << hex << flag << dec << endl ;
	  }
	}
      
	// print the MCParticle collection
      } else if(evt->getCollection( *name )->getTypeName() == LCIO::SIMTRACKERHIT ){
      
	int nHits =  col->getNumberOfElements() ;
	cout << nHits << " hits : " ;
	int nPrint = nHits>0 ? 1 : 0 ;

	if(!nPrint ) cout << endl ;
	for( int i=0 ; i< nPrint ; i++ ){
	  const SimTrackerHit* hit = 
	    dynamic_cast<const SimTrackerHit*>( col->getElementAt( i ) ) ;
	
	  const double* x =  hit->getPosition() ;
	  cout << "    hit -  dEdx: " 
	       << hit->getdEdx() 
	       << "  mc: " << hit->getMCParticle()->getPDG() 
	       << "  pos: " 
	       << x[0] << ", " << x[1] << ", " << x[2] 
	       << endl ;   

	}
      } else if(evt->getCollection( *name )->getTypeName() == LCIO::LCFLOATVEC ){
	
	int nHits =  col->getNumberOfElements() ;
	cout << nHits << " vectors: " ;
	int nPrint = nHits>0 ? 1 : 0 ;
	
	if(!nPrint ) cout << endl ;
	for( int i=0 ; i< nPrint ; i++ ){
	  const LCFloatVec* vec = 
	    dynamic_cast<const LCFloatVec*>( col->getElementAt( i ) ) ;
	  
	  cout << " values(" << i << "): " ;
	  for(unsigned int k=0 ; k< vec->size() ; k++ )
	    cout <<  (*vec)[k]  << ", " ;
	  cout << endl ;   

	}
      } else if(evt->getCollection( *name )->getTypeName() == LCIO::MCPARTICLE ){
      
      
	int nHits =  col->getNumberOfElements() ;
	cout << nHits << " particles : " ;
	int nPrint = nHits>0 ? 1 : 0 ;

	if(!nPrint ) cout << endl ;
	for( int i=0 ; i< nPrint ; i++ ){
	  const MCParticle* part = 
	    dynamic_cast<const MCParticle*>( col->getElementAt( i ) ) ;
	
	  cout << "           " << part->getPDG() << " p: " 
	       <<  part->getMomentum()[0]  << ", "
	       <<  part->getMomentum()[1]  << ", "
	       <<  part->getMomentum()[2]  
	       << endl ;	
	
	}

      } else {
	cout << endl ;
      } 
    }
    cout << endl ;
  }


  void LCTOOLS::printSimTrackerHits(const EVENT::LCCollection* col ){

  }

  void LCTOOLS::printSimCalorimeterHits(const EVENT::LCCollection* col ){

  }

  void LCTOOLS::printMCParticles(const EVENT::LCCollection* col ) {

    if( col->getTypeName() != LCIO::MCPARTICLE ){

      cout << " collection not of type " << LCIO::MCPARTICLE << endl ;
      return ;
    }

    cout << endl 
	 << "--------------- " << "print out of "  << LCIO::MCPARTICLE << " collection "
	 << "--------------- " << endl ;

    cout << endl 
	 << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;
    
    int nParticles =  col->getNumberOfElements() ;


    // get a list of all mother particles
    std::vector<const MCParticle*> moms ; 
    for( int k=0; k<nParticles; k++){

      // get the particle from the collection - needs a cast !
      const MCParticle* part =  dynamic_cast<const MCParticle*>( col->getElementAt( k ) ) ;
      
      if( part->getParentData() == 0 )
	moms.push_back( part ) ;
    }

    // now loop over mothers and print daughters recursively
    int index = 0 ;
    std::vector<const MCParticle*>::const_iterator  mom ; 
    for( mom = moms.begin() ; mom != moms.end() ; mom++){

      cout << index++ << ": "
	   <<  (*mom)->getPDG() << " | " 
	   <<  (*mom)->getMomentum()[0]  << ", "
	   <<  (*mom)->getMomentum()[1]  << ", "
	   <<  (*mom)->getMomentum()[2]  
	   << endl ;	

      //      index  = printDaughterParticles( *mom, index ) ;
    }

    int nPrint = nParticles > MAX_HITS ? MAX_HITS : nParticles ;

    if(!nPrint ) cout << endl ;
    for( int i=0 ; i< nPrint ; i++ ){

      // get the particle from the collection - needs a cast !
      const MCParticle* part =  dynamic_cast<const MCParticle*>( col->getElementAt( i ) ) ;
      


      cout <<  part->getPDG() << " | " 
	   <<  part->getMomentum()[0]  << ", "
	   <<  part->getMomentum()[1]  << ", "
	   <<  part->getMomentum()[2]   << "   "
	   << "me: " <<  part 
	   << " mom: " << part->getParentData() ;
      
      if( part->getNumberOfDaughters() > 0 ) 
	cout <<  " - daughter: " << part->getDaughter(0) ;
      cout    << endl ;	
      
    }

    cout << endl 
	 << "-------------------------------------------------------------------------------- " 
	 << endl ;
  }

  int LCTOOLS::printDaughterParticles(const MCParticle* part, int index){

    // print this particles daughters
    for(int i=0; i<part->getNumberOfDaughters();i++){
      
      cout << index++ << ": "
	   <<  part->getPDG() << " | " 
	   <<  part->getMomentum()[0]  << ", "
	   <<  part->getMomentum()[1]  << ", "
	   <<  part->getMomentum()[2]  
	   << endl ;	
    }
    // and now the same for the grand daughters
    for(int i=0; i<part->getNumberOfDaughters();i++){

      index = printDaughterParticles( part->getDaughter(i) , index ) ;
    }

    return index ;

  }

}; // namespace

