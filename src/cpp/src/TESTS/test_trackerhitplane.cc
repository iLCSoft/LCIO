////////////////////////////////////////
//  test lcio::TrackerHitPlane
////////////////////////////////////////

#include "tutil.h"
#include "lcio.h"

#include "EVENT/LCIO.h"
#include "IO/LCReader.h"
#include "IO/LCWriter.h"
#include "IMPL/LCEventImpl.h" 
#include "IMPL/LCCollectionVec.h"
#include "IMPL/TrackerHitPlaneImpl.h"
//#include "UTIL/Operators.h"

#include "UTIL/ILDConf.h"

//#include <iostream>

using namespace std ;
using namespace lcio ;

//static const int NRUN = 10 ;
static const int NEVENT = 3 ; // events
static const int NHITS = 10 ;  // tracker hits per event

static string FILEN = "trackerhitplane.slcio" ;

// replace mytest with the name of your test
const static string testname="test_trackerhitplane";

//=============================================================================

int main(int /*argc*/, char** /*argv*/ ){

    // this should be the first line in your test
    TEST MYTEST=TEST( testname, std::cout );

    try{


        MYTEST.LOG( " writing TrackerPlaneHits " );

        // create sio writer
        LCWriter* lcWrt = LCFactory::getInstance()->createLCWriter()  ;

        lcWrt->open( FILEN , LCIO::WRITE_NEW ) ;

        // EventLoop - create some events and write them to the file
        for(int i=0;i<NEVENT;i++){

            // we need to use the implementation classes here 
            LCEventImpl*  evt = new LCEventImpl() ;


            evt->setRunNumber( 4711  ) ;
            evt->setEventNumber( i ) ;

	    //            LCCollectionVec* trkHits = new LCCollectionVec(  LCIO::TRACKERHIT )  ;
            LCCollectionVec* trkHits = new LCCollectionVec(  LCIO::TRACKERHITPLANE )  ;

            for(int j=0;j<NHITS;j++){

                TrackerHitPlaneImpl* trkHit = new TrackerHitPlaneImpl ;

                trkHit->setEDep( i*j*117. ) ;
                // trkHit->setdEdx( i*j*117. ) ;
                trkHit->setEDepError( (i+j)*.3 ) ;

                double pos[3] = { 1.*i, 1.*j, 1.*i*j } ;
                trkHit->setPosition( pos ) ;

		// float cov[6] = { i, j, i+j ,  2*i, 2*j, 2*(i+j) , } ;
		// trkHit->setCovMatrix( cov );

                trkHits->addElement( trkHit ) ;
            }
            evt->addCollection( trkHits , "TrackerPlaneHits") ;

            lcWrt->writeEvent(evt) ;

            delete evt ;
        }


        lcWrt->close() ;

        MYTEST.LOG(" reading back TrackerPlaneHits from file " ) ;

        // create sio reader
        LCReader* lcRdr = LCFactory::getInstance()->createLCReader()  ;

        lcRdr->open( FILEN ) ;

        for(int i=0;i<NEVENT;i++){

            //std::cout << " testing event " << i << std::endl ;

            LCEvent*  evt =  lcRdr->readNextEvent() ;

            MYTEST( evt->getRunNumber() ,  4711 , " run number " )  ;

            MYTEST(  evt->getEventNumber() ,  i , " event number "  ) ;

            LCCollection* trkHits = evt->getCollection( "TrackerPlaneHits") ;

            for(int j=0;j<NHITS;j++) {

                //std::cout << " testing hit " << j << std::endl ;

	      TrackerHitPlane* trkHit = dynamic_cast<TrackerHitPlane*>(trkHits->getElementAt(j)) ;
	      //               TrackerHit* trkHit = dynamic_cast<TrackerHit*>(trkHits->getElementAt(j)) ;

                //std::cout << *trkHit << std::endl ;

                MYTEST( trkHit->getEDep() ,  i*j*117. , "EDep" ) ;

                // MYTEST( trkHit->getdEdx() ,  i*j*117. , "dEdx" ) ;
                // remove float converstion and check what happens ;)
                MYTEST( trkHit->getEDepError() ,  float((i+j)*.3) , "EDepError" ) ;
                //MYTEST( trkHit->getEDepError() ,  (i+j)*.3 , "EDepError" ) ;

                const double* pos = trkHit->getPosition() ;

                MYTEST( pos[0] , i , " pos[0] " ) ;
                MYTEST( pos[1] , j , " pos[1] " ) ;
                MYTEST( pos[2] , i*j , " pos[2] " ) ;


                const FloatVec& cov = trkHit->getCovMatrix() ;
		
		// covariance should be size 6
		MYTEST( cov.size()  , 6u , " cov.size() != 6" ) ;

                MYTEST( cov[0] , 0 , " cov[0] " ) ;
                MYTEST( cov[1] , 0 , " cov[1] " ) ;
                MYTEST( cov[2] , 0 , " cov[2] " ) ;
                MYTEST( cov[3] , 0 , " cov[3] " ) ;
                MYTEST( cov[4] , 0 , " cov[4] " ) ;
                MYTEST( cov[5] , 0 , " cov[5] " ) ;
		

            }
        }
        lcRdr->close() ;


    } catch( Exception &e ){
        MYTEST.FAILED( e.what() );
    }

    return 0;
}

//=============================================================================

