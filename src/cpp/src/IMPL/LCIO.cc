#include "EVENT/LCIO.h"

namespace EVENT{
// ANSI-C++ requires initialization outside the class definition
// this has to be solved differently....
//
const char* LCIO::LCEVENT = "LCEvent" ;
const char* LCIO::LCCOLLECTION = "LCCollection" ;
const char* LCIO::LCRUNHEADER = "LCRunHeader" ;
const char* LCIO::MCPARTICLE = "MCParticle" ;
const char* LCIO::SIMCALORIMETERHIT = "SimCalorimeterHit" ;
const char* LCIO::RAWCALORIMETERHIT = "RawCalorimeterHit" ;
const char* LCIO::CALORIMETERHIT = "CalorimeterHit" ;
const char* LCIO::SIMTRACKERHIT = "SimTrackerHit" ;
const char* LCIO::TPCHIT = "TPCHit" ;
const char* LCIO::TRACKERRAWDATA = "TrackerRawData" ;
const char* LCIO::TRACKERDATA = "TrackerData" ;
const char* LCIO::TRACKERPULSE = "TrackerPulse" ;
const char* LCIO::TRACKERHIT = "TrackerHit" ;
const char* LCIO::LCSTRVEC = "LCStrVec" ;
const char* LCIO::LCFLOATVEC = "LCFloatVec" ;
const char* LCIO::LCINTVEC = "LCIntVec" ;
const char* LCIO::TRACK= "Track" ; 
const char* LCIO::CLUSTER = "Cluster" ; 
const char* LCIO::RECONSTRUCTEDPARTICLE = "ReconstructedParticle" ; 
const char* LCIO::LCRELATION = "LCRelation" ; 
const char* LCIO::LCGENERICOBJECT = "LCGenericObject" ; 
const char* LCIO::PARTICLEID = "ParticleID" ; 
const char* LCIO::VERTEX = "Vertex" ; 

const char* LCIO::CellIDEncoding = "CellIDEncoding" ; 

  // Hash function for faster lookup in string maps.
  //  Code copied from ROOT TMath::Hash (http://root.cern.ch).
  unsigned Hash(const void *txt, int ntxt) {
    // Calculates hash index from any char string.
    // Based on precalculated table of 256 specially selected random numbers.
    //
    //   For string:  i = TMath::Hash(string,nstring);
    //   For int:     i = TMath::Hash(&intword,sizeof(int));
    //   For pointer: i = TMath::Hash(&pointer,sizeof(void*));
    //
    //   Limitation: for ntxt>256 calculates hash only from first 256 bytes
    //
    //              V.Perev
    
    
    const char *uc = (const char*) txt;
    unsigned  u = 0, uu = 0;
    
    static unsigned utab[256] =
      {0xb93f6fc0,0x553dfc51,0xb22c1e8c,0x638462c0,0x13e81418,0x2836e171,0x7c4abb90,0xda1a4f39
       ,0x38f211d1,0x8c804829,0x95a6602d,0x4c590993,0x1810580a,0x721057d4,0x0f587215,0x9f49ce2a
       ,0xcd5ab255,0xab923a99,0x80890f39,0xbcfa2290,0x16587b52,0x6b6d3f0d,0xea8ff307,0x51542d5c
       ,0x189bf223,0x39643037,0x0e4a326a,0x214eca01,0x47645a9b,0x0f364260,0x8e9b2da4,0x5563ebd9
       ,0x57a31c1c,0xab365854,0xdd63ab1f,0x0b89acbd,0x23d57d33,0x1800a0fd,0x225ac60a,0xd0e51943
       ,0x6c65f669,0xcb966ea0,0xcbafda95,0x2e5c0c5f,0x2988e87e,0xc781cbab,0x3add3dc7,0x693a2c30
       ,0x42d6c23c,0xebf85f26,0x2544987e,0x2e315e3f,0xac88b5b5,0x7ebd2bbb,0xda07c87b,0x20d460f1
       ,0xc61c3f40,0x182046e7,0x3b6c3b66,0x2fc10d4a,0x0780dfbb,0xc437280c,0x0988dd07,0xe1498606
       ,0x8e61d728,0x4f1f3909,0x040a9682,0x49411b29,0x391b0e1c,0xd7905241,0xdd77d95b,0x88426c13
       ,0x33033e58,0xe158e30e,0x7e342647,0x1e09544b,0x4637353d,0x18ea0924,0x39212b08,0x12580ae8
       ,0x269a6f06,0x3e10b73b,0x123db33b,0x085412da,0x3bb5f464,0xd9b2d442,0x103d26bb,0xd0038bab
       ,0x45b6177f,0xfb48f1fe,0x99074c55,0xb545e82e,0x5f79fd0d,0x570f3ae4,0x57e43255,0x037a12ae
       ,0x4357bdb2,0x337c2c4d,0x2982499d,0x2ab72793,0x3900e7d1,0x57a6bb81,0x7503609b,0x3f39c0d0
       ,0x717b389d,0x5748034f,0x4698162b,0x5801b97c,0x1dfd5d7e,0xc1386d1c,0xa387a72a,0x084547e4
       ,0x2e54d8e9,0x2e2f384c,0xe09ccc20,0x8904b71e,0x3e24edc5,0x06a22e16,0x8a2be1df,0x9e5058b2
       ,0xe01a2f16,0x03325eed,0x587ecfe6,0x584d9cd3,0x32926930,0xe943d68c,0xa9442da8,0xf9650560
       ,0xf003871e,0x1109c663,0x7a2f2f89,0x1c2210bb,0x37335787,0xb92b382f,0xea605cb5,0x336bbe38
       ,0x08126bd3,0x1f8c2bd6,0xba6c46f2,0x1a4d1b83,0xc988180d,0xe2582505,0xa8a1b375,0x59a08c49
       ,0x3db54b48,0x44400f35,0x272d4e7f,0x5579f733,0x98eb590e,0x8ee09813,0x12cc9301,0xc85c402d
       ,0x135c1039,0x22318128,0x4063c705,0x87a8a3fa,0xfc14431f,0x6e27bf47,0x2d080a19,0x01dba174
       ,0xe343530b,0xaa1bfced,0x283bb2c8,0x5df250c8,0x4ff9140b,0x045039c1,0xa377780d,0x750f2661
       ,0x2b108918,0x0b152120,0x3cbc251f,0x5e87b350,0x060625bb,0xe068ba3b,0xdb73ebd7,0x66014ff3
       ,0xdb003000,0x161a3a0b,0xdc24e142,0x97ea5575,0x635a3cab,0xa719100a,0x256084db,0xc1f4a1e7
       ,0xe13388f2,0xb8199fc9,0x50c70dc9,0x08154211,0xd60e5220,0xe52c6592,0x584c5fe1,0xfe5e0875
       ,0x21072b30,0x3370d773,0x92608fe2,0x2d013d93,0x53414b3c,0x2c066142,0x64676644,0x0420887c
       ,0x35c01187,0x6822119b,0xf9bfe6df,0x273f4ee4,0x87973149,0x7b41282d,0x635d0d1f,0x5f7ecc1e
       ,0x14c3608a,0x462dfdab,0xc33d8808,0x1dcd995e,0x0fcb11ba,0x11755914,0x5a62044b,0x37f76755
       ,0x345bd058,0x8831c2b5,0x204a8468,0x3b0b1cd2,0x444e56f4,0x97a93e2c,0xd5f15067,0x266a95fa
       ,0xff4f8036,0x6160060d,0x930c472f,0xed922184,0x37120251,0xc0add74f,0x1c0bc89d,0x018d47f2
       ,0xff59ef66,0xd1901a17,0x91f6701b,0x0960082f,0x86f6a8f3,0x1154fecd,0x9867d1de,0x0945482f
       ,0x790ffcac,0xe5610011,0x4765637e,0xa745dbff,0x841fdcb3,0x4f7372a0,0x3c05013d,0xf1ac4ab7
       ,0x3bc5b5cc,0x49a73349,0x356a7f67,0x1174f031,0x11d32634,0x4413d301,0x1dd285c4,0x3fae4800
      };
    
    if (ntxt > 255) ntxt = 255;
    
    for ( ; ntxt--; uc++) {
      uu = uu<<1 ^ utab[(*uc) ^ ntxt];
      u ^= uu;
    }
    return u;
  }
  
  
}
