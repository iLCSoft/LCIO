
#include "lcio.h"

#include "IO/LCWriter.h"
#include "EVENT/LCIO.h"
#include "DATA/LCFloatVec.h"

#include "IMPL/LCEventImpl.h" 
#include "IMPL/LCRunHeaderImpl.h" 
#include "IMPL/LCCollectionVec.h"
#include "IMPL/SimCalorimeterHitImpl.h"
#include "IMPL/SimTrackerHitImpl.h"
#include "IMPL/MCParticleImpl.h" 
#include "IMPL/LCFlagImpl.h" 
#include "IMPL/LCTOOLS.h"

#include <cstdlib>
#include <iostream>
#include <sstream>


using namespace std ;
using namespace lcio ;

static const int NRUN = 10 ;
static const int NEVENT = 10 ; // events
static const int NMCPART = 10 ;  // mc particles per event
static const int NHITS = 50 ;  // calorimeter hits per event

static string FILEN = "simjob.slcio" ;


/** Simple test program to demonstrate writing of data with lcio.
 */

int main(int argc, char** argv ){
  
  try{
    // create sio writer
    LCWriter* lcWrt = LCFactory::getInstance()->createLCWriter() ;
    
    if( argc > 1 ) { FILEN = argv[1] ; }
    
    try{  lcWrt->open( FILEN ) ;
    }
    catch( IOException& e ){    
      cout << e.what() << endl ;
      return 0 ;
    }
//     int status ;
//     if( (status = lcWrt->open( FILEN ))  != LCIO::SUCCESS ) {
//       cout << " couldn't open file  : " << FILEN << " status: " << status << endl ;
//       return 0 ;
//     }
    
    // loop over runs
    for(int rn=0;rn<NRUN;rn++){
      
      LCRunHeaderImpl* runHdr = new LCRunHeaderImpl ; 
      runHdr->setRunNumber( rn ) ;
      
      string detName("D09TileHcal")  ;
      runHdr->setDetectorName( detName ) ;
      
      stringstream description ; 
      description << " run: " << rn <<" just for testing lcio  - no physics !" ;
      runHdr->setDescription( description.str()  ) ;
      
      string ecalName("ECAL007") ;
      runHdr->addActiveSubdetector( ecalName ) ;
      
      string tpcName("TPC4711") ;
      runHdr->addActiveSubdetector( tpcName ) ;
      
      lcWrt->writeRunHeader( runHdr ) ;
      
      // EventLoop - create some events and write them to the file
      for(int i=0;i<NEVENT;i++){
	
	// we need to use the implementation classes here 
	LCEventImpl*  evt = new LCEventImpl() ;
	
	
	evt->setRunNumber(  rn   ) ;
	evt->setEventNumber( i ) ;
	evt->setDetectorName( detName ) ;
	
	// create and add some mc particles 
	LCCollectionVec* mcVec = new LCCollectionVec( LCIO::MCPARTICLE )  ;
	
	MCParticle* mom = 0 ;
	for(int j=0;j<NMCPART;j++){
	  MCParticleImpl* mcp = new MCParticleImpl ;
	  
	  mcp->setPDG( 101 + j*100  ) ;
	  mcp->setParent(  mom  );
	  
	  float p[3] = { 2./1024. , 4./1024. , 8./1024. } ;
	  mcp->setMomentum( p ) ;
	  
	  mom =  mcp ;  // one body decays :-)
	  mcVec->push_back( mcp ) ;
	}
	
	// now add some calorimeter hits
	LCCollectionVec* calVec = new LCCollectionVec( LCIO::SIMCALORIMETERHIT )  ;
      
	// set flag for long format (including position )
	// and PDG 
	LCFlagImpl chFlag(0) ;
	chFlag.setBit( LCIO::CHBIT_LONG ) ;
	chFlag.setBit( LCIO::CHBIT_PDG ) ;
	calVec->setFlag( chFlag.getFlag()  ) ;
	
	
	for(int j=0;j<NHITS;j++){
	  
	  SimCalorimeterHitImpl* hit = new SimCalorimeterHitImpl ;
	  
	  hit->setEnergy( 3.1415 * rand()/RAND_MAX   )  ;
	  
	  float pos[3] = { 1.1* rand()/RAND_MAX , 2.2* rand()/RAND_MAX , 3.3* rand()/RAND_MAX } ;
	  
	  hit->setPosition( pos ) ;
	  
	  calVec->push_back( hit ) ;
	  
	  // assign the hits randomly to MC particles
	  float rn =  .99999*rand()/RAND_MAX ;
	  int mcIndx = static_cast<int>( NMCPART * rn ) ;
	  
	  // in order to access a MCParticle,  we need a dynamic cast as the 
	  // LCCollection returns an LCIOObject - this is like vectors in Java 
	  hit->addMCParticleContribution(  dynamic_cast<const MCParticle*>(mcVec->getElementAt( mcIndx )) , 
					   0.314159, 0.1155 ) ; // no pdg
	  
	}
	
	// and finally some tracker hits
	// with some user extensions (4 floats) per track:
	// we just create a parallel collection of float vectors
	LCCollectionVec* trkVec = new LCCollectionVec( LCIO::SIMTRACKERHIT )  ;
	LCCollectionVec* extVec = new LCCollectionVec( LCIO::LCFLOATVEC )  ;
	
	for(int j=0;j<NHITS;j++){
	  
	  SimTrackerHitImpl* hit = new SimTrackerHitImpl ;
	  LCFloatVec* ext = new LCFloatVec ;
	  
	  
	  hit->setdEdx( 30e-9 ) ; 
	  
	  double pos[3] = { 1.1* rand()/RAND_MAX , 2.2* rand()/RAND_MAX , 3.3* rand()/RAND_MAX } ;
	  
	  // fill the extension vector
	  ext->push_back( 3.14159 ) ; 
	  for(int k=0;k<3;k++) ext->push_back(  pos[k] * 0.1  ) ;
	  
	  hit->setPosition( pos ) ; 
	  
	  // assign the hits randomly to MC particles
	  float rn =  .99999*rand()/RAND_MAX ;
	  int mcIndx = static_cast<int>( NMCPART * rn ) ;
	  
	  hit->setMCParticle( dynamic_cast<const MCParticle*>(mcVec->getElementAt( mcIndx ) ) ) ;
	  
	  
	  trkVec->push_back( hit ) ;
	  extVec->push_back( ext ) ;
	}
	
	
	// add all collection to the event
	evt->addCollection( (LCCollection*) mcVec , "MCParticle" ) ;
	evt->addCollection( (LCCollection*) calVec , ecalName ) ;
	evt->addCollection( (LCCollection*) trkVec , tpcName ) ;
	evt->addCollection( (LCCollection*) extVec , tpcName+"UserExtension" ) ;
	
	
	
	// test: add a collection for one event only:
	if( rn == NRUN-1 && i == 0 ) { // first event oif last run
	  LCCollectionVec* addExtVec = new LCCollectionVec( LCIO::LCFLOATVEC )  ;
	  LCFloatVec* addExt = new LCFloatVec ;
	  addExt->push_back( 1. );
	  addExt->push_back( 2. );
	  addExt->push_back( 3. );
	  addExt->push_back( 4. );
	  addExtVec->push_back( addExt ) ;
	  evt->addCollection( (LCCollection*) addExtVec , "AdditionalExtension" ) ;
	}


      
	// write the event to the file
	lcWrt->writeEvent( evt ) ;
	
	// dump the event to the screen 
	LCTOOLS::dumpEvent( evt ) ;
	
	
	// we created the event so we need to take care of deleting it ...
	delete evt ;
	
	if( ! (i%100) ) cout << ". " << flush  ;
	
      } // evt loop
    } // run loop
    
    cout << endl 
	 << " created  " << NRUN << " runs with  " << NRUN*NEVENT << " events"  
	 << endl << endl ;
    
    
    
    lcWrt->close() ;
  
  } catch( Exception& ex){

    cout << " an excpetion occured: " << endl ;
    cout << "  " << ex.what() << endl ;
    return 1 ;
  }

  return 0 ;
}

