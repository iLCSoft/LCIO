////////////////////////////////////////
// example for LCIO tests
////////////////////////////////////////

#include "tutil.h"
#include "lcio.h"

#include "EVENT/LCIO.h"
//#include "IO/LCWriter.h"
//#include "IMPL/LCEventImpl.h" 
//#include "IMPL/LCCollectionVec.h"
#include "IMPL/MCParticleImpl.h"
#include "IMPL/ReconstructedParticleImpl.h"
#include "UTIL/LCFourVector.h"


#include <iostream>

using namespace std ;
using namespace lcio ;

// replace mytest with the name of your test
const static string testname="test_fourvector";

//=============================================================================

int main(int /*argc*/, char** /*argv*/ ){
    
    // this should be the first line in your test
    TEST MYTEST=TEST( testname, std::cout );
    
    MCParticleImpl* mcp = new MCParticleImpl ;
    
    float p[3] ;
    p[0] = 1.0 ;
    p[1] = 2.0 ;
    p[2] = 3.0 ;

    mcp->setMomentum( p ) ;
    mcp->setMass( 42. ) ;

    LCFourVector<MCParticle> mcp4v( mcp ) ;
    
    MYTEST( mcp4v.px()  , 1.0 ,  "px != 1.0 "   ) ;
    MYTEST( mcp4v.py()  , 2.0 ,  "py != 2.0 "   ) ;
    MYTEST( mcp4v.pz()  , 3.0 ,  "pz != 3.0 "   ) ;
   
    MYTEST( mcp4v.m()  , 42.0 ,  " e != 42.0 "   ) ;
    
    return 0;
}

//=============================================================================

