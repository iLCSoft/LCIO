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
  evt = lcReader->readNextEvent(LCIO::UPDATE);
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

  
  if( col !=0  &&( col->getTypeName() == LCIO::RECONSTRUCTEDPARTICLE ) 
      &&  col->getNumberOfElements() > 0 ){

    cout << col->getTypeName();
    
    ReconstructedParticleImpl* recP = dynamic_cast<ReconstructedParticleImpl*>( col->getElementAt( 0 ) ) ;

    VertexImpl* vtx = new VertexImpl ;
    
    vtx->setPrimary(true);
    vtx->setChi2(1.01);
    vtx->setProbability(0.0032);
    vtx->setPosition(0.3453,.45345354,2.345354);
    vtx->setAssociatedParticle(recP);
   
    Vertex* v;
    v=dynamic_cast<Vertex*>(vtx);
    
    cout << endl<<endl;
    cout << "Vertex Details:" << endl<<endl;
    cout << "Primary:\t" << v->isPrimary() << endl;
    cout << "Position:\tx:" << v->getPosition()[0] << "\ty: " << v->getPosition()[1] << "\tz: " << v->getPosition()[2] << endl;
    cout << "Chi2:\t\t" << v->getChi2() << endl;
    cout << "Probability:\t" << v->getProbability() << endl;
    cout << "AParticle:\t" << v->getAssociatedParticle()->getType() << endl;
    
    recP->setStartVertex( v );
    
    cout << "recP->getStartVertex()->getChi2() : " << recP->getStartVertex()->getChi2()<<endl;
    //cout << "recP->getEndVertex()->getChi2() : " << recP->getEndVertex()->getChi2()<<endl;

    // do not delete if vertex attached to collection and event !!!!
    delete vtx;

  }else{

    cout << " collection not of type " << LCIO::RECONSTRUCTEDPARTICLE << " or empty ! " << endl ;
  }

  
  lcReader->close() ;
  delete lcReader ;
  
  return 0 ;
}

  
