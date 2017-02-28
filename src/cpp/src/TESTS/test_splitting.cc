////////////////////////////////////////
//  test splitting
////////////////////////////////////////

#include "tutil.h"
#include "lcio.h"

#include "EVENT/LCIO.h"
#include "IO/LCReader.h"
#include "IO/LCWriter.h"
#include "UTIL/LCSplitWriter.h"
#include "IMPL/LCEventImpl.h" 
#include "IMPL/LCCollectionVec.h"
#include "IMPL/ClusterImpl.h"
#include "IMPL/LCFlagImpl.h"

//#include <iostream>
#include <cstdio>

using namespace std ;
using namespace lcio ;

//static const int NRUN = 10 ;
static const int NEVENT = 10 ; // events
static const int NHITS = 100 ;  // calorimeter hits per event
static const int SPLIT_SIZE_KB = 3 ;

static string FILE_BASENAME = "splitting" ;
static string FILE_EXTENSION = ".slcio" ;

// replace mytest with the name of your test
const static string testname="test_splitting";

//=============================================================================

int main(int /*argc*/, char** /*argv*/ ){

    // this should be the first line in your test
    TEST MYTEST=TEST( testname, std::cout );

    try{

        MYTEST.LOG( "test splitting" );


        vector<string> inputFilesVec ;
        inputFilesVec.push_back( FILE_BASENAME + ".000" + FILE_EXTENSION );
        inputFilesVec.push_back( FILE_BASENAME + ".001" + FILE_EXTENSION );
        inputFilesVec.push_back( FILE_BASENAME + ".002" + FILE_EXTENSION );

        // remove files if needed
        for(unsigned i=0 ; i<inputFilesVec.size() ; i++){
            remove( inputFilesVec[i].c_str() );
        }

        // create sio writer
        //LCWriter* lcWrt = LCFactory::getInstance()->createLCWriter()  ;
        LCWriter* lcWrt = new LCSplitWriter( LCFactory::getInstance()->createLCWriter(), SPLIT_SIZE_KB*1024  ) ;

        //lcWrt->open( FILE_BASENAME + FILE_EXTENSION, LCIO::WRITE_NEW ) ;
        lcWrt->open( FILE_BASENAME + FILE_EXTENSION );

        // EventLoop - create some events and write them to the file
        for(int i=0;i<NEVENT;i++){

            // we need to use the implementation classes here 
            LCEventImpl*  evt = new LCEventImpl() ;


            evt->setRunNumber( 4711  ) ;
            evt->setEventNumber( i ) ;

            LCCollectionVec* clusters = new LCCollectionVec( LCIO::CLUSTER )  ;

            for(int j=0;j<NHITS;j++){
                ClusterImpl* clu = new ClusterImpl ;
                clu->setEnergy( i*j*117. ) ;
                float pos[3] = { float(i) , float(j) ,float(i*j) } ;
                clu->setPosition( pos ) ;
                clusters->addElement( clu ) ;
            }

            evt->addCollection( clusters , "Clusters") ;

            lcWrt->writeEvent(evt) ;

            delete evt ;
        }


        lcWrt->close() ;

        MYTEST.LOG("test reading back from split files" ) ;

        // create sio reader
        LCReader* lcRdr = LCFactory::getInstance()->createLCReader()  ;

        lcRdr->open( inputFilesVec ) ;

        for(int i=0;i<NEVENT;i++){

            //std::cout << " testing event " << i << std::endl ;

            LCEvent*  evt =  lcRdr->readNextEvent() ;

            MYTEST( evt->getRunNumber() ,  4711 , " run number " )  ;

            MYTEST(  evt->getEventNumber() ,  i , " event number "  ) ;

            LCCollection* clusters = evt->getCollection( "Clusters") ;

            for(int j=0;j<NHITS;j++) {

                //std::cout << " testing hit " << j << std::endl ;

                Cluster* clu = dynamic_cast<Cluster*>(clusters->getElementAt(j)) ;

                MYTEST( clu->getEnergy() ,  i*j*117. , "energy" ) ;

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

