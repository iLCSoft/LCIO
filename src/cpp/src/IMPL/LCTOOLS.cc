#include "IMPL/LCTOOLS.h"
#include "EVENT/LCCollection.h"
#include "EVENT/CalorimeterHit.h"
#include "EVENT/TrackerHit.h"
#include "EVENT/LCIO.h"
#include "EVENT/MCParticle.h"
#include "EVENT/LCFloatVec.h"

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
    
    const StringVec* strVec = evt->getCollectionNames() ;

    // loop over collections:
    for( StringVec::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
    
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
      } else if(evt->getCollection( *name )->getTypeName() == LCIO::TRACKERHIT ){
      
	int nHits =  col->getNumberOfElements() ;
	cout << nHits << " hits : " ;
	int nPrint = nHits>0 ? 1 : 0 ;

	if(!nPrint ) cout << endl ;
	for( int i=0 ; i< nPrint ; i++ ){
	  const TrackerHit* hit = 
	    dynamic_cast<const TrackerHit*>( col->getElementAt( i ) ) ;
	
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

}; // namespace

