////////////////////////////////////////
// example for LCIO tests
////////////////////////////////////////

#include "tutil.h"
#include "lcio.h"

#include "EVENT/LCIO.h"
#include "IO/LCWriter.h"
#include "IMPL/LCEventImpl.h" 
#include "IMPL/LCCollectionVec.h"

#include <iostream>

using namespace std ;
using namespace lcio ;

// replace mytest with the name of your test
const static string testname="mytest";

//=============================================================================

int main(int argc, char** argv ){
    
    // this should be the first line in your test
    TEST MYTEST=TEST( testname, std::cout );
    
    // write your test here
    for( int i=0; i<3; i++){
        MYTEST.LOG( "this is just a test..." );
    }

    //if( true ){
    if( false ){
        // force test program to fail in this way:
        MYTEST.FAILED( "oops, something went wrong..." );
    }

    // example with a try&catch fail condition:
    //
    ///////////////////////////////////////////////////////////////////////
    //LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
    //try{
    //    lcReader->open( "blub.slcio" ) ;
    //}
    //catch( Exception &e ){
    //    MYTEST.FAILED( e.what() );
    //}
    //lcReader->close();
    //delete lcReader;
    ///////////////////////////////////////////////////////////////////////

    return 0;
}

//=============================================================================

