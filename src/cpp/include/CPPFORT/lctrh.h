/**Header file for the f77-wrapper functions of the Track Class.
 * 
 * @author H. Vogt
 * @version $Id: lctrh.h,v 1.3 2004-09-15 13:29:08 gaede Exp $
 */
#include "cfortran.h"
#include "cpointer.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

PTRTYPE lctrhcreate() ;
int     lctrhdelete( PTRTYPE trh ) ;

int     lctrhid( PTRTYPE trh ) ;
int     lctrhgetposition( PTRTYPE trh, double* pos ) ;
int     lctrhgetcovmatrix( PTRTYPE trh, float* cvmtx ) ;
float   lctrhgetdedx( PTRTYPE trh ) ;
float   lctrhgettime( PTRTYPE trh ) ;
//char*   lctrhgettype( PTRTYPE trh ) ;
int   lctrhgettype( PTRTYPE trh ) ;

PTRTYPE lctrhgetrawhits( PTRTYPE trh ) ;

int     lctrhsetposition( PTRTYPE trh, double* pos ) ;
int     lctrhsetcovmatrix( PTRTYPE trh, float* cvmtx ) ;
int     lctrhsetdedx( PTRTYPE trh, float dedx ) ;
int     lctrhsettime( PTRTYPE trh, float time ) ;
int     lctrhsettype( PTRTYPE trh, int type ) ;
int     lctrhsetrawhit( PTRTYPE trh, PTRTYPE hit ) ;


// now the fortran wrappers from cfortran.h              
extern "C"{
  
FCALLSCFUN0(CFORTRANPNTR, lctrhcreate, LCTRHCREATE, lctrhcreate ) ;
FCALLSCFUN1(INT, lctrhdelete, LCTRHDELETE, lctrhdelete, CFORTRANPNTR ) ;

FCALLSCFUN1(INT, lctrhid, LCTRHID, lctrhid, CFORTRANPNTR) ;
FCALLSCFUN2(INT, lctrhgetposition, LCTRHGETPOSITION, lctrhgetposition, CFORTRANPNTR, DOUBLEV ) ;
FCALLSCFUN2(INT, lctrhgetcovmatrix, LCTRHGETCOVMATRIX, lctrhgetcovmatrix, CFORTRANPNTR, FLOATV) ;
FCALLSCFUN1(FLOAT, lctrhgetdedx, LCTRHGETDEDX, lctrhgetdedx, CFORTRANPNTR) ;
FCALLSCFUN1(FLOAT, lctrhgettime, LCTRHGETTIME, lctrhgettime, CFORTRANPNTR) ;
  //FCALLSCFUN1(STRING, lctrhgettype, LCTRHGETTYPE, lctrhgettype, CFORTRANPNTR ) ;
FCALLSCFUN1(INT, lctrhgettype, LCTRHGETTYPE, lctrhgettype, CFORTRANPNTR ) ;
FCALLSCFUN1(CFORTRANPNTR, lctrhgetrawhits,  LCTRHGETRAWHITS, lctrhgetrawhits, CFORTRANPNTR) ;

FCALLSCFUN2(INT, lctrhsetposition, LCTRHSETPOSITION, lctrhsetposition, CFORTRANPNTR, DOUBLEV) ;
FCALLSCFUN2(INT, lctrhsetcovmatrix, LCTRHSETCOVMATRIX, lctrhsetcovmatrix, CFORTRANPNTR, FLOATV) ;
FCALLSCFUN2(INT, lctrhsetdedx, LCTRHSETDEDX, lctrhsetdedx, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN2(INT, lctrhsettime, LCTRHSETTIME, lctrhsettime, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN2(INT, lctrhsettype, LCTRHSETTYPE, lctrhsettype, CFORTRANPNTR, INT ) ;
FCALLSCFUN2(INT, lctrhsetrawhit, LCTRHSETRAWHIT, lctrhsetrawhit, CFORTRANPNTR, CFORTRANPNTR) ;

}
  
           
