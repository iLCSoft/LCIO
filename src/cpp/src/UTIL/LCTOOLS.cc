#include "UTIL/LCTOOLS.h"

#include "EVENT/LCCollection.h"
#include "EVENT/SimCalorimeterHit.h"
#include "EVENT/CalorimeterHit.h"
#include "EVENT/SimTrackerHit.h"
#include "EVENT/TPCHit.h"
#include "EVENT/LCIO.h"
#include "EVENT/MCParticle.h"
#include "EVENT/LCFloatVec.h"
#include "EVENT/LCIntVec.h"
#include "IMPL/LCFlagImpl.h"
#include "EVENT/Track.h"
#include "EVENT/Cluster.h"
#include "EVENT/ReconstructedParticle.h"

#include "EVENT/LCRelation.h"
#include "LCIOSTLTypes.h"

#ifdef CLHEP
#include "UTIL/LCFourVector.h"
#endif
#include "UTIL/LCObjectHandle.h"

#include <map>
#include <set>
#include <cstdio>

using namespace std ;
using namespace EVENT ;
using namespace IMPL ;


namespace UTIL {


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
    
      LCCollection* col = evt->getCollection( *name ) ;
    
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
      else if( evt->getCollection( *name )->getTypeName() == LCIO::TPCHIT ){
	  
	printTPCHits( col ) ;

      }
      else if( evt->getCollection( *name )->getTypeName() == LCIO::TRACKERHIT ){
	  
	printTrackerHits( col ) ;

      }
      else if( evt->getCollection( *name )->getTypeName() == LCIO::SIMCALORIMETERHIT ){
	  
	printSimCalorimeterHits( col ) ;

      }
      else if( evt->getCollection( *name )->getTypeName() == LCIO::CALORIMETERHIT ){
	  
	printCalorimeterHits( col ) ;

      }
      else if( evt->getCollection( *name )->getTypeName() == LCIO::LCFLOATVEC ){
	  
	printLCFloatVecs( col ) ;

      }
      else if( evt->getCollection( *name )->getTypeName() == LCIO::LCINTVEC ){
	  
	printLCIntVecs( col ) ;

      }
      else if( evt->getCollection( *name )->getTypeName() == LCIO::TRACK ){
	  
	printTracks( col ) ;

      }
      else if( evt->getCollection( *name )->getTypeName() == LCIO::CLUSTER ){
	  
	printClusters( col ) ;

      }
      else if( evt->getCollection( *name )->getTypeName() == LCIO::RECONSTRUCTEDPARTICLE ){
	  
	printReconstructedParticles( col ) ;

      }



    }

    // now dump the relations in the event

    const StringVec* relVec = evt->getRelationNames() ;
    

    for(StringVec::const_iterator name = relVec->begin() ; name != relVec->end() ; name++){
    
      LCRelation* rel = evt->getRelation( *name ) ;
      
      cout << endl 
	   << " relation name : " << *name 
	   << endl ;
      
      printRelation( rel ) ;
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
    
      LCCollection* col = evt->getCollection( *name ) ;
    
    
      // print SimCalorimeterHit collections:
      if(evt->getCollection( *name )->getTypeName() == LCIO::SIMCALORIMETERHIT ){
      
	int nHits =  col->getNumberOfElements() ;
	cout << nHits << " hits - first hit: " ;
	int nPrint = nHits>0 ? 1 : 0 ;
	int flag = col->getFlag() ;

	if(!nPrint ) cout << endl ;
	for( int i=0 ; i< nPrint ; i++ ){
	
	  SimCalorimeterHit* hit = 
	    dynamic_cast<SimCalorimeterHit*>( col->getElementAt( i ) ) ;
	
	  cout << "    hit -  e: "  << hit->getEnergy() ;

	  if( LCFlagImpl(flag).bitSet( LCIO::CHBIT_LONG ) ){

	    const float* x =  hit->getPosition() ;
	    cout << "  pos: " << x[0] << ", " << x[1] << ", " << x[2] << endl ;   

	  } else{
	    cout << "  short hits: position not available! " << hex << flag << dec << endl ;
	  }
	}
      }
      else if(evt->getCollection( *name )->getTypeName() == LCIO::CALORIMETERHIT ){
	
	int nHits =  col->getNumberOfElements() ;
	cout << nHits << " hits - first hit: " ;
	int nPrint = nHits>0 ? 1 : 0 ;
	int flag = col->getFlag() ;
	
	if(!nPrint ) cout << endl ;
	for( int i=0 ; i< nPrint ; i++ ){
	  
	  CalorimeterHit* hit = 
	    dynamic_cast<CalorimeterHit*>( col->getElementAt( i ) ) ;
	  
	  cout << "    hit -  e: "  << hit->getEnergy() ;

	  if( LCFlagImpl(flag).bitSet( LCIO::CHBIT_LONG ) ){
	    
	    const float* x =  hit->getPosition() ;
	    cout << "  pos: " << x[0] << ", " << x[1] << ", " << x[2] << endl ;   
	    
	  } else{
	    cout << "  short hits: position not available! " << hex << flag << dec << endl ;
	  }
	}
	
      } else if(evt->getCollection( *name )->getTypeName() == LCIO::SIMTRACKERHIT ){
      
	int nHits =  col->getNumberOfElements() ;
	cout << nHits << " hits : " ;
	int nPrint = nHits>0 ? 1 : 0 ;

	if(!nPrint ) cout << endl ;
	for( int i=0 ; i< nPrint ; i++ ){
	  SimTrackerHit* hit = 
	    dynamic_cast<SimTrackerHit*>( col->getElementAt( i ) ) ;
// 
// Since hit may not have MCParticle, need to check that it exists
//
	  int pdgid = 0;
	  if(hit->getMCParticle() > 0)pdgid = 
		 hit->getMCParticle()->getPDG();
      
	
	  const double* x =  hit->getPosition() ;
	  cout << "    hit -  dEdx: " 
	       << hit->getdEdx() 
	       << "  mc: " << pdgid 
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
	  LCFloatVec* vec = 
	    dynamic_cast<LCFloatVec*>( col->getElementAt( i ) ) ;
	  
	  cout << " values(" << i << "): " ;
	  for(unsigned int k=0 ; k< vec->size() ; k++ )
	    cout <<  (*vec)[k]  << ", " ;
	  cout << endl ;   
	  
	}
      } else if(evt->getCollection( *name )->getTypeName() == LCIO::LCINTVEC ){
	
	int nHits =  col->getNumberOfElements() ;
	cout << nHits << " vectors: " ;
	int nPrint = nHits>0 ? 1 : 0 ;
	
	if(!nPrint ) cout << endl ;
	for( int i=0 ; i< nPrint ; i++ ){
	  LCIntVec* vec = 
	    dynamic_cast<LCIntVec*>( col->getElementAt( i ) ) ;
	  
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
	  MCParticle* part = 
	    dynamic_cast<MCParticle*>( col->getElementAt( i ) ) ;
	  
	  cout << "           " << part->getPDG() << " p: " 
	       <<  part->getMomentum()[0]  << ", "
	       <<  part->getMomentum()[1]  << ", "
	       <<  part->getMomentum()[2]  
	       << endl ;	
	  
	}
      } else if( evt->getCollection( *name )->getTypeName() == LCIO::TPCHIT ){
	
	int nHits =  col->getNumberOfElements() ;
	cout << nHits << " hits - first : " ;
	int nPrint = nHits>0 ? 1 : 0 ;
	
	if(!nPrint ) cout << endl ;
	for( int i=0 ; i< nPrint ; i++ ){

	    LCObjectHandle<TPCHit> hit( col->getElementAt( i ) )  ;
	  
	  cout << " id: " << hit->getCellID() 
	       << " t: " << hit->getTime()  
	       << " charge: " << hit->getCharge()  
	       << " quality: " << hit->getQuality()  
	       << endl ;	
	}	
      } else {
	cout << endl ;
      } 
    }
    cout << endl ;
  }


  void LCTOOLS::printTracks(const EVENT::LCCollection* col ){
    if( col->getTypeName() != LCIO::TRACK ){
      
      cout << " collection not of type " << LCIO::TRACK << endl ;
      return ;
    }
    cout << endl 
	 << "--------------- " << "print out of "  << LCIO::TRACK << " collection "
	 << "--------------- " << endl ;
    
    cout << endl 
	 << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;
    
    LCFlagImpl flag( col->getFlag() ) ;
    cout << "     LCIO::TRBIT_HITS : " << flag.bitSet( LCIO::TRBIT_HITS ) << endl ;

    int nTracks =  col->getNumberOfElements() ;
    int nPrint = nTracks > MAX_HITS ? MAX_HITS : nTracks ;
    
    std::cout << endl
	      << " [   id   ] |   type   |    d0    |  phi     | omega    |    z0     | tan lambda|   reference point(x,y,z)        |    dEdx  |  dEdxErr |   chi2   "
	      << endl 
	      << "------------|----------|----------|----------|----------|-----------|-----------|---------------------------------|----------|----------|-------- "
	      << endl ;
    
    for( int i=0 ; i< nPrint ; i++ ){
      
      Track* trk = 
      	dynamic_cast<Track*>( col->getElementAt( i ) ) ;
      
      printf(" [%8.8x] | %8.8x | %4.2e | %4.2e | %4.2e | %5.3e | %5.3e | (%5.3e,%5.3e,%5.3e) | %4.2e | %4.2e | %4.2e \n"
	     , trk->id()
	     , trk->getType() 
	     , trk->getD0() 
	     , trk->getPhi()
	     , trk->getOmega() 
	     , trk->getZ0() 
	     , trk->getTanLambda()
// 	     , trk->getMomentum() 
// 	     , trk->getTheta() 
// 	     , trk->getPhi()
// 	     , trk->getD0() 
// 	     , trk->getZ0() 
	     , trk->getReferencePoint()[0]
	     , trk->getReferencePoint()[1]
	     , trk->getReferencePoint()[2]
	     , trk->getdEdx() 
	     , trk->getdEdxError() 
	     , trk->getChi2() 
	     ) ;
      cout << " errors: " ;
      for(int l=0;l<15;l++){
	printf("%4.2e, ", trk->getCovMatrix()[l] ) ; 
      }
      cout << endl << " tracks(id): " ;
      const TrackVec& tracks = trk->getTracks() ;

      for(unsigned int l=0;l<tracks.size();l++){
		printf("[%8.8x], ",  tracks[l]->id() ) ; 
      }
      cout << endl ;
      if( flag.bitSet( LCIO::TRBIT_HITS ) ) {
	cout << " hits ->" ;
	// 	const StringVec& hitColNames = trk->getHitCollectionNames() ;
	// 	for(unsigned int j=0;j<hitColNames.size();j++){
	// 	  cout << " " << hitColNames[j] << ": " ;
	// 	  const IntVec& hits = trk->getHitIndicesForCollection( hitColNames[j] ) ;
	// 	  for(unsigned int k=0;k<hits.size();k++){
	// 	    cout << hits[k] <<"," ;
	// 	  }
	// 	}
	const TrackerHitVec& hits= trk->getTrackerHits() ; 
	for(unsigned int k=0;k<hits.size();k++){
	  printf("[%8.8x] ", hits[k]->id() ) ;
	  //	  cout << "[" << hits[k]->id() <<"] " ;
	}
	cout << endl  ;
      }
      cout << " radius of innermost hit " << trk->getRadiusOfInnermostHit() << " / mm , "   
	<< " subdetector Hit numbers : " ;
      for( int i=0 ; i< trk->getSubdetectorHitNumbers().size() ; i++) {
	cout << trk->getSubdetectorHitNumbers()[i] << ", " ;
      }
      cout << endl ;
//       cout << trk->getType()     << " | "
// 	   << trk->getMomentum() << " | "
// 	   << trk->getTheta()     << " | "
// 	   << trk->getPhi()     << " | "
// 	   << trk->getD0()     << " | "
// 	   << trk->getZ0()     << " | "
// 	   << endl ;

      cout << "------------|----------|----------|----------|----------|-----------|-----------|---------------------------------|----------|----------|-------- "
	   << endl ;
    }
  }
  
  void LCTOOLS::printSimTrackerHits(const EVENT::LCCollection* col ){
    
    if( col->getTypeName() != LCIO::SIMTRACKERHIT ){
      
      cout << " collection not of type " << LCIO::SIMTRACKERHIT << endl ;
      return ;
    }
    
    cout << endl 
	 << "--------------- " << "print out of "  << LCIO::SIMTRACKERHIT << " collection "
	 << "--------------- " << endl ;
    
    cout << endl 
	 << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;
    
    LCFlagImpl flag( col->getFlag() ) ;
    cout << "     LCIO::THBIT_BARREL : " << flag.bitSet( LCIO::THBIT_BARREL ) << endl ;
    
    
    int nHits =  col->getNumberOfElements() ;
    int nPrint = nHits > MAX_HITS ? MAX_HITS : nHits ;
    
    std::cout << endl
	      << " cellID(bytes)| position (x,y,z) | dEdx | time  | PDG of MCParticle" 
	      << endl 
	      << endl ;
    
    for( int i=0 ; i< nPrint ; i++ ){
      
      SimTrackerHit* hit = 
	dynamic_cast<SimTrackerHit*>( col->getElementAt( i ) ) ;
      
      int id0 = hit->getCellID() ;
// 
// Since hit may not have MCParticle, need to check that it exists
//
	  int pdgid = 0;
	  if(hit->getMCParticle() > 0)pdgid = 
		 hit->getMCParticle()->getPDG();
      
      cout << i << ": "
	// 	   << hit->getCellID() << " | "
	   << ((id0& 0xff000000)>>24) << "/" 
	   << ((id0& 0x00ff0000)>>16) << "/" 
	   << ((id0& 0x0000ff00)>> 8) << "/" 
	   << ((id0& 0x000000ff)>> 0) << " | "
	   << hit->getPosition()[0] << ", "
	   << hit->getPosition()[1]<< ", "
	   << hit->getPosition()[2] << ") | " 
	   << hit->getdEdx () << " | "
	   << hit->getTime () << " | "
  	   << pdgid  
	   << endl ;
      
    }
    cout << endl 
	 << "-------------------------------------------------------------------------------- " 
	 << endl ;
    
  }

  void LCTOOLS::printTrackerHits(const EVENT::LCCollection* col ){
    
    if( col->getTypeName() != LCIO::TRACKERHIT ){
      
      cout << " collection not of type " << LCIO::TRACKERHIT << endl ;
      return ;
    }
    
    cout << endl 
	 << "--------------- " << "print out of "  << LCIO::TRACKERHIT << " collection "
	 << "--------------- " << endl ;
    
    cout << endl 
	 << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;
    
    LCFlagImpl flag( col->getFlag() ) ;
    cout << "     LCIO::THBIT_BARREL : " << flag.bitSet( LCIO::THBIT_BARREL ) << endl ;
    
    
    int nHits =  col->getNumberOfElements() ;
    int nPrint = nHits > MAX_HITS ? MAX_HITS : nHits ;
    
    std::cout << endl
	      << " [   id   ] | position (x,y,z)                | dEdx      | time     | [rawhitid] |  type" 
      //              << "  [508] | (1.800e+01,0.000e+00,0.000e+00) | 3.142e+00 | 1.235e-01"
	      << endl 
	      << endl ;
    
    for( int i=0 ; i< nPrint ; i++ ){
      
      TrackerHit* hit = 
	dynamic_cast<TrackerHit*>( col->getElementAt( i ) ) ;
      
      printf(" [%8.8x] | (%5.3e,%5.3e,%5.3e) | %5.3e | %5.3e | [%8.8x] | %s\n" 
	     , hit->id() 
	     , hit->getPosition()[0] 
	     , hit->getPosition()[1] 
	     , hit->getPosition()[2] 
	     , hit->getdEdx() 
	     , hit->getTime() 
	     , hit->getRawDataHit()->id() 
	     , hit->getType().c_str() 
	     ) ;
    }
    cout << endl 
	 << "-------------------------------------------------------------------------------- " 
	 << endl ;
    
  }
  
  void LCTOOLS::printTPCHits(const EVENT::LCCollection* col ) {
    
    if( col->getTypeName() != LCIO::TPCHIT ){
      
      cout << " collection not of type " << LCIO::TPCHIT << endl ;
      return ;
    }
    
    cout << endl 
	 << "--------------- " << "print out of "  << LCIO::TPCHIT << " collection "
	 << "--------------- " << endl ;
    
    cout << endl 
	 << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;
    
    LCFlagImpl flag( col->getFlag() ) ;
    cout << "  -> LCIO::TPCBIT_RAW   : " << flag.bitSet( LCIO::TPCBIT_RAW ) << endl ;
    cout << "  -> LCIO::TPCBIT_PTR   : " << flag.bitSet( LCIO::TPCBIT_PTR ) << endl ;
    
    int nHits =  col->getNumberOfElements() ;
    int nPrint = nHits > MAX_HITS ? MAX_HITS : nHits ;
    
    std::cout << endl
	      << " [   id   ]  | cellId0 | time | charge | quality " 
	      << endl << "  -> raw data (bytes) : "
	      << endl 
	      << endl ;
    
    for( int i=0 ; i< nPrint ; i++ ){
      
      TPCHit* hit = 
	dynamic_cast<TPCHit*>( col->getElementAt( i ) ) ;
      
      int id0 = hit->getCellID()  ;
      //      std::cout << hit->getCellID() << " | " 
      printf( " [%8.8x] |", hit->id() ) ;
      std::cout	<< ((id0& 0xff000000)>>24) << "/" 
		<< ((id0& 0x00ff0000)>>16) << "/" 
		<< ((id0& 0x0000ff00)>> 8) << "/" 
		<< ((id0& 0x000000ff)>> 0) << " | "
		<< hit->getTime() << " | "
		<< hit->getCharge() << " | ["
		<< hit->getQuality() << "] "
		<< std::endl ;
      if( flag.bitSet( LCIO::TPCBIT_RAW ) ){

	int nWords = hit->getNRawDataWords() ;
	std::cout << "  ->  " ;

	for(int  j=0;j<nWords;j++){
	  int rawData = hit->getRawDataWord( j ) ;
	  std::cout << ((rawData& 0xff000000)>>24) << ", " 
		    << ((rawData& 0x00ff0000)>>16) << ", " 
		    << ((rawData& 0x0000ff00)>> 8) << ", " 
		    << ((rawData& 0x000000ff)>> 0)  ;
	  if( j < nWords-1) std::cout << ", " ;
	}

	std::cout << std::endl ;

      }
    }
    cout << endl 
	 << "-------------------------------------------------------------------------------- " 
	 << endl ;
  }

  void LCTOOLS::printLCFloatVecs( const EVENT::LCCollection* col ) {

    if( col->getTypeName() != LCIO::LCFLOATVEC ){

      cout << " collection not of type " << LCIO::LCFLOATVEC << endl ;
      return ;
    }

    cout << endl 
	 << "--------------- " << "print out of "  << LCIO::LCFLOATVEC << " collection (user extension) "
	 << "--------------- " << endl ;

    cout << endl 
	 << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;
 
    int nHits =  col->getNumberOfElements() ;
    int nPrint = nHits > MAX_HITS ? MAX_HITS : nHits ;

    std::cout << endl
	      << " element index: val0, val1, ..." 
	      << endl 
	      << endl ;
    
    for( int i=0 ; i< nPrint ; i++ ){
      
      LCFloatVec* vec = 
	dynamic_cast<LCFloatVec*>( col->getElementAt( i ) ) ;
      
      std::cout << i << ": " ;
      for(unsigned int j=0;j< vec->size();j++){

	std::cout << (*vec)[j] ;
	if( j<vec->size()-1) std::cout << ", " ;

	if( ! ( (j+1) % 10)  ) std::cout << endl << "     " ;
      }
      std::cout << std::endl ;
    }
    cout << endl 
	 << "-------------------------------------------------------------------------------- " 
	 << endl ;

  }
  void LCTOOLS::printLCIntVecs( const EVENT::LCCollection* col ) {

    if( col->getTypeName() != LCIO::LCINTVEC ){

      cout << " collection not of type " << LCIO::LCINTVEC << endl ;
      return ;
    }

    cout << endl 
	 << "--------------- " << "print out of "  << LCIO::LCINTVEC << " collection (user extension) "
	 << "--------------- " << endl ;

    cout << endl 
	 << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;
 
    int nHits =  col->getNumberOfElements() ;
    int nPrint = nHits > MAX_HITS ? MAX_HITS : nHits ;

    std::cout << endl
	      << " element index: val0, val1, ..." 
	      << endl 
	      << endl ;
    
    for( int i=0 ; i< nPrint ; i++ ){
      
      LCIntVec* vec = 
	dynamic_cast<LCIntVec*>( col->getElementAt( i ) ) ;
      
      std::cout << i << ": " ;
      for(unsigned int j=0;j< vec->size();j++){

	std::cout << (*vec)[j] ;
	if( j<vec->size()-1) std::cout << ", " ;

	if( ! ( (j+1) % 10)  ) std::cout << endl << "     " ;
      }
      std::cout << std::endl ;
    }
    cout << endl 
	 << "-------------------------------------------------------------------------------- " 
	 << endl ;

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
    cout << "     LCIO::CHBIT_ID1   :  " << flag.bitSet( LCIO::CHBIT_ID1 ) << endl ;
    cout << "     LCIO::CHBIT_PDG    : " << flag.bitSet( LCIO::CHBIT_PDG ) << endl ;

    int nHits =  col->getNumberOfElements() ;
    int nPrint = nHits > MAX_HITS ? MAX_HITS : nHits ;

    std::cout << endl
	      << " cellId0 | cellId1 | energy | position (x,y,z) | nMCParticles " 
	      << endl << "           -> MC contribution: prim. PDG |  energy | time | sec. PDG  "
	      << endl 
	      << endl ;
    
    for( int i=0 ; i< nPrint ; i++ ){
      
      SimCalorimeterHit* hit = 
	dynamic_cast<SimCalorimeterHit*>( col->getElementAt( i ) ) ;
      
      int id0 = hit->getCellID0() ;
      int id1 = hit->getCellID1() ;
	    
      cout << i << ": "
// 	   << hit->getCellID0() << " | "
// 	   << hit->getCellID1() << " | "
	   << ((id0& 0xff000000)>>24) << "/" 
	   << ((id0& 0x00ff0000)>>16) << "/" 
	   << ((id0& 0x0000ff00)>> 8) << "/" 
	   << ((id0& 0x000000ff)>> 0) << " | "
	   << ((id1& 0xff000000)>>24) << "/" 
	   << ((id1& 0x00ff0000)>>16) << "/" 
	   << ((id1& 0x0000ff00)>> 8) << "/" 
	   << ((id1& 0x000000ff)>> 0) << " | "
	
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

  void LCTOOLS::printCalorimeterHits(const EVENT::LCCollection* col ){

    if( col->getTypeName() != LCIO::CALORIMETERHIT ){

      cout << " collection not of type " << LCIO::CALORIMETERHIT << endl ;
      return ;
    }

    cout << endl 
	 << "--------------- " << "print out of "  << LCIO::CALORIMETERHIT << " collection "
	 << "--------------- " << endl ;

    cout << endl 
	 << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;
 
    LCFlagImpl flag( col->getFlag() ) ;
    cout << "  -> LCIO::CHBIT_LONG   : " << flag.bitSet( LCIO::CHBIT_LONG ) << endl ;
    cout << "     LCIO::CHBIT_BARREL : " << flag.bitSet( LCIO::CHBIT_BARREL ) << endl ;
    cout << "     LCIO::CHBIT_ID1    : " << flag.bitSet( LCIO::CHBIT_ID1 ) << endl ;
    cout << "     LCIO::CHBIT_PDG    : " << flag.bitSet( LCIO::CHBIT_PDG ) << endl ;

    int nHits =  col->getNumberOfElements() ;
    int nPrint = nHits > MAX_HITS ? MAX_HITS : nHits ;

    std::cout << endl
	      << " [   id   ] |  cellId0(bytes) | cellId1(bytes) | energy/amplitude | position (x,y,z) " 
	      << endl ;
    
    for( int i=0 ; i< nPrint ; i++ ){
      
      CalorimeterHit* hit = 
	dynamic_cast<CalorimeterHit*>( col->getElementAt( i ) ) ;
      
      int id0 = hit->getCellID0() ;
      int id1 = hit->getCellID1() ;
      
      printf( " [%8.8x] | " , hit->id() ) ;
	// 	   << hit->getCellID0() << " | "
	// 	   << hit->getCellID1() << " | "
	cout << ((id0& 0xff000000)>>24) << "/" 
	   << ((id0& 0x00ff0000)>>16) << "/" 
	   << ((id0& 0x0000ff00)>> 8) << "/" 
	   << ((id0& 0x000000ff)>> 0) << " | "
	   << ((id1& 0xff000000)>>24) << "/" 
	   << ((id1& 0x00ff0000)>>16) << "/" 
	   << ((id1& 0x0000ff00)>> 8) << "/" 
	   << ((id1& 0x000000ff)>> 0) << " | "
	   << hit->getEnergy() << " | (" ;
      
      if( flag.bitSet( LCIO::CHBIT_LONG ) ){
	
	cout << hit->getPosition()[0] << ", "
	     << hit->getPosition()[1]<< ", "
	     << hit->getPosition()[2] << ") | " ;
      }else{
	cout << "   no position avaliable  ) | " ;
      }
      cout << endl ;

    }
    cout << endl 
	 << "-------------------------------------------------------------------------------- " 
	 << endl ;
  }

  void LCTOOLS::printClusters(const EVENT::LCCollection* col ){
    if( col->getTypeName() != LCIO::CLUSTER ){
      
      cout << " collection not of type " << LCIO::CLUSTER << endl ;
      return ;
    }
    cout << endl 
	 << "--------------- " << "print out of "  << LCIO::CLUSTER << " collection "
	 << "--------------- " << endl ;
    
    cout << endl 
	 << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;
    
    LCFlagImpl flag( col->getFlag() ) ;
    cout << "     LCIO::CLBIT_HITS : " << flag.bitSet( LCIO::CLBIT_HITS ) << endl ;
    
    int nClusters =  col->getNumberOfElements() ;
    int nPrint = nClusters > MAX_HITS ? MAX_HITS : nClusters ;
    
    std::cout << endl
	      << " [   id   ] |  type | energy    |      position ( x,y,z)          |  theta   |  phi     | EMweight |HADweight |MUweight"
	      << endl	      
	      << "------------|-------|-----------|---------------------------------|----------|----------|----------|----------|---------"
	      << endl ;
    
    for( int i=0 ; i< nPrint ; i++ ){
      
      Cluster* clu = 
      	dynamic_cast<Cluster*>( col->getElementAt( i ) ) ;
      
      printf(" [%8.8x] |0x%5x| %5.3e | (%5.3e,%5.3e,%5.3e) | %4.2e | %4.2e | %4.2e | %4.2e | %4.2e \n"
	     , clu->id()
	     , clu->getType() 
	     , clu->getEnergy() 
	     , clu->getPosition()[0]
	     , clu->getPosition()[1]
	     , clu->getPosition()[2]
	     , clu->getTheta() 
	     , clu->getPhi()
	     , clu->getParticleType()[0] 
	     , clu->getParticleType()[1] 
	     , clu->getParticleType()[2] 
	     ) ;
      cout << " errors (6 pos)/( 3 dir): (" ;
      for(int l=0;l<6;l++){
	printf("%4.2e, ", clu->getPositionError()[l] ) ; 
      }
      cout << ")/("  ;
      for(int l=0;l<3;l++){
	printf("%4.2e, ", clu->getDirectionError()[l] ) ; 
      }
      cout << ")" << endl ;
      cout << " clusters(e): " ;
      const ClusterVec& clusters = clu->getClusters() ;

      for(unsigned int l=0;l<clusters.size();l++){
		printf("%4.2e, ",  clusters[l]->getEnergy() ) ; 
      }
      cout << endl ;
      if( flag.bitSet( LCIO::CLBIT_HITS ) ) {
	cout << " hits ->";
// 	const StringVec& hitColNames = clu->getHitCollectionNames() ;
// 	for(unsigned int j=0;j<hitColNames.size();j++){
// 	  cout << " " << hitColNames[j] << ": " ;

// 	  const IntVec& hits = clu->getHitIndicesForCollection( hitColNames[j] ) ;
// 	  const FloatVec& wgts = clu->getHitContributionsForCollection( hitColNames[j] ) ;

// 	  for(unsigned int k=0;k<hits.size();k++){
// 	    cout << hits[k] <<" ["<< wgts[k] <<"], " ;
// 	  }
// 	}

	const CalorimeterHitVec& hits= clu->getCalorimeterHits() ; 
	const FloatVec& contr = clu->getHitContributions() ; 
	for(unsigned int k=0;k<hits.size();k++){
	  printf( " [%8.8x] (%4.3e), " , hits[k]->id(), contr[k] ) ;
	  // 	  cout << "0x" << hits[k]  << "(" << contr[k] << "), " ;
	}
	cout << dec << endl  ;


      }
      cout << endl 
	   << "-------|-----------|---------------------------------|----------|----------|----------|----------|---------"
	   << endl ;
    }
      cout << endl 
	   << "-------------------------------------------------------------------------------- " 
	   << endl ;
  }


  void LCTOOLS::printReconstructedParticles( const EVENT::LCCollection* col ){

    if( col->getTypeName() != LCIO::RECONSTRUCTEDPARTICLE ){
      
      cout << " collection not of type " << LCIO::RECONSTRUCTEDPARTICLE << endl ;
      return ;
    }
    cout << endl 
	 << "--------------- " << "print out of "  << LCIO::RECONSTRUCTEDPARTICLE << " collection "
	 << "--------------- " << endl ;
    
    cout << endl 
	 << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;
    
//     LCFlagImpl flag( col->getFlag() ) ;
//     cout << "     LCIO::CLBIT_HITS : " << flag.bitSet( LCIO::CLBIT_HITS ) << endl ;
    
    int nReconstructedParticles =  col->getNumberOfElements() ;
    int nPrint = nReconstructedParticles > MAX_HITS ? MAX_HITS : nReconstructedParticles ;
    
    std::cout << endl
	      << " [   id   ] |pri|type|     momentum( px,py,pz)         | energy   | mass     | charge    |      position ( x,y,z)  "
	      << endl	      
	      << "----------|---|----|---------------------------------|----------|----------|-----------|-------------------------"
	      << endl ;
    
    for( int i=0 ; i< nPrint ; i++ ){
      
#ifdef CLHEP
      ReconstructedParticle4V recP( col->getElementAt( i ) ) ;
#else
      ReconstructedParticle* recP = 
      	dynamic_cast<ReconstructedParticle*>( col->getElementAt( i ) ) ;
#endif

      
      int primary = recP->isPrimary() ;
      int type =  recP->getType() ;

      printf(" [%8.8x] | %1d | %2d | (%5.3e,%5.3e,%5.3e) | %4.2e | %4.2e | %4.2e | (%5.3e,%5.3e,%5.3e) \n"
	     //	     , reinterpret_cast<int> ( recP )
	     , recP->id()
	     , primary
	     , type
	     , recP->getMomentum()[0]
	     , recP->getMomentum()[1]
	     , recP->getMomentum()[2]
	     , recP->getEnergy() 
	     , recP->getMass() 
	     , recP->getCharge()
	     , recP->getReferencePoint()[0] 
	     , recP->getReferencePoint()[1] 
	     , recP->getReferencePoint()[2] 
	     ) ;
      cout << "    covariance( px,py,pz,E) : (" ;
      for(int l=0;l<10;l++){
 	printf("%4.2e, ", recP->getCovMatrix()[l] ) ; 
      }
      cout << ")" << endl ;

      cout << "    particles ( [   id   ] (weight) ): " ;
      for(unsigned int l=0;l<recP->getParticles().size();l++){
	printf("[%8.8x] (%f), ", recP->getParticles()[l]->id() , 
	       recP->getParticleWeights()[l]  ) ; 
      }
      cout << endl ;
      cout << "    tracks ( [   id   ] (weight) ): " ;
      for(unsigned int l=0;l<recP->getTracks().size();l++){
	printf("[%8.8x] (%f), ",  recP->getTracks()[l]->id() ,
	       recP->getTrackWeights()[l]  ) ; 
      }
      cout << endl ;
      cout << "    clusters ( [   id   ] (weight) ): " ;
      for(unsigned int l=0;l<recP->getClusters().size();l++){
	printf("[%8.8x] (%f), ",  recP->getClusters()[l]->id() ,
	       recP->getClusterWeights()[l]  ) ; 
      }
      cout << endl ;
      cout << "    MCParticles ( [   id   ] (weight) ): " ;
      for(unsigned int l=0;l<recP->getMCParticles().size();l++){
	printf("[%8.8x] (%f), ",  recP->getMCParticles()[l]->id() ,
	       recP->getMCParticleWeights()[l]  ) ; 
      }
      cout << endl ;

//       cout << endl ;
//       if( flag.bitSet( LCIO::CLBIT_HITS ) ) {
// 	cout << " hits ->";
// 	const StringVec& hitColNames = recP->getHitCollectionNames() ;
// 	for(unsigned int j=0;j<hitColNames.size();j++){
// 	  cout << " " << hitColNames[j] << ": " ;

// 	  const IntVec& hits = recP->getHitIndicesForCollection( hitColNames[j] ) ;
// 	  const FloatVec& wgts = recP->getHitContributionsForCollection( hitColNames[j] ) ;

// 	  for(unsigned int k=0;k<hits.size();k++){
// 	    cout << hits[k] <<" ["<< wgts[k] <<"], " ;
// 	  }
// 	}
//       }
      cout  << "----------|---|----|---------------------------------|----------|----------|-----------|-------------------------"
	    << endl ;
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



    // fill map with particle pointers and collection indices
    typedef std::map< MCParticle*, int > PointerToIndexMap ;
    PointerToIndexMap p2i_map ;
    std::vector<MCParticle*> moms ;

    for( int k=0; k<nParticles; k++){

      MCParticle* part =  dynamic_cast<MCParticle*>( col->getElementAt( k ) ) ;
      p2i_map[ part ] = k ; 

      moms.push_back( part ) ;
    }

    std::cout << endl
	      << " [   id   ] | index [parents] | [daughters] | PDG | (px, py, pz) | GenStatus | SimStatus | vertex (x,y,z) | endpoint(x,y,z)" 
	      << " | mass | charge | energy"
	      << endl 
	      << endl ;


//    // now loop over mothers and print daughters recursively
//     int index = 0 ;
//     std::vector<MCParticle*>::const_iterator  mom ; 
//     for( mom = moms.begin() ; mom != moms.end() ; mom++){

// loop over collection - preserve order
    for(  int index = 0 ; index < nParticles ; index++){
      

#ifdef CLHEP
      MCParticle4V part( col->getElementAt( index ) ) ;
#else
      MCParticle* part =  dynamic_cast<MCParticle*>( col->getElementAt( index ) ) ;
#endif
      printf(" [%8.8x] |", part->id() );
      cout << index << " [" ;
      for(int k=0;k<part->getNumberOfParents();k++){
	if(k>0) cout << "," ;
	cout << p2i_map[ part->getParent(k) ]  ;
      }
      cout << "] | [" ;
      for(int k=0;k<part->getNumberOfDaughters();k++){
	if(k>0) cout << "," ;
	cout << p2i_map[ part->getDaughter(k) ]  ;
      }
      cout << "] | " ;
      cout <<  part->getPDG() << " | ("
	   <<  part->getMomentum()[0]  << ", "
	   <<  part->getMomentum()[1]  << ", "
	   <<  part->getMomentum()[2]  << ") | "
	   <<  part->getGeneratorStatus() << " | "
	   <<  hex << part->getSimulatorStatus() << dec << " | ("
	   <<  part->getVertex()[0]    << ", "
	   <<  part->getVertex()[1]    << ", "
	   <<  part->getVertex()[2]    << ") | (" ;

      if( part->getEndpoint() != 0 ){
	cout <<  part->getEndpoint()[0]  << ", "
	     <<  part->getEndpoint()[1]  << ", "
	     <<  part->getEndpoint()[2]  << ") | " ;
      }else{
	cout << " not set ) | " ; 
      }
      cout <<  part->getMass()         << " | " 
	   <<  part->getCharge()       << " | " 
	   <<  part->getEnergy()      
#ifdef CLHEP
	//---- DEBUG
	   << " m(4V) : " << part.m()
	   << " e(4V) : " << part.e()
	//---- DEBUG
#endif
	   << endl ;	


    }

    cout << endl 
	 << "-------------------------------------------------------------------------------- " 
	 << endl ;
  }
//   void LCTOOLS::printMCParticles(const EVENT::LCCollection* col ) {
    
//     if( col->getTypeName() != LCIO::MCPARTICLE ){
      
//       cout << " collection not of type " << LCIO::MCPARTICLE << endl ;
//       return ;
//     }
    
//     cout << endl 
// 	 << "--------------- " << "print out of "  << LCIO::MCPARTICLE << " collection "
// 	 << "--------------- " << endl ;
    
//     cout << endl 
// 	 << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;
    
//     int nParticles =  col->getNumberOfElements() ;

//     // get a list of all mother particles
//     std::vector<MCParticle*> moms ; 
//     for( int k=0; k<nParticles; k++){

//       // get the particle from the collection - needs a cast !
//       MCParticle* part =  dynamic_cast<MCParticle*>( col->getElementAt( k ) ) ;
      
//       if( part->getNumberOfParents() == 0 )
// 	moms.push_back( part ) ;
//     }

//     std::cout << endl
// 	      << " index [motherIndex] | (px, py, pz) | HepEvtStatus | vertex (x,y,z) | endpoint(x,y,z)" 
// 	      << " | mass | charge | energy"
// 	      << endl 
// 	      << endl ;


//     // now loop over mothers and print daughters recursively
//     int index = 0 ;
//     std::vector<MCParticle*>::const_iterator  mom ; 
//     for( mom = moms.begin() ; mom != moms.end() ; mom++){

//       cout << index++ << " [-] "
// 	   <<  (*mom)->getPDG() << " | ("
// 	   <<  (*mom)->getMomentum()[0]  << ", "
// 	   <<  (*mom)->getMomentum()[1]  << ", "
// 	   <<  (*mom)->getMomentum()[2]  << ") | "
// 	   <<  (*mom)->getHepEvtStatus() << " | ("
// 	   <<  (*mom)->getVertex()[0]    << ", "
// 	   <<  (*mom)->getVertex()[1]    << ", "
// 	   <<  (*mom)->getVertex()[2]    << ") | ("
// 	   <<  (*mom)->getEndpoint()[0]  << ", "
// 	   <<  (*mom)->getEndpoint()[1]  << ", "
// 	   <<  (*mom)->getEndpoint()[2]  << ") | "
// 	   <<  (*mom)->getMass()         << " | " 
// 	   <<  (*mom)->getCharge()       << " | " 
// 	   <<  (*mom)->getEnergy()      
// 	   << endl ;	


//       index  = printDaughterParticles( *mom, index ) ;
//     }

//     cout << endl 
// 	 << "-------------------------------------------------------------------------------- " 
// 	 << endl ;
//   }


  void LCTOOLS::printRelation( const EVENT::LCRelation* rel_const ) {

    LCRelation* rel = const_cast< LCRelation* >( rel_const )  ; // FIXME: the relation interface needs to properly support const
    
    cout << " relation from " << rel->getFromType() << " to " << rel->getToType() << endl ; 
    
    int nRel = rel->numberOfRelations() ;
    
    cout <<  " |  [from_id]  |  [to_id]   | weight "  << endl ;
    
    // need to get a set of from objects first
    set<LCObject*> objs ;
    for( int i=0; i<nRel ; i++ ) {
      objs.insert( rel->getRelation( i )  ) ;
    } 

    for( std::set< LCObject* >::const_iterator iter = objs.begin() ; iter != objs.end() ; iter++ ){

      LCObject* from = *iter ;
      
      for( int j=0; j < rel->numberOfRelations( from ) ; j++ ){
	
	printf(" | [%8.8x] |  [[%8.8x]   | %5.3e \n" 
	       , from->id() 
	       , rel->getRelation( from, j )->id()  
	       , rel->getWeight(  from, j )
	       );
	
      }
      
    }
  }

  int LCTOOLS::printDaughterParticles(const MCParticle* part, int index){

    int motherIndex = index - 1 ;
    // print this particles daughters
    for(int i=0; i<part->getNumberOfDaughters();i++){
      
      MCParticle* d =  part->getDaughter(i) ;

      cout << index++ << " [" << motherIndex << "] " 
	   <<  d->getPDG() << " | ("
	   <<  d->getMomentum()[0]  << ", "
	   <<  d->getMomentum()[1]  << ", "
	   <<  d->getMomentum()[2]  << ") | "
	   <<  part->getGeneratorStatus() << " | "
	   <<  part->getSimulatorStatus() << " | ("
	   <<  d->getVertex()[0]    << ", "
	   <<  d->getVertex()[1]    << ", "
	   <<  d->getVertex()[2]    << ") | (" ;
      if( part->getEndpoint() != 0 ){
	cout <<  part->getEndpoint()[0]  << ", "
	     <<  part->getEndpoint()[1]  << ", "
	     <<  part->getEndpoint()[2]  << ") | " ;
      }else{
	cout << " not set ) | " ; 
      }
      cout <<  d->getMass()         << " | " 
	   <<  d->getCharge()       << " | " 
	   <<  d->getEnergy()      
	   << endl ;	

      index = printDaughterParticles( part->getDaughter(i) , index ) ;
    }
    return index ;

  }

}; // namespace

