#include "IMPL/LCTOOLS.h"

using namespace std ;
using namespace EVENT ;


namespace IMPL {


  void LCTOOLS::dumpEvent(const LCEvent* evt){
    
    // the event:
    cout << "event  : " 
	 << evt->getEventNumber() 
	 << " - run " << evt->getRunNumber()
	 << " detector : "  <<  evt->getDetectorName()
	 << " - collections  : "
	 << endl ;
    
    StringVec* strVec = evt->getCollectionNames() ;
    int nCol = strVec->size() ;
    
    // loop over collections:
    for( StringVec::iterator name = strVec->begin() ; name != strVec->end() ; name++){
    
      cout << "     " <<  *name <<   " " 
	   <<   evt->getCollection( *name )->getTypeName() << " : "  ; 
    
      const LCCollection* col = evt->getCollection( *name ) ;
    
    
      // print CalorimeterHit collections:
      if(evt->getCollection( *name )->getTypeName() == LCIO::CALORIMETERHIT ){
      
	int nHits =  col->getNumberOfElements() ;
	cout << nHits << " hits - first hit: " ;
	int nPrint = nHits>0 ? 1 : 0 ;

	if(!nPrint ) cout << endl ;
	for( int i=0 ; i< nPrint ; i++ ){
	
	  const CalorimeterHit* hit = 
	    dynamic_cast<const CalorimeterHit*>( col->getElementAt( i ) ) ;
	
	  const float* x =  hit->getPosition() ;
	  cout << "    hit -  e: " 
	       << hit->getEnergy() << "  pos: " 
	       << x[0] << ", " << x[1] << ", " << x[2] 
	       << endl ;   
	}
      
	// print the MCParticle collection
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

}; // namespace

