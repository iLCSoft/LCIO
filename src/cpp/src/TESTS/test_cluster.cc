////////////////////////////////////////
//  test lcio::Cluster
////////////////////////////////////////

#include "tutil.h"
#include "lcio.h"

#include "EVENT/LCIO.h"
#include "IO/LCReader.h"
#include "IO/LCWriter.h"
#include "IMPL/LCEventImpl.h" 
#include "IMPL/LCCollectionVec.h"
#include "IMPL/ClusterImpl.h"
#include "IMPL/LCFlagImpl.h"

//#include <iostream>

using namespace std ;
using namespace lcio ;

//static const int NRUN = 10 ;
static const int NEVENT = 3 ; // events
static const int NHITS = 50 ;  // calorimeter hits per event

static string FILEN = "cluster.slcio" ;

// replace mytest with the name of your test
const static string testname="test_cluster";

//=============================================================================

int main(int /*argc*/, char** /*argv*/ ){

    // this should be the first line in your test
    TEST MYTEST=TEST( testname, std::cout );

    try{


        MYTEST.LOG( " writing Clusters " );

        // create sio writer
        LCWriter* lcWrt = LCFactory::getInstance()->createLCWriter()  ;

        lcWrt->open( FILEN , LCIO::WRITE_NEW ) ;

        // EventLoop - create some events and write them to the file
        for(int i=0;i<NEVENT;i++){

            // we need to use the implementation classes here 
            LCEventImpl*  evt = new LCEventImpl() ;


            evt->setRunNumber( 4711  ) ;
            evt->setEventNumber( i ) ;

            LCCollectionVec* clusters = new LCCollectionVec( LCIO::CLUSTER )  ;
            LCCollectionVec* clustersErr = new LCCollectionVec( LCIO::CLUSTER )  ;

            for(int j=0;j<NHITS;j++){
                ClusterImpl* clu = new ClusterImpl ;
                clu->setEnergy( i*j*117. ) ;
		float pos[3] = { float(i) , float(j) ,float(i*j) } ;
                clu->setPosition( pos ) ;
                clusters->addElement( clu ) ;
            }
            for(int j=0;j<NHITS;j++){
                ClusterImpl* clu = new ClusterImpl ;
                clu->setEnergy( i*j*117. ) ;
                clu->setEnergyError( i*j*0.117 ) ;
		float pos[3] = { float(i) , float(j) ,float(i*j) } ;
                clu->setPosition( pos ) ;
                clustersErr->addElement( clu ) ;
            }
            evt->addCollection( clusters , "Clusters") ;
            evt->addCollection( clustersErr , "ClustersWithEnergyError") ;

            lcWrt->writeEvent(evt) ;

            delete evt ;
        }


        lcWrt->close() ;

        MYTEST.LOG(" reading back Clusters from file " ) ;

        // create sio reader
        LCReader* lcRdr = LCFactory::getInstance()->createLCReader()  ;

        lcRdr->open( FILEN ) ;

        for(int i=0;i<NEVENT;i++){

            //std::cout << " testing event " << i << std::endl ;

            LCEvent*  evt =  lcRdr->readNextEvent() ;

            MYTEST( evt->getRunNumber() ,  4711 , " run number " )  ;

            MYTEST(  evt->getEventNumber() ,  i , " event number "  ) ;

            LCCollection* clusters = evt->getCollection( "Clusters") ;
            LCCollection* clustersErr = evt->getCollection( "ClustersWithEnergyError") ;

            for(int j=0;j<NHITS;j++) {

                //std::cout << " testing hit " << j << std::endl ;

                Cluster* clu = dynamic_cast<Cluster*>(clusters->getElementAt(j)) ;

                MYTEST( clu->getEnergy() ,  i*j*117. , "energy" ) ;

                const float* pos = clu->getPosition() ;

                MYTEST( pos[0] , i , " pos[0] " ) ;
                MYTEST( pos[1] , j , " pos[1] " ) ;
                MYTEST( pos[2] , i*j , " pos[2] " ) ;

            }
            for(int j=0;j<NHITS;j++) {

                //std::cout << " testing hit " << j << std::endl ;

                Cluster* clu = dynamic_cast<Cluster*>(clustersErr->getElementAt(j)) ;

                MYTEST( clu->getEnergy() ,  i*j*117. , "energy" ) ;
                MYTEST( clu->getEnergyError() , float(i*j*0.117)  , "energy error" ) ;

                const float* pos = clu->getPosition() ;

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

