////////////////////////////////////////
// example for LCIO tests
////////////////////////////////////////

#include "tutil.h"
#include "lcio.h"


#include "UTIL/LCTrackerConf.h"

#include <iostream>

using namespace std ;

// replace mytest with the name of your test
const static string testname="test_lctrackercellid";

//=============================================================================

int main(int /*argc*/, char** /*argv*/ ){
    
    // this should be the first line in your test
    TEST MYTEST=TEST( testname, std::cout );
    
    
    MYTEST.LOG( "testing the LCTrackerCellID ... " );
    
    // set a different encoding string
    lcio::LCTrackerCellID::instance().set_encoding_string("subdet:4,side:-2,layer:10,module:6,sensor:10" ) ;

    
    MYTEST( lcio::LCTrackerCellID::subdet() , 0u , " subdet index != 0 " ) ;
    MYTEST( lcio::LCTrackerCellID::side()   , 1u , " side   index != 1 " ) ;
    MYTEST( lcio::LCTrackerCellID::layer()  , 2u , " layer  index != 2 " ) ;
    MYTEST( lcio::LCTrackerCellID::module() , 3u , " module index != 3 " ) ;
    MYTEST( lcio::LCTrackerCellID::sensor() , 4u , " sensor index != 4 " ) ;


    MYTEST( lcio::LCTrackerCellID::encoding_string() , std::string("subdet:4,side:-2,layer:10,module:6,sensor:10") , "wrong encoding string ! " ) ;


    bool exception = false ;
    
    try {
      lcio::LCTrackerCellID::instance().set_encoding_string("hello" ) ;
    } catch( std::logic_error& ){
      exception = true ;
    }
      
    if( ! exception ){

      MYTEST.FAILED( "oops, should not be possible to change the encoding string after it has been accessed " );
    }


    return 0;
}

//=============================================================================

