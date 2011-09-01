////////////////////////////////////////
//  test lcio::TrackState
////////////////////////////////////////

#include "tutil.h"
#include "lcio.h"
//
//#include "EVENT/LCIO.h"
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

        MYTEST.LOG( " testing TrackState " );

        TrackStateImpl t,tt;
        t.setOmega(2);
        tt=t;
        cout << "DEBUG t: " << t.getOmega() << endl;
        cout << "DEBUG tt: " << tt.getOmega() << endl;


        float cov[15] = { 1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,13.,14.,15. } ;
        float ref[3] = { 1. , 2. , 3. } ;

        //TrackStateImpl* trkstate = new TrackStateImpl(
        //    k,                  // location
        //    ( i*j*k * .1 ),     // d0
        //    ( (i+j+k) * .3 ),   // phi
        //    ( (i+j+k) * .1 ),   // omega
        //    ( i*j*k * .1 ),     // z0
        //    ( (i+j+k) * .2 ),   // tanlambda
        //    cov,
        //    ref
        //);

        //MYTEST( trackstates[k]->getOmega(),  float( (i+j+k) * .1 ), "getOmega" ) ;
        //MYTEST( trackstates[k]->getTanLambda(),  float( (i+j+k) * .2 ), "getTanLambda" ) ;
        //MYTEST( trackstates[k]->getPhi(),  float( (i+j+k) * .3 ), "getPhi" ) ;
        //MYTEST( trackstates[k]->getD0(),  float( i*j*k * .1 ), "getD0" ) ;
        //MYTEST( trackstates[k]->getZ0(),  float( i*j*k * .1 ), "getZ0" ) ;

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

