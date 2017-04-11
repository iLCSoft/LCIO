#include "lcio.h"

#include "IO/LCWriter.h"
#include "IO/LCReader.h"
#include "IO/LCEventListener.h"
#include "IO/LCRunListener.h"

#include "EVENT/LCIO.h"
#include "EVENT/TrackerRawData.h"

#include "IMPL/LCEventImpl.h" 
#include "IMPL/LCCollectionVec.h"
#include "IMPL/SimCalorimeterHitImpl.h"
#include "IMPL/CalorimeterHitImpl.h"
#include "IMPL/MCParticleImpl.h" 
#include "IMPL/TrackerHitImpl.h" 
#include "IMPL/TrackImpl.h" 
#include "IMPL/ClusterImpl.h" 
#include "IMPL/ReconstructedParticleImpl.h" 
#include "IMPL/VertexImpl.h" 
#include "UTIL/LCTOOLS.h"
#include "UTIL/PIDHandler.h"
#include "IMPL/LCRelationImpl.h"

#include "UTIL/LCRelationNavigator.h"
#include "UTIL/BitSet32.h"

#include "CalibrationConstant.h"

// M_PI is non ansi ...
#define M_PI 3.14159265358979323846

#include <iostream>
#include <algorithm>

using namespace std ;
using namespace lcio ;

static std::string FILEN = "simjob.slcio" ;
static std::string OUTFILEN = "recjob.slcio" ;
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
  LCWriter* lcWrt{} ;
  int nEvent{0} ;
  
public:
  
  RunEventProcessor(const RunEventProcessor&) = delete ;
  RunEventProcessor operator=(const RunEventProcessor&) = delete ;

  RunEventProcessor() : nEvent(0) {
    
    // open outputfile
    lcWrt = LCFactory::getInstance()->createLCWriter() ;

    try{ 
      lcWrt->setCompressionLevel( 9 ) ;
      lcWrt->open( OUTFILEN , LCIO::WRITE_NEW ) ; 
    } 
    
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
  
  void processEvent( LCEvent* evt ) { /* used for 'read only' access*/ 


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
    std::string tpcHitName( "TrackerRawDataExample" ) ;
    
    // in order to be able to point back to hits, we need to create 
    // generic TrackerHits from the TrackerRawDatas first

    LCCollection* tpcHits = evt->getCollection( tpcHitName) ;

    
    LCCollectionVec* trkhitVec = new LCCollectionVec( LCIO::TRACKERHIT )  ;
    int nTrackerRawDatas = tpcHits->getNumberOfElements() ;

    for(int j=0;j<nTrackerRawDatas;j++){
      TrackerHitImpl* trkHit = new TrackerHitImpl ;

      TrackerRawData* tpcRawHit =  dynamic_cast<TrackerRawData*> ( tpcHits->getElementAt(j)  ) ;

      trkHit->setTime(   tpcRawHit->getTime() ) ;

      int cellID = tpcRawHit->getCellID0() ;
      double pos[3]  = { double (cellID & 0xff) ,  double ( (cellID & 0xff00)>>8 ),   double( (cellID & 0xff0000)>>16 ) } ;
      trkHit->setPosition(  pos  ) ;

      trkHit->rawHits().push_back( tpcRawHit ) ; // store the original raw data hit
      trkHit->rawHits().push_back( tpcRawHit ) ; // for testing add the same raw hit twice

      FloatVec cov(6) ;
      cov[0] = 1. ;
      cov[1] = 2. ;
      cov[2] = 3. ;
      cov[3] = 4. ;
      cov[4] = 5. ;
      cov[5] = 6. ;
      trkHit->setCovMatrix( cov ) ;

      trkhitVec->addElement( trkHit ) ;
    }

    // set the parameters to decode the type information in the collection
    // for the time being this has to be done manually
    // in the future we should provide a more convenient mechanism to 
    // decode this sort of meta information
    StringVec typeNames ;
    IntVec typeValues ;
    typeNames.push_back( LCIO::TPCHIT ) ;
    typeValues.push_back( 1 ) ;
    trkhitVec->parameters().setValues("TrackerHitTypeNames" , typeNames ) ;
    trkhitVec->parameters().setValues("TrackerHitTypeValues" , typeValues ) ;
    
    
    evt->addCollection( trkhitVec , "TrackerHits") ;


    LCCollectionVec* trkVec = new LCCollectionVec( LCIO::TRACK )  ;

    // if we want to point back to the hits we need to set the flag
    // LCFlagImpl trkFlag(0) ;
    // trkFlag.setBit( LCIO::TRBIT_HITS ) ;
    // trkVec->setFlag( trkFlag.getFlag()  ) ;

    trkVec->setFlag( UTIL::make_bitset32(  LCIO::TRBIT_HITS ) ) ;
    

    int nTrk = 5 ;
    for( int i=0; i < nTrk ; i ++ ){
      
      TrackImpl* trk = new TrackImpl ;
      trk->setTypeBit( 7 ) ;
      trk->setOmega(  (i+1)*1.1 ) ;
      trk->setTanLambda( (i+1)* M_PI / 10. ) ;
      trk->setPhi( (i+1)* M_PI / 5. ) ;
      trk->setD0( i+1 ) ;
      trk->setZ0( (i+1)*10. ) ;
      trk->setChi2( 1.01 ) ;
      trk->setNdf( 42 ) ;

      trk->setRadiusOfInnermostHit( 3.141592 ) ;

      // set the hit numbers 
      const int NTRACKER = 3 ; 
      const int VTXINDEX = 0 ;
      const int SITINDEX = 1 ;
      const int TPCINDEX = 2 ;
      StringVec trackerNames ;
      trackerNames.resize(  NTRACKER ) ;
      trackerNames[VTXINDEX] = "VTX" ;
      trackerNames[SITINDEX] = "SIT" ;
      trackerNames[TPCINDEX] = "TPC" ;

      trkVec->parameters().setValues( "TrackSubdetectorNames" , trackerNames ) ;
      
      trk->subdetectorHitNumbers().resize( NTRACKER ) ;
      trk->subdetectorHitNumbers()[ VTXINDEX ] = 12 ;
      trk->subdetectorHitNumbers()[ SITINDEX ] = 24 ;
      trk->subdetectorHitNumbers()[ TPCINDEX ] = 36 ;

      trk->setdEdx( 3.14159 ) ;
      trk->setdEdxError( 42. ) ;
      float cov[15] = { 1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,13.,14.,15. } ;
      trk->setCovMatrix( cov ) ;
      float ref[3] = { 12. ,123456789. , .0987654321 } ;
      trk->setReferencePoint( ref ) ;
      
      // add some random hits 
      int iHit1 = (int) ( double (trkhitVec->size()) * rand() / RAND_MAX )    ;
      int iHit2 = (int) ( double (trkhitVec->size()) * rand() / RAND_MAX )    ;
      int iHit3 = (int) ( double (trkhitVec->size()) * rand() / RAND_MAX )    ;
      
      trk->addHit( dynamic_cast<TrackerHit*>( (*trkhitVec)[iHit1] ) ) ;
      trk->addHit( dynamic_cast<TrackerHit*>( (*trkhitVec)[iHit2] ) ) ;
      trk->addHit( dynamic_cast<TrackerHit*>( (*trkhitVec)[iHit3] ) ) ;
      
      
      // add tracks that where used to create this track
      if( trkVec->size() > 1 ){
	trk->addTrack( dynamic_cast<TrackImpl*> ( (*trkVec)[ trkVec->size() - 1 ] ) ) ;
	trk->addTrack( dynamic_cast<TrackImpl*> ( (*trkVec)[ trkVec->size() - 2 ] ) ) ;
      }
      
      trkVec->addElement( trk ) ;
    }


    evt->addCollection(  trkVec , "SomeTracks" ) ;


    // create some clusters and add them to the event
    std::string simcalHitName( "ECAL007" ) ;

    LCCollection* simcalHits = evt->getCollection( simcalHitName ) ;

    //     // create a collection with copied simhits and modify these
    //     // (test copy constructor  - NOT YET AVAILABLE FOR OTHER CLASSES !)
    //     LCCollection* modifiedSimCalHits = new LCCollectionVec( LCIO::SIMCALORIMETERHIT );

    LCCollectionVec* clusterVec = new LCCollectionVec( LCIO::CLUSTER )  ;
    LCCollectionVec* calHits = new LCCollectionVec( LCIO::CALORIMETERHIT )  ;
    // in order to be able to point back to hits, we need to create 
    // generic CalorimeterHits from the SimCalorimeterHits first


    LCCollectionVec* scRel = new LCCollectionVec(LCIO::LCRELATION ) ;
    scRel->parameters().setValue( "RelationFromType" ,  LCIO::CALORIMETERHIT ) ;
    scRel->parameters().setValue( "RelationToType"   ,  LCIO::SIMCALORIMETERHIT ) ;
    
    int nSimHits = simcalHits->getNumberOfElements() ;
    for(int j=0;j<nSimHits;j++){

      CalorimeterHitImpl* calHit = new CalorimeterHitImpl ;
      SimCalorimeterHit* simcalHit =  dynamic_cast<SimCalorimeterHit*> ( simcalHits->getElementAt(j)  ) ;

      //      std::cout << " adding new calorimeter hit and relation : " << j << " : "  << calHit << " - " << simcalHit << std::endl ;

      calHit->setEnergy(   simcalHit->getEnergy()  ) ;
      calHit->setCellID0(  simcalHit->getCellID0() ) ;
      calHit->setPosition( simcalHit->getPosition()) ;

      //       scRel->addRelation( calHit , simcalHit , 0.5 ) ;
      //       scRel->addRelation( calHit , simcalHit , 0.5 ) ;
      scRel->addElement( new LCRelationImpl( calHit , simcalHit , 0.5 ) ) ;
      scRel->addElement( new LCRelationImpl( calHit , simcalHit , 0.5 ) ) ;
      scRel->addElement( new LCRelationImpl( calHit , simcalHit , 0.5 ) ) ;
      calHits->addElement( calHit ) ;

      //       // create a copy of sim hit and modify it
      //       SimCalorimeterHitImpl* mSimHit = new SimCalorimeterHitImpl( *simcalHit ) ;
      //       mSimHit->setEnergy(  mSimHit->getEnergy() * 1000. ) ;
      //       modifiedSimCalHits->addElement( mSimHit ) ;

    }
    evt->addCollection( calHits , "CalorimeterHits") ;
    //     evt->addCollection( modifiedSimCalHits , "ModifiedSimCalorimeterHits") ;

    // LCFlagImpl relFlag(0) ;
    // relFlag.setBit( LCIO::LCREL_WEIGHTED ) ;
    // scRel->setFlag( relFlag.getFlag()  ) ;
    scRel->setFlag( UTIL::make_bitset32(  LCIO::LCREL_WEIGHTED )) ;

    evt->addCollection( scRel , "CalorimeterHitsSimRel" ) ;
    //    evt->addRelation( scRel , "CalorimeterHitsSimRel" ) ;
    



    if( evt->getEventNumber() == 0 && evt->getRunNumber() == 0 ) {


      //------  the following is some example code on how to access the relation: --------------
      // create a navigation object from a collection
      LCRelationNavigator rel( scRel ) ; 

      std::cout << "Relation example for first event: " 
		<< " [" << rel.getFromType() << " - "  << rel.getToType()  << "] " 
		<< std::endl ;

      int nCalHits = calHits->getNumberOfElements() ;
      for(int j=0; j < nCalHits ; j++){
	CalorimeterHit* calHit = dynamic_cast<CalorimeterHit*>( calHits->getElementAt(j) ) ;
	
	std::cout << "   relations for object " << dec << calHit->id()  
	  ; // << std::endl ;
	
	const LCObjectVec& simHits = rel.getRelatedToObjects( calHit ) ;
	const FloatVec& weights = rel.getRelatedToWeights( calHit ) ;

	for(int k=0,nSH = simHits.size(); k<nSH;k++){
	  
	  std::cout << " [" << simHits[k]->id() << "] (" 
		    <<  weights[k]  << ") "  ;
	}
	std::cout << dec << std::endl ;
      }


      // -------------------------------------------------------------------------------------
      
      // add some calibration constants as generic user objects

      LCCollectionVec* calVec = new LCCollectionVec( LCIO::LCGENERICOBJECT )  ;
      for(int j=0;j<nCalHits;j++){
	
	CalorimeterHit* calHit = dynamic_cast<CalorimeterHit*>( calHits->getElementAt(j) ) ;
	
	CalibrationConstant* cCon  = new CalibrationConstant( calHit->getCellID0() ,
							      1.*j , 0.01*j );
	calVec->addElement( cCon ) ;
      }    
      
      evt->addCollection(  calVec , "Calibration" ) ;
    }

    // debug test: add empty collection of LCGenericObjects
    LCCollectionVec* emtpyCol = new LCCollectionVec( LCIO::LCGENERICOBJECT )  ;
    evt->addCollection(  emtpyCol , "EmptyLCGenericObject" ) ;

    // -------------------------------------------------------------------------------------
    
    // if we want to point back to the hits we need to set the flag
    // LCFlagImpl clusterFlag(0) ;
    // clusterFlag.setBit( LCIO::CLBIT_HITS ) ;
    // clusterVec->setFlag( clusterFlag.getFlag()  ) ;
    clusterVec->setFlag( UTIL::make_bitset32( LCIO::CLBIT_HITS )) ; 
   
    StringVec shapeParams ;
    shapeParams.push_back("Shape_trans") ;
    shapeParams.push_back("Shape_long") ;
    shapeParams.push_back("Shape_axis_x") ;
    shapeParams.push_back("Shape_axis_y") ;
    shapeParams.push_back("Shape_axis_z") ;
    shapeParams.push_back("Shape_quality") ;
    
    clusterVec->parameters().setValues( "ClusterShapeParameters" , shapeParams ) ;
    
    //     IntVec algoIDs ;
    //     enum {
    //       RunEventProcessorID  = 1 ,
    //       anotherAlgorithmID,
    //       andYetAnotherAlgorithmID
    //     }	;
    
    //     algoIDs.push_back( RunEventProcessorID ) ;
    //     algoIDs.push_back( anotherAlgorithmID ) ;
    //     algoIDs.push_back( andYetAnotherAlgorithmID ) ;
    
    //     StringVec algoNames ;
    //     algoNames.push_back("recojob-RunEventProcessor") ;
    //     algoNames.push_back("anotherAlgorithm") ;
    //     algoNames.push_back("andYetAnotherAlgorithm") ;
    
    //     clusterVec->parameters().setValues( "PIDAlgorithmTypeName" , algoNames ) ;
    //     clusterVec->parameters().setValues( "PIDAlgorithmTypeID" , algoIDs ) ;
    
    
    
    if( calHits ){
      
      int nHits = calHits->getNumberOfElements() ;
      int nCluster = nHits / 10 ;
      
      
      
      PIDHandler cluPidHandler( clusterVec ) ;
	
      StringVec pNames ;
      pNames.push_back( "param0" );
      pNames.push_back( "param1" );
      pNames.push_back( "param2" );
	
      IntVec algoIDs(3) ;
      algoIDs[0] = cluPidHandler.addAlgorithm( "recojobRunEventProcessor" , pNames ) ;
      algoIDs[1] = cluPidHandler.addAlgorithm( "anotherAlgorithm" , pNames ) ;
      algoIDs[2] = cluPidHandler.addAlgorithm( "andYetAnotherAlgorithm" , pNames ) ;
	
      for( int i=0; i < nCluster ; i ++ ){
	
	ClusterImpl* cluster = new ClusterImpl ;

	// 	int type = ( Cluster::COMBINED << 16 | Cluster::CHARGED  ) ;
	cluster->setTypeBit( 1 ) ;
	cluster->setTypeBit( 7 ) ;
	cluster->setTypeBit( 11 ) ;
	
	cluster->setEnergy(  (i+1)*1.1 ) ;
	float pos[3] = { 12. ,123456789. , .0987654321 } ;
	cluster->setPosition( pos ) ;
	float errpos[6] = { 1.,2.,3.,4.,5.,6.} ; 
	cluster->setPositionError( errpos ) ;
	cluster->setITheta( (i+1)* M_PI / 10. ) ;
	cluster->setIPhi( (i+1)* M_PI / 5. ) ;
	float errdir[6] = { 1.,2.,3.} ;
	cluster->setDirectionError( errdir ) ;

	// set the cluster ashape variables
	float shapeArray[6] = { 1.,2.,3.,3.,2.,1.} ;
	FloatVec shape ;
	copy( &shapeArray[0] , &shapeArray[5] , back_inserter( shape ) ) ;
	cluster->setShape( shape ) ;

	// 	// add some particle ids
	// 	int nPID = 5 ;
	// 	for(int j=0;j<nPID;j++){
	// 	  ParticleIDImpl* pid = new ParticleIDImpl ;
	// 	  pid->setLikelihood( (double) j / nPID ) ;
	// 	  pid->setType( j ) ;
	// 	  pid->setPDG( -11 ) ;
	// 	  pid->setAlgorithmType( RunEventProcessorID ) ;

	// 	  for(int k=0;k<3;k++){
	// 	    pid->addParameter( k*.1 ) ;
	// 	  }
	// 	  cluster->addParticleID( pid ) ;
	// 	}      
	// add some particle ids
	int nPID =  algoIDs.size() ;

	for(int j=0;j<nPID;j++){

	  
	  // some parameters
	  FloatVec fv( pNames.size()  ) ;
	  for( unsigned k=0 ; k < pNames.size() ; k++){
	    fv[k] =  j*1000.+k*.1  ;
	  }
	  
	  cluPidHandler.setParticleID( cluster,  
				       j  , // user type
				       22 , // PDG 
				       1.*j / nPID , // likelihood
				       algoIDs[j] ,
				       fv ) ;

	}      


	// add some subdetector energies
	const int NCALORIMETER = 2 ;
	const int ECALINDEX = 0 ;
	const int HCALINDEX = 1 ;
	StringVec detNames ;
	detNames.resize(  NCALORIMETER ) ;
	detNames[ECALINDEX] = "Ecal" ;
	detNames[HCALINDEX] = "Hcal" ;
	clusterVec->parameters().setValues( "ClusterSubdetectorNames" , detNames ) ;
	

	cluster->subdetectorEnergies().resize( NCALORIMETER )  ;
	cluster->subdetectorEnergies()[ ECALINDEX ] = 42.42 ;
	cluster->subdetectorEnergies()[ HCALINDEX ] = 24.24 ;

	// add some random hits 
	int iHit1 = (int) ( double (calHits->size()) * rand() / RAND_MAX )    ;
	int iHit2 = (int) ( double (calHits->size()) * rand() / RAND_MAX )    ;
	int iHit3 = (int) ( double (calHits->size()) * rand() / RAND_MAX )    ;
	
	cluster->addHit( dynamic_cast<CalorimeterHit*>( (*calHits)[iHit1] ) , 1.0 ) ;
	cluster->addHit( dynamic_cast<CalorimeterHit*>( (*calHits)[iHit2] ) , 2.0 ) ;
	cluster->addHit( dynamic_cast<CalorimeterHit*>( (*calHits)[iHit3] ) , 3.0 ) ;
	
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

    // add some vertices
    LCCollectionVec* vertexVec = new LCCollectionVec( LCIO::VERTEX ) ;

    //EXP: INDEX MAP - UNDER DEVELOPMENT
    //UTIL::IndexMap imvtx(vertexVec, "AlgorithmNames", "AlgorithmTypes");
    
    for(int i=0; i < (nRecP+1); i++){
      VertexImpl* vtx = new VertexImpl ;
      if(i==0){
	vtx->setPrimary(true);
      }else{
	vtx->setPrimary(false);
      }
      /*
      //EXP: INDEX MAP - UNDER DEVELOPMENT
      
      switch(i){
      case 0: vtx->setAlgorithmType( imvtx.encode( "ZvTop" ) ); break;
      case 1: vtx->setAlgorithmType( imvtx.encode( "ZvKin" ) ); break;
      case 5: vtx->setAlgorithmType( imvtx.encode( "SimAnnealing" ) ); break;
      default: break;
      }
      */

      //EXP: INDEX MAP V2 - UNDER DEVELOPMENT
      switch(rand()%7){
      case 0: vtx->setAlgorithmType( "ZvTop" ); break;
      case 1: vtx->setAlgorithmType( "ZvKin" ); break;
      case 2: vtx->setAlgorithmType( "42" ); break;
      case 3: vtx->setAlgorithmType( "SimAnnealing" ); break;
      case 5: vtx->setAlgorithmType( "_Test" ); break;
      default: break;
      }
      
      vtx->setChi2(1+i*.01);
      vtx->setProbability(0.0032+i*.01);
      vtx->setPosition(0.3453+i*.01,.45345354+i*.01,2.345354+i*.01);

      FloatVec cov(6) ;
      cov[0] = 1. ;
      cov[1] = 2. ;
      cov[2] = 3. ;
      cov[3] = 4. ;
      cov[4] = 5. ;
      cov[5] = 6. ;
      vtx->setCovMatrix( cov ) ;
      for(int j=0;j<3;j++){
	vtx->addParameter( j*.1 ) ;
      }
      
      vertexVec->addElement ( vtx ) ;
    }

    evt->addCollection( vertexVec, "SomeVertices" ) ;
    
    // add some reconstructed particles
    LCCollectionVec* particleVec = new LCCollectionVec( LCIO::RECONSTRUCTEDPARTICLE )  ;
    //     particleVec->parameters().setValues( "PIDAlgorithmTypeName" , algoNames ) ;
    //     particleVec->parameters().setValues( "PIDAlgorithmTypeID" , algoIDs ) ;
    
    if( particleVec ){

      PIDHandler recPIDHandler ( particleVec ) ;
	
      StringVec pNames ;
      pNames.push_back( "param0" );
      pNames.push_back( "param1" );
      pNames.push_back( "param2" );
      pNames.push_back( "param3" );
      pNames.push_back( "param4" );

      IntVec aIDs(4) ;
      aIDs[0] = recPIDHandler.addAlgorithm( "recojobRunEventProcessor" , pNames ) ;
      aIDs[1] = recPIDHandler.addAlgorithm( "anotherAlgorithm" , pNames ) ;
      aIDs[2] = recPIDHandler.addAlgorithm( "andYetAnotherAlgorithm" , pNames ) ;
      aIDs[3] = recPIDHandler.addAlgorithm( "andEvenAFourthAlgorithm" , pNames ) ;
	
      for(int i=0;i<nRecP;i++){
	ReconstructedParticleImpl * part = new ReconstructedParticleImpl ;
	part->setType( 42 ) ;
	  
	float p[3] = { 1.1 , 2.2 , 3.3 } ;
	part->setMomentum( p ) ;
	part->setEnergy(  i*101.101 ) ;
	  
	float covA[] =  { 1.,2.,3.,4.,5.,6.,7.,8.,9.,10. } ;
	FloatVec cov(10) ;
	for(int j=0;j<10;j++) cov[j] = covA[j] ;
	  
	  
	part->setCovMatrix( cov) ;
	part->setMass( 0.511*i ) ;
	part->setCharge( -2./3. ) ;
	float x[3] = { 10.,20.,30. } ;
	part->setReferencePoint( x )  ;
	  
	//associate vertices
	part->setStartVertex( dynamic_cast<Vertex*>( vertexVec->getElementAt(i) )  ) ;
	VertexImpl* v = dynamic_cast<VertexImpl*>( vertexVec->getElementAt(i+1) ) ;
	//part->setEndVertex( v ) ;
	//associate particles to vertices
	v->setAssociatedParticle( dynamic_cast<ReconstructedParticle*>( part ) ) ;
	  
	// 	// add some particle ids
	// 	int nPID = 5 ;
	// 	for(int j=0;j<nPID;j++){
	// 	  ParticleIDImpl* pid = new ParticleIDImpl ;
	// 	  pid->setLikelihood( (double) j / nPID ) ;
	// 	  pid->setType( j ) ;
	// 	  pid->setPDG( -11 ) ;
	// 	  pid->setAlgorithmType( algoIDs[0] ) ;
	// 	  for(int k=0;k<3;k++){
	// 	    pid->addParameter( k*.1 ) ;
	// 	  }
	// 	  part->addParticleID( pid ) ;
	// 	  if( j == 2 ) 
	// 	    part->setParticleIDUsed( pid ) ;
	// 	}      
	  
	// add some particle ids
	int nPID =  aIDs.size() ;
	  
	for(int j=0;j<nPID;j++){
	    
	  // some parameters
	  FloatVec fv( pNames.size()  ) ;
	  for( unsigned k=0 ; k < pNames.size() ; k++){
	    fv[k] =  j*1000.+k*.1  ;
	  }
	    
	  recPIDHandler.setParticleID( part,  
				       j*j  , // user type
				       -11 , // PDG 
				       42.*j / nPID , // likelihood
				       aIDs[j] ,
				       fv ) ;
		  
    recPIDHandler.setParticleID( part,  
				       j*j  , // user type
				       13 , // PDG 
				       42.*j / nPID , // likelihood
				       aIDs[j] ,
				       fv ) ;
	    
	  if( j == 2 ) 

	    recPIDHandler.setParticleIDUsed( part,  aIDs[j] ) ;

	}      
	       
	part->setGoodnessOfPID( 0.7 ) ;
	  
	// some other particles
	if( i > 1  ){
	  ReconstructedParticle* p1 = 
	    dynamic_cast<ReconstructedParticle*> ( particleVec->getElementAt(i-1) ) ;
	  ReconstructedParticle* p2 = 
	    dynamic_cast<ReconstructedParticle*> ( particleVec->getElementAt(i-2) ) ;
	  part->addParticle( p1 ) ;
	  part->addParticle( p2 ) ;
	}
	//a track
	int iTrk = (int) ( double (trkVec->size()) * rand() / RAND_MAX )    ;
	Track* trk = dynamic_cast<Track*> ( trkVec->getElementAt( iTrk ) ) ;
	part->addTrack( trk ) ;
	  
	// a cluster 
	int iClu = (int) ( double (clusterVec->size()) *  rand() / RAND_MAX )  ;
	Cluster* clu = dynamic_cast<Cluster*> ( clusterVec->getElementAt( iClu ) ) ;
	part->addCluster( clu ) ;
	  
	//       // and finaly an MCParticle
	//       LCCollection* mcVec = evt->getCollection( LCIO::MCPARTICLE )  ;
	//       int iMCP = (int) ( double (mcVec->getNumberOfElements()) *  rand() / RAND_MAX ) ;
	//       MCParticle* mcp = dynamic_cast<MCParticle*>( mcVec->getElementAt( iMCP ) ) ;
	//       part->addMCParticle( mcp , 0.5 ) ;
	  
	particleVec->addElement( part ) ;
      }
    }

    evt->addCollection( particleVec, "ReconstructedParticle" )  ;

    nEvent ++ ;


    
    LCTOOLS::dumpEvent( evt ) ;


    lcWrt->writeEvent( evt ) ;
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

  }
  

  void processRunHeader( LCRunHeader* run){

    // just copy run headers to the outputfile
    lcWrt->writeRunHeader( run ) ;
  }

  void modifyRunHeader(LCRunHeader*  /*run*/){/*  we don't modify anything */;}


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

