
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
#include "UTIL/LCRelationNavigator.h"
#include "UTIL/LCStdHepRdr.h"

#include <cstdlib>
#include <iostream>
#include <sstream>


using namespace std ;
using namespace lcio ;

static const int NEVENT = 10 ; // events

static string OUTFILENAME = "stdhepjob.slcio" ;

char *  INFILENAME;


/** Simple test program to demonstrate reading of binary .stdhep generator files.
 * Writes MCParticle collections to the outputfile stdhepjob.slcio.
 */

int main(int argc, char** argv ){
  
  try{

    if(argc < 2)
      {
	cout << "Usage: stdhepjob xxxx.stdhep " << endl;
        return 1;
      }
    
    INFILENAME = argv[1];
      
    // create sio writer
    LCWriter* lcWrt = LCFactory::getInstance()->createLCWriter() ;
      
    lcWrt->open( OUTFILENAME , LCIO::WRITE_NEW )  ;

    LCRunHeaderImpl* runHdr = new LCRunHeaderImpl ; 
    runHdr->setRunNumber( 0 ) ;
      
    string detName("Unknown")  ;
    runHdr->setDetectorName( detName ) ;
      
    stringstream description ; 
    description << " just for testing LCStdHepRdr" ;
    runHdr->setDescription( description.str()  ) ;
    lcWrt->writeRunHeader( runHdr ) ;


    // Open an instance of the StdHep Reader with the given filename
    LCStdHepRdr * rdr = new LCStdHepRdr(INFILENAME);
      
    for(int i=0;i<NEVENT;i++){
	
      LCEventImpl*  evt = new LCEventImpl() ;
	
	
      evt->setRunNumber(  0   ) ;
      evt->setEventNumber( i ) ;
      evt->setDetectorName( detName ) ;
	
      LCCollectionVec* mcVec = rdr->readEvent()  ;
	
	// add all collections to the event
      evt->addCollection( mcVec , "MCParticle" ) ;

      lcWrt->writeEvent( evt ) ;
	
    } // evt loop

    delete runHdr ;

    lcWrt->close() ;
    delete lcWrt ;

  
  } catch( Exception& ex){

    cout << " an excpetion occured: " << endl ;
    cout << "  " << ex.what() << endl ;
    return 1 ;
  }

  return 0 ;
}

