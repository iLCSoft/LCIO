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
#include "IMPL/LCFlagImpl.h" 
#include "IMPL/LCTOOLS.h"


#include <cstdlib>
#include <iostream>

using namespace std ;
using namespace lcio ;


static  char* FILEN = "simjob.slcio" ;
static char* OUTFILEN = "recjob.slcio" ;
static const int NHITS = 50 ;  // calorimeter hits per event


/** Example of reading events from the file, add sth. to the event
 * and write it to a new file. This uses the listener mechanism to read 
 * the input file (run headers and events) record by record.
 */


// class for processing run and event records 
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
	 << " added collection HCALReco with " << NHITS << " hits" 
	 << " to   " << nEvent <<" events"  
	 << " and added one extra MCParticle to each event."
	 << endl << endl ;
  }
  
  void analyze( const LCEvent * evt ) { /* used for read only access*/ 

    // trying to modify objects here will cause a ReadOnlyExcpetion
    // uncomment the following code to try:

//     LCCollection* mcVec = evt->getCollection( LCIO::MCPARTICLE )  ;
//     int NMCPART = mcVec->getNumberOfElements() ;
//     for(int i=0 ; i< NMCPART ; i++ ){
//       MCParticleImpl* part =  dynamic_cast<MCParticleImpl*>( mcVec->getElementAt(i)) ;
//       part->setPDG(1234) ;   // <<<<< not allowed !
//     }
    
  }
  
  void update( LCEvent * evt ) {
    // this is our event loop code
    
    // read collection with MCParticles
    LCCollection* mcVec = evt->getCollection( LCIO::MCPARTICLE )  ;
    
    int NMCPART = mcVec->getNumberOfElements() ;
    
    // here we can modify existing objects that have been read from a stream:
    for(int i=0 ; i< NMCPART ; i++ ){
      MCParticleImpl* part =  dynamic_cast<MCParticleImpl*>( mcVec->getElementAt( i )) ;
      part->setPDG(1234) ;
    }


    // create a new collection to be added to the event
    // for simplicity just add some calorimeter hits (don't have cluster class yet) 
    
    LCCollectionVec* calVec = new LCCollectionVec( LCIO::SIMCALORIMETERHIT )  ;

    // set flag for short format (not including position )
    // and no PDG     
    LCFlagImpl chFlag ;
    chFlag.unsetBit( LCIO::CHBIT_LONG ) ;  // not really necessary - just for demonstration
    chFlag.unsetBit( LCIO::CHBIT_PDG ) ;
    calVec->setFlag( chFlag.getFlag()  ) ;

    
    for(int j=0;j<NHITS;j++){
      SimCalorimeterHitImpl* hit = new SimCalorimeterHitImpl ;
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
    
    try{

      evt->addCollection( (LCCollection*) calVec , "HCALReco" ) ;

    }catch(EventException& e){

      cout << "excpetion: " << e.what() << endl ;
      // no need to exit...
    }


    
    LCTOOLS::dumpEvent( evt ) ;
    
    lcWrt->writeEvent( evt ) ;
    nEvent ++ ;

  }

  void update(LCRunHeader* run){/*no_op*/;}

  // don't manipulate run headers - use analyze 
  void analyze(const LCRunHeader* run){

    // just copy run headers to the outputfile
    lcWrt->writeRunHeader( run ) ;

  }

} ;

//=============================================================================

int main(int argc, char** argv ){
  
  //  try{ // a large try block for debugging ....
    srand(1234) ;
    
    // create reader and writer for input and output streams 
    LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
    
    
    // read file name from command line 
    if( argc > 1 ) { FILEN = argv[1] ; }
    if( argc > 2 ) { OUTFILEN = argv[2] ; }

    try{  lcReader->open( FILEN ) ; } 
    
    catch( IOException& e){
      cout << "Can't open file : " << e.what()  << endl ;
      exit(1) ;
    }
    
    // create a new RunEventProcessor, register it with the reader
    // and read and proccess the whole stream 
    {
      RunEventProcessor evtProc ;
      
      lcReader->registerLCRunListener( &evtProc ) ; 
      lcReader->registerLCEventListener( &evtProc ) ; 
      
      lcReader->readStream() ;
      try{ 
	//lcReader->readStream() ;
      }
      catch(IOException &e){
	cout<< " io error: " <<  e.what() << endl ;
      }
      catch(exception& ex){
	cout << "something went wrong:  " << ex.what() << endl ; 
      }
      
    } 
    
    lcReader->close() ;
    
//  }  catch(exception& ex){
//    cout << "something went wrong:  " << ex.what() << endl ; 
//  }
  
  return 0 ;
  
}

//=============================================================================

