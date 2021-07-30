/**Header file for the f77-wrapper functions of the CalorimeterHitImpl class.
 * 
 * @author F. Gaede
 * @version Oct 11, 2003
 */

#include "cfortran.h"
#include "cpointer.h"

#include "deprecation.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux


LCIO_DEPRECATED_CAPI PTRTYPE lccahcreate() ;
LCIO_DEPRECATED_CAPI int lccahdelete( PTRTYPE calhit ) ;
LCIO_DEPRECATED_CAPI int lccahid( PTRTYPE calhit ) ;

LCIO_DEPRECATED_CAPI int lccahgetcellid0( PTRTYPE calhit )  ;
LCIO_DEPRECATED_CAPI int lccahgetcellid1( PTRTYPE calhit )  ;
LCIO_DEPRECATED_CAPI float lccahgetenergy( PTRTYPE calhit )  ;
LCIO_DEPRECATED_CAPI float lccahgetenergyerr( PTRTYPE calhit )  ;
LCIO_DEPRECATED_CAPI float lccahgettime( PTRTYPE calhit )  ;
LCIO_DEPRECATED_CAPI int lccahgetposition( PTRTYPE calhit, float * )  ;
LCIO_DEPRECATED_CAPI int   lccahgettype( PTRTYPE calhit ) ;
LCIO_DEPRECATED_CAPI PTRTYPE lccahgetrawhit( PTRTYPE calhit ) ;

LCIO_DEPRECATED_CAPI int lccahsetcellid0( PTRTYPE calhit, int id0) ;
LCIO_DEPRECATED_CAPI int lccahsetcellid1( PTRTYPE calhit, int id1) ;
LCIO_DEPRECATED_CAPI int lccahsetenergy( PTRTYPE calhit, float en) ;
LCIO_DEPRECATED_CAPI int lccahsetenergyerr( PTRTYPE calhit, float enerr) ;
LCIO_DEPRECATED_CAPI int lccahsettime( PTRTYPE calhit, float time) ;
LCIO_DEPRECATED_CAPI int lccahsetposition( PTRTYPE calhit, float pos[3])  ;
LCIO_DEPRECATED_CAPI int lccahsettype( PTRTYPE calhit, int type ) ;
LCIO_DEPRECATED_CAPI int lccahsetrawhit( PTRTYPE calhit, PTRTYPE rawHit ) ;

// now the fortran wrappers from cfortran.h
extern "C"{
FCALLSCFUN0(CFORTRANPNTR, lccahcreate,LCCAHCREATE, lccahcreate ) 
FCALLSCFUN1(INT, lccahdelete, LCCAHDELETE, lccahdelete, CFORTRANPNTR ) 
FCALLSCFUN1(INT, lccahid, LCCAHID, lccahid, CFORTRANPNTR ) 

FCALLSCFUN1(INT, lccahgetcellid0, LCCAHGETCELLID0, lccahgetcellid0, CFORTRANPNTR ) 
FCALLSCFUN1(INT, lccahgetcellid1, LCCAHGETCELLID1, lccahgetcellid1, CFORTRANPNTR ) 
FCALLSCFUN1(FLOAT, lccahgetenergy, LCCAHGETENERGY, lccahgetenergy, CFORTRANPNTR ) 
FCALLSCFUN1(FLOAT, lccahgetenergyerr, LCCAHGETENERGYERR, lccahgetenergyerr, CFORTRANPNTR ) 
FCALLSCFUN1(FLOAT, lccahgettime, LCCAHGETTIME, lccahgettime, CFORTRANPNTR )
FCALLSCFUN2(INT, lccahgetposition,LCCAHGETPOSITION,lccahgetposition, CFORTRANPNTR, FLOATV ) 
FCALLSCFUN1(INT, lccahgettype, LCCAHGETTYPE, lccahgettype, CFORTRANPNTR )

FCALLSCFUN1(CFORTRANPNTR, lccahgetrawhit, LCCAHGETRAWHIT, lccahgetrawhit, CFORTRANPNTR ) 

FCALLSCFUN2(INT, lccahsetcellid0,LCCAHSETCELLID0,lccahsetcellid0, CFORTRANPNTR, INT ) 
FCALLSCFUN2(INT, lccahsetcellid1,LCCAHSETCELLID1,lccahsetcellid1, CFORTRANPNTR, INT ) 
FCALLSCFUN2(INT, lccahsetenergy,LCCAHSETENERGY,lccahsetenergy, CFORTRANPNTR, FLOAT ) 
FCALLSCFUN2(INT, lccahsetenergyerr,LCCAHSETENERGYERR,lccahsetenergyerr, CFORTRANPNTR, FLOAT ) 
FCALLSCFUN2(INT, lccahsettime,LCCAHSETTIME,lccahsettime, CFORTRANPNTR, FLOAT ) 
FCALLSCFUN2(INT, lccahsetposition,LCCAHSETPOSITION,lccahsetposition, CFORTRANPNTR, FLOATV ) 
FCALLSCFUN2(INT, lccahsettype, LCCAHSETTYPE, lccahsettype, CFORTRANPNTR, INT ) 
FCALLSCFUN2(INT, lccahsetrawhit, LCCAHSETRAWHIT, lccahsetrawhit, CFORTRANPNTR, INT ) 
}

