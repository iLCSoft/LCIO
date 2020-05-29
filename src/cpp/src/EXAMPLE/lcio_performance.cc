#include "lcio.h"

#include "IO/LCWriter.h"
#include "IO/LCReader.h"
#include "IO/LCEventListener.h"
#include "MT/LCReader.h"
#include "MT/LCReaderListener.h"

#include "EVENT/LCIO.h"
#include "DATA/LCFloatVec.h"
#include "DATA/LCIntVec.h"

#include "IMPL/LCEventImpl.h" 
#include "IMPL/LCRunHeaderImpl.h" 
#include "IMPL/LCCollectionVec.h"
#include "IMPL/SimCalorimeterHitImpl.h"
#include "IMPL/SimTrackerHitImpl.h"
#include "IMPL/MCParticleImpl.h" 
//#include "IMPL/LCFlagImpl.h" 
#include "IMPL/LCTOOLS.h"

#include "IMPL/TrackerRawDataImpl.h"
#include "IMPL/TrackerDataImpl.h"
#include "IMPL/TrackerPulseImpl.h"

#include "UTIL/LCRelationNavigator.h"
#include "UTIL/LCTime.h"
//#include "UTIL/BitField64.h"
#include "UTIL/CellIDEncoder.h"
#include "UTIL/LCTypedVector.h"
#include "UTIL/LCSplitWriter.h"
#include "UTIL/BitSet32.h"

// #include "UTIL/LCIOTypeInfo.h"


#include <cstdlib>
#include <iostream>
#include <sstream>


using namespace std ;
using namespace lcio ;

static const int NRUN = 50 ;
static const int NEVENT = 100 ; // events
static const int NMCPART = 100 ;  // mc particles per event
static const int NHITS = 50 ;  // calorimeter hits per event

static string FILEN = "lcioperf.slcio" ;


class ReadPerformanceProcessorMT : public MT::LCReaderListener {
  
public:
  ReadPerformanceProcessorMT(const ReadPerformanceProcessorMT &) = delete ;
  ReadPerformanceProcessorMT& operator=(const ReadPerformanceProcessorMT &) = delete ;
  
  ReadPerformanceProcessorMT( MT::LCReader *reader ) : 
    _lcReader(reader) {
    
  }
  
  void processEvent( MT::LCEventPtr ) {
    clock_t endTime = clock() ;
    _totalReadTime += (endTime - _startTime) / static_cast<double>( CLOCKS_PER_SEC ) ;
    _nEvents ++;
    _startTime = clock() ;
  }
    
  void processRunHeader( MT::LCRunHeaderPtr  ) {}
  
  void readStream() {
    _startTime = clock() ;
    _lcReader->readStream( this ) ;
    std::cout << "Read " << _nEvents << " events" << std::endl ;
    std::cout << "Total read time: " << _totalReadTime << " secs" << std::endl ;
    std::cout << "Mean read time:  " << _totalReadTime / _nEvents << " secs / event" << std::endl ;
  }
  
private:
  clock_t         _startTime {0} ;
  double          _totalReadTime {0.} ;
  unsigned int    _nEvents {0} ;
  MT::LCReader   *_lcReader {nullptr} ;
};



class ReadPerformanceProcessor : public LCEventListener {
  
public:
  ReadPerformanceProcessor(const ReadPerformanceProcessor &) = delete ;
  ReadPerformanceProcessor& operator=(const ReadPerformanceProcessor &) = delete ;
  
  ReadPerformanceProcessor( LCReader *reader ) : 
    _lcReader(reader) {
    
  }
  
  void processEvent( LCEvent* ) {
    clock_t endTime = clock() ;
    _totalReadTime += (endTime - _startTime) / static_cast<double>( CLOCKS_PER_SEC ) ;
    _nEvents ++;
    _startTime = clock() ;
  }
    
  void modifyEvent( LCEvent* ) {}
  
  void readStream() {
    _startTime = clock() ;
    _lcReader->readStream() ;
    std::cout << "Read " << _nEvents << " events" << std::endl ;
    std::cout << "Total read time: " << _totalReadTime << " secs" << std::endl ;
    std::cout << "Mean read time:  " << _totalReadTime / _nEvents << " secs / event" << std::endl ;
  }
  
private:
  clock_t         _startTime {0} ;
  double          _totalReadTime {0.} ;
  unsigned int    _nEvents {0} ;
  LCReader       *_lcReader {nullptr} ;
};

//struct MyTrackLink : public LCObjectLinkTraits< SimTrackerHit, MyTrackLink > {} ;

/** Performance profiling of LCIO writer and reader
 */

int main(int argc, char** argv ){
  
  double totalWritingTime = 0. ;
  
  if( argc > 1 ) { FILEN = argv[1] ; }
  
  try {

    // create sio writer
    LCWriter* lcWrt = LCFactory::getInstance()->createLCWriter()  ;
    lcWrt->setCompressionLevel( 1 ) ;             
    lcWrt->open( FILEN , LCIO::WRITE_NEW )  ;
    
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
	LCTime now ;
	evt->setTimeStamp( now.timeStamp()  ) ;
	evt->setDetectorName( detName ) ;
	
	evt->setWeight( 1.*rand()/RAND_MAX ) ;

	evt->parameters().setValue("Description"," event can have it's own set of parameters" ) ;
	evt->parameters().setValue("Thrust", (float) 0.671 ) ;

 	FloatVec fv ;
 	fv.push_back( 1.1 ) ;
 	fv.push_back( 2.2 ) ;
 	fv.push_back( 3.3 ) ;
	evt->parameters().setValues( "SomeNumbers" , fv ) ; 
	
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
	  float p[3] = { float(j*1.) , float(4./1024.) , float(8./1024.) } ;
	  mcp->setMomentum( p ) ;
	  mcp->setMass( .135 ) ;

	  // create and add some daughters
	  for(int k=0;k<3;k++){
	    MCParticleImpl* d1 = new MCParticleImpl ;

	    d1->setPDG( 1000 * (j+1) + 100 * (k+1)  ) ;
	    float pd1[3] = {  float(k*1.) ,  float(4.1) ,  float(8.1) } ;
	    d1->setMomentum( pd1 ) ;
	    d1->setMass( .135 ) ;

	    for(int l=0;l<2;l++){
	      MCParticleImpl* d2 = new MCParticleImpl ;
	      
	      d2->setPDG( 1000 * (j+1) + 100 * (k+1) + 10 *  (l+1)  ) ;
	      float pd2[3] = {  float(l*1.) ,  float(0.41) ,  float(4.1) } ;
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
	// LCFlagImpl chFlag(0) ;
	// chFlag.setBit( LCIO::CHBIT_LONG ) ;
	// chFlag.setBit( LCIO::CHBIT_STEP ) ;
	// calVec->setFlag( chFlag.getFlag()  ) ;
	
	calVec->setFlag( UTIL::make_bitset32(  LCIO::CHBIT_LONG, LCIO::CHBIT_STEP ) );



	std::string cellIDEncoding( "M:3,S-1:3,I:9,J:9,K-1:6") ;// old Mokka convention

// 	std::string cellIDEncoding( "M:3,S-1:3,I:9,J:9,K-1:6,Bla:34:6") ;// for testing cellid1

	CellIDEncoder<SimCalorimeterHitImpl> b( cellIDEncoding , calVec ) ;

	for(int j=0;j<NHITS;j++){
	  
	  SimCalorimeterHitImpl* hit = new SimCalorimeterHitImpl ;
	  
	  hit->setEnergy( 3.1415 * rand()/RAND_MAX   )  ;
	  
	  float pos[3] = {  float(1.1* rand()/RAND_MAX) ,  float(2.2* rand()/RAND_MAX) ,  float(3.3* rand()/RAND_MAX) } ;
	  
	  // cell indices
	  b["M"] = j % 8 ;
	  b["S-1"] = (j+2) % 8 ;
	  b["I"] = j % 512 ;
	  b["J"] = (j+128) % 512 ;
	  b["K-1"] = (j+32) % 64 ;

	  b.setCellID( hit ) ;

// 	  hit->setCellID0( b.lowWord()  ) ;
// 	  hit->setCellID1( b.highWord() ) ;
	  
	  hit->setPosition( pos ) ;
	  
	  calVec->push_back( hit ) ;
	  
	  // assign the hits randomly to MC particles
	  double rnd =   .99999*rand()/RAND_MAX ;
	  int mcIndx = static_cast<int>( NMCPART * rnd ) ;
	  
	  // in order to access a MCParticle,  we need a dynamic cast as the 
	  // LCCollection returns an LCIOObject - this is like vectors in Java 
	  hit->addMCParticleContribution(  dynamic_cast<MCParticle*>(mcVec->getElementAt( mcIndx )) , 
					   0.314159, 0.1155, 42.,  1, pos ) ;
	  
	}
	
	// -------- data can be modified as long as is not not made persistent --------

	for(int j=0;j<NHITS;j++){
	  SimCalorimeterHitImpl* existingHit 
	    = dynamic_cast<SimCalorimeterHitImpl*>( calVec->getElementAt(j) ) ; // << Ok now

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

	// LCFlagImpl thFlag(0) ;
	// thFlag.setBit( LCIO::THBIT_MOMENTUM ) ;
	// trkVec->setFlag( thFlag.getFlag()  ) ;
	trkVec->setFlag( UTIL::make_bitset32( LCIO::THBIT_MOMENTUM ) ) ; 

	LCTypedVector<MCParticle> mcpTV( mcVec ) ;

	CellIDEncoder<SimTrackerHitImpl> cd( "i:8,j:8,k:8" ,trkVec )  ;
	
	for(int j=0;j<NHITS;j++){
	  
	  SimTrackerHitImpl* hit = new SimTrackerHitImpl ;

	  cd["i"] = j ;
	  cd["j"] = j + 100 ;
	  cd["k"] = j + 200 ;

	  cd.setCellID( hit ) ;

	  LCFloatVec* extF = new LCFloatVec ;
	  LCIntVec*   extI = new LCIntVec ;
	  
	  //hit->setdEdx( 30e-9 ) ; 
	  hit->setEDep( 30e-9 ) ; 

	  double pos[3] = { 1.1* rand()/RAND_MAX , 2.2* rand()/RAND_MAX , 3.3* rand()/RAND_MAX } ;
	  
	  hit->setPosition( pos ) ; 
	  
	  // assign the hits randomly to MC particles
	  float rnd =  .99999*rand()/RAND_MAX ;
	  int mcIndx = static_cast<int>( NMCPART * rnd ) ;
	  

// 	  hit->setMCParticle( dynamic_cast<MCParticle*>(mcVec->getElementAt( mcIndx ) ) ) ;
	  hit->setMCParticle( mcpTV[ mcIndx ]  ) ;
	  
	  hit->setMomentum( 1. , 2. , 3. ) ; 
	  hit->setPathLength( .042 ) ;

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
 	//---- write a subset of MCParticle to the event ------
 	LCCollectionVec* mcSubVec = new LCCollectionVec( LCIO::MCPARTICLE )  ;
 	mcSubVec->setSubset(true) ;
	
 	for(int j=0;j< mcVec->getNumberOfElements() ; j++ ){
	  
 	  MCParticle* p = dynamic_cast< MCParticle*>( mcVec->getElementAt(j) )  ;
	  if( p->getDaughters().size() == 0 )
	    mcSubVec->addElement( p ) ;
 	}
 	evt->addCollection( mcSubVec , "FinalMCParticles" ) ;
 	//-----------------------------------------------------



	// even though this is a simjob we can store 'real data' objects :)

#define WRITE_TRACKERRAWDATA 1
#ifdef WRITE_TRACKERRAWDATA
	//--- write some new TPC raw data collections to the file 
	LCCollectionVec* tpcRawVec = new LCCollectionVec( LCIO::TRACKERRAWDATA )  ;
	
	for(int j=0;j<NHITS;j++){

	  TrackerRawDataImpl* tpcRaw = new TrackerRawDataImpl ;
	  
	  tpcRaw->setCellID0( j ) ;
	  tpcRaw->setTime( -j  ) ;
	  
	  if( j % 2 ) { // test two ways of setting the charge
	    ShortVec adcValues ;
	    adcValues.push_back( 42 ) ;
	    adcValues.push_back( 43 ) ;
	    adcValues.push_back( 44 ) ;
	    adcValues.push_back( 45 ) ;
	    tpcRaw->setADCValues( adcValues ) ;
	  } else {
	    tpcRaw->adcValues().push_back( 42 ) ;
	    tpcRaw->adcValues().push_back( 43 ) ;
	    tpcRaw->adcValues().push_back( 44 ) ;
	    tpcRaw->adcValues().push_back( 45 ) ;
	  }
	  tpcRawVec->addElement( tpcRaw ) ;
	}
	evt->addCollection( tpcRawVec , "TrackerRawDataExample" ) ;

	//---- test new relation navigator object
	LCRelationNavigator relNav( LCIO::TRACKERRAWDATA, LCIO::SIMTRACKERHIT ) ;
	
	for(int j=0;j<NHITS;j++){
	  relNav.addRelation( tpcRawVec->getElementAt(j) , trkVec->getElementAt(j) , 0.42 ) ;
	  
//   	  tpcRawVec->getElementAt(j)->link<MyTrackLink>() = 
//  	  (*tpcRawVec)[j]->link< MyTrackLink >()  =
// 	    dynamic_cast<SimTrackerHit*>( (*trkVec)[j] );

	}
	evt->addCollection( relNav.createLCCollection() , "TPCRawFADCMCTruth" ) ;
	

	//------ corrected data
	
	LCCollectionVec* tpcCorrectedVec = new LCCollectionVec( LCIO::TRACKERDATA )  ;
	
	for(int j=0;j<NHITS;j++){

	  TrackerDataImpl* tpcCorrected = new TrackerDataImpl ;
	  
	  tpcCorrected->setCellID0( j ) ;
	  tpcCorrected->setTime( -j  ) ;
	  
	  tpcCorrected->chargeValues().push_back( 42.12345 ) ;
	  tpcCorrected->chargeValues().push_back( 43.09876 ) ;
	  tpcCorrected->chargeValues().push_back( 44.12345 ) ;
	  tpcCorrected->chargeValues().push_back( 45.09876 ) ;

	  tpcCorrectedVec->addElement( tpcCorrected ) ;
	}
	evt->addCollection( tpcCorrectedVec , "TrackerDataExample" ) ;
	
	// ------ pulses

	LCCollectionVec* tpcPulseVec = new LCCollectionVec( LCIO::TRACKERPULSE )  ;
	
	IntVec qualityBits ;
	qualityBits.push_back(0) ;
	qualityBits.push_back(1) ;

	StringVec bitNames ;
	bitNames.push_back("GOOD") ;
	bitNames.push_back("BAD") ;

	tpcPulseVec->parameters().setValues("TrackerPulseQualityNames", bitNames ); 
	tpcPulseVec->parameters().setValues("TrackerPulseQualityValues", qualityBits ); 
	
	for(int j=0;j<NHITS;j++){

	  TrackerPulseImpl* tpcPulse = new TrackerPulseImpl ;
	  
	  tpcPulse->setCellID0( j ) ;
	  tpcPulse->setTime( 3.1415 + 0.1 * j  ) ;
	  tpcPulse->setCharge( 3.1415 - 0.1 * j  ) ;

	  if( j % 2 ) {
	    tpcPulse->setQualityBit( qualityBits[0] ) ;
	  } else {

	    tpcPulse->setQualityBit( qualityBits[1] ) ;
	    
	    TrackerData* corr = 
	      dynamic_cast<TrackerData*> ( tpcCorrectedVec->getElementAt(j) ) ; 
	    tpcPulse->setTrackerData( corr ) ;
	  }

	  tpcPulseVec->addElement( tpcPulse ) ;
	}
	evt->addCollection( tpcPulseVec , "TrackerPulseExample" ) ;

	//-----------------------------------------------------
#endif // WRITE_TRACKERRAWDATA

#define WRITE_VTXRAWHITS 1
#ifdef WRITE_VTXRAWHITS

	//--- write some VTX raw hits  to the file - using the TrackerPulse
	LCCollectionVec* vtxRawVec = new LCCollectionVec( LCIO::TRACKERPULSE )  ;
	
	for(int j=0;j<NHITS;j++){

	  TrackerPulseImpl* vtxRaw = new TrackerPulseImpl ;
	  
	  vtxRaw->setCellID0( 0xBebaFeca ) ;
	  vtxRaw->setCellID1( 0xCafeBabe ) ;
	  vtxRaw->setTime( j  ) ;
	  vtxRaw->setCharge( 42 + j  ) ;
	  
	  vtxRawVec->addElement( vtxRaw ) ;
	}
	evt->addCollection( vtxRawVec , "SiliconRawHitExample" ) ;

	//-----------------------------------------------------
#endif // WRITE_VTXRAWHITS



	// write the event to the file
  clock_t start = clock() ;
	lcWrt->writeEvent( evt ) ;
  clock_t end = clock() ;
  
  totalWritingTime += (end - start) / static_cast<double>( CLOCKS_PER_SEC ) ;

	// ------------ IMPORTANT ------------- !
	// we created the event so we need to delete it ...
	delete evt ;
	// -------------------------------------

      } // evt loop

      delete runHdr ;

    } // run loop
    
    lcWrt->close() ;
    delete lcWrt ;
    
    std::cout << "Written " << NRUN << " runs and " << NRUN*NEVENT << " events" << std::endl ;
    std::cout << "Total writing time: " << totalWritingTime << " secs" << std::endl ;
    std::cout << "Mean writing time:  " << totalWritingTime / (NRUN*NEVENT) << " secs / event" << std::endl ;
    
    {
      // create reader and writer for input and output streams 
      LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
      lcReader->open( FILEN ) ;
      ReadPerformanceProcessor listener( lcReader ) ;
      lcReader->registerLCEventListener( &listener ) ;
      std::cout << "With standard LCReader: " << std::endl ;
      listener.readStream() ;      
      delete lcReader ;
    }

    
    {
      MT::LCReader mtReader( MT::LCReader::directAccess ) ;
      mtReader.open( FILEN ) ;
      ReadPerformanceProcessorMT listener( &mtReader ) ;
      std::cout << "With MT LCReader: " << std::endl ;
      listener.readStream() ;
    }
    
    {
      MT::LCReader mtReader( MT::LCReader::directAccess | MT::LCReader::lazyUnpack ) ;
      mtReader.open( FILEN ) ;
      ReadPerformanceProcessorMT listener( &mtReader ) ;
      std::cout << "With Lazy MT LCReader: " << std::endl ;
      listener.readStream() ;
    }
    
    
  } catch( Exception& ex){

    cout << " an excpetion occured: " << endl ;
    cout << "  " << ex.what() << endl ;
    return 1 ;
  }

  return 0 ;
}

