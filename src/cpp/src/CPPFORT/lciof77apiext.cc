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
#include "EVENT/LCIntVec.h"
#include "EVENT/LCFloatVec.h"
#include "EVENT/LCStrVec.h"

#include <string>
#include <vector>

// typedef std::vector<std::string> LCStrVec ;
                                                                                           
#include <iostream>


using namespace lcio ;
using namespace std ;
using namespace HEPEVTIMPL ;


static std::vector<std::string> filenamelist ;

int do_set_method (LCParameters& params, const char* method, const char* key, PTRTYPE vector);
int do_get_method (const LCParameters& params, const char* method, const char* key, PTRTYPE vector);


int lcrdropenchain( PTRTYPE reader, void* filenamesv , const int nfiles , const int nchfilename ){


  try {
//  if an old filenamelist exist clear it

    if ( filenamelist.size() > 0 ) filenamelist.clear() ;
    int elemlen = nchfilename +1 ;
    PTRTYPE stringpos = reinterpret_cast<PTRTYPE>( filenamesv ) ;
    for (int j=0;j < nfiles;j++)
    {
      char* filename = reinterpret_cast<char*>( stringpos ) ;
      filenamelist.push_back( filename ) ;
      stringpos = stringpos + elemlen ;
    }
    LCReader* lcReader = reinterpret_cast<LCReader*>( (reader) ) ;
    lcReader->open( filenamelist ) ;
    return LCIO::SUCCESS ;
  }catch( Exception& e1) {
    std::cerr << "Exception in LCRdrOpenChain: " << e1.what() << std::endl ;
    return LCIO::ERROR ;
  }
}

int lcwriterunheader( PTRTYPE writer , const int irun , const char* detname ,
			  const char* description , void* sdnamevec , const int nsubd, 
			  const int nchsd){

  try {
    LCWriter* lcWriter = reinterpret_cast<LCWriter*>( (writer) ) ;
    LCRunHeaderImpl* runHdr =  new LCRunHeaderImpl ;
    runHdr->setRunNumber( irun ) ;
    runHdr->setDetectorName( detname ) ;
    runHdr->setDescription( description ) ;

    int elemlen = nchsd + 1 ;
    PTRTYPE stringpos = reinterpret_cast<PTRTYPE>( sdnamevec ) ;
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

    PTRTYPE stringpos ;
    stringpos = reinterpret_cast<PTRTYPE>( detname ) ;
    char* detnam = reinterpret_cast<char*>( stringpos ) ;
    const char* detectorname = runHdr->getDetectorName().c_str() ;
    strcpy(detnam,detectorname) ;
    stringpos = reinterpret_cast<PTRTYPE>( description ) ;
    char* descr = reinterpret_cast<char*>( stringpos ) ;
    const char* rundescription = runHdr->getDescription().c_str() ;
    strcpy(descr,rundescription) ;

    const std::vector<std::string>* strVec = runHdr->getActiveSubdetectors() ;

    int elemlen = nchsubd + 1;
    *nsubd  = strVec->size() ;
    stringpos = reinterpret_cast<PTRTYPE>( sdnamevec ) ;
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
  PTRTYPE stringpos = reinterpret_cast<PTRTYPE>( detname ) ;
  char* detnam = reinterpret_cast<char*>( stringpos ) ;
  const char* detectorname = lcEvent->getDetectorName().c_str() ;
  strcpy(detnam,detectorname) ;
  return LCIO::SUCCESS ;
}
int lcdumprunheader( PTRTYPE runheader ){
  LCRunHeader* runhdr = reinterpret_cast<LCRunHeader*>( (runheader) ) ;
  LCTOOLS::dumpRunHeader( runhdr ) ;
  return LCIO::SUCCESS ;
}

int lcdumpevent( PTRTYPE event ){
  LCEventImpl* lcEvent = reinterpret_cast<LCEventImpl*>( (event) ) ;
  LCTOOLS::dumpEvent(  lcEvent ) ;
  return LCIO::SUCCESS ;
}

int lcdumpeventdetailed ( PTRTYPE event ){
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


  const double*  tmp  = lcMCParticle->getMomentum() ;

  for(int k=0;k<3;k++)  *momentum++ = (float) tmp[k] ;
  *mass              = lcMCParticle->getMass() ;
  *charge            = lcMCParticle->getCharge() ;
  *ndaughters        = lcMCParticle->getDaughters().size() ;
  return LCIO::SUCCESS ;
}


int lcaddsimtrackerhit( PTRTYPE collectionvec, int cellID0, double* pos, float dEdx, 
			    float time, PTRTYPE mcp ){

  LCCollectionVec* lcCollectionVec = reinterpret_cast<LCCollectionVec*>( (collectionvec) ) ;
  SimTrackerHitImpl* hit = new SimTrackerHitImpl ;
  MCParticle* mmcp = f2c_pointer<MCParticle,LCObject>( mcp ) ;

  hit->setCellID0( cellID0 ) ;
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
  const int Nelements = lcHit->getNMCContributions() ;
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


PTRTYPE lcobjectvectorcreate( PTRTYPE* objectv, const int nobjv ){
  LCObjectVec* objVec = new LCObjectVec ;
  for(int j=0;j<nobjv;j++) {
    LCObject* obj = f2c_pointer<LCObject,LCObject>( objectv[j] ) ;
    objVec->push_back( obj ) ;
  }
  return reinterpret_cast<PTRTYPE>( objVec ) ;
}

PTRTYPE lcintvectorcreate( int* intv, const int nintv ){
  LCIntVec* intVec = new LCIntVec ;
  for(int j=0;j<nintv;j++) intVec->push_back( intv[j] ) ;
  return reinterpret_cast<PTRTYPE>( intVec ) ;
}

PTRTYPE lcfloatvectorcreate( float* floatv, const int nfloatv ){
  LCFloatVec* floatVec = new LCFloatVec ;
  for(int j=0;j<nfloatv;j++) floatVec->push_back( floatv[j] ) ;
  return reinterpret_cast<PTRTYPE>( floatVec ) ;
}

PTRTYPE lcstringvectorcreate( void* stringv, const int nstringv, const int nchstringv){
  LCStrVec* stringVec = new LCStrVec ;
  int elemlen = nchstringv + 1;
  PTRTYPE stringpos = 0 ;
  stringpos = reinterpret_cast<PTRTYPE>( stringv ) ;
  for(int j=0;j<nstringv;j++){
    const std::string& mystring = reinterpret_cast<char*>( stringpos ) ;
    stringVec->push_back( mystring ) ;
    stringpos = stringpos + elemlen ;
  }
  return reinterpret_cast<PTRTYPE>( stringVec ) ;
}

int lcgetintvector( PTRTYPE vector, int* intv, int* nintv ){
  LCIntVec* intVec =  f2c_pointer<LCIntVec,LCObject>(vector) ;
  int intVecLength = 0;
  intVecLength = intVec->size() ;
  if (intVecLength > *nintv) {
    std::cerr << "Warning in lcgetintvector: vector size " << intVecLength
              << " larger then target array size " << *nintv << std::endl ;
    intVecLength = *nintv ;
  }
  else {
    *nintv = intVecLength ;
  }
  for (int j=0;j < intVecLength;j++)  *intv++ = (*intVec)[j] ;
  return LCIO::SUCCESS ;
}

int lcgetfloatvector( PTRTYPE vector, float* floatv, int* nfloatv ){
  LCFloatVec* floatVec =  f2c_pointer<LCFloatVec,LCObject>(vector) ;
  int floatVecLength = 0 ;
  floatVecLength = floatVec->size() ;
  if (floatVecLength > *nfloatv) {
    std::cerr << "Warning in lcgetfloatvector: vector size " << floatVecLength
              << " larger then target array size " << *nfloatv << std::endl ;
    floatVecLength = *nfloatv ;
  }
  else {
    *nfloatv = floatVecLength ;
  }
  for (int j=0;j < floatVecLength;j++)  *floatv++ = (*floatVec)[j] ;
  return LCIO::SUCCESS ;
}


int lcgetstringvector( PTRTYPE vector, void* stringv, int* nstringv, const int nchstringv){
  LCStrVec* stringVec = reinterpret_cast<LCStrVec*>(vector) ;
  int stringVecLength = 0 ;
  stringVecLength = stringVec->size() ;
  if (stringVecLength > *nstringv) {
    std::cerr << "Warning in lcgetstringvector: vector size " << stringVecLength
              << " larger then target array size " << *nstringv << std::endl ;
    stringVecLength =  *nstringv ;
  }
  else {
    *nstringv = stringVecLength ;
  }
  int elemlen = nchstringv + 1;
  PTRTYPE stringpos = 0 ;
  stringpos = reinterpret_cast<PTRTYPE>( stringv ) ;
  for (int j=0;j < stringVecLength;j++) {
    char* outstring = const_cast<char*>( (*stringVec)[j].c_str() );
    char* tmpstring = reinterpret_cast<char*>( stringpos ) ;
    strcpy(tmpstring,outstring) ;
    stringpos = stringpos + elemlen ;
  }
  return LCIO::SUCCESS ;
}


PTRTYPE intvectorcreate( int* intv, const int nintv ){
  IntVec* intVec = new IntVec ;
  for(int j=0;j<nintv;j++) intVec->push_back( intv[j] ) ;
  return reinterpret_cast<PTRTYPE>( intVec ) ;
}

int intvectordelete( PTRTYPE vector ){
  IntVec* intVec =  reinterpret_cast<IntVec*>(vector) ;
  delete intVec ;
  return LCIO::SUCCESS ;
}

PTRTYPE floatvectorcreate( float* floatv, const int nfloatv ){
  FloatVec* floatVec = new FloatVec ;
  for(int j=0;j<nfloatv;j++) floatVec->push_back( floatv[j] ) ;
  return reinterpret_cast<PTRTYPE>( floatVec ) ;
}

int floatvectordelete( PTRTYPE vector ){
  FloatVec* floatVec = reinterpret_cast<FloatVec*>(vector) ;
  delete floatVec ;
  return LCIO::SUCCESS ;
}

PTRTYPE stringvectorcreate( void* stringv, const int nstringv, const int nchstringv){
  StringVec* stringVec = new StringVec ;
  int elemlen = nchstringv + 1;
  PTRTYPE stringpos = 0 ;
  stringpos = reinterpret_cast<PTRTYPE>( stringv ) ;
  for(int j=0;j<nstringv;j++){
    const std::string& mystring = reinterpret_cast<char*>( stringpos ) ;
    stringVec->push_back( mystring ) ;
    stringpos = stringpos + elemlen ;
  }
  return reinterpret_cast<PTRTYPE>( stringVec ) ;
}

int stringvectordelete( PTRTYPE vector ){
  StringVec* stringVec = reinterpret_cast<StringVec*>(vector) ;
  delete stringVec ;
  return LCIO::SUCCESS ;
}

PTRTYPE pointervectorcreate( PTRTYPE* pointerv, const int npointerv ){
  PointerVec* pointerVec = new PointerVec ;
  for(int j=0;j<npointerv;j++) pointerVec->push_back( pointerv[j] ) ;
  return reinterpret_cast<PTRTYPE>( pointerVec ) ;
}

int pointervectordelete( PTRTYPE vector ){
  PointerVec* pointerVec = reinterpret_cast<PointerVec*>(vector) ;
  delete pointerVec ;
  return LCIO::SUCCESS ;
}


int getintvector( PTRTYPE vector, int* intv, int* nintv ){
  IntVec* intVec =  reinterpret_cast<IntVec*>(vector) ;
  int intVecLength = 0;
  intVecLength = intVec->size() ;
  if (intVecLength > *nintv) {
    std::cerr << "Warning in getintvector: vector size " << intVecLength
              << " larger then target array size " << *nintv << std::endl ;
    intVecLength = *nintv ;
  }
  else {
    *nintv = intVecLength ;
  }
  for (int j=0;j < intVecLength;j++)  *intv++ = (*intVec)[j] ;
  return LCIO::SUCCESS ;
}

int getfloatvector( PTRTYPE vector, float* floatv, int* nfloatv ){
  FloatVec* floatVec =  reinterpret_cast<FloatVec*>(vector) ;
  int floatVecLength = 0 ;
  floatVecLength = floatVec->size() ;
  if (floatVecLength > *nfloatv) {
    std::cerr << "Warning in getfloatvector: vector size " << floatVecLength
              << " larger then target array size " << *nfloatv << std::endl ;
    floatVecLength = *nfloatv ;
  }
  else {
    *nfloatv = floatVecLength ;
  }
  for (int j=0;j < floatVecLength;j++)  *floatv++ = (*floatVec)[j] ;
  return LCIO::SUCCESS ;
}


int getstringvector( PTRTYPE vector, void* stringv, int* nstringv, const int nchstringv){
  StringVec* stringVec = reinterpret_cast<StringVec*>(vector) ;
  int stringVecLength = 0 ;
  stringVecLength = stringVec->size() ;
  if (stringVecLength > *nstringv) {
    std::cerr << "Warning in getstringvector: vector size " << stringVecLength
              << " larger then target array size " << *nstringv << std::endl ;
    stringVecLength =  *nstringv ;
  }
  else {
    *nstringv = stringVecLength ;
  }
  int elemlen = nchstringv + 1;
  PTRTYPE stringpos = 0 ;
  stringpos = reinterpret_cast<PTRTYPE>( stringv ) ;
  for (int j=0;j < stringVecLength;j++) {
    char* outstring = const_cast<char*>( (*stringVec)[j].c_str() );
    char* tmpstring = reinterpret_cast<char*>( stringpos ) ;
    strcpy(tmpstring,outstring) ;
    stringpos = stringpos + elemlen ;
  }
  return LCIO::SUCCESS ;
}

int getpointervector( PTRTYPE vector, PTRTYPE* pointerv, int* npointerv ){
  PointerVec* pointerVec =  reinterpret_cast<PointerVec*>(vector) ;
  int pointerVecLength = 0;
  pointerVecLength = pointerVec->size() ;
  if (pointerVecLength > *npointerv) {
    std::cerr << "Warning in getpointervector: vector size " << pointerVecLength
              << " larger then target array size " << *npointerv << std::endl ;
    pointerVecLength = *npointerv ;
  }
  else {
    *npointerv = pointerVecLength ;
  }
  for (int j=0;j < pointerVecLength;j++)  *pointerv++ = (*pointerVec)[j] ;
  return LCIO::SUCCESS ;
}



int lcsetparameters (const char* class_name, PTRTYPE classp, const char* method, const char* key, PTRTYPE vecp){
  const std::string & classname = class_name ;
  if      (classname == LCIO::LCRUNHEADER) {
    LCRunHeaderImpl* rhd =  reinterpret_cast<LCRunHeaderImpl*>(classp) ;
    return do_set_method (rhd->parameters(), method, key, vecp) ;
  }
  else if (classname == LCIO::LCEVENT) {
    LCEventImpl* evt = reinterpret_cast<LCEventImpl*>(classp) ;
    return do_set_method (evt->parameters(), method, key, vecp) ;
  }
  else if (classname == LCIO::LCCOLLECTION) {
    LCCollectionVec* col = reinterpret_cast<LCCollectionVec*>(classp) ;
    return do_set_method (col->parameters(), method, key, vecp) ;
  }
  else {
    std::cerr << "Warning in lcsetparameters: unknown class name " << classname << std::endl ;
    return LCIO::ERROR ;
  }
}

int lcgetparameters (const char* class_name, PTRTYPE classp, const char* method, const char* key, PTRTYPE vecp){
  const std::string & classname = class_name ;
  if      (classname == LCIO::LCRUNHEADER) {
    LCRunHeaderImpl* rhd =  reinterpret_cast<LCRunHeaderImpl*>(classp) ;
    return do_get_method (rhd->getParameters(), method, key, vecp) ;
  }
  else if (classname == LCIO::LCEVENT) {
    LCEventImpl* evt = reinterpret_cast<LCEventImpl*>(classp) ;
    return do_get_method (evt->getParameters(), method, key, vecp) ;
  }
  else if (classname == LCIO::LCCOLLECTION) {
    LCCollectionVec* col = reinterpret_cast<LCCollectionVec*>(classp) ;
    return do_get_method (col->getParameters(), method, key, vecp) ;
  }
  else {
    std::cerr << "Warning in lcgetparameters: unknown class name " << classname << std::endl ;
    return LCIO::ERROR ;
  }

}

int do_set_method (LCParameters& params, const char* my_method, const char* key, PTRTYPE vector) {
  const std::string & method = my_method ;
  if      (method == string("setIntValues")) {
    IntVec* intVec =  reinterpret_cast<IntVec*>(vector) ;
    params.setValues(key, *intVec) ;
    return LCIO::SUCCESS ;
  }
  else if (method == string("setFloatValues")) {
    FloatVec* floatVec =  reinterpret_cast<FloatVec*>(vector) ;
    params.setValues(key, *floatVec) ;
    return LCIO::SUCCESS ;
  }
  else if (method == string("setStringValues")) {
    StringVec* stringVec = reinterpret_cast<StringVec*>(vector) ;
    params.setValues(key, *stringVec) ;
    return LCIO::SUCCESS ;
  }
  else {
    std::cerr << "Warning in lcsetparameters: unknown method name " << method << std::endl ;
    return LCIO::ERROR ;
  }
}

int do_get_method (const LCParameters& params, const char* my_method, const char* key, PTRTYPE vector) {
  const std::string & method = my_method ;
  if      (method == string("getIntValues")) {
    IntVec* intVec =  f2c_pointer<IntVec,LCObject>(vector) ;
    params.getIntVals(key, *intVec) ;
    return LCIO::SUCCESS ;
  }
  else if (method == string("getFloatValues")) {
    FloatVec* floatVec =  f2c_pointer<FloatVec,LCObject>(vector) ;
    params.getFloatVals(key, *floatVec) ;
    return LCIO::SUCCESS ;
  }
  else if (method == string("getStringValues")) {
    StringVec* stringVec = f2c_pointer<StringVec,LCObject>(vector) ;
    params.getStringVals(key, *stringVec) ;
    return LCIO::SUCCESS ;
  }
  else if (method == string("getIntKeys")) {
    StringVec* stringVec = f2c_pointer<StringVec,LCObject>(vector) ;
    params.getIntKeys(*stringVec) ;
    return LCIO::SUCCESS ;
  }
  else if (method == string("getFloatKeys")) {
    StringVec* stringVec = f2c_pointer<StringVec,LCObject>(vector) ;
    params.getFloatKeys(*stringVec) ;
    return LCIO::SUCCESS ;
  }
  else if (method == string("getStringtKeys")) {
    StringVec* stringVec = f2c_pointer<StringVec,LCObject>(vector) ;
    params.getStringKeys(*stringVec) ;
    return LCIO::SUCCESS ;
  }
  else {
    std::cerr << "Warning in lcgetparameters: unknown method name " << method << std::endl ;
    return LCIO::ERROR ;
  }
}


