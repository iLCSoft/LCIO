
#include "lcio.h"

#include "IO/LCWriter.h"
#include "EVENT/LCIO.h"
#include "DATA/LCFloatVec.h"
#include "DATA/LCIntVec.h"

#include "IMPL/LCEventImpl.h" 
#include "IMPL/LCRunHeaderImpl.h" 
#include "IMPL/LCCollectionVec.h"
#include "IMPL/SimCalorimeterHitImpl.h"
#include "IMPL/SimTrackerHitImpl.h"
#include "IMPL/MCParticleImpl.h" 
#include "IMPL/LCFlagImpl.h" 
#include "IMPL/LCTOOLS.h"
#include "IMPL/TPCHitImpl.h"
#include "IMPL/LCRelationNavigatorImpl.h"

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
    
    // loop over runs
    for(int rn=0;rn<NRUN;rn++){
      
      // create sio writer
      LCWriter* lcWrt = LCFactory::getInstance()->createLCWriter() ;
      
      if( argc > 1 ) { FILEN = argv[1] ; }
      
      if( rn==0 )
	lcWrt->open( FILEN , LCIO::WRITE_NEW )  ;
      else
	lcWrt->open( FILEN , LCIO::WRITE_APPEND )  ;

      // NB: in order to test writing multiple files we create a new LCWriter
      // for every run even though we are in fact writing to one file only;
      // so for a simple job writing one file the 
      // 'createLCWriter/open' and 'close/delete' will be outside the run loop...


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
      

      // add some parameters to the run header 
//       StringVec sv1 ;
//       sv1.push_back("simjob.cc") ;
//       runHdr->parameters().setValues( "SimulationProgram" , sv1 ) ; 
      runHdr->parameters().setValue( "SimulationProgram" , "simjob.cc" ) ; 
      IntVec iv(3) ;
      iv[0] = 1 ;
      iv[1] = 2 ;
      iv[2] = 3 ;
      runHdr->parameters().setValues( "SomeIndices" , iv ) ; 
      
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
	
	// debug only - don't write MCParticles to output file:
	// mcVec->setTransient() ;

	// debug only - add the same particle to more than one collection
	//LCCollectionVec* mcVec2 = new LCCollectionVec( LCIO::MCPARTICLE )  ;

	MCParticleImpl* mom = new MCParticleImpl ;
	mom->setPDG( 1  ) ;
	float p0[3] = { 0. , 0. , 1000. } ;
	mom->setMomentum( p0 ) ;
	mom->setMass( 3.01 ) ;

	for(int j=0;j<NMCPART;j++){

	  MCParticleImpl* mcp = new MCParticleImpl ;

	  mcp->setPDG( 1000 * (j+1)  ) ;
	  float p[3] = { j*1. , 4./1024. , 8./1024. } ;
	  mcp->setMomentum( p ) ;
	  mcp->setMass( .135 ) ;

	  // create and add some daughters
	  for(int k=0;k<3;k++){
	    MCParticleImpl* d1 = new MCParticleImpl ;

	    d1->setPDG( 1000 * (j+1) + 100 * (k+1)  ) ;
	    float pd1[3] = { k*1. , 4.1 , 8.1 } ;
	    d1->setMomentum( pd1 ) ;
	    d1->setMass( .135 ) ;

	    for(int l=0;l<2;l++){
	      MCParticleImpl* d2 = new MCParticleImpl ;
	      
	      d2->setPDG( 1000 * (j+1) + 100 * (k+1) + 10 *  (l+1)  ) ;
	      float pd2[3] = { l*1. , 0.41 , 4.1 } ;
	      d2->setMomentum( pd2 ) ;
	      d2->setMass( .135 ) ;
	      
	      double ep[3] = { 1.111111 , 2.2222222, 3.3333333 } ;
	      d2->setEndpoint( ep ) ;
	      //	      d2->setSimulatorStatus( 1234 ) ;
	      d2->setCreatedInSimulation(true) ;
	      d2->setBackscatter(true)         ;
	      d2->setDecayedInTracker(true)    ;
	      d2->setDecayedInCalorimeter(false);
	      d2->setHasLeftDetector(false)     ;
	      d2->setStopped(true)             ;

	      d2->addParent( d1 );
	      mcVec->push_back( d2 ) ;

	      // debug only - add the same particle to more than one collection
	      //mcVec2->push_back( d2 ) ;
	    }
	    d1->addParent( mcp );
	    mcVec->push_back( d1 ) ;
	  }
	  
	  mcp->addParent( mom );
	  mcVec->push_back( mcp ) ;
	}
	mcVec->push_back( mom ) ;
	
	// now add some calorimeter hits
	LCCollectionVec* calVec = new LCCollectionVec( LCIO::SIMCALORIMETERHIT )  ;
      
	// set flag for long format (including position )
	// and PDG and cellid1
	LCFlagImpl chFlag(0) ;
	chFlag.setBit( LCIO::CHBIT_LONG ) ;
	chFlag.setBit( LCIO::CHBIT_PDG ) ;
	chFlag.setBit( LCIO::CHBIT_ID1 ) ;
	calVec->setFlag( chFlag.getFlag()  ) ;
	
	
	for(int j=0;j<NHITS;j++){
	  
	  SimCalorimeterHitImpl* hit = new SimCalorimeterHitImpl ;
	  
	  hit->setEnergy( 3.1415 * rand()/RAND_MAX   )  ;
	  
	  float pos[3] = { 1.1* rand()/RAND_MAX , 2.2* rand()/RAND_MAX , 3.3* rand()/RAND_MAX } ;
	  
	  hit->setCellID0( 1024 ) ;
	  hit->setCellID1( 65535 ) ;

	  hit->setPosition( pos ) ;
	  
	  calVec->push_back( hit ) ;
	  
	  // assign the hits randomly to MC particles
	  float rn =  .99999*rand()/RAND_MAX ;
	  int mcIndx = static_cast<int>( NMCPART * rn ) ;
	  
	  // in order to access a MCParticle,  we need a dynamic cast as the 
	  // LCCollection returns an LCIOObject - this is like vectors in Java 
	  hit->addMCParticleContribution(  dynamic_cast<MCParticle*>(mcVec->getElementAt( mcIndx )) , 
					   0.314159, 0.1155 ) ; // no pdg
	  
	}
	
	// -------- data can be modified as long as is not not made persistent --------

	for(int j=0;j<NHITS;j++){
	  SimCalorimeterHitImpl* existingHit 
	    = dynamic_cast<SimCalorimeterHitImpl*>( calVec->getElementAt(j) ) ; // << Ok now

 	  //	    = dynamic_cast<SimCalorimeterHitImpl*>( (*calVec)[j] ) ;  // << not needed 
	  
	  existingHit->addMCParticleContribution( dynamic_cast<MCParticle*>
						  (mcVec->getElementAt(0)), 
						  0.1, 0. ) ;
	}

	// and finally some tracker hits
	// with some user extensions (4 floats and 2 ints) per track:
	// we just need to create parallel collections of float and int vectors
	LCCollectionVec* trkVec = new LCCollectionVec( LCIO::SIMTRACKERHIT )  ;
	LCCollectionVec* extFVec = new LCCollectionVec( LCIO::LCFLOATVEC )  ;
	LCCollectionVec* extIVec = new LCCollectionVec( LCIO::LCINTVEC )  ;
	
	for(int j=0;j<NHITS;j++){
	  
	  SimTrackerHitImpl* hit = new SimTrackerHitImpl ;
	  LCFloatVec* extF = new LCFloatVec ;
	  LCIntVec*   extI = new LCIntVec ;
	  
	  hit->setdEdx( 30e-9 ) ; 

	  double pos[3] = { 1.1* rand()/RAND_MAX , 2.2* rand()/RAND_MAX , 3.3* rand()/RAND_MAX } ;
	  
	  hit->setPosition( pos ) ; 
	  
	  // assign the hits randomly to MC particles
	  float rn =  .99999*rand()/RAND_MAX ;
	  int mcIndx = static_cast<int>( NMCPART * rn ) ;
	  
	  hit->setMCParticle( dynamic_cast<MCParticle*>(mcVec->getElementAt( mcIndx ) ) ) ;
	  
	  
	  // fill the extension vectors (4 floats, 2 ints)
	  extF->push_back( 3.14159 ) ;  
	  for(int k=0;k<3;k++) extF->push_back(  pos[k] * 0.1  ) ;

	  extI->push_back( 123456789 ) ;
	  extI->push_back( mcIndx ) ;

	  // add the hit and the extensions to their corresponding collections
	  trkVec->push_back( hit ) ;
	  extFVec->push_back( extF ) ;
	  extIVec->push_back( extI ) ;
	}
	
	
	// add all collections to the event
	evt->addCollection( mcVec , "MCParticle" ) ;

	//deubg only 
	//evt->addCollection( mcVec2, "MCParticle2" ) ;

	evt->addCollection( calVec , ecalName ) ;
	evt->addCollection( trkVec , tpcName ) ;
	evt->addCollection( extFVec , tpcName+"UserFloatExtension" ) ;
	evt->addCollection( extIVec , tpcName+"UserIntExtension" ) ;
	
	// test: add a collection for one event only:
// 	if( rn == NRUN-1 && i == 0 ) { // first event o last run
	if( rn == 1 && i == 0 ) { // first event o last run
	  LCCollectionVec* addExtVec = new LCCollectionVec( LCIO::LCFLOATVEC )  ;
	  LCFloatVec* addExt = new LCFloatVec ;
	  addExt->push_back( 1. );
	  addExt->push_back( 2. );
	  addExt->push_back( 3. );
	  addExt->push_back( 4. );
	  addExtVec->push_back( addExt ) ;
	  evt->addCollection( addExtVec , "AdditionalExtension" ) ;
	}

	// even though this is a simjob we can store 'real data' objects :)
	// --- for example we can store TPC hits ------------

	LCCollectionVec* TPCVec = new LCCollectionVec( LCIO::TPCHIT )  ;

	//---- test new relation navigator object
	LCRelationNavigatorImpl* relNav = 
	  new LCRelationNavigatorImpl( LCIO::TPCHIT, LCIO::SIMTRACKERHIT ) ;

	bool storeRawData = true ;

	LCFlagImpl tpcFlag(0) ;
	if(  storeRawData )  // if we want to store the raw data we need to set the flag
	  tpcFlag.setBit( LCIO::TPCBIT_RAW ) ;
	  tpcFlag.setBit( LCIO::TPCBIT_PTR ) ;
	TPCVec->setFlag( tpcFlag.getFlag()  ) ;
	
	for(int j=0;j<NHITS;j++){
	  
	  TPCHitImpl* tpcHit = new TPCHitImpl ;
	  
	  //---- test new relation navigator object
	  relNav->addRelation( tpcHit , trkVec->getElementAt(j) , 0.95  ) ;
	  
	  tpcHit->setCellID( j ) ;
	  tpcHit->setTime( 0.1234567 ) ;
	  tpcHit->setCharge( 3.14159 ) ;
	  tpcHit->setQuality(  0xbad ) ;

	  if(  storeRawData ) {
	    int rawData[10] ;
	    // fill some random numbers 
	    int size =   int( (double(rand()) / RAND_MAX ) * 10 )  ;   
	    for(int k=0;k<size;k++){
	      rawData[k] = int( (double(rand()) / RAND_MAX ) * INT_MAX ) ;   
	    }

	    tpcHit->setRawData( rawData , size ) ;
	  }

	  TPCVec->push_back( tpcHit ) ;
	}	
	evt->addCollection( TPCVec , "TPCRawFADC" ) ;
	evt->addCollection( relNav->createLCCollection() , "TPCRawFADCMCTruth" ) ;

	//--------------  all for TPC --------------------


	// write the event to the file
	lcWrt->writeEvent( evt ) ;
	
	// dump the event to the screen 
	LCTOOLS::dumpEvent( evt ) ;

	// ------------ IMPORTANT ------------- !
	// we created the event so we need to delete it ...
	delete evt ;
	// -------------------------------------

	//  dont use this (compatibility with Fortran simjob.F)
	//  if( ! (i%100) ) cout << ". " << flush  ;
	
      } // evt loop

      delete runHdr ;

      lcWrt->close() ;
      delete lcWrt ;

    } // run loop
    
    cout << endl 
	 << " created  " << NRUN << " runs with  " << NRUN*NEVENT << " events"  
	 << endl << endl ;
    
    
    
  
  } catch( Exception& ex){

    cout << " an excpetion occured: " << endl ;
    cout << "  " << ex.what() << endl ;
    return 1 ;
  }

  return 0 ;
}

