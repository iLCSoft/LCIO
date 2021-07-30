/**Header file for the f77-wrapper functions of the Track Class.
 * 
 * @author H. Vogt
 * @version $Id: lctrh.h,v 1.10 2010-06-02 10:59:33 engels Exp $
 */
#include "cfortran.h"
#include "cpointer.h"

#include "deprecation.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

LCIO_DEPRECATED_CAPI PTRTYPE lctrhcreate() ;
LCIO_DEPRECATED_CAPI int     lctrhdelete( PTRTYPE trh ) ;

LCIO_DEPRECATED_CAPI int     lctrhid( PTRTYPE trh ) ;
LCIO_DEPRECATED_CAPI int     lctrhgetposition( PTRTYPE trh, double* pos ) ;
LCIO_DEPRECATED_CAPI int     lctrhgetcovmatrix( PTRTYPE trh, float* cvmtx ) ;
LCIO_DEPRECATED_CAPI float   lctrhgetdedx( PTRTYPE trh ) ;
LCIO_DEPRECATED_CAPI float   lctrhgetedep( PTRTYPE trh ) ;
LCIO_DEPRECATED_CAPI float   lctrhgetedeperr( PTRTYPE trh ) ;
LCIO_DEPRECATED_CAPI float   lctrhgettime( PTRTYPE trh ) ;
//char*   lctrhgettype( PTRTYPE trh ) ;
LCIO_DEPRECATED_CAPI int   lctrhgettype( PTRTYPE trh ) ;
LCIO_DEPRECATED_CAPI int     lctrhgetquality( PTRTYPE trh )  ;
LCIO_DEPRECATED_CAPI int lctrhgetcellid0( PTRTYPE trh )  ;
LCIO_DEPRECATED_CAPI int lctrhgetcellid1( PTRTYPE trh )  ;

LCIO_DEPRECATED_CAPI PTRTYPE lctrhgetrawhits( PTRTYPE trh ) ;

LCIO_DEPRECATED_CAPI int     lctrhsetposition( PTRTYPE trh, double* pos ) ;
LCIO_DEPRECATED_CAPI int     lctrhsetcovmatrix( PTRTYPE trh, float* cvmtx ) ;
LCIO_DEPRECATED_CAPI int     lctrhsetdedx( PTRTYPE trh, float dedx ) ;
LCIO_DEPRECATED_CAPI int     lctrhsetedep( PTRTYPE trh, float e ) ;
LCIO_DEPRECATED_CAPI int     lctrhsetedeperr( PTRTYPE trh, float e ) ;
LCIO_DEPRECATED_CAPI int     lctrhsettime( PTRTYPE trh, float time ) ;
LCIO_DEPRECATED_CAPI int     lctrhsettype( PTRTYPE trh, int type ) ;
LCIO_DEPRECATED_CAPI int     lctrhsetquality( PTRTYPE trh, int q );
LCIO_DEPRECATED_CAPI int     lctrhsetcellid0( PTRTYPE trh, int id0) ;
LCIO_DEPRECATED_CAPI int     lctrhsetcellid1( PTRTYPE trh, int id1) ;
LCIO_DEPRECATED_CAPI int     lctrhaddrawhit( PTRTYPE trh, PTRTYPE rawhit ) ;


// now the fortran wrappers from cfortran.h              
extern "C"{
  
FCALLSCFUN0(CFORTRANPNTR, lctrhcreate, LCTRHCREATE, lctrhcreate ) 
FCALLSCFUN1(INT, lctrhdelete, LCTRHDELETE, lctrhdelete, CFORTRANPNTR ) 

FCALLSCFUN1(INT, lctrhid, LCTRHID, lctrhid, CFORTRANPNTR) 
FCALLSCFUN2(INT, lctrhgetposition, LCTRHGETPOSITION, lctrhgetposition, CFORTRANPNTR, DOUBLEV ) 
FCALLSCFUN2(INT, lctrhgetcovmatrix, LCTRHGETCOVMATRIX, lctrhgetcovmatrix, CFORTRANPNTR, FLOATV) 
FCALLSCFUN1(FLOAT, lctrhgetdedx, LCTRHGETDEDX, lctrhgetdedx, CFORTRANPNTR)
FCALLSCFUN1(FLOAT, lctrhgetedep, LCTRHGETEDEP, lctrhgetedep, CFORTRANPNTR) 
FCALLSCFUN1(FLOAT, lctrhgetedeperr, LCTRHGETEDEPERR, lctrhgetedeperr, CFORTRANPNTR) 
FCALLSCFUN1(FLOAT, lctrhgettime, LCTRHGETTIME, lctrhgettime, CFORTRANPNTR) 
  //FCALLSCFUN1(STRING, lctrhgettype, LCTRHGETTYPE, lctrhgettype, CFORTRANPNTR ) 
FCALLSCFUN1(INT, lctrhgettype, LCTRHGETTYPE, lctrhgettype, CFORTRANPNTR ) 
FCALLSCFUN1(INT, lctrhgetquality,LCTRHGETQUALITY, lctrhgetquality,CFORTRANPNTR)
FCALLSCFUN1(INT, lctrhgetcellid0, LCTRHGETCELLID0, lctrhgetcellid0, CFORTRANPNTR )
FCALLSCFUN1(INT, lctrhgetcellid1, LCTRHGETCELLID1, lctrhgetcellid1, CFORTRANPNTR )
FCALLSCFUN1(CFORTRANPNTR, lctrhgetrawhits,  LCTRHGETRAWHITS, lctrhgetrawhits, CFORTRANPNTR) 

FCALLSCFUN2(INT, lctrhsetposition, LCTRHSETPOSITION, lctrhsetposition, CFORTRANPNTR, DOUBLEV) 
FCALLSCFUN2(INT, lctrhsetcovmatrix, LCTRHSETCOVMATRIX, lctrhsetcovmatrix, CFORTRANPNTR, FLOATV) 
FCALLSCFUN2(INT, lctrhsetdedx, LCTRHSETDEDX, lctrhsetdedx, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lctrhsetedep, LCTRHSETEDEP, lctrhsetedep, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lctrhsetedeperr, LCTRHSETEDEPERR, lctrhsetedeperr, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lctrhsettime, LCTRHSETTIME, lctrhsettime, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lctrhsettype, LCTRHSETTYPE, lctrhsettype, CFORTRANPNTR, INT )
FCALLSCFUN2(INT, lctrhsetquality, LCTRHSETQUALITY, lctrhsetquality, CFORTRANPNTR, INT )
FCALLSCFUN2(INT, lctrhsetcellid0,LCTRHSETCELLID0,lctrhsetcellid0, CFORTRANPNTR, INT )
FCALLSCFUN2(INT, lctrhsetcellid1,LCTRHSETCELLID1,lctrhsetcellid1, CFORTRANPNTR, INT )

FCALLSCFUN2(INT, lctrhaddrawhit, LCTRHADDRAWHIT, lctrhaddrawhit, CFORTRANPNTR, CFORTRANPNTR) 

}
  
           
