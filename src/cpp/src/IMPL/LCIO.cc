#include "EVENT/LCIO.h"

namespace EVENT{
// ANSI-C++ requires initialization outside the class definition
// this has to be solved differently....
//
const char* LCIO::LCEVENT = "LCEvent" ;
const char* LCIO::LCRUNHEADER = "LCRunHeader" ;
const char* LCIO::MCPARTICLE = "MCParticle" ;
const char* LCIO::SIMCALORIMETERHIT = "SimCalorimeterHit" ;
const char* LCIO::CALORIMETERHIT = "CalorimeterHit" ;
const char* LCIO::SIMTRACKERHIT = "SimTrackerHit" ;
const char* LCIO::LCFLOATVEC = "LCFloatVec" ;
const char* LCIO::LCINTVEC = "LCIntVec" ;
};
