#ifndef MCParticleHistograms_h
#define MCParticleHistograms_h

//#include "lcio.h"


#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TDirectory.h"

#include "LCIOModule.h"

#include <cstdlib>
#include <iostream>

using namespace std ;
using namespace lcio ;

/** Minimodule that creates some root histograms from the MCParticle collection 
 * in an LCIO file.
 */

class MCParticleHistograms : public LCIOModule {
  
public:
  
  MCParticleHistograms(const char* moduleName) ; 
  ~MCParticleHistograms() ; 
  
  void processEvent(  LCEvent * evt )  ;
  void processRunHeader( LCRunHeader* run) ;
  void initModule() ;
  
private:
  TFile* hfile ; // the root file

  // create a directory for every run
  TDirectory * runDir ;

  // some histograms
  TH1F* hEMother ;    // energy of mother particles
  TH1F* hTypeMother ; // type of mother particles 
  TH1F* hEstable ;    // energy of stable particles
  TH1F* hTypestable ; // type of stable particles

  bool firstEventInRun ;
};


#endif
