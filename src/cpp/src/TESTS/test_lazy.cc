////////////////////////////////////////
// example for LCIO tests
////////////////////////////////////////

#include "tutil.h"

#include "IO/LCWriter.h"
#include "MT/LCReader.h"
#include "EVENT/LCCollection.h" 

#include <iostream>


// replace mytest with the name of your test
const static std::string testname="lazy";

//=============================================================================

int main(int /*argc*/, char** /*argv*/ ){
    
    // this should be the first line in your test
    TEST MYTEST=TEST( testname, std::cout );

    // example with a try&catch fail condition:
    //
    ///////////////////////////////////////////////////////////////////////
    
    MT::LCReader reader( MT::LCReader::directAccess | MT::LCReader::lazyUnpack ) ;
    
    try {   
       reader.open( "c_sim.slcio" ) ;
       std::unique_ptr<EVENT::LCEvent> event = nullptr ;
       while( 1 ) {
         event = reader.readNextEvent() ;
         if( nullptr == event ) {
           break ;
         }
         auto colnames = event->getCollectionNames() ;         
         for( auto colname : *colnames ) {
           auto collection = event->getCollection( colname ) ;
           std::cout << "Event no " << event->getEventNumber() << ", collection: type '" << collection->getTypeName() << "', name '" << colname << "'" << std::endl ;
         }
         std::cout << std::endl ;
       }
    }
    catch( EVENT::Exception &e ){
       MYTEST.FAILED( e.what() );
    }

    reader.close();

    ///////////////////////////////////////////////////////////////////////

    return 0;
}

//=============================================================================

