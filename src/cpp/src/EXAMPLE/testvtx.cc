#include "lcio.h"

#include "IO/LCReader.h"
#include "IMPL/LCTOOLS.h"
#include "EVENT/LCRunHeader.h"
#include "EVENT/LCCollection.h"
#include "EVENT/ReconstructedParticle.h"
#include "IMPL/ReconstructedParticleImpl.h"
#include "IMPL/VertexImpl.h"

static const char* FILEN = "recjob.slcio" ; // default file name 

using namespace std ;
using namespace lcio ;

////////////////////////////////////////////////////////////////////////////////
// This file is temporary and ONLY for the purpose of testing the Vertex Class!!
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv ){

  LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
  lcReader->open( FILEN ) ;

  cout << " will open and read from file: " << FILEN << endl ;  

  LCRunHeader *runHdr ;
  
  // use a try catch block here: if sth. went wrong with reading the run data we 
  // still can try and read the event data - see below
  try{
    //read the first header
    runHdr = lcReader->readNextRunHeader();
    LCTOOLS::dumpRunHeader( runHdr ) ;
/*
    // loop over all run headers
    while( ( runHdr = lcReader->readNextRunHeader() ) != 0 ){
      
      LCTOOLS::dumpRunHeader( runHdr ) ;
//       cout << "  Run : " << runHdr->getRunNumber() 
// 	   << " - "      << runHdr->getDetectorName() 
// 	   << ":  "      << runHdr->getDescription()  << endl ;

    }
*/    
  }catch(IOException& e){
    cout << " io error when reading run data : " << e.what() << endl ;
  }
  cout << endl ;
  
  lcReader->close() ;
  
  
  // now loop over the file again and dump event data

  lcReader->open( FILEN ) ;

  cout << " reopened " << FILEN << " for reading " << endl ; 
  
  LCEvent* evt ;
  //int nEvents = 0 ;

  //read an event
  evt = lcReader->readNextEvent();
  LCTOOLS::dumpEvent( evt ) ;
/*
  //----------- the event loop -----------
  while( (evt = lcReader->readNextEvent()) != 0 ) {
    
    //LCTOOLS::dumpEvent( evt ) ;
    
    nEvents ++ ;
  } 
  // -------- end of event loop -----------
  
  cout << endl <<  "  " <<  nEvents << " events read from file: " << FILEN << endl  ;
*/


  LCCollection* col = evt->getCollection("ReconstructedParticle") ;
  cout << col->getTypeName();
  
  if( col->getTypeName() != LCIO::RECONSTRUCTEDPARTICLE ){
    cout << " collection not of type " << LCIO::RECONSTRUCTEDPARTICLE << endl ;
  }
  else{

#ifdef USE_CLHEP
      ReconstructedParticle4V recP( col->getElementAt( 0 ) ) ;
#else
      ReconstructedParticle* recP =
        dynamic_cast<ReconstructedParticle*>( col->getElementAt( 0 ) ) ;
#endif
  

  VertexImpl * vtx=new VertexImpl;
  
  vtx->setPrimary(true);
  vtx->setChi2(1.01);
  vtx->setProbability(0.0032);
  vtx->setPosition(0.3453,.45345354,2.345354);
  vtx->setAssociatedParticle(recP);
  
  cout << endl<<endl;
  cout << "Vertex Details:" << endl<<endl;
  cout << "Primary:\t" << vtx->isPrimary() << endl;
  cout << "Position:\tx:" << vtx->getPosition()[0] << "\ty: " << vtx->getPosition()[1] << "\tz: " << vtx->getPosition()[2] << endl;
  cout << "Chi2:\t\t" << vtx->getChi2() << endl;
  cout << "Probability:\t" << vtx->getProbability() << endl;
  cout << "AParticle:\t" << vtx->getAssociatedParticle()->getCharge() << endl;
  
  delete vtx;

  }
  
  lcReader->close() ;
  delete lcReader ;

  return 0 ;
}

  
