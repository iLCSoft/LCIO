
#include "lcio.h"

#include "IO/LCWriter.h"
#include "EVENT/LCIO.h"

#include "IMPL/LCEventImpl.h" 
#include "IMPL/LCCollectionVec.h"
#include "IMPL/CalorimeterHitImpl.h"
#include "IMPL/MCParticleImpl.h" 
#include "IMPL/LCTOOLS.h"

#include <cstdlib>
#include <iostream>

using namespace std ;
using namespace lcio ;


static const char* FILEN = "simjob.sio" ;
static const char* OUTFILEN = "recjob.sio" ;
static const int NHITS = 50 ;  // calorimeter hits per event


/** Example of reading events from the file, add ath. to the event
 * and write it to a new file.
 */

int main(int argc, char** argv ){
  
  srand(1234) ;
  

  // create reader and writer for input and output streams 

  LCWriter* lcWrt = LCFactory::getInstance()->createLCWriter() ;
  LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;

  ;
  if( lcWrt->open( OUTFILEN ) != LCIO::SUCCESS ) {
    cout << " can't open file: " <<   OUTFILEN  << endl ;
    exit(1) ;
  } 

  if( lcReader->open( FILEN )  != LCIO::SUCCESS ) {
    cout << " can't open file: " << FILEN     << endl ;
    exit(1) ;
  } 
  

  LCEvent* evt ;
  
  // read next event in update mode as we want to add a collection
  int nEvent = 0 ;
  while( (evt = lcReader->readNextEvent( LCIO::UPDATE ) ) != 0 ){ 
    
    // read collection with MCParticles
    LCCollection* mcVec = evt->getCollection( LCIO::MCPARTICLE )  ;

    int NMCPART = mcVec->getNumberOfElements() ;
    
    // create a new collection to be added to the event
    // for simplicity just add some calorimeter hits (don't have cluster class yet) 

    LCCollectionVec* calVec = new LCCollectionVec( LCIO::CALORIMETERHIT )  ;
    
    for(int j=0;j<NHITS;j++){
      CalorimeterHitImpl* hit = new CalorimeterHitImpl ;
      hit->setEnergy( 3.1415 * rand()/RAND_MAX   )  ;
      float pos[3] = { 1.1* rand()/RAND_MAX , 2.2* rand()/RAND_MAX , 3.3* rand()/RAND_MAX } ;
      hit->setPosition( pos ) ;
      calVec->push_back( hit ) ;

      // assign the hits randomly to MC particles
      float rn =  1.*rand()/RAND_MAX ;
      int mcIndx = static_cast<int>( NMCPART * rn ) ;
      hit->addMCParticleContribution(  dynamic_cast<const MCParticle*>(mcVec->getElementAt( mcIndx )) , 
				       0.2876 , 0.007 , 565656 ) ;

    }
       
    // add sth. to en existing collection - this needs a cast to the 
    // collection implementation type
    // not the nicest solution, but it keeps the API a lot simpler...
    // another possibility is to create a copy of the collection add 
    // the data and then remove the old collection and add the new one.
    // a bit more complicated but wouldn't need a cast ....
    LCCollectionVec* colVec = dynamic_cast<LCCollectionVec*>(mcVec) ;
    if(colVec){
      
      MCParticleImpl* part = new MCParticleImpl ;
      
      part->setPDG( 1234 ) ;
      part->setParent( dynamic_cast<const MCParticle*>( colVec->getElementAt(0) )) ;
      
      colVec->push_back( part ) ;
      
      
    } else {
      cout << " couldn't do dynamic_cast<LCCollectionVec*>( const LCCollection*) " 
	   << endl ;
    }
    
    if( evt->addCollection( (LCCollection*) calVec , "HCALReco" ) != LCIO::SUCCESS ) {
      cout << " could not add new collection to the event :  " << "HCALReco"  << endl ;
      exit(1)  ;
    }

    LCTOOLS::dumpEvent( evt ) ;
    
    lcWrt->writeEvent( evt ) ;
    nEvent ++ ;
  }
    
  cout << endl 
       << " added collection HCALReco with " << NHITS << " hits" 
       << " to   " << nEvent <<" events"  
       << " and added one extra MCParticle to each event."
       << endl << endl ;


  lcReader->close() ;
  lcWrt->close()  ;
  
  return 0 ;
  
}

