#ifndef CalorimeterHitHistograms_h
#define CalorimeterHitHistograms_h

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TDirectory.h"

#include "LCIOModule.h"

#include <cstdlib>
#include <iostream>

using namespace std ;
using namespace lcio ;


/** Minimodule that creates some root histograms from all SimCalorimeterHit and
 * CalorimeterHit (real data) collections in an LCIO file.
 */
class CalorimeterHitHistograms : public LCIOModule {
  
public:
  
  CalorimeterHitHistograms(const char* moduleName) ; 
  ~CalorimeterHitHistograms() ; 
  
  void processEvent(  LCEvent * evt )  ;
  void processRunHeader( LCRunHeader* run) ;
  void initModule() ;
  
private:
  TFile* hfile ; // the root file

  // create a directory for every run
  TDirectory * runDir ;

  // some histograms
  TH1F* hEhit ;    // hit energy
  TH1F* hElong ;   // longitudenal energy distribution
  TH1F* hEradial ; // radial energy distribution
  TH2F* hhitmap ;  // energy distribution in x,y

  bool firstEventInRun ;
};


#endif
