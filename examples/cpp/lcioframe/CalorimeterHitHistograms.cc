#include "CalorimeterHitHistograms.h"
#include "LCIOFrameSteer.h"

#include "EVENT/LCIO.h"
#include "EVENT/LCCollection.h"
#include "EVENT/SimCalorimeterHit.h"
#include "EVENT/CalorimeterHit.h"
#include "IMPL/LCFlagImpl.h"
#include "MyRootFile.h"

#include <sstream>

//------- need one global instance of  this module ----------------------------
CalorimeterHitHistograms calorimeterHitHistograms("CalorimeterHitHistograms") ;
//-----------------------------------------------------------------------------


CalorimeterHitHistograms::CalorimeterHitHistograms(const char* moduleName) 
  : LCIOModule(moduleName ),
    hfile(0),
    firstEventInRun( true ) {
  
} 

CalorimeterHitHistograms::~CalorimeterHitHistograms(){
  if(hfile) 
    hfile->Write() ;
}

void CalorimeterHitHistograms::processEvent(  LCEvent * evt ) {
  
  // as we look at arbitrary files we don't know the names of 
  // the collections with calorimeterhits
  // thus we loop over all collections and create subfolder with collection names
  
  typedef const std::vector< std::string > StringVec ;
  typedef  std::vector< const std::string* > StringPVec ;
  StringVec* strVec = evt->getCollectionNames() ;
  StringPVec calCols ;

  if(firstEventInRun){
    
    for( StringVec::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
      
      LCCollection* col = evt->getCollection( *name ) ;
      
      if(  col->getTypeName() == LCIO::SIMCALORIMETERHIT || 
	   col->getTypeName() == LCIO::CALORIMETERHIT  ){
	
	calCols.push_back( &(*name) ) ; 
	
	// make a subfolder and create histograms for every collection 
	runDir->mkdir( name->c_str() ) ;
	runDir->cd(  name->c_str()  ) ;
	
	hEhit =  new TH1F("hEhit"," hit energy",500,0.,.001);  
	hEhit->SetBit(TH1::kCanRebin) ;
	
	hElong =  new TH1F("hElong"," hit energy longitudenal",100,0.,10.);  
	hElong->SetBit(TH1::kCanRebin) ;
	
	hEradial =  new TH1F("hEradial"," hit energy radial",50,0.,10);  
	hEradial->SetBit(TH1::kCanRebin) ;
	
      }
    }
    firstEventInRun = false  ;
  }
  // now loop over cal collections

  for( StringPVec::const_iterator name = calCols.begin() ; name != calCols.end() ; name++){
      
    LCCollection* col = evt->getCollection( **name ) ;

    int nHits =  col->getNumberOfElements() ;

    if(  col->getTypeName() == LCIO::SIMCALORIMETERHIT ){
      for( int i=0 ; i< nHits ; i++ ){
	
	SimCalorimeterHit* hit = dynamic_cast<SimCalorimeterHit*>( col->getElementAt( i ) ) ;
	hEhit->Fill(  hit->getEnergy() ) ;
	
	LCFlagImpl flag( col->getFlag() ) ;
	if( flag.bitSet( LCIO::CHBIT_LONG )){  // postion stored
	  
	  hElong->Fill( hit->getPosition()[2] ,  hit->getEnergy() ) ;
	  float radius = sqrt( hit->getPosition()[0]*hit->getPosition()[0]
			       +hit->getPosition()[1]*hit->getPosition()[1] ) ;
	  hEradial->Fill( radius ,  hit->getEnergy() ) ;
	  
	}
      }
      
    }else{ // real data calorimeter hit
      
      for( int i=0 ; i< nHits ; i++ ){
	CalorimeterHit* hit = dynamic_cast<CalorimeterHit*>( col->getElementAt( i ) ) ;
	hEhit->Fill(  hit->getEnergy() ) ;
	
	LCFlagImpl flag( col->getFlag() ) ;
	if( flag.bitSet( LCIO::CHBIT_LONG )){  // postion stored
	  
	  hElong->Fill( hit->getPosition()[2] ,  hit->getEnergy() ) ;
	  float radius = sqrt( hit->getPosition()[0]*hit->getPosition()[0]
			       +hit->getPosition()[1]*hit->getPosition()[1] ) ;
	  hEradial->Fill( radius ,  hit->getEnergy() ) ;
	}
      }
    }
  } // cal collections
} 


void CalorimeterHitHistograms::processRunHeader( LCRunHeader* run) {

   firstEventInRun = true ;

   // create a folder this run - if it doesn't exist yet   
   stringstream runFolder ;
   runFolder << "run_" <<  run->getRunNumber() << ends ;
   
   runDir = dynamic_cast<TDirectory*>(  hfile->Get( runFolder.str().c_str() ) ) ;

   if( runDir == 0 )
     runDir =  hfile->mkdir( runFolder.str().c_str()   );
   runDir->cd() ;
}

void CalorimeterHitHistograms::initModule() {

  // open / reopen the root file

  std::stringstream rootFileName ;
  rootFileName << LCIOFrameSteer::baseFileName << ".root" << std::ends ;

  hfile = MyRootFile::getInstance( rootFileName.str().c_str() ) ; 
  
}
  
  
