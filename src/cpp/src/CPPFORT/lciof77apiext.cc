#include "CPPFORT/lciof77apiext.h"

#include "lcio.h" 
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/SimTrackerHitImpl.h"
#include "IMPL/SimCalorimeterHitImpl.h"
#include "IMPL/LCTOOLS.h"

#include "CPPFORT/HEPEVT.h"

#include <iostream>


using namespace lcio ;
using namespace std ;
using namespace HEPEVTIMPL ;



PTRTYPE lcwriterunheader( PTRTYPE writer , const int irun , const char* detname , 
			  const char* description , void* sdnamevec , const int nsubd, 
			  const int nchsd){

  try {
    LCWriter* lcWriter = reinterpret_cast<LCWriter*>( (writer) ) ;
    LCRunHeaderImpl* runHdr =  new LCRunHeaderImpl ;
    runHdr->setRunNumber( irun ) ;
    runHdr->setDetectorName( detname ) ;
    runHdr->setDescription( description ) ;

    int elemlen = nchsd + 1 ;
    int stringpos = reinterpret_cast<int>( sdnamevec ) ;
    for (int j=0;j < nsubd;j++)
    {
        char* subdetectorname = reinterpret_cast<char*>( stringpos ) ;
        runHdr->addActiveSubdetector( subdetectorname ) ;
        stringpos = stringpos + elemlen ;
    }
    lcWriter->writeRunHeader( runHdr ) ;
    delete runHdr ;
    return LCIO::SUCCESS ;
  }catch( Exception& e1) {
    std::cerr << "Exception in LCWriteRunHeader: " << e1.what() << std::endl ;
    return LCIO::ERROR ;
  }
}

PTRTYPE lcreadnextrunheader(PTRTYPE reader , int* irun , void* detname , void* description ,
                            void* sdnamevec , int* nsubd , const int nchsubd){

  LCReader* lcReader = reinterpret_cast<LCReader*>( (reader) ) ;
  try {
    LCRunHeader* runHdr = lcReader->readNextRunHeader() ;
    *irun = runHdr->getRunNumber() ;

    int stringpos ;
    stringpos = reinterpret_cast<int>( detname ) ;
    char* detname = reinterpret_cast<char*>( stringpos ) ;
    const char* detectorname = runHdr->getDetectorName().c_str() ;
    strcpy(detname,detectorname) ;
    stringpos = reinterpret_cast<int>( description ) ;
    char* descr = reinterpret_cast<char*>( stringpos ) ;
    const char* rundescription = runHdr->getDescription().c_str() ;
    strcpy(descr,rundescription) ;

    const std::vector<std::string>* strVec = runHdr->getActiveSubdetectors() ;

    int elemlen = nchsubd + 1;
    *nsubd  = strVec->size() ;
    stringpos = reinterpret_cast<int>( sdnamevec ) ;
    for( std::vector<std::string>::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
      std::string out = *name ;
      char* tmpname = reinterpret_cast<char*>( stringpos ) ;
      const char* subdname = const_cast<char*>( out.c_str() ) ;
      strcpy(tmpname,subdname) ;
      stringpos = stringpos + elemlen ;
    }

    return reinterpret_cast<PTRTYPE>(runHdr) ;
  }catch( Exception& e1) {
    std::cerr << "Exception in LCReadNextRunHeader: " << e1.what() << std::endl ;
    return LCIO::ERROR ;
  }
}



int lcseteventheader( PTRTYPE event, const int irun, const int ievent, const int timestamp, const char* detname ){
  LCEventImpl* lcEventImpl = reinterpret_cast<LCEventImpl*>( (event) ) ;
  lcEventImpl->setRunNumber( irun ) ;
  lcEventImpl->setEventNumber( ievent ) ;
  lcEventImpl->setTimeStamp( timestamp ) ;
  lcEventImpl->setDetectorName( detname ) ;
  return LCIO::SUCCESS ;
}

int lcgeteventheader( PTRTYPE event, int* irun, int* ievent, int* timestamp, void* detname ){
  LCEventImpl* lcEvent = reinterpret_cast<LCEventImpl*>( (event) ) ;
  *irun = lcEvent->getRunNumber() ;
  *ievent = lcEvent->getEventNumber() ;
  *timestamp = lcEvent->getTimeStamp() ;
  int stringpos = reinterpret_cast<int>( detname ) ;
  char* detnam = reinterpret_cast<char*>( stringpos ) ;
  const char* detectorname = lcEvent->getDetectorName().c_str() ;
  strcpy(detnam,detectorname) ;
  return LCIO::SUCCESS ;
}

PTRTYPE lcdumpevent( PTRTYPE event ){
  LCEventImpl* lcEvent = reinterpret_cast<LCEventImpl*>( (event) ) ;
  LCTOOLS::dumpEvent(  lcEvent ) ;
  return LCIO::SUCCESS ;
}

PTRTYPE lcdumpeventdetailed ( PTRTYPE event ){
  LCEventImpl* lcEvent = reinterpret_cast<LCEventImpl*>( (event) ) ;
  LCTOOLS::dumpEventDetailed(  lcEvent ) ;
  return LCIO::SUCCESS ;
}


int lcgetmcparticledata( PTRTYPE mcparticle, int* pdg, int* genstatus, int* simstatus
			 , double* prodvtx, float* momentum, float* mass, float* charge, 
			 int* ndaughters ){

  MCParticle* lcMCParticle = f2c_pointer<MCParticle,LCObject>( mcparticle ) ;

  *pdg               = lcMCParticle->getPDG() ;
  *genstatus         = lcMCParticle->getGeneratorStatus() ;
  *simstatus         = lcMCParticle->getSimulatorStatus() ;
  const double* dtmp = lcMCParticle->getVertex() ;
  for(int k=0;k<3;k++)  *prodvtx++  = dtmp[k] ;
  const float*  tmp  = lcMCParticle->getMomentum() ;
  for(int k=0;k<3;k++)  *momentum++ = tmp[k] ;
  *mass              = lcMCParticle->getMass() ;
  *charge            = lcMCParticle->getCharge() ;
  *ndaughters        = lcMCParticle->getNumberOfDaughters() ;
  return LCIO::SUCCESS ;
}


int lcaddsimtrackerhit( PTRTYPE collectionvec, int cellID, double* pos, float dEdx, 
			    float time, PTRTYPE mcp ){

  LCCollectionVec* lcCollectionVec = reinterpret_cast<LCCollectionVec*>( (collectionvec) ) ;
  SimTrackerHitImpl* hit = new SimTrackerHitImpl ;
  MCParticle* mmcp = reinterpret_cast<MCParticle*>( mcp ) ;

  hit->setCellID( cellID ) ;
  hit->setPosition( pos ) ;
  hit->setdEdx( dEdx ) ;
  hit->setTime( time ) ;
  hit->setMCParticle( mmcp ) ;

  lcCollectionVec->push_back( hit ) ;
  return LCIO::SUCCESS ;
}

int lcgetsimtrackerhit( PTRTYPE collection, int i, int* cellID, double* pos, float* dEdx, float* time, PTRTYPE* mcp ){
  LCCollectionVec* lcCollection = reinterpret_cast<LCCollectionVec*>( (collection) ) ;
  SimTrackerHit* hit =  dynamic_cast<SimTrackerHit*>( lcCollection->getElementAt( i-1 ) ) ;

  *cellID = hit->getCellID() ;
  const double* tmp = hit->getPosition() ;
  for(int k=0;k<3;k++)  *pos++ = tmp[k] ;
  *dEdx   = hit->getdEdx() ;
  *time   = hit->getTime() ;
  *mcp    = reinterpret_cast<PTRTYPE>( hit->getMCParticle() ) ;
  return LCIO::SUCCESS ;
}


PTRTYPE lcaddsimcalohit(  PTRTYPE col, int cellID0, int cellID1, float energy, float* pos ) {
  LCCollectionVec* lcCollection = reinterpret_cast<LCCollectionVec*>( col ) ;
  SimCalorimeterHitImpl* lcHit = new SimCalorimeterHitImpl ;
  lcHit->setCellID0( cellID0 ) ;
  lcHit->setCellID1( cellID1 ) ;
  lcHit->setEnergy( energy ) ;
  lcHit->setPosition( pos ) ;
  lcCollection->addElement( lcHit ) ;
  return C2F_POINTER( LCObject*, lcHit ) ;
}


PTRTYPE lcgetsimcalohit( PTRTYPE collection, int i, int* cellID0, int* cellID1, 
			 float* energy, float* pos ) {

  LCCollectionVec* lcCollection = reinterpret_cast<LCCollectionVec*>( collection ) ;
  
  // checking on i !
  const int Nelements = lcCollection->getNumberOfElements() ;
  if (i < 0 && i > Nelements-1) {
    std::cerr << "Exception in LCGetSimCaloHit: index " << i << " out of range" << std::endl ;
    return LCIO::ERROR ;
  }
  
  SimCalorimeterHit* lcHit = dynamic_cast<SimCalorimeterHit*>( lcCollection->getElementAt( i-1 ) ) ;
  *cellID0 = lcHit->getCellID0() ;
  *cellID1 = lcHit->getCellID1() ;
  *energy  = lcHit->getEnergy() ;
  const float* tmp = lcHit->getPosition() ;
  for(int k=0;k<3;k++)  *pos++ = tmp[k] ;

  return C2F_POINTER( LCObject*, lcHit ) ;
}

int lcgetsimcalohitmccont( PTRTYPE hit, int i, PTRTYPE* mcp, float* energy, float* time, int* pdg ) {

  SimCalorimeterHitImpl* lcHit = f2c_pointer<SimCalorimeterHitImpl,LCObject>( hit ) ;
// checking on i !
  const int Nelements = lcHit->getNMCParticles() ;
  if (i < 0 && i > Nelements-1) {
    std::cerr << "Exception in lcgetsimcalohitmccont: index " << i << " out of range" << std::endl ;
    return LCIO::ERROR ;
  }

  *mcp    = C2F_POINTER( LCObject*, lcHit->getParticleCont(i-1)  )  ;
  *energy = lcHit->getEnergyCont( i-1 ) ;
  *time   = lcHit->getTimeCont( i-1 ) ;
  *pdg    = lcHit->getPDGCont( i-1 ) ;
  return LCIO::SUCCESS ;
}

int hepevt2lcio( PTRTYPE evtout ){
  LCEventImpl* lcEvent = reinterpret_cast<LCEventImpl*>( evtout ) ;
  try { 
    HEPEVT::fromHepEvt(  lcEvent ) ;
    return LCIO::SUCCESS ;
  }catch( Exception& e1) {
    std::cerr << "Exception in lcio2hepevt: " << e1.what() << std::endl ;
    return LCIO::ERROR ;
  }
}


int lcio2hepevt( PTRTYPE event ){
  LCEvent* lcEvent = reinterpret_cast<LCEventImpl*>( event ) ;
  try { 
    HEPEVT::toHepEvt(  lcEvent ) ;
    return LCIO::SUCCESS ;
  }catch( Exception& e1) {
    std::cerr << "Exception in hepevt2lcio: " << e1.what() << std::endl ;
    return LCIO::ERROR ;
  }
}

