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
      else if( evt->getCollection( *name )->getTypeName() == LCIO::LCFLOATVEC ){
	  
	printLCFloatVecs( col ) ;

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

    cout << "  SimTrackerHits not implemented yet  " << endl ;
  }

  void LCTOOLS::printLCFloatVecs( const EVENT::LCCollection* col ) {

    cout << "  LCFloatVecs not implemented yet  " << endl ;

  }

  void LCTOOLS::printSimCalorimeterHits(const EVENT::LCCollection* col ){

    if( col->getTypeName() != LCIO::SIMCALORIMETERHIT ){

      cout << " collection not of type " << LCIO::SIMCALORIMETERHIT << endl ;
      return ;
    }

    cout << endl 
	 << "--------------- " << "print out of "  << LCIO::SIMCALORIMETERHIT << " collection "
	 << "--------------- " << endl ;

    cout << endl 
	 << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;
 
    LCFlagImpl flag( col->getFlag() ) ;
    cout << "  -> LCIO::CHBIT_LONG   : " << flag.bitSet( LCIO::CHBIT_LONG ) << endl ;
    cout << "     LCIO::CHBIT_BARREL : " << flag.bitSet( LCIO::CHBIT_BARREL ) << endl ;
    cout << "     LCIO::CHBIT_POSZ   : " << flag.bitSet( LCIO::CHBIT_POSZ ) << endl ;
    cout << "     LCIO::CHBIT_PDG    : " << flag.bitSet( LCIO::CHBIT_PDG ) << endl ;

    int nHits =  col->getNumberOfElements() ;
    int nPrint = nHits > MAX_HITS ? MAX_HITS : nHits ;

    std::cout << endl
	      << " cellId0 | cellId1 | energy | position (x,y,z) | nMCParticles " 
	      << endl << "           -> MC contribution: prim. PDG |  energy | time | sec. PDG  "
	      << endl 
	      << endl ;
    
    for( int i=0 ; i< nPrint ; i++ ){
      
      const SimCalorimeterHit* hit = 
	dynamic_cast<const SimCalorimeterHit*>( col->getElementAt( i ) ) ;
      
	    
      cout << i << ": "
	   << hit->getCellID0() << " | "
	   << hit->getCellID1() << " | "
	
	   << hit->getEnergy() << " | (" ;
      
      if( flag.bitSet( LCIO::CHBIT_LONG ) ){
      
	cout << hit->getPosition()[0] << ", "
	     << hit->getPosition()[1]<< ", "
	     << hit->getPosition()[2] << ") | " ;
      }else{
	cout << "   no position avaliable  ) | " ;
      }
      cout << hit->getNMCParticles() 
	   << endl ;
      
      for(int k=0;k < hit->getNMCParticles();k++){

	try{
	  cout << "           ->   " 
	       << hit->getParticleCont( k)->getPDG() << ": " 
	       << hit->getEnergyCont( k)             << " | " 
	       << hit->getTimeCont( k)               << " | " ;

	  if( flag.bitSet( LCIO::CHBIT_PDG ) )
	    cout << hit->getPDGCont( k) << " | " << endl ;
	  else
	    cout << " no PDG" << endl ;
	} 

	catch(exception& e){
	  cout << e.what() << endl ;
	}

      }


    }
    cout << endl 
	 << "-------------------------------------------------------------------------------- " 
	 << endl ;
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

    std::cout << endl
	      << " index [motherIndex] | (px, py, pz) | HepEvtStatus | vertex (x,y,z) | endpoint(x,y,z)" 
	      << " | energy | charge "
	      << endl 
	      << endl ;


    // now loop over mothers and print daughters recursively
    int index = 0 ;
    std::vector<const MCParticle*>::const_iterator  mom ; 
    for( mom = moms.begin() ; mom != moms.end() ; mom++){

      cout << index++ << " [-] "
	   <<  (*mom)->getPDG() << " | ("
	   <<  (*mom)->getMomentum()[0]  << ", "
	   <<  (*mom)->getMomentum()[1]  << ", "
	   <<  (*mom)->getMomentum()[2]  << ") | "
	   <<  (*mom)->getHepEvtStatus() << " | ("
	   <<  (*mom)->getVertex()[0]    << ", "
	   <<  (*mom)->getVertex()[1]    << ", "
	   <<  (*mom)->getVertex()[2]    << ") | ("
	   <<  (*mom)->getEndpoint()[0]  << ", "
	   <<  (*mom)->getEndpoint()[1]  << ", "
	   <<  (*mom)->getEndpoint()[2]  << ") | "
	   <<  (*mom)->getEnergy()       << " | " 
	   <<  (*mom)->getCharge() 
	   << endl ;	


      index  = printDaughterParticles( *mom, index ) ;
    }

    cout << endl 
	 << "-------------------------------------------------------------------------------- " 
	 << endl ;
  }


  int LCTOOLS::printDaughterParticles(const MCParticle* part, int index){

    int motherIndex = index - 1 ;
    // print this particles daughters
    for(int i=0; i<part->getNumberOfDaughters();i++){
      
      const MCParticle* d =  part->getDaughter(i) ;

      cout << index++ << " [" << motherIndex << "] " 
	   <<  d->getPDG() << " | ("
	   <<  d->getMomentum()[0]  << ", "
	   <<  d->getMomentum()[1]  << ", "
	   <<  d->getMomentum()[2]  << ") | "
	   <<  d->getHepEvtStatus() << " | ("
	   <<  d->getVertex()[0]    << ", "
	   <<  d->getVertex()[1]    << ", "
	   <<  d->getVertex()[2]    << ") | ("
	   <<  d->getEndpoint()[0]  << ", "
	   <<  d->getEndpoint()[1]  << ", "
	   <<  d->getEndpoint()[2]  << ") | "
	   <<  d->getEnergy()       << " | " 
	   <<  d->getCharge() 
	   << endl ;	

      index = printDaughterParticles( part->getDaughter(i) , index ) ;
    }
    return index ;

  }

}; // namespace

