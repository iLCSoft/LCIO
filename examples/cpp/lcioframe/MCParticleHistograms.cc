#include "MCParticleHistograms.h"
#include "LCIOFrameSteer.h"

#include "EVENT/LCIO.h"
#include "EVENT/LCCollection.h"
#include "EVENT/MCParticle.h"
#include "IMPL/LCFlagImpl.h"

#include "MyRootFile.h"

#include <sstream>
  
//------- need one global instance of  this module ----------------
MCParticleHistograms mcParticleHistograms("MCParticleHistograms") ;
//-----------------------------------------------------------------


MCParticleHistograms::MCParticleHistograms(const char* moduleName) 
  : LCIOModule(moduleName ),
    hfile(0),
    firstEventInRun( true ) {
} 

MCParticleHistograms::~MCParticleHistograms(){

  if(hfile) 
    hfile->Write() ;

}

void MCParticleHistograms::processEvent(  LCEvent * evt ) {
  
  // as we look at arbitrary files we don't know the names of 
  // the collections with calorimeterhits
  // thus we loop over all collections and create subfolder with collection names
  
  typedef const std::vector< std::string > StringVec ;
  StringVec* strVec = evt->getCollectionNames() ;
  
  for( StringVec::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
    
    LCCollection* col = evt->getCollection( *name ) ;
    
    if(  col->getTypeName() == LCIO::MCPARTICLE ){
      
      if(firstEventInRun){

	// make a subfolder and create histograms for every collection 
	runDir->mkdir( name->c_str() ) ;
	runDir->cd(  name->c_str()  ) ;


	hEMother =  new TH1F("hEMother","energy of mother particles",100,0.,10.);  
	hEMother->SetBit(TH1::kCanRebin) ;
	
	hTypeMother =  new TH1F("hTypeMother"," type of mother particles",4000,-2000.,2000.);  
	hTypeMother->SetBit(TH1::kCanRebin) ;

	hEstable =  new TH1F("hEstable","energy of stable particles",100,0.,10.);  
	hEstable->SetBit(TH1::kCanRebin) ;
	
	hTypestable =  new TH1F("hTypestable"," type of stable particles",4000,-2000.,2000.);  
	hTypestable->SetBit(TH1::kCanRebin) ;
	

	firstEventInRun = false  ;
      }

      int nHits =  col->getNumberOfElements() ;

      for( int i=0 ; i< nHits ; i++ ){
	  
	MCParticle* particle = dynamic_cast<MCParticle*>( col->getElementAt( i ) ) ;

	if( particle->getNumberOfParents() == 0 ){  // mother
	  hEMother->Fill(  particle->getEnergy() ) ;
	  hTypeMother->Fill(  particle->getPDG() ) ;
	}
	if( particle->getNumberOfDaughters() == 0 ){  // 'stable' 
	  hEstable->Fill(  particle->getEnergy() ) ;
	  hTypestable->Fill(  particle->getPDG() ) ;
	}
      }
    }
  } // loop over collections
}

void MCParticleHistograms::processRunHeader( LCRunHeader* run) {

   firstEventInRun = true ;

   // create a folder this run  - if it doesn't exist yet  
   stringstream runFolder ;
   runFolder << "run_" <<  run->getRunNumber() << ends ;
   
   runDir = dynamic_cast<TDirectory*>(  hfile->Get( runFolder.str().c_str() ) ) ;

   if( runDir == 0 )
     runDir =  hfile->mkdir( runFolder.str().c_str() );
   runDir->cd() ;

}

void MCParticleHistograms::initModule() {

  // open / reopen the root file

  std::stringstream rootFileName ;
  rootFileName << LCIOFrameSteer::baseFileName << ".root" << std::ends ;

  hfile = MyRootFile::getInstance( rootFileName.str().c_str() ) ; 
  
}
  
  
