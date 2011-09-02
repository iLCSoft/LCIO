////////////////////////////////////////
//  test lcio::TrackState
////////////////////////////////////////

#include "tutil.h"
#include "lcio.h"

#include "EVENT/TrackState.h"
#include "IMPL/TrackStateImpl.h"

//#include "UTIL/Operators.h"

#include <sstream>
#include <assert.h>

using namespace std ;
using namespace lcio ;

// replace mytest with the name of your test
const static string testname="test_trackstate";

//=============================================================================

int main(int argc, char** argv ){

    // this should be the first line in your test
    TEST MYTEST=TEST( testname, std::cout );

    try{

        MYTEST.LOG( "testing TrackState" );



        MYTEST.LOG( "test default constructor" );

        TrackStateImpl a;

        MYTEST( a.getLocation(), TrackStateImpl::Location::AtOther, "getLocation" ) ;
        MYTEST( a.getD0(),  float( .0 ), "getD0" ) ;
        MYTEST( a.getPhi(),  float( .0 ), "getPhi" ) ;
        MYTEST( a.getOmega(),  float( .0 ), "getOmega" ) ;



        MYTEST.LOG( "test constructor with arguments" );

        float cov[15] = { 1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,13.,14.,15. } ;
        float ref[3] = { 1. , 2. , 3. } ;

        TrackStateImpl b
        (
            TrackStateImpl::Location::AtLastHit,    // location
            .1,   // d0
            .2,   // phi
            .3,   // omega
            .4,   // z0
            .5,   // tanlambda
            cov,
            ref
        );

        MYTEST( b.getLocation(), TrackStateImpl::Location::AtLastHit, "getLocation" ) ;
        MYTEST( b.getD0(),  float( .1 ), "getD0" ) ;
        MYTEST( b.getPhi(),  float( .2 ), "getPhi" ) ;
        MYTEST( b.getOmega(),  float( .3 ), "getOmega" ) ;



        MYTEST.LOG( "test default copy constructor" );

        TrackStateImpl c(b);

        MYTEST( c.getLocation(), TrackStateImpl::Location::AtLastHit, "getLocation" ) ;
        MYTEST( c.getD0(),  float( .1 ), "getD0" ) ;
        MYTEST( c.getPhi(),  float( .2 ), "getPhi" ) ;
        MYTEST( c.getOmega(),  float( .3 ), "getOmega" ) ;



        MYTEST.LOG( "test default assignment operator" );

        TrackStateImpl d=b;

        MYTEST( d.getLocation(), TrackStateImpl::Location::AtLastHit, "getLocation" ) ;
        MYTEST( d.getD0(),  float( .1 ), "getD0" ) ;
        MYTEST( d.getPhi(),  float( .2 ), "getPhi" ) ;
        MYTEST( d.getOmega(),  float( .3 ), "getOmega" ) ;



        MYTEST.LOG( "test default copy constructor using EVENT::TrackState pointer" );


        EVENT::TrackState * p = new TrackStateImpl(d) ;

        MYTEST( p->getLocation(), TrackStateImpl::Location::AtLastHit, "getLocation" ) ;
        MYTEST( p->getD0(),  float( .1 ), "getD0" ) ;
        MYTEST( p->getPhi(),  float( .2 ), "getPhi" ) ;
        MYTEST( p->getOmega(),  float( .3 ), "getOmega" ) ;



        MYTEST.LOG( "test default copy constructor using two EVENT::TrackState pointers" );


        // the ugly way... have to use a dynamic or static_cast to TrackStateImpl..
        EVENT::TrackState * pp = new TrackStateImpl(static_cast<TrackStateImpl&>(*p)) ;

        MYTEST( pp->getLocation(), TrackStateImpl::Location::AtLastHit, "getLocation" ) ;
        MYTEST( pp->getD0(),  float( .1 ), "getD0" ) ;
        MYTEST( pp->getPhi(),  float( .2 ), "getPhi" ) ;
        MYTEST( pp->getOmega(),  float( .3 ), "getOmega" ) ;




        //const FloatVec& cov = trackstates[k]->getCovMatrix() ;

        //for( unsigned int l=0 ; l<cov.size() ; l++ ){
        //    stringstream ss;
        //    ss << " cov[" << l << "] " ;
        //    MYTEST( cov[l] , float(k+(l+1)) , ss.str() ) ;
        //}

        //const float* ref = trackstates[k]->getReferencePoint() ;

        //for( unsigned int l=0 ; l<3 ; l++ ){
        //    stringstream ss;
        //    ss << " ref[" << l << "] " ;
        //    MYTEST( ref[l] , float(k*(l+1)) , ss.str() ) ;
        //}
        //        
        //// Test getClosestTrackState( float x, float y, float z )
        //const TrackState* trackstate = trk->getClosestTrackState( 0, 0, 0 ) ;
        //assert( trackstate != NULL );
        //ref = trackstate->getReferencePoint() ;

        ////std::cout << " closest trackstate " <<  endl << header(trackstate) << lcio_short<EVENT::TrackState>(trackstate) << endl ;

        //for( unsigned int k=0 ; k<3 ; k++ ){
        //    stringstream ss;
        //    ss << " closest trackstate ref[" << k << "] " ;
        //    MYTEST( ref[k] , float(k+1) , ss.str() ) ;
        //}

        //// Test getTrackState( int location )
        //trackstate = trk->getTrackState( TrackState::Location::AtIP ) ;
        //assert( trackstate != NULL );
        //ref = trackstate->getReferencePoint() ;

        ////std::cout << " trackstate AtIP" <<  endl << header(trackstate) << lcio_short<EVENT::TrackState>(trackstate) << endl ;

        //for( unsigned int k=0 ; k<3 ; k++ ){
        //    stringstream ss;
        //    ss << " trackstate AtIP ref[" << k << "] " ;
        //    MYTEST( ref[k] , float(k+1) , ss.str() ) ;
        //}

    } catch( Exception &e ){
        MYTEST.FAILED( e.what() );
    }

    return 0;
}

//=============================================================================

