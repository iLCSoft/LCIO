////////////////////////////////////////
// example for LCIO tests
////////////////////////////////////////

#include "tutil.h"
#include "lcio.h"
#include <cstdlib>

// #include "EVENT/LCIO.h"
// #include "IO/LCWriter.h"
// #include "IMPL/LCEventImpl.h" 
// #include "IMPL/LCCollectionVec.h"

#include "SIO/LCIORandomAccess.h"
#include "SIO/LCIORandomAccessMgr.h"
#include "SIO/RunEventMap.h"

#include "UTIL/LCTOOLS.h"

#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

using namespace std ;
using namespace lcio ;

using namespace SIO ;

// replace mytest with the name of your test
const static string testname="random_access";

//=============================================================================

int main(int argc, char** argv ){
    
    // this should be the first line in your test
    TEST MYTEST=TEST( testname, std::cout );
    
    
    MYTEST.LOG( "  ----------------------------   testing class RunEvent"  ) ;

    // fill a few RunEvents into a set - test ordering with operator<()

    set< RunEvent > reSet ;
    reSet.insert( RunEvent(  123456, -1 ) )  ;
    reSet.insert( RunEvent(  123456,  1 ) )  ;
    reSet.insert( RunEvent(  123456,  12 ) )  ;
    reSet.insert( RunEvent(  123450, -1 ) )  ;
    reSet.insert( RunEvent(  123450, 112345 ) )  ;

    reSet.insert(  EVENT::long64(123400) << 32   | (12345678LL & 0xffffffff)  ) ;

    MYTEST( reSet.size() , unsigned(6)  ,  " set< RunEvent>.size() != 6 " )  ;

    std::vector< RunEvent > v ; // copy to vector for easier comparison
    std::copy( reSet.begin() , reSet.end() , std::back_inserter( v ) ) ;

//     int d=-1 ;
//     for( std::set< RunEvent >::iterator it = reSet.begin() ; it != reSet.end() ; ++it ){
//       std::cout << " set[" << ++d <<"] " << *it  << std::endl ;
//     }

    std::stringstream err ;
    err << " v[0] : " << v[0]  << std::endl 
	<< " v[1] : " << v[1]  << std::endl 
	<< " v[2] : " << v[2]  << std::endl 
	<< " v[3] : " << v[3]  << std::endl 
	<< " v[4] : " << v[4]  << std::endl 
	<< " v[5] : " << v[5]  << std::endl  ;

    MYTEST( v[0] < v[1] , true ,  err.str() )  ;
    MYTEST( v[1] < v[2] , true ,  err.str() )  ;
    MYTEST( v[2] < v[3] , true ,  err.str() )  ;
    MYTEST( v[3] < v[4] , true ,  err.str() )  ;
    MYTEST( v[4] < v[5] , true ,  err.str() )  ;


    MYTEST.LOG( "  -------------------------------------   testing class RunEventMap"  ) ;

    RunEventMap map ;

    int count = 0  ;
    for( std::set< RunEvent >::iterator it = reSet.begin() ; it != reSet.end() ; ++it , ++count ){
      
      //  std::cout << " set[" << count <<"] " << *it  << std::endl ;
      map.add( *it , 1024 * count ) ;
    }
    MYTEST( map.getPosition( v[2] ), 2048 ,  "  EventMap::getPosition() return for exisiting event "  )  ;

    MYTEST( unsigned (map.getNumberOfRunRecords() + map.getNumberOfEventRecords() ),
	    map.size(), "RunEventMap invariant: nRun+nEvt = map.size " );

    MYTEST( map.getNumberOfEventRecords(), 4 , "RunEventMap::getNumberOfEventRecords()  " );
    MYTEST( map.getNumberOfRunRecords(), 2 , "RunEventMap::getNumberOfRunRecords()  " );

    map.add(  v[2] , 2048000 ) ; // overwrite existing entry

    MYTEST( map.getPosition( v[2] ), 2048000 ,  " test if EventMap::add()  overwrites existing entry "  )  ;

    MYTEST( map.getNumberOfEventRecords(), 4 , "RunEventMap::getNumberOfEventRecords() after addition of duplicate (overwrite) " );



    //if( true ){
    if( false ){
        // force test program to fail in this way:
        MYTEST.FAILED( "oops, something went wrong..." );
    }


    MYTEST.LOG( "  -------------------------------------    test random access in file simjob.slcio - file must exist in : "  ) ;
    std::system("pwd") ;

    // simjob.slcio has written 100 events in 10 runs, closing and re-opening the file after every run 
    //  this tests writing the random access records in append mode ...
    //  here we make just one simple test of reading a given event

    LCReader* lcReader = LCFactory::getInstance()->createLCReader( IO::LCReader::directAccess ) ;

    //    LCReader* lcReader = LCFactory::getInstance()->createLCReader( ) ;

    try{

       lcReader->open( "c_sim.slcio" ) ;
      
      LCEvent* evt = lcReader->readEvent( 3 , 4 ) ;

      MYTEST( evt !=0  , true  , " LCReader::readEvent( 3 , 4  ) - evt is NULL" );
      MYTEST( evt->getRunNumber() , 3 , " LCReader::readEvent( 3, 4  ) - run number is not 3" );
      MYTEST( evt->getEventNumber() , 4 , " LCReader::readEvent( 3, 4  ) - event number is not 4" );

    }
    catch( Exception &e ){
      
      MYTEST.FAILED( e.what() );
    }

    lcReader->close();
    
    delete lcReader;
    ///////////////////////////////////////////////////////////////////////

    return 0;
}

//=============================================================================

