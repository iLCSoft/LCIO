#include "lcio.h"

#include "IO/LCWriter.h"
#include "IO/LCReader.h"
#include "IO/LCEventListener.h"
#include "IO/LCRunListener.h"

#include "EVENT/LCIO.h"

#include "IMPL/LCEventImpl.h" 
#include "IMPL/LCCollectionVec.h"
#include "IMPL/SimCalorimeterHitImpl.h"
#include "IMPL/MCParticleImpl.h" 
#include "IMPL/TrackImpl.h" 
#include "IMPL/ClusterImpl.h" 
#include "IMPL/ReconstructedParticleImpl.h" 
#include "IMPL/LCFlagImpl.h" 
#include "IMPL/LCTOOLS.h"

//#include <math.h>
//#include <cmath>
//#include <cstdlib>
// M_PI is non ansi ...
#define M_PI 3.14159265358979323846

#include <iostream>

using namespace std ;
using namespace lcio ;


static char* FILEN = "simjob.slcio" ;
static char* OUTFILEN = "recjob.slcio" ;
static const int NHITS = 50 ;  // calorimeter hits per event
static const int nRecP = 10 ; // number of reconstructed particles

/** Example of reading events from the file, add sth. to the event
 * and write it to a new file. This uses the listener mechanism to read 
 * the input file (run headers and events) record by record. <br>
 * 
 *  The RunEventProcessor class is defined for processing run and event records.
 *  This is our analysis module.
 *  For simplicity it is defined in the same file - in a real world application 
 *  it should of course be defined in separate header and source files.
 *  
 */

class RunEventProcessor : public LCRunListener, public LCEventListener{
  
protected:
  LCWriter* lcWrt ;
  int nEvent ;
  
public:
  
  RunEventProcessor() : nEvent(0) {
    
    // open outputfile
    lcWrt = LCFactory::getInstance()->createLCWriter() ;

    try{ lcWrt->open( OUTFILEN , LCIO::WRITE_NEW ) ; } 
    
    catch(IOException& e){
      cout << "[RunEventProcessor()] Can't open file for writing -  " 
	   << e.what()  << endl ;
      exit(1) ;
    }
    
  }
  
  ~RunEventProcessor(){

    // close outputfile
    lcWrt->close()  ;

    cout << endl 
	 << " added collection: 'SomeClusters' and 'SomeTracks'" 
	 << " to   " << nEvent <<" events"  
	 << " and added one extra MCParticle to each event."
	 << endl << endl ;
    delete lcWrt ;
  }
  
  void processEvent( LCEvent * evt ) { /* used for 'read only' access*/ 

    // this is our event loop code
    
    // read collection with MCParticles
    //    LCCollection* mcVec = evt->getCollection( LCIO::MCPARTICLE )  ;
    //    int NMCPART = mcVec->getNumberOfElements() ;
    

    // ---- trying to modify objects here would cause a ReadOnlyExcpetion. e.g. -----
//         for(int i=0 ; i< NMCPART ; i++ ){
//           MCParticleImpl* part =  dynamic_cast<MCParticleImpl*>( mcVec->getElementAt( i )) ;
//           part->setPDG(1234) ;      // <<<<< ------- will cause ReadOnlyException ---------
//         }
    // ---- also  adding  sth. to en existing collection is not allowed here ----
    //     MCParticleImpl* part = new MCParticleImpl ;
    //     part->setPDG( 1234 ) ;
    //     part->setParent( dynamic_cast<MCParticle*>( mcVec->getElementAt(0) )) ;
    //     mcVec->addElement( part ) ;  // <<<<< ------- will cause ReadOnlyException ---------
    


    // create some tracks and add them to the event
    std::string simHitName( "TPC4711" ) ;
    std::string tpcHitName( "TPCRawFADC" ) ;

    LCCollection* simHits = evt->getCollection( simHitName ) ;
    LCCollection* tpcHits = evt->getCollection( tpcHitName) ;


    LCCollectionVec* trkVec = new LCCollectionVec( LCIO::TRACK )  ;

    // if we want to point back to the hits we need to set the flag
    LCFlagImpl trkFlag(0) ;
    trkFlag.setBit( LCIO::TRBIT_HITS ) ;
    trkVec->setFlag( trkFlag.getFlag()  ) ;
    
    if( simHits ){

      int nSimHits = simHits->getNumberOfElements() ;
      int nTrk = nSimHits / 10 ;

      for( int i=0; i < nTrk ; i ++ ){

	TrackImpl* trk = new TrackImpl ;
	trk->setType( TrackData::TPC ) ;
	trk->setMomentum(  (i+1)*1.1 ) ;
	trk->setTheta( (i+1)* M_PI / 10. ) ;
	trk->setPhi( (i+1)* M_PI / 5. ) ;
	trk->setD0( i+1 ) ;
	trk->setZ0( (i+1)*10. ) ;
	trk->setChi2( 1.01 ) ;
	trk->setdEdx( 3.14159 ) ;
	trk->setdEdxError( 42. ) ;
	float cov[15] = { 1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,13.,14.,15. } ;
	trk->setCovMatrix( cov ) ;

	// add the hits used to create this track
	for( int j=0; j < 10  ; j ++ ){
	  trk->addHitIndex( simHitName ,  i*10+j  ) ;   
	}
	trkVec->addElement( trk ) ;
      }
    }

    if( tpcHits ){

      int nTPCHits = tpcHits->getNumberOfElements() ;
      int nTrk = nTPCHits / 10 ;

      for( int i=0; i < nTrk ; i ++ ){

	TrackImpl* trk = new TrackImpl ;
	trk->setType( TrackData::TPC ) ;
	trk->setMomentum(  (i+1)*1.1 ) ;
	trk->setTheta( (i+1)* M_PI / 10. ) ;
	trk->setPhi( (i+1)* M_PI / 5. ) ;
	trk->setD0( i+1 ) ;
	trk->setZ0( (i+1)*10. ) ;
	trk->setChi2( 1.01 ) ;
	trk->setdEdx( 3.14159 ) ;
	trk->setdEdxError( 42. ) ;
	float cov[15] = { 1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,13.,14.,15. } ;
	trk->setCovMatrix( cov ) ;
	float ref[3] = { 12. ,123456789. , .0987654321 } ;
	trk->setReferencePoint( ref ) ;
	// add the hits used to create this track
	for( int j=0; j < 10  ; j ++ ){
	  trk->addHitIndex( tpcHitName ,  i*10+j  ) ;   
	}
	// add tracks that where used to create this track
	if( trkVec->size() > 1 ){
	  trk->addTrack( dynamic_cast<TrackImpl*> ( (*trkVec)[ trkVec->size() - 1 ] ) ) ;
	  trk->addTrack( dynamic_cast<TrackImpl*> ( (*trkVec)[ trkVec->size() - 2 ] ) ) ;
	}

	trkVec->addElement( trk ) ;
      }
    }



    evt->addCollection(  trkVec , "SomeTracks" ) ;



    // create some clusters and add them to the event
    std::string simcalHitName( "ECAL007" ) ;

    LCCollection* simcalHits = evt->getCollection( simcalHitName ) ;


    LCCollectionVec* clusterVec = new LCCollectionVec( LCIO::CLUSTER )  ;

    // if we want to point back to the hits we need to set the flag
    LCFlagImpl clusterFlag(0) ;
    clusterFlag.setBit( LCIO::CLBIT_HITS ) ;
    clusterVec->setFlag( clusterFlag.getFlag()  ) ;
    
    if( simcalHits ){
      
      int nSimHits = simcalHits->getNumberOfElements() ;
      int nCluster = nSimHits / 10 ;
      
      for( int i=0; i < nCluster ; i ++ ){
	
	ClusterImpl* cluster = new ClusterImpl ;

	int type = ( Cluster::COMBINED << 16 | Cluster::CHARGED  ) ;
	cluster->setType( type ) ;

	cluster->setEnergy(  (i+1)*1.1 ) ;
	float pos[3] = { 12. ,123456789. , .0987654321 } ;
	cluster->setPosition( pos ) ;
	float errpos[6] = { 1.,2.,3.,4.,5.,6.} ; 
	cluster->setPositionError( errpos ) ;
	cluster->setTheta( (i+1)* M_PI / 10. ) ;
	cluster->setPhi( (i+1)* M_PI / 5. ) ;
	float errdir[6] = { 1.,2.,3.} ;
	cluster->setDirectionError( errdir ) ;

	float shape[6] = { 1.,2.,3.,3.,2.,1.} ;
	cluster->setShape( shape ) ;

	cluster->setEMWeight( .333)  ;
	cluster->setHADWeight( .333)  ;
	cluster->setMuonWeight( .333)  ;

	// add the hits used to create this cluster
	for( int j=0; j < 10  ; j ++ ){
	  cluster->addHitIndex( simcalHitName ,  i*10+j ,  1. ) ;   
	}

	// add clusters that where used to create this cluster
	if( clusterVec->size() > 1 ){
	  cluster->addCluster( dynamic_cast<ClusterImpl*> 
			       ( (*clusterVec)[ clusterVec->size() - 1 ] ) ) ;
	  cluster->addCluster( dynamic_cast<ClusterImpl*> 
			       ( (*clusterVec)[ clusterVec->size() - 2 ] ) ) ;
	}
	

	clusterVec->addElement( cluster ) ;
      }
    }

    evt->addCollection(  clusterVec , "SomeClusters" ) ;

    

    // add some reconstructed particles
    LCCollectionVec* particleVec = new LCCollectionVec( LCIO::RECONSTRUCTEDPARTICLE )  ;
    
    for(int i=0;i<nRecP;i++){
      ReconstructedParticleImpl* part = new ReconstructedParticleImpl ;
      int typeFlag = ( 1 << 31 )  ;  // primary flag
      part->setTypeFlag(  typeFlag |  ReconstructedParticle::SINGLE ) ;
      float p[3] = { 1.1 , 2.2 , 3.3 } ;
      part->setMomentum( p ) ;
      part->setEnergy(  i*101.101 ) ;
      float cov[10] = { 1.,2.,3.,4.,5.,6.,7.,8.,9.,10. } ;
      part->setCovMatrix( cov) ;
      part->setMass( 0.511*i ) ;
      part->setCharge( -2./3. ) ;
      float x[3] = { 10.,20.,30. } ;
      part->setReferencePoint( x )  ;
      
      // add some particle ids
      int nPID = 5 ;
      for(int j=0;j<nPID;j++){
	ParticleIDImpl* pid = new ParticleIDImpl ;
	pid->setProbability( (double) j / nPID ) ;
	pid->setTypeID( -11 ) ;
	pid->setIdentifier("recojob-RunEventProcessor") ;
	for(int k=0;k<3;k++){
	  pid->addParameter( k*.1 ) ;
	}
	part->addParticleID( pid ) ;
      }      
      // some other particles
      if( i > 1  ){
	ReconstructedParticle* p1 = 
	  dynamic_cast<ReconstructedParticle*> ( particleVec->getElementAt(i-1) ) ;
	ReconstructedParticle* p2 = 
	  dynamic_cast<ReconstructedParticle*> ( particleVec->getElementAt(i-2) ) ;
	part->addParticle( p1 , 0.1 ) ;
	part->addParticle( p2 , 0.9 ) ;
      }
      //a track
      int iTrk = (int) ( double (trkVec->size()) * rand() / RAND_MAX )    ;
      Track* trk = dynamic_cast<Track*> ( trkVec->getElementAt( iTrk ) ) ;
      part->addTrack( trk ) ;

      // a cluster 
      int iClu = (int) ( double (clusterVec->size()) *  rand() / RAND_MAX )  ;
      Cluster* clu = dynamic_cast<Cluster*> ( clusterVec->getElementAt( iClu ) ) ;
      part->addCluster( clu ) ;

      // and finaly an MCParticle
      LCCollection* mcVec = evt->getCollection( LCIO::MCPARTICLE )  ;
      int iMCP = (int) ( double (mcVec->getNumberOfElements()) *  rand() / RAND_MAX ) ;
      MCParticle* mcp = dynamic_cast<MCParticle*>( mcVec->getElementAt( iMCP ) ) ;
      part->addMCParticle( mcp , 0.5 ) ;

      particleVec->addElement( part ) ;
    }

    evt->addCollection( particleVec, "ReconstructedParticle" )  ;

    nEvent ++ ;

  }

  void modifyEvent( LCEvent * evt ) {

    // here we can modify existing objects that have been read from a stream:
    LCCollection* mcVec = evt->getCollection( LCIO::MCPARTICLE )  ;
    int NMCPART = mcVec->getNumberOfElements() ;
    for(int i=0 ; i< NMCPART ; i++ ){
      // in order to have access to the set-methods we need to cast to the implementation
      // of MCParticle 
      MCParticleImpl* part =  dynamic_cast<MCParticleImpl*>( mcVec->getElementAt(i)) ;
      part->setPDG(1234) ;   // <<<<< modifying persistent data
    }
    // or we could add sth. to existing collections
    MCParticleImpl* part = new MCParticleImpl ;
    part->setPDG( 1234 ) ;
    part->addParent( dynamic_cast<MCParticle*>( mcVec->getElementAt(0) )) ;
    mcVec->addElement( part ) ;  // <<<< adding to collections

    LCTOOLS::dumpEvent( evt ) ;
    
    lcWrt->writeEvent( evt ) ;
   }
  

  void processRunHeader( LCRunHeader* run){

    // just copy run headers to the outputfile
    lcWrt->writeRunHeader( run ) ;
  }

  void modifyRunHeader(LCRunHeader* run){/*  we don't modify anything */;}


} ;

//=============================================================================

int main(int argc, char** argv ){
  
    srand(1234) ;
    
    // create reader and writer for input and output streams 
    LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
    
    
    // read file name from command line 
    if( argc > 1 ) { FILEN = argv[1] ; }
    if( argc > 2 ) { OUTFILEN = argv[2] ; }

    lcReader->open( FILEN ) ;
    // we could catch the exception here - but this not really needed
    // as long as we exit anyhow if the file could not be opened...
    //     try{  lcReader->open( FILEN ) ; } 
    //     catch( IOException& e){
    //       cout << "Can't open file : " << e.what()  << endl ;
    //       exit(1) ;
    //     }
    
    // create a new RunEventProcessor, register it with the reader
    // and read and proccess the whole stream 
    {
      RunEventProcessor evtProc ;
      
      lcReader->registerLCRunListener( &evtProc ) ; 
      lcReader->registerLCEventListener( &evtProc ) ; 
      
      lcReader->readStream() ;

//             lcReader->readStream( 5) ; // debugging: only read 4 events 
//             lcReader->readStream( 10000 ) ; // debugging: provoke EndOfDataException
      
    } 
    
    lcReader->close() ;
    delete lcReader ;
    return 0 ;
}

//=============================================================================

