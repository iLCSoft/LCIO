////////////////////////////////////////
//  test lcio::TrackerHit
////////////////////////////////////////

#include "tutil.h"
#include "lcio.h"

#include "EVENT/LCIO.h"
#include "IO/LCReader.h"
#include "IO/LCWriter.h"
#include "IMPL/LCEventImpl.h" 
#include "IMPL/LCCollectionVec.h"
#include "IMPL/TrackerHitImpl.h"
#include "IMPL/LCFlagImpl.h"

//#include <iostream>

using namespace std ;
using namespace lcio ;

//static const int NRUN = 10 ;
static const int NEVENT = 10 ; // events
static const int NHITS = 1000 ;  // tracker hits per event

static string FILEN = "trackerhits.slcio" ;

// replace mytest with the name of your test
const static string testname="test_trackerhit";

//=============================================================================

int main(int argc, char** argv ){

    // this should be the first line in your test
    TEST MYTEST=TEST( testname, std::cout );

    try{


        MYTEST.LOG( " writing TrackerHits " );

        // create sio writer
        LCWriter* lcWrt = LCFactory::getInstance()->createLCWriter()  ;

        lcWrt->open( FILEN , LCIO::WRITE_NEW ) ;

        // EventLoop - create some events and write them to the file
        for(int i=0;i<NEVENT;i++){

            // we need to use the implementation classes here 
            LCEventImpl*  evt = new LCEventImpl() ;


            evt->setRunNumber( 4711  ) ;
            evt->setEventNumber( i ) ;

            LCCollectionVec* trkHits = new LCCollectionVec( LCIO::TRACKERHIT )  ;

            for(int j=0;j<NHITS;j++){
                TrackerHitImpl* trkHit = new TrackerHitImpl ;
                trkHit->setCharge( i*j*117. ) ;
                trkHit->setChargeError( (i+j)*.3 ) ;
                double pos[3] = { i, j, i*j } ;
                trkHit->setPosition( pos ) ;
                trkHits->addElement( trkHit ) ;
            }
            evt->addCollection( trkHits , "TrackerHits") ;

            lcWrt->writeEvent(evt) ;

            delete evt ;
        }


        lcWrt->close() ;

        MYTEST.LOG(" reading back TrackerHits from file " ) ;

        // create sio reader
        LCReader* lcRdr = LCFactory::getInstance()->createLCReader()  ;

        lcRdr->open( FILEN ) ;

        for(int i=0;i<NEVENT;i++){

            //std::cout << " testing event " << i << std::endl ;

            LCEvent*  evt =  lcRdr->readNextEvent() ;

            MYTEST( evt->getRunNumber() ,  4711 , " run number " )  ;

            MYTEST(  evt->getEventNumber() ,  i , " event number "  ) ;

            LCCollection* trkHits = evt->getCollection( "TrackerHits") ;

            for(int j=0;j<NHITS;j++) {

                //std::cout << " testing hit " << j << std::endl ;

                TrackerHit* trkHit = dynamic_cast<TrackerHit*>(trkHits->getElementAt(j)) ;

                MYTEST( trkHit->getCharge() ,  i*j*117. , "charge" ) ;
                // remove float converstion and check what happens ;)
                MYTEST( trkHit->getChargeError() ,  float((i+j)*.3) , "charge error" ) ;
                //MYTEST( trkHit->getChargeError() ,  (i+j)*.3 , "charge error" ) ;

                const double* pos = trkHit->getPosition() ;

                MYTEST( pos[0] , i , " pos[0] " ) ;
                MYTEST( pos[1] , j , " pos[1] " ) ;
                MYTEST( pos[2] , i*j , " pos[2] " ) ;

            }
        }
        lcRdr->close() ;


    } catch( Exception &e ){
        MYTEST.FAILED( e.what() );
    }

    return 0;
}

//=============================================================================

