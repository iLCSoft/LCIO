/**Header file for the f77-wrapper functions of the Track Class.
 * 
 * @author H. Vogt
 * @version May 10, 2004
 */
#include "cfortran.h"
#include "cpointer.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

PTRTYPE lctrkcreate() ;
int     lctrkdelete( PTRTYPE trk ) ;

int     lctrkid( PTRTYPE trk ) ;
int     lctrkgettype( PTRTYPE trk ) ;
// float   lctrkgetmomentum( PTRTYPE trk ) ;
float   lctrkgetomega( PTRTYPE trk ) ;
// float   lctrkgettheta( PTRTYPE trk ) ;
float   lctrkgettanlambda( PTRTYPE trk ) ;
float   lctrkgetphi( PTRTYPE trk ) ;
float   lctrkgetd0( PTRTYPE trk ) ;
float   lctrkgetz0( PTRTYPE trk ) ;
int     lctrkgetcovmatrix( PTRTYPE trk, float* cvmtx ) ;
int     lctrkgetreferencepoint( PTRTYPE trk, float* refpoint ) ;
int     lctrkisreferencepointpca( PTRTYPE trk ) ;
float   lctrkgetchi2( PTRTYPE trk ) ;
int     lctrkgetndf( PTRTYPE trk ) ;
float   lctrkgetdedx( PTRTYPE trk ) ;
float   lctrkgetdedxerror( PTRTYPE trk ) ;
PTRTYPE lctrkgettracks( PTRTYPE trk ) ;
PTRTYPE lctrkgettrackerhits( PTRTYPE trk ) ;

// int     lctrksettype( PTRTYPE trk, int type ) ;
int     lctrksettypebit( PTRTYPE trk, int index ) ;
// int     lctrksetmomentum( PTRTYPE trk, float p ) ;
// int     lctrksettheta( PTRTYPE trk, float theta ) ;
int     lctrksetomega( PTRTYPE trk, float omega ) ;
int     lctrksettanlambda( PTRTYPE trk, float tanlambda ) ;
int     lctrksetphi( PTRTYPE trk, float phi ) ;
int     lctrksetd0( PTRTYPE trk, float d0 ) ;
int     lctrksetz0( PTRTYPE trk, float z0 ) ;
int     lctrksetcovmatrix( PTRTYPE trk, float* cvmtx ) ;
int     lctrksetreferencepoint( PTRTYPE trk, float* refpoint ) ;
int     lctrksetisreferencepointpca( PTRTYPE trk , int val) ;
int     lctrksetchi2( PTRTYPE trk, float chi2 ) ;
int     lctrksetndf( PTRTYPE trk, int ndf ) ;
int     lctrksetdedx( PTRTYPE trk, float dedx ) ;
int     lctrksetdedxerror( PTRTYPE trk, float dedxerr ) ;
int     lctrkaddtrack( PTRTYPE trk, PTRTYPE track ) ;
int     lctrkaddhit( PTRTYPE trk, PTRTYPE hit ) ;
                                                  

// now the fortran wrappers from cfortran.h              
extern "C"{
  
FCALLSCFUN0(CFORTRANPNTR, lctrkcreate, LCTRKCREATE, lctrkcreate ) ;
FCALLSCFUN1(INT, lctrkdelete, LCTRKDELETE, lctrkdelete, CFORTRANPNTR ) ;

FCALLSCFUN1(INT, lctrkid, LCTRKID, lctrkid, CFORTRANPNTR) ;
FCALLSCFUN1(INT, lctrkgettype, LCTRKGETTYPE, lctrkgettype, CFORTRANPNTR) ;
// FCALLSCFUN1(FLOAT, lctrkgetmomentum, LCTRKGETMOMENTUM, lctrkgetmomentum, CFORTRANPNTR) ;
FCALLSCFUN1(FLOAT, lctrkgetomega, LCTRKGETOMEGA, lctrkgetomega, CFORTRANPNTR) ;
FCALLSCFUN1(FLOAT, lctrkgettanlambda, LCTRKGETTANLAMBDA, lctrkgettanlambda, CFORTRANPNTR) ;
FCALLSCFUN1(FLOAT, lctrkgetphi, LCTRKGETPHI, lctrkgetphi, CFORTRANPNTR) ;
FCALLSCFUN1(FLOAT, lctrkgetd0, LCTRKGETD0, lctrkgetd0, CFORTRANPNTR) ;
FCALLSCFUN1(FLOAT, lctrkgetz0, LCTRKGETZ0, lctrkgetz0, CFORTRANPNTR) ;
FCALLSCFUN2(INT, lctrkgetcovmatrix, LCTRKGETCOVMATRIX, lctrkgetcovmatrix, CFORTRANPNTR, FLOATV) ;
FCALLSCFUN2(INT, lctrkgetreferencepoint, LCTRKGETREFERENCEPOINT, lctrkgetreferencepoint, CFORTRANPNTR, FLOATV) ;
FCALLSCFUN1(INT, lctrkisreferencepointpca, LCTRKISREFERENCEPOINTPCA, lctrkisreferencepointpca, CFORTRANPNTR) ;
FCALLSCFUN1(FLOAT, lctrkgetchi2, LCTRKGETCHI2, lctrkgetchi2, CFORTRANPNTR) ;
FCALLSCFUN1(INT, lctrkgetndf, LCTRKGETNDF, lctrkgetndf, CFORTRANPNTR) ;
FCALLSCFUN1(FLOAT, lctrkgetdedx, LCTRKGETDEDX, lctrkgetdedx, CFORTRANPNTR) ;
FCALLSCFUN1(FLOAT, lctrkgetdedxerror, LCTRKGETDEDXERROR, lctrkgetdedxerror, CFORTRANPNTR) ;
FCALLSCFUN1(CFORTRANPNTR, lctrkgettracks,  LCTRKGETTRACKS, lctrkgettracks, CFORTRANPNTR) ;
FCALLSCFUN1(CFORTRANPNTR, lctrkgettrackerhits,  LCTRKGETTRACKERHITS, lctrkgettrackerhits, CFORTRANPNTR) ;

// FCALLSCFUN2(INT, lctrksettype, LCTRKSETTYPE, lctrksettype, CFORTRANPNTR, INT) ;
FCALLSCFUN2(INT, lctrksettypebit, LCTRKSETTYPEBIT, lctrksettypebit, CFORTRANPNTR, INT) ;
FCALLSCFUN2(INT, lctrksetomega, LCTRKSETOMEGA, lctrksetomega, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN2(INT, lctrksettanlambda, LCTRKSETTANLAMBDA, lctrksettanlambda, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN2(INT, lctrksetphi, LCTRKSETPHI, lctrksetphi, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN2(INT, lctrksetd0, LCTRKSETD0, lctrksetd0, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN2(INT, lctrksetz0, LCTRKSETZ0, lctrksetz0, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN2(INT, lctrksetcovmatrix, LCTRKSETCOVMATRIX, lctrksetcovmatrix, CFORTRANPNTR, FLOATV) ;
FCALLSCFUN2(INT, lctrksetreferencepoint, LCTRKSETREFERENCEPOINT, lctrksetreferencepoint, CFORTRANPNTR, FLOATV) ;
FCALLSCFUN2(INT,lctrksetisreferencepointpca, LCTRKSETISREFERENCEPOINTPCA, lctrksetisreferencepointpca, CFORTRANPNTR, INT) ;
FCALLSCFUN2(INT, lctrksetchi2, LCTRKSETCHI2, lctrksetchi2, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN2(INT, lctrksetndf, LCTRKSETNDF, lctrksetndf, CFORTRANPNTR, INT) ;
FCALLSCFUN2(INT, lctrksetdedx, LCTRKSETDEDX, lctrksetdedx, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN2(INT, lctrksetdedxerror, LCTRKSETDEDXERROR, lctrksetdedxerror, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN2(INT, lctrkaddtrack, LCTRKADDTRACK, lctrkaddtrack, CFORTRANPNTR, CFORTRANPNTR) ;
FCALLSCFUN2(INT, lctrkaddhit, LCTRKADDHIT, lctrkaddhit, CFORTRANPNTR, CFORTRANPNTR) ;

}
  
           
