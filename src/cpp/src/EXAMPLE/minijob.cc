
#include "lcio.h"

#include "IO/LCWriter.h"
#include "EVENT/LCIO.h"
#include "DATA/LCFloatVec.h"

#include "IMPL/LCEventImpl.h" 
#include "IMPL/LCRunHeaderImpl.h" 
#include "IMPL/LCCollectionVec.h"
#include "IMPL/SimCalorimeterHitImpl.h"
//#include "IMPL/SimTrackerHitImpl.h"
#include "IMPL/MCParticleImpl.h" 
#include "IMPL/LCFlagImpl.h" 
#include "IMPL/LCTOOLS.h"

#include <cstdlib>
#include <iostream>


using namespace std ;
using namespace lcio ;

static const int NRUN = 1 ;
static const int NEVENT = 1 ; // events
static const int NMCPART = 1 ;  // mc particles per event
static const int NHITS = 1 ;  // calorimeter hits per event

static string FILEN = "minijob.sio" ;


/** Simple test program to demonstrate writing of data with lcio.
 * mininmal data file for debugging
 */

int main(int argc, char** argv ){
  
  // read file name from command line (only argument) 
  if( argc > 1 ) {
    FILEN = argv[1] ;
  }
  
  // create sio writer
  LCWriter* lcWrt = LCFactory::getInstance()->createLCWriter() ;
  
  try{  lcWrt->open( FILEN ) ;
  }
  catch( IOException& e ){    cout << e.what() << endl ;
    return 0 ;
  }

  // loop over runs
  for(int rn=0;rn<NRUN;rn++){
    
    LCRunHeaderImpl* runHdr = new LCRunHeaderImpl ; 
    runHdr->setRunNumber( rn ) ;
    
    string detName("MiniDet")  ;
    runHdr->setDetectorName( detName ) ;

    string description("minimal file") ;
    runHdr->setDescription( description ) ;
    
    string ecalName("ECALmini") ;
    runHdr->addActiveSubdetector( ecalName ) ;

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
	
	mcp->setPDG( 101 + j*100 ) ;
	mcp->setParent(  mom  );
	
	float p[3] = { 2.*1024. , 4.*1024. , 8.*1024. } ;
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
	
	hit->setEnergy( 3.1415 )  ;
	
	float pos[3] = { 1. , 2. , 4. } ;
	
	hit->setPosition( pos ) ;
	
	calVec->push_back( hit ) ;
	

	hit->addMCParticleContribution(  dynamic_cast<MCParticle*>(mcVec->getElementAt(0)) , 
					 1. , 2. , 4 ) ; 
	
      }
      
      evt->addCollection( (LCCollection*) mcVec , "MCParticle" ) ;
      evt->addCollection( (LCCollection*) calVec , ecalName ) ;
      
      // dump the event to the screen 
      LCTOOLS::dumpEvent( evt ) ;
      
      // write the event to the file
      lcWrt->writeEvent( evt ) ;
      
      // we created the event so we need to take care of deleting it ...
      delete evt ;
      
    } // evt loop
  } // run loop
    
  cout << endl 
       << " created  " << NRUN << " runs with  " << NRUN*NEVENT << " events"  
       << endl << endl ;

  lcWrt->close() ;
  
  return 0 ;
}

