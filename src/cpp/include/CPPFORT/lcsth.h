/**Header file for the f77-wrapper functions of the SimTrackerHitImpl class.
 * 
 * @author F. Gaede
 * @version Oct 10, 2003
 */
#include "cfortran.h"
#include "cpointer.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

PTRTYPE lcsthcreate() ;
int lcsthdelete( PTRTYPE hit ) ;

int lcsthgetcellid( PTRTYPE hit )  ;
double lcsthgetposition( PTRTYPE hit, int index )  ;
float lcsthgetmomentum( PTRTYPE hit, int index )  ;
float lcsthgetpathlength ( PTRTYPE hit )  ;
float lcsthgetdedx( PTRTYPE hit )  ;
float lcsthgettime( PTRTYPE hit )  ;

PTRTYPE lcsthgetmcparticle( PTRTYPE hit )  ;

int lcsthsetcellid( PTRTYPE hit, int id ) ;
int lcsthsetposition( PTRTYPE hit, double pos[3] )  ;
int lcsthsetmomentum( PTRTYPE hit, float pos[3] )  ;
int lcsthsetmomentumxyz( PTRTYPE hit, float px, float py, float pz ) ;
int lcsthsetpathlength( PTRTYPE hit, float pathLength ) ;
int lcsthsetdedx( PTRTYPE hit, float dEdX )  ;
int lcsthsettime( PTRTYPE hit, float t )  ;
int lcsthsetmcparticle( PTRTYPE hit,  PTRTYPE  particle )  ;

// now the fortran wrappers from cfortran.h
extern "C"{
  
FCALLSCFUN0(CFORTRANPNTR, lcsthcreate, LCSTHCREATE, lcsthcreate ) 
FCALLSCFUN1(INT, lcsthdelete, LCSTHDELETE, lcsthdelete, CFORTRANPNTR ) 

FCALLSCFUN1(INT, lcsthgetcellid,LCSTHGETCELLID,lcsthgetcellid,CFORTRANPNTR) 
FCALLSCFUN2(DOUBLE, lcsthgetposition, LCSTHGETPOSITION, lcsthgetposition, CFORTRANPNTR,  INT ) 
FCALLSCFUN2(FLOAT, lcsthgetmomentum, LCSTHGETMOMENTUM, lcsthgetmomentum, CFORTRANPNTR,  INT ) 
FCALLSCFUN1(FLOAT, lcsthgetpathlength,LCSTHGETPATHLENGTH,lcsthgetpathlength,CFORTRANPNTR)
FCALLSCFUN1(FLOAT, lcsthgetdedx,LCSTHGETDEDX,lcsthgetdedx,CFORTRANPNTR) 
FCALLSCFUN1(FLOAT, lcsthgettime,LCSTHGETTIME,lcsthgettime,CFORTRANPNTR) 

FCALLSCFUN1(CFORTRANPNTR,lcsthgetmcparticle,LCSTHGETMCPARTICLE,lcsthgetmcparticle,CFORTRANPNTR) 

FCALLSCFUN2(INT, lcsthsetcellid, LCSTHSETCELLID, lcsthsetcellid, CFORTRANPNTR, INT ) 
FCALLSCFUN2(INT, lcsthsetposition, LCSTHSETPOSITION, lcsthsetposition, CFORTRANPNTR, DOUBLEV ) 
FCALLSCFUN2(INT, lcsthsetmomentum, LCSTHSETMOMENTUM, lcsthsetmomentum, CFORTRANPNTR, FLOATV ) 
FCALLSCFUN4(INT, lcsthsetmomentumxyz, LCSTHSETMOMENTUMXYZ, lcsthsetmomentumxyz, CFORTRANPNTR,
            FLOAT, FLOAT, FLOAT )
FCALLSCFUN2(INT, lcsthsetpathlength, LCSTHSETPATHLENGTH, lcsthsetpathlength, CFORTRANPNTR, FLOAT )
FCALLSCFUN2(INT, lcsthsetdedx, LCSTHSETDEDX, lcsthsetdedx, CFORTRANPNTR, FLOAT ) 
FCALLSCFUN2(INT, lcsthsettime, LCSTHSETTIME, lcsthsettime, CFORTRANPNTR, FLOAT ) 
FCALLSCFUN2(INT, lcsthsetmcparticle, LCSTHSETMCPARTICLE, lcsthsetmcparticle, CFORTRANPNTR, CFORTRANPNTR ) 
}

