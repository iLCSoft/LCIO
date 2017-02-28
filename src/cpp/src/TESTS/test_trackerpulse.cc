////////////////////////////////////////
//  test lcio::TrackerPulse
////////////////////////////////////////

#include "tutil.h"
#include "lcio.h"

#include "EVENT/LCIO.h"
#include "IO/LCReader.h"
#include "IO/LCWriter.h"
#include "IMPL/LCEventImpl.h" 
#include "IMPL/LCCollectionVec.h"
#include "IMPL/TrackerPulseImpl.h"
#include "IMPL/LCFlagImpl.h"

//#include <iostream>

using namespace std ;
using namespace lcio ;

//static const int NRUN = 10 ;
static const int NEVENT = 10 ; // events
static const int NPULSES = 1000 ;  // tracker pulses per event

static string FILEN = "trackerpulses.slcio" ;

// replace mytest with the name of your test
const static string testname="test_trackerpulse";

//=============================================================================

int main(int /*argc*/, char** /*argv*/ ){

    // this should be the first line in your test
    TEST MYTEST=TEST( testname, std::cout );

    try{


        MYTEST.LOG( " writing TrackerPulses " );

        // create sio writer
        LCWriter* lcWrt = LCFactory::getInstance()->createLCWriter()  ;

        lcWrt->open( FILEN , LCIO::WRITE_NEW ) ;

        // EventLoop - create some events and write them to the file
        for(int i=0;i<NEVENT;i++){

            // we need to use the implementation classes here 
            LCEventImpl*  evt = new LCEventImpl() ;


            evt->setRunNumber( 4711  ) ;
            evt->setEventNumber( i ) ;

            LCCollectionVec* trkPulses = new LCCollectionVec( LCIO::TRACKERPULSE )  ;
            LCCollectionVec* trkPulsesCov = new LCCollectionVec( LCIO::TRACKERPULSE )  ;

            LCFlagImpl flag( trkPulsesCov->getFlag() ) ;
            flag.setBit( LCIO::TRAWBIT_CM ) ;
            trkPulsesCov->setFlag( flag.getFlag() ) ;


            for(int j=0;j<NPULSES;j++){
                TrackerPulseImpl* trkPulse = new TrackerPulseImpl ;
                trkPulse->setTime( 3.1415 + 0.1 * i ) ;
                //trkPulse->setTimeError( (i+j) * .003 ) ;
                trkPulse->setCharge( 3.1415 + 0.1 * j ) ;
                //trkPulse->setChargeError( (i+j)*.005 ) ;
                
                trkPulses->addElement( trkPulse ) ;
            }
            for(int j=0;j<NPULSES;j++){
                TrackerPulseImpl* trkPulse = new TrackerPulseImpl ;
                trkPulse->setTime( 3.1415 + 0.1 * i ) ;
                //trkPulse->setTimeError( (i+j) * .003 ) ;
                trkPulse->setCharge( 3.1415 + 0.1 * j ) ;
                //trkPulse->setChargeError( (i+j)*.005 ) ;

		float cov[3] = { float(i) , float(j) ,float(i*j) } ;
		trkPulse->setCovMatrix( cov );

                trkPulsesCov->addElement( trkPulse ) ;
            }

            evt->addCollection( trkPulses , "TrackerPulses") ;
            evt->addCollection( trkPulsesCov , "TrackerPulsesWithCovMatrix") ;

            lcWrt->writeEvent(evt) ;

            delete evt ;
        }


        lcWrt->close() ;

        MYTEST.LOG(" reading back TrackerPulses from file " ) ;

        // create sio reader
        LCReader* lcRdr = LCFactory::getInstance()->createLCReader()  ;

        lcRdr->open( FILEN ) ;

        for(int i=0;i<NEVENT;i++){

            //std::cout << " testing event " << i << std::endl ;

            LCEvent*  evt =  lcRdr->readNextEvent() ;

            MYTEST( evt->getRunNumber() ,  4711 , " run number " )  ;

            MYTEST(  evt->getEventNumber() ,  i , " event number "  ) ;

            LCCollection* trkPulses = evt->getCollection( "TrackerPulses") ;

            //MYTEST.LOG(" reading back TrackerPulses from file " ) ;

            for(int j=0;j<NPULSES;j++) {

                //std::cout << " testing pulse " << j << std::endl ;

                TrackerPulse* trkPulse = dynamic_cast<TrackerPulse*>(trkPulses->getElementAt(j)) ;

                MYTEST( trkPulse->getTime(),  float( 3.1415 + 0.1 * i ), "time" ) ;
                //MYTEST( trkPulse->getTimeError(),  float( (i + j) * .003 ), "time error" ) ;
                MYTEST( trkPulse->getCharge(),  float( 3.1415 + 0.1 * j ), "charge" ) ;
                //MYTEST( trkPulse->getChargeError(), float( (i + j) * .005 ), "charge error" ) ;


                const FloatVec& cov = trkPulse->getCovMatrix() ;
                
                // should be initialized to 0
                MYTEST( cov[0] , 0 , " cov[0] " ) ;
                MYTEST( cov[1] , 0 , " cov[1] " ) ;
                MYTEST( cov[2] , 0 , " cov[2] " ) ;

            }

            LCCollection* trkPulsesCov = evt->getCollection( "TrackerPulsesWithCovMatrix") ;

            //MYTEST.LOG(" reading back TrackerPulsesWithCovMatrix from file " ) ;

            for(int j=0;j<NPULSES;j++) {

                //std::cout << " testing pulse " << j << std::endl ;

                TrackerPulse* trkPulse = dynamic_cast<TrackerPulse*>(trkPulsesCov->getElementAt(j)) ;

                MYTEST( trkPulse->getTime(),  float( 3.1415 + 0.1 * i ), "time" ) ;
                //MYTEST( trkPulse->getTimeError(),  float( (i + j) * .003 ), "time error" ) ;
                MYTEST( trkPulse->getCharge(),  float( 3.1415 + 0.1 * j ), "charge" ) ;
                //MYTEST( trkPulse->getChargeError(), float( (i + j) * .005 ), "charge error" ) ;
                const FloatVec& cov = trkPulse->getCovMatrix() ;
                
                MYTEST( cov[0] , i , " cov[0] " ) ;
                MYTEST( cov[1] , j , " cov[1] " ) ;
                MYTEST( cov[2] , i*j , " cov[2] " ) ;
            }

        }
        lcRdr->close() ;


    } catch( Exception &e ){
        MYTEST.FAILED( e.what() );
    }

    return 0;
}

//=============================================================================

