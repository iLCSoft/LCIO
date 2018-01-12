////////////////////////////////////////
// example for LCIO tests
////////////////////////////////////////

#include "tutil.h"
#include "lcio.h"
#include <cstdlib>

// #include "EVENT/LCIO.h"
// #include "IO/LCWriter.h"
// #include "IMPL/LCEventImpl.h" 


#include "IMPL/LCRunHeaderImpl.h"

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

int main(int /*argc*/, char** /*argv*/ ){
    
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


    MYTEST.LOG( "  -------------------------------------    test random access in file c_sim.slcio - file must exist in : "  ) ;
    if( !std::system("pwd") )
	std::cout << "?" << std::endl ;

    // simjob.slcio has written 100 events in 10 runs, closing and re-opening the file after every run 
    //  this tests writing the random access records in append mode ...
    //  here we make just one simple test of reading a given event

    LCReader* lcReader = LCFactory::getInstance()->createLCReader( IO::LCReader::directAccess ) ;

    // LCReader* lcReader = LCFactory::getInstance()->createLCReader( ) ;

    try{
      
      lcReader->open( "c_sim.slcio" ) ;
      
      MYTEST( lcReader->getNumberOfRuns() , 10 , " LCReader::getNumberOfRuns() - number of run headers is not 10" );

      MYTEST( lcReader->getNumberOfEvents() , 100 , " LCReader::getNumberOfEvents() - number of events is not 100" );


      IntVec runs ;
      IntVec events ;

      lcReader->getRuns( runs ) ;
      lcReader->getEvents( events ) ;

      MYTEST( runs.size() , 10u , " LCReader::getRuns( runs )  - size 'runs' is not 10" );

      MYTEST( events.size()  , 200u , " LCReader::getEvents( events ) - size of 'events' is not 200" );

      // for(int i=0,nElements= events.size() /2 ; i < nElements ; ++i){
      // 	std::cout << " test_random_access - i*2 =  " << i*2 << " runnum = " 
      // 		  <<   events[ 2*i     ] << " 2*i + 1  " <<  2*i + 1 <<  " evtnum = " << events[ 2*i + 1 ]  << std::endl ;
      // }

      MYTEST( events[ 42 * 2 ], 4 , " LCReader::getEvents( events ) - events[ 42 * 2 ] is not run 4 " );

      MYTEST( events[ 42 * 2 + 1 ], 2 , " LCReader::getEvents( events ) - events[ 42 * 2  +1 ] is not event 2 " );


     // test that we can still use read next for runheaders .....
      LCRunHeader* rHdr = lcReader->readNextRunHeader() ;
      MYTEST( rHdr->getRunNumber() , 0 , " LCReader::readNextRunHeader() - run number is not 0" );
      
      LCEvent* evt = lcReader->readNextEvent() ;
      MYTEST( evt->getEventNumber() , 0 , " LCReader::readNextEvent() - event number is not 0" );
      MYTEST( evt->getRunNumber() , 0 , " LCReader::readNextEvent() - run number is not 0" );

      
      evt = lcReader->readEvent( 3 , 4 ) ;

      MYTEST( evt !=0  , true  , " LCReader::readEvent( 3 , 4  ) - evt is NULL" );
      MYTEST( evt->getRunNumber() , 3 , " LCReader::readEvent( 3, 4  ) - run number is not 3" );
      MYTEST( evt->getEventNumber() , 4 , " LCReader::readEvent( 3, 4  ) - event number is not 4" );



      // test direct access of run  headers
      rHdr = lcReader->readRunHeader( 7 ) ;
      MYTEST( rHdr->getRunNumber() , 7 , " LCReader::readRunHeader() - run number is not 7" );
      rHdr = lcReader->readRunHeader( 3 ) ;
      MYTEST( rHdr->getRunNumber() , 3 , " LCReader::readRunHeader() - run number is not 3" );

      rHdr = lcReader->readRunHeader( 0 , EVENT::LCIO::UPDATE ) ;
      //rHdr = lcReader->readRunHeader( 0 ) ;
      IMPL::LCRunHeaderImpl* rhImpl = dynamic_cast< IMPL::LCRunHeaderImpl*> ( rHdr ) ;
      bool notNull = rhImpl != 0 ;
      MYTEST( notNull , true , " readRunHeader not of type IMPL::LCRunHeaderImpl " );

      rhImpl->setRunNumber( 42 ) ;
      MYTEST( rHdr->getRunNumber() , 42 , " LCReader::readRunHeader() - run number to changed to 42 in update mode " );
 

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

