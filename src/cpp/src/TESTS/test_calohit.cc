////////////////////////////////////////
//  test lcio::CalorimeterHit
////////////////////////////////////////

#include "tutil.h"
#include "lcio.h"

#include "EVENT/LCIO.h"
#include "IO/LCReader.h"
#include "IO/LCWriter.h"
#include "IMPL/LCEventImpl.h" 
#include "IMPL/LCCollectionVec.h"
#include "IMPL/CalorimeterHitImpl.h"
#include "IMPL/LCFlagImpl.h"

//#include <iostream>

using namespace std ;
using namespace lcio ;

//static const int NRUN = 10 ;
static const int NEVENT = 10 ; // events
static const int NHITS = 1000 ;  // calorimeter hits per event

static string FILEN = "calohit.slcio" ;

// replace mytest with the name of your test
const static string testname="test_calohit";

//=============================================================================

int main(int argc, char** argv ){
  
  // this should be the first line in your test
  TEST MYTEST=TEST( testname, std::cout );
  
  try{
    
    
    MYTEST.LOG( " writing CalorimeterHits " );
    
    // create sio writer
    LCWriter* lcWrt = LCFactory::getInstance()->createLCWriter()  ;
    
    lcWrt->open( FILEN , LCIO::WRITE_NEW ) ;
    
    // EventLoop - create some events and write them to the file
    for(int i=0;i<NEVENT;i++){
      
      // we need to use the implementation classes here 
      LCEventImpl*  evt = new LCEventImpl() ;
      
      
      evt->setRunNumber( 4711  ) ;
      evt->setEventNumber( i ) ;
      
      LCCollectionVec* calHits = new LCCollectionVec( LCIO::CALORIMETERHIT )  ;
      LCCollectionVec* calHitsErr = new LCCollectionVec( LCIO::CALORIMETERHIT )  ;
      LCFlagImpl calFlag( calHits->getFlag() ) ;
      calFlag.setBit( LCIO::RCHBIT_LONG ) ;
      calHits->setFlag( calFlag.getFlag()  ) ;

      calFlag.setBit( LCIO::RCHBIT_ENERGY_ERROR ) ;
      calHitsErr->setFlag( calFlag.getFlag()  ) ;
 
      for(int j=0;j<NHITS;j++){
	CalorimeterHitImpl* calHit = new CalorimeterHitImpl ;
	calHit->setEnergy( i*j*117. ) ;
	calHit->setCellID0( i+100000 + j ) ;
	float pos[3] = { i , j ,i*j } ;
	calHit->setPosition( pos ) ;
	calHits->addElement( calHit ) ;
      }
      for(int j=0;j<NHITS;j++){
	CalorimeterHitImpl* calHit = new CalorimeterHitImpl ;
	calHit->setEnergy( i*j*117. ) ;
	calHit->setEnergyError( i*j*0.117 ) ;
	calHit->setCellID0( i+100000 + j ) ;
	float pos[3] = { i , j ,i*j } ;
	calHit->setPosition( pos ) ;
	calHitsErr->addElement( calHit ) ;
      }
      evt->addCollection( calHits , "CalorimeterHits") ;
      evt->addCollection( calHitsErr , "CalorimeterHitsWithEnergyError") ;
      
      lcWrt->writeEvent(evt) ;
      
      delete evt ;
    }
    
    
    lcWrt->close() ;
    
    MYTEST.LOG(" reading back CalorimeterHits from file " ) ;
    
    // create sio reader
    LCReader* lcRdr = LCFactory::getInstance()->createLCReader()  ;
    
    lcRdr->open( FILEN ) ;
    
    for(int i=0;i<NEVENT;i++){
      
      //std::cout << " testing event " << i << std::endl ;

      LCEvent*  evt =  lcRdr->readNextEvent() ;
      
      MYTEST( evt->getRunNumber() ,  4711 , " run number " )  ;
      
      MYTEST(  evt->getEventNumber() ,  i , " event number "  ) ;
      
      LCCollection* calHits = evt->getCollection( "CalorimeterHits") ;
      LCCollection* calHitsErr = evt->getCollection( "CalorimeterHitsWithEnergyError") ;
      
      for(int j=0;j<NHITS;j++) {
	
	//std::cout << " testing hit " << j << std::endl ;
	
	CalorimeterHit* calHit = dynamic_cast<CalorimeterHit*>(calHits->getElementAt(j)) ;
	
        MYTEST( calHit->getEnergy() ,  i*j*117. , "energy" ) ;
        MYTEST( calHit->getCellID0() , i+100000 + j , " cellid0 " ) ;
	
        const float* pos = calHit->getPosition() ;
	
	MYTEST( pos[0] , i , " pos[0] " ) ;
	MYTEST( pos[1] , j , " pos[1] " ) ;
	MYTEST( pos[2] , i*j , " pos[2] " ) ;
	
      }
      for(int j=0;j<NHITS;j++) {
	
	//std::cout << " testing hit " << j << std::endl ;
	
	CalorimeterHit* calHit = dynamic_cast<CalorimeterHit*>(calHitsErr->getElementAt(j)) ;
	
        MYTEST( calHit->getEnergy() ,  i*j*117. , "energy" ) ;
        MYTEST( calHit->getEnergyError() , float(i*j*0.117)  , "energy error" ) ;
        MYTEST( calHit->getCellID0() , i+100000 + j , " cellid0 " ) ;
	
        const float* pos = calHit->getPosition() ;
	
	MYTEST( pos[0] , i , " pos[0] " ) ;
	MYTEST( pos[1] , j , " pos[1] " ) ;
	MYTEST( pos[2] , i*j , " pos[2] " ) ;
	
      }
     }
    lcRdr->close() ;
  
  
  } catch( Exception &e ){
     MYTEST.FAILED( e.what() );
  }
  
  return 0;
}

//=============================================================================

