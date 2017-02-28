////////////////////////////////////////
//  test lcio::Track
////////////////////////////////////////

#include "tutil.h"
#include "lcio.h"

#include "EVENT/LCIO.h"
#include "IO/LCReader.h"
#include "IO/LCWriter.h"
#include "IMPL/LCEventImpl.h" 
#include "IMPL/LCCollectionVec.h"
#include "IMPL/TrackImpl.h"
#include "IMPL/TrackStateImpl.h"

#include "UTIL/Operators.h"
#include "UTIL/LCIterator.h"

#include <sstream>
#include <assert.h>

using namespace std ;
using namespace lcio ;

//static const int NRUN = 10 ;
static const int NEVENT = 3 ; // events
static const int NTRACKS = 10 ;  // tracks per event

static string FILEN = "tracks.slcio" ;

// replace mytest with the name of your test
const static string testname="test_tracks";

//=============================================================================

int main(int /*argc*/, char** /*argv*/ ){

    // this should be the first line in your test
    TEST MYTEST=TEST( testname, std::cout );

    try{

        MYTEST.LOG( " writing Tracks " );

        // create sio writer
        LCWriter* lcWrt = LCFactory::getInstance()->createLCWriter()  ;

        lcWrt->open( FILEN , LCIO::WRITE_NEW ) ;


        // EventLoop - create some events and write them to the file
        for( int i=0 ; i<NEVENT ; i++ ){

            // we need to use the implementation classes here 
            LCEventImpl*  evt = new LCEventImpl() ;


            evt->setRunNumber( 4711  ) ;
            evt->setEventNumber( i ) ;

            LCCollectionVec* tracks = new LCCollectionVec( LCIO::TRACK )  ;
            LCCollectionVec* tracksCopies = new LCCollectionVec( LCIO::TRACK )  ;
            LCCollectionVec* tracksWithMultipleStates = new LCCollectionVec( LCIO::TRACK )  ;
            LCCollectionVec* tracksWithMultipleStatesCopies = new LCCollectionVec( LCIO::TRACK )  ;

            for( int j=0 ; j<NTRACKS ; j++ ){

                TrackImpl* trk = new TrackImpl ;

                trk->setTypeBit( 7 ) ;
                trk->setOmega(  (i+j) * .1 ) ;
                trk->setTanLambda( (i+j) * .2 ) ;
                trk->setPhi( (i+j) * .3 ) ;
                trk->setD0( i * .1 ) ;
                trk->setZ0( j * .1 ) ;
                trk->setChi2( 1.01 ) ;
                trk->setNdf( 42 ) ;
                trk->setRadiusOfInnermostHit( 3.14159265 ) ;
                trk->setdEdx( 3.14159265 ) ;
                trk->setdEdxError( 42. ) ;

                float cov[15] = { 1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,13.,14.,15. } ;
                trk->setCovMatrix( cov ) ;

                float ref[3] = { 1. , 2. , 3. } ;
                trk->setReferencePoint( ref ) ;

                // add tracks that where used to create this track
                if( tracks->size() > 1 ){
                    trk->addTrack( dynamic_cast<TrackImpl*> ( (*tracks)[ tracks->size() - 1 ] ) ) ;
                    trk->addTrack( dynamic_cast<TrackImpl*> ( (*tracks)[ tracks->size() - 2 ] ) ) ;
                }

                TrackImpl* trkc = new TrackImpl(*trk) ;

                tracks->addElement( trk ) ;
                tracksCopies->addElement( trkc ) ;
            }

            for( int j=0 ; j<NTRACKS ; j++ ){
                TrackImpl* trk = new TrackImpl ;

                trk->setTypeBit( 7 ) ;

                trk->setOmega(  (i+j) * .1 ) ;
                trk->setTanLambda( (i+j) * .2 ) ;
                trk->setPhi( (i+j) * .3 ) ;
                trk->setD0( i * .1 ) ;
                trk->setZ0( j * .1 ) ;
                trk->setChi2( 1.01 ) ;
                trk->setNdf( 42 ) ;
                trk->setRadiusOfInnermostHit( 3.14159265 ) ;
                trk->setdEdx( 3.14159265 ) ;
                trk->setdEdxError( 42. ) ;

                float cov[15] = { 1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,13.,14.,15. } ;
                trk->setCovMatrix( cov ) ;

                float ref[3] = { 1. , 2. , 3. } ;
                trk->setReferencePoint( ref ) ;

                // add some additional TrackStates
                for( int ki=1 ; ki < 4 ; ki++ ){
		     
		    float k( ki );

                    float covL[15] = { k+1,k+2,k+3,k+4,k+5,k+6,k+7,k+8,k+9,k+10,k+11,k+12,k+13,k+14,k+15 } ;
                    float refL[3] = { k*1 , k*2 , k*3 } ;

                    TrackStateImpl* trkstate = new TrackStateImpl(
                            k,                  // location
                            ( i*j*k * .1 ),     // d0
                            ( (i+j+k) * .3 ),   // phi
                            ( (i+j+k) * .1 ),   // omega
                            ( i*j*k * .1 ),     // z0
                            ( (i+j+k) * .2 ),   // tanlambda
                            covL,
                            refL
                            );
                    trk->addTrackState( trkstate ) ;
                }

                TrackImpl* trkc = new TrackImpl(*trk) ;

                tracksWithMultipleStates->addElement( trk ) ;
                tracksWithMultipleStatesCopies->addElement( trkc ) ;
            }

            evt->addCollection( tracks , "Tracks") ;
            evt->addCollection( tracksCopies , "TracksCopies") ;
            evt->addCollection( tracksWithMultipleStates , "TracksWithMultipleStates") ;
            evt->addCollection( tracksWithMultipleStatesCopies , "TracksWithMultipleStatesCopies") ;

            lcWrt->writeEvent(evt) ;

            delete evt ;
        }


        lcWrt->close() ;
        delete lcWrt;

        MYTEST.LOG(" reading back Tracks from file " ) ;

        // create sio reader
        LCReader* lcRdr = LCFactory::getInstance()->createLCReader()  ;

        lcRdr->open( FILEN ) ;

        for( int i=0 ; i<NEVENT ; i++ ){

            //std::cout << " testing event " << i << std::endl ;

            LCEvent*  evt =  lcRdr->readNextEvent() ;

            MYTEST( evt->getRunNumber() ,  4711 , " run number " )  ;

            MYTEST(  evt->getEventNumber() ,  i , " event number "  ) ;

            // LCCollection* tracks = evt->getCollection( "Tracks") ;

            // MYTEST.LOG(" reading back Tracks (one TrackState only) " ) ;

            // for( int j=0 ; j<NTRACKS ; j++ ){

            //     //std::cout << " testing track " << j << std::endl ;

            //     Track* trk = dynamic_cast<Track*>(tracks->getElementAt(j)) ;

            int j=0 ;

            LCIterator<Track> it( evt, "Tracks"  ) ;
            while( Track* trk = it.next() ) {

                MYTEST( trk->getOmega(),  float( (i+j) * .1 ), "getOmega" ) ;
                MYTEST( trk->getTanLambda(),  float( (i+j) * .2 ), "getTanLambda" ) ;
                MYTEST( trk->getPhi(),  float( (i+j) * .3 ), "getPhi" ) ;
                MYTEST( trk->getD0(),  float( i * .1 ), "getD0" ) ;
                MYTEST( trk->getZ0(),  float( j * .1 ), "getZ0" ) ;

                MYTEST( trk->getChi2(), float(1.01) , "getChi2" ) ;
                MYTEST( trk->getNdf(), 42 , "getNdf" ) ;
                MYTEST( trk->getdEdx(), float(3.14159265) , "getdEdx" ) ;
                MYTEST( trk->getdEdxError(), float(42.) , "getdEdxError" ) ;
                MYTEST( trk->getRadiusOfInnermostHit(), float(3.14159265) , "getRadiusOfInnermostHit" ) ;

                const FloatVec& cov = trk->getCovMatrix() ;

                for( unsigned int k=0 ; k<cov.size() ; k++ ){
                    stringstream ss;
                    ss << " cov[" << k << "] " ;
                    MYTEST( cov[k] , float(k+1) , ss.str() ) ;
                }
                const float* ref = trk->getReferencePoint() ;

                for( unsigned int k=0 ; k<3 ; k++ ){
                    stringstream ss;
                    ss << " ref[" << k << "] " ;
                    MYTEST( ref[k] , float(k+1) , ss.str() ) ;
                }

                ++j ;
            }

            stringstream ssG;
            ssG <<  " read back " << it->getNumberOfElements() << " tracks  from collection " << "Tracks using the LCIterator<Track>.\n" ; 
            MYTEST.LOG( ssG.str()  ) ;


            j=0 ;

            LCIterator<Track> itc( evt, "TracksCopies"  ) ;
            while( Track* trk = itc.next() ) {

                MYTEST( trk->getOmega(),  float( (i+j) * .1 ), "getOmega" ) ;
                MYTEST( trk->getTanLambda(),  float( (i+j) * .2 ), "getTanLambda" ) ;
                MYTEST( trk->getPhi(),  float( (i+j) * .3 ), "getPhi" ) ;
                MYTEST( trk->getD0(),  float( i * .1 ), "getD0" ) ;
                MYTEST( trk->getZ0(),  float( j * .1 ), "getZ0" ) ;

                MYTEST( trk->getChi2(), float(1.01) , "getChi2" ) ;
                MYTEST( trk->getNdf(), 42 , "getNdf" ) ;
                MYTEST( trk->getdEdx(), float(3.14159265) , "getdEdx" ) ;
                MYTEST( trk->getdEdxError(), float(42.) , "getdEdxError" ) ;
                MYTEST( trk->getRadiusOfInnermostHit(), float(3.14159265) , "getRadiusOfInnermostHit" ) ;

                const FloatVec& cov = trk->getCovMatrix() ;

                for( unsigned int k=0 ; k<cov.size() ; k++ ){
                    stringstream ss;
                    ss << " cov[" << k << "] " ;
                    MYTEST( cov[k] , float(k+1) , ss.str() ) ;
                }
                const float* ref = trk->getReferencePoint() ;

                for( unsigned int k=0 ; k<3 ; k++ ){
                    stringstream ss;
                    ss << " ref[" << k << "] " ;
                    MYTEST( ref[k] , float(k+1) , ss.str() ) ;
                }

                ++j ;
            }

            ssG.str("");
            ssG <<  " read back " << it->getNumberOfElements() << " tracks  from collection " << "TracksCopies using the LCIterator<Track>.\n" ; 
            MYTEST.LOG( ssG.str()  ) ;




            LCCollection* tracksWithMultipleStates = evt->getCollection( "TracksWithMultipleStates") ;
            LCCollection* tracksWithMultipleStatesCopies = evt->getCollection( "TracksWithMultipleStatesCopies") ;

            MYTEST.LOG(" reading back TracksWithMultipleStates " ) ;

            for( j=0 ; j<NTRACKS ; j++ ){

                //std::cout << " testing track " << j << std::endl ;

                Track* trk = dynamic_cast<Track*>(tracksWithMultipleStates->getElementAt(j)) ;

                MYTEST( trk->getOmega(),  float( (i+j) * .1 ), "getOmega" ) ;
                MYTEST( trk->getTanLambda(),  float( (i+j) * .2 ), "getTanLambda" ) ;
                MYTEST( trk->getPhi(),  float( (i+j) * .3 ), "getPhi" ) ;
                MYTEST( trk->getD0(),  float( i * .1 ), "getD0" ) ;
                MYTEST( trk->getZ0(),  float( j * .1 ), "getZ0" ) ;

                MYTEST( trk->getChi2(), float(1.01) , "getChi2" ) ;
                MYTEST( trk->getNdf(), 42 , "getNdf" ) ;
                MYTEST( trk->getdEdx(), float(3.14159265) , "getdEdx" ) ;
                MYTEST( trk->getdEdxError(), float(42.) , "getdEdxError" ) ;
                MYTEST( trk->getRadiusOfInnermostHit(), float(3.14159265) , "getRadiusOfInnermostHit" ) ;

                const FloatVec& cov = trk->getCovMatrix() ;

                for( unsigned int k=0 ; k<cov.size() ; k++ ){
                    stringstream ss;
                    ss << " cov[" << k << "] " ;
                    MYTEST( cov[k] , float(k+1) , ss.str() ) ;
                }

                const float* ref = trk->getReferencePoint() ;

                for( unsigned int k=0 ; k<3 ; k++ ){
                    stringstream ss;
                    ss << " ref[" << k << "] " ;
                    MYTEST( ref[k] , float(k+1) , ss.str() ) ;
                }

                // more than one trackstate
                const TrackStateVec& trackstates = trk->getTrackStates() ;

                for( unsigned int k=1 ; k<trackstates.size() ; k++ ){

                    //std::cout << " testing trackstate " << k << std::endl ;
                    //std::cout << " testing trackstate " <<  endl << header(trackstates[k]) << lcio_short<EVENT::TrackState>(trackstates[k]) << endl ;

                    MYTEST( trackstates[k]->getOmega(),  float( (i+j+k) * .1 ), "getOmega" ) ;
                    MYTEST( trackstates[k]->getTanLambda(),  float( (i+j+k) * .2 ), "getTanLambda" ) ;
                    MYTEST( trackstates[k]->getPhi(),  float( (i+j+k) * .3 ), "getPhi" ) ;
                    MYTEST( trackstates[k]->getD0(),  float( i*j*k * .1 ), "getD0" ) ;
                    MYTEST( trackstates[k]->getZ0(),  float( i*j*k * .1 ), "getZ0" ) ;

                    const FloatVec& covM = trackstates[k]->getCovMatrix() ;

                    for( unsigned int l=0 ; l<covM.size() ; l++ ){
                        stringstream ss;
                        ss << " cov[" << l << "] " ;
                        MYTEST( covM[l] , float(k+(l+1)) , ss.str() ) ;
                    }

                    const float* refL = trackstates[k]->getReferencePoint() ;

                    for( unsigned int l=0 ; l<3 ; l++ ){
                        stringstream ss;
                        ss << " ref[" << l << "] " ;
                        MYTEST( refL[l] , float(k*(l+1)) , ss.str() ) ;
                    }
                }



                // ---------- test copies -------------------------------
                Track* trkc = dynamic_cast<Track*>(tracksWithMultipleStatesCopies->getElementAt(j)) ;
                MYTEST( trkc->getOmega(),  float( (i+j) * .1 ), "getOmega" ) ;
                MYTEST( trkc->getTanLambda(),  float( (i+j) * .2 ), "getTanLambda" ) ;
                MYTEST( trkc->getPhi(),  float( (i+j) * .3 ), "getPhi" ) ;
                MYTEST( trkc->getD0(),  float( i * .1 ), "getD0" ) ;
                MYTEST( trkc->getZ0(),  float( j * .1 ), "getZ0" ) ;

                MYTEST( trkc->getChi2(), float(1.01) , "getChi2" ) ;
                MYTEST( trkc->getNdf(), 42 , "getNdf" ) ;
                MYTEST( trkc->getdEdx(), float(3.14159265) , "getdEdx" ) ;
                MYTEST( trkc->getdEdxError(), float(42.) , "getdEdxError" ) ;
                MYTEST( trkc->getRadiusOfInnermostHit(), float(3.14159265) , "getRadiusOfInnermostHit" ) ;

                const FloatVec& covc = trkc->getCovMatrix() ;

                for( unsigned int k=0 ; k<covc.size() ; k++ ){
                    stringstream ss;
                    ss << " cov[" << k << "] " ;
                    MYTEST( covc[k] , float(k+1) , ss.str() ) ;
                }

                ref = trkc->getReferencePoint() ;

                for( unsigned int k=0 ; k<3 ; k++ ){
                    stringstream ss;
                    ss << " ref[" << k << "] " ;
                    MYTEST( ref[k] , float(k+1) , ss.str() ) ;
                }

                // more than one trackstate
                const TrackStateVec& trackstatesc = trkc->getTrackStates() ;

                for( unsigned int k=1 ; k<trackstatesc.size() ; k++ ){

                    MYTEST( trackstatesc[k]->getOmega(),  float( (i+j+k) * .1 ), "getOmega" ) ;
                    MYTEST( trackstatesc[k]->getTanLambda(),  float( (i+j+k) * .2 ), "getTanLambda" ) ;
                    MYTEST( trackstatesc[k]->getPhi(),  float( (i+j+k) * .3 ), "getPhi" ) ;
                    MYTEST( trackstatesc[k]->getD0(),  float( i*j*k * .1 ), "getD0" ) ;
                    MYTEST( trackstatesc[k]->getZ0(),  float( i*j*k * .1 ), "getZ0" ) ;

                    const FloatVec& covM = trackstatesc[k]->getCovMatrix() ;

                    for( unsigned int l=0 ; l<covM.size() ; l++ ){
                        stringstream ss;
                        ss << " cov[" << l << "] " ;
                        MYTEST( covM[l] , float(k+(l+1)) , ss.str() ) ;
                    }

                    const float* refL = trackstatesc[k]->getReferencePoint() ;

                    for( unsigned int l=0 ; l<3 ; l++ ){
                        stringstream ss;
                        ss << " ref[" << l << "] " ;
                        MYTEST( refL[l] , float(k*(l+1)) , ss.str() ) ;
                    }
                }

                // ---------- end test copies -------------------------------



                // Test getClosestTrackState( float x, float y, float z )
                const TrackState* trackstate = trk->getClosestTrackState( 0, 0, 0 ) ;
                assert( trackstate != NULL );
                ref = trackstate->getReferencePoint() ;

                //std::cout << " closest trackstate " <<  endl << header(trackstate) << lcio_short<EVENT::TrackState>(trackstate) << endl ;

                for( unsigned int k=0 ; k<3 ; k++ ){
                    stringstream ss;
                    ss << " closest trackstate ref[" << k << "] " ;
                    MYTEST( ref[k] , float(k+1) , ss.str() ) ;
                }

                // Test getTrackState( int location )
                trackstate = trk->getTrackState( TrackState::AtIP ) ;
                assert( trackstate != NULL );
                ref = trackstate->getReferencePoint() ;

                //std::cout << " trackstate AtIP" <<  endl << header(trackstate) << lcio_short<EVENT::TrackState>(trackstate) << endl ;

                for( unsigned int k=0 ; k<3 ; k++ ){
                    stringstream ss;
                    ss << " trackstate AtIP ref[" << k << "] " ;
                    MYTEST( ref[k] , float(k+1) , ss.str() ) ;
                }
            }

        }
        lcRdr->close() ;
        delete lcRdr;


        } catch( Exception &e ){
            MYTEST.FAILED( e.what() );
        }

        return 0;
    }

    //=============================================================================

