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

int main(int /*argc*/, char** /*argv*/ ){

    // this should be the first line in your test
    TEST MYTEST=TEST( testname, std::cout );

    try{

        MYTEST.LOG( "testing TrackState" );



        MYTEST.LOG( "test default constructor" );

        TrackStateImpl a;

        MYTEST( a.getLocation(), TrackState::AtOther, "getLocation" ) ;
        MYTEST( a.getD0(),  float( .0 ), "getD0" ) ;
        MYTEST( a.getPhi(),  float( .0 ), "getPhi" ) ;
        MYTEST( a.getOmega(),  float( .0 ), "getOmega" ) ;



        MYTEST.LOG( "test constructor with arguments" );

        float cov[15] = { 1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,13.,14.,15. } ;
        float ref[3] = { 1. , 2. , 3. } ;

        TrackStateImpl b
        (
            TrackState::AtLastHit,    // location
            .1,   // d0
            .2,   // phi
            .3,   // omega
            .4,   // z0
            .5,   // tanlambda
            cov,
            ref
        );

        MYTEST( b.getLocation(), TrackState::AtLastHit, "getLocation" ) ;
        MYTEST( b.getD0(),  float( .1 ), "getD0" ) ;
        MYTEST( b.getPhi(),  float( .2 ), "getPhi" ) ;
        MYTEST( b.getOmega(),  float( .3 ), "getOmega" ) ;



        MYTEST.LOG( "test default copy constructor" );

        TrackStateImpl c(b);

        MYTEST( c.getLocation(), TrackState::AtLastHit, "getLocation" ) ;
        MYTEST( c.getD0(),  float( .1 ), "getD0" ) ;
        MYTEST( c.getPhi(),  float( .2 ), "getPhi" ) ;
        MYTEST( c.getOmega(),  float( .3 ), "getOmega" ) ;



        MYTEST.LOG( "test default assignment operator" );

        TrackStateImpl d=b;

        MYTEST( d.getLocation(), TrackState::AtLastHit, "getLocation" ) ;
        MYTEST( d.getD0(),  float( .1 ), "getD0" ) ;
        MYTEST( d.getPhi(),  float( .2 ), "getPhi" ) ;
        MYTEST( d.getOmega(),  float( .3 ), "getOmega" ) ;



        MYTEST.LOG( "test default copy constructor using EVENT::TrackState pointer" );

        const EVENT::TrackState * p = new TrackStateImpl(d) ;

        MYTEST( p->getLocation(), TrackState::AtLastHit, "getLocation" ) ;
        MYTEST( p->getD0(),  float( .1 ), "getD0" ) ;
        MYTEST( p->getPhi(),  float( .2 ), "getPhi" ) ;
        MYTEST( p->getOmega(),  float( .3 ), "getOmega" ) ;


        const FloatVec& covv = p->getCovMatrix() ;

        for( unsigned int i=0 ; i<covv.size() ; i++ ){
            stringstream ss;
            ss << " covv[" << i << "] " ;
            MYTEST( covv[i] , float(i+1) , ss.str() ) ;
        }

        const float* refp = p->getReferencePoint() ;

        for( unsigned int i=0 ; i<3 ; i++ ){
            stringstream ss;
            ss << " refp[" << i << "] " ;
            MYTEST( refp[i] , float(i+1) , ss.str() ) ;
        }



        MYTEST.LOG( "test default copy constructor using two EVENT::TrackState pointers" );

        // the ugly way... have to use a dynamic or static_cast to TrackStateImpl..
        const EVENT::TrackState * pp = new TrackStateImpl(static_cast<const TrackStateImpl&>(*p)) ;

        MYTEST( pp->getLocation(), TrackState::AtLastHit, "getLocation" ) ;
        MYTEST( pp->getD0(),  float( .1 ), "getD0" ) ;
        MYTEST( pp->getPhi(),  float( .2 ), "getPhi" ) ;
        MYTEST( pp->getOmega(),  float( .3 ), "getOmega" ) ;

        delete pp;
        pp=NULL;



        MYTEST.LOG( "test default copy constructor using two EVENT::TrackState pointers" );

        // the nicer way... just passing the EVENT::TrackState object
        pp = new TrackStateImpl( *p ) ;

        MYTEST( pp->getLocation(), TrackState::AtLastHit, "getLocation" ) ;
        MYTEST( pp->getD0(),  float( .1 ), "getD0" ) ;
        MYTEST( pp->getPhi(),  float( .2 ), "getPhi" ) ;
        MYTEST( pp->getOmega(),  float( .3 ), "getOmega" ) ;


        delete pp;
        pp=NULL;


        delete p;
        p=NULL;


    } catch( Exception &e ){
        MYTEST.FAILED( e.what() );
    }

    return 0;
}

//=============================================================================

