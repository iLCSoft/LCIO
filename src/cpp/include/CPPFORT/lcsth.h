/**Header file for the f77-wrapper functions of the SimTrackerHitImpl class.
 * 
 * @author F. Gaede
 * @version Oct 10, 2003
 */
#include "cfortran.h"
#include "cpointer.h"

#include "deprecation.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

LCIO_DEPRECATED_CAPI PTRTYPE lcsthcreate() ;
LCIO_DEPRECATED_CAPI int lcsthdelete( PTRTYPE hit ) ;

LCIO_DEPRECATED_CAPI int lcsthgetcellid( PTRTYPE hit )  ;
LCIO_DEPRECATED_CAPI int lcsthgetcellid0( PTRTYPE hit )  ;
LCIO_DEPRECATED_CAPI int lcsthgetcellid1( PTRTYPE hit )  ;
LCIO_DEPRECATED_CAPI double lcsthgetposition( PTRTYPE hit, int index )  ;
LCIO_DEPRECATED_CAPI float lcsthgetmomentum( PTRTYPE hit, int index )  ;
LCIO_DEPRECATED_CAPI float lcsthgetpathlength ( PTRTYPE hit )  ;
LCIO_DEPRECATED_CAPI float lcsthgetdedx( PTRTYPE hit )  ;
LCIO_DEPRECATED_CAPI float lcsthgetedep( PTRTYPE hit ) ;
LCIO_DEPRECATED_CAPI float lcsthgettime( PTRTYPE hit )  ;
LCIO_DEPRECATED_CAPI int lcsthgetquality( PTRTYPE hit ) ;

LCIO_DEPRECATED_CAPI PTRTYPE lcsthgetmcparticle( PTRTYPE hit )  ;

LCIO_DEPRECATED_CAPI int lcsthsetcellid0( PTRTYPE hit, int id ) ;
LCIO_DEPRECATED_CAPI int lcsthsetcellid1( PTRTYPE hit, int id ) ;
LCIO_DEPRECATED_CAPI int lcsthsetposition( PTRTYPE hit, double pos[3] )  ;
LCIO_DEPRECATED_CAPI int lcsthsetmomentum( PTRTYPE hit, float pos[3] )  ;
LCIO_DEPRECATED_CAPI int lcsthsetmomentumxyz( PTRTYPE hit, float px, float py, float pz ) ;
LCIO_DEPRECATED_CAPI int lcsthsetpathlength( PTRTYPE hit, float pathLength ) ;
LCIO_DEPRECATED_CAPI int lcsthsetdedx( PTRTYPE hit, float dEdX )  ;
LCIO_DEPRECATED_CAPI int lcsthsetedep( PTRTYPE hit, float e ) ;
LCIO_DEPRECATED_CAPI int lcsthsettime( PTRTYPE hit, float t )  ;
LCIO_DEPRECATED_CAPI int lcsthsetmcparticle( PTRTYPE hit,  PTRTYPE  particle )  ;
LCIO_DEPRECATED_CAPI int lcsthsetquality( PTRTYPE hit, int quality ) ;

// now the fortran wrappers from cfortran.h
extern "C"{
  
FCALLSCFUN0(CFORTRANPNTR, lcsthcreate, LCSTHCREATE, lcsthcreate ) 
FCALLSCFUN1(INT, lcsthdelete, LCSTHDELETE, lcsthdelete, CFORTRANPNTR ) 

FCALLSCFUN1(INT, lcsthgetcellid,LCSTHGETCELLID,lcsthgetcellid,CFORTRANPNTR) 
FCALLSCFUN1(INT, lcsthgetcellid0,LCSTHGETCELLID0,lcsthgetcellid0,CFORTRANPNTR) 
FCALLSCFUN1(INT, lcsthgetcellid1,LCSTHGETCELLID1,lcsthgetcellid1,CFORTRANPNTR) 
FCALLSCFUN2(DOUBLE, lcsthgetposition, LCSTHGETPOSITION, lcsthgetposition, CFORTRANPNTR,  INT ) 
FCALLSCFUN2(FLOAT, lcsthgetmomentum, LCSTHGETMOMENTUM, lcsthgetmomentum, CFORTRANPNTR,  INT ) 
FCALLSCFUN1(FLOAT, lcsthgetpathlength,LCSTHGETPATHLENGTH,lcsthgetpathlength,CFORTRANPNTR)
FCALLSCFUN1(FLOAT, lcsthgetdedx,LCSTHGETDEDX,lcsthgetdedx,CFORTRANPNTR) 
FCALLSCFUN1(FLOAT, lcsthgetedep,LCSTHGETEDEP,lcsthgetedep,CFORTRANPNTR) 
FCALLSCFUN1(FLOAT, lcsthgettime,LCSTHGETTIME,lcsthgettime,CFORTRANPNTR) 
FCALLSCFUN1(INT, lcsthgetquality,LCSTHGETQUALITY,lcsthgetquality,CFORTRANPNTR)

FCALLSCFUN1(CFORTRANPNTR,lcsthgetmcparticle,LCSTHGETMCPARTICLE,lcsthgetmcparticle,CFORTRANPNTR) 

FCALLSCFUN2(INT, lcsthsetcellid0, LCSTHSETCELLID0, lcsthsetcellid0, CFORTRANPNTR, INT )
FCALLSCFUN2(INT, lcsthsetcellid1, LCSTHSETCELLID1, lcsthsetcellid1, CFORTRANPNTR, INT ) 
FCALLSCFUN2(INT, lcsthsetposition, LCSTHSETPOSITION, lcsthsetposition, CFORTRANPNTR, DOUBLEV ) 
FCALLSCFUN2(INT, lcsthsetmomentum, LCSTHSETMOMENTUM, lcsthsetmomentum, CFORTRANPNTR, FLOATV ) 
FCALLSCFUN4(INT, lcsthsetmomentumxyz, LCSTHSETMOMENTUMXYZ, lcsthsetmomentumxyz, CFORTRANPNTR,
            FLOAT, FLOAT, FLOAT )
FCALLSCFUN2(INT, lcsthsetpathlength, LCSTHSETPATHLENGTH, lcsthsetpathlength, CFORTRANPNTR, FLOAT )
FCALLSCFUN2(INT, lcsthsetdedx, LCSTHSETDEDX, lcsthsetdedx, CFORTRANPNTR, FLOAT ) 
FCALLSCFUN2(INT, lcsthsetedep, LCSTHSETEDEP, lcsthsetedep, CFORTRANPNTR, FLOAT ) 
FCALLSCFUN2(INT, lcsthsettime, LCSTHSETTIME, lcsthsettime, CFORTRANPNTR, FLOAT ) 
FCALLSCFUN2(INT, lcsthsetmcparticle, LCSTHSETMCPARTICLE, lcsthsetmcparticle, CFORTRANPNTR, CFORTRANPNTR ) 
FCALLSCFUN2(INT, lcsthsetquality, LCSTHSETQUALITY, lcsthsetquality, CFORTRANPNTR, INT )
}

