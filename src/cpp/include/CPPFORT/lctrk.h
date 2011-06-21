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
// int     lctrktesttype( PTRTYPE trk , int bit ) ;

float   lctrkgetd0( PTRTYPE trk ) ;
float   lctrkgetphi( PTRTYPE trk ) ;
float   lctrkgetomega( PTRTYPE trk ) ;
float   lctrkgetz0( PTRTYPE trk ) ;
float   lctrkgettanlambda( PTRTYPE trk ) ;

int     lctrkgetcovmatrix( PTRTYPE trk, float* cvmtx ) ;
int     lctrkgetreferencepoint( PTRTYPE trk, float* refpoint ) ;
//int     lctrkisreferencepointpca( PTRTYPE trk ) ;
float   lctrkgetchi2( PTRTYPE trk ) ;
int     lctrkgetndf( PTRTYPE trk ) ;
float   lctrkgetdedx( PTRTYPE trk ) ;
float   lctrkgetdedxerror( PTRTYPE trk ) ;
float   lctrkgetradiusofinnermosthit(  PTRTYPE trk ) ;
int     lctrkgetsubdetectorhitnumbers( PTRTYPE trk, int* intv, int* nintv) ;
PTRTYPE lctrkgettracks( PTRTYPE trk ) ;
PTRTYPE lctrkgettrackerhits( PTRTYPE trk ) ;

int     lctrksettypebit( PTRTYPE trk, int index, int val ) ;
int     lctrksetomega( PTRTYPE trk, float omega ) ;
int     lctrksettanlambda( PTRTYPE trk, float tanlambda ) ;
int     lctrksetphi( PTRTYPE trk, float phi ) ;
int     lctrksetd0( PTRTYPE trk, float d0 ) ;
int     lctrksetz0( PTRTYPE trk, float z0 ) ;
int     lctrksetcovmatrix( PTRTYPE trk, float* cvmtx ) ;
int     lctrksetreferencepoint( PTRTYPE trk, float* refpoint ) ;
//int     lctrksetisreferencepointpca( PTRTYPE trk , int val) ;
int     lctrksetchi2( PTRTYPE trk, float chi2 ) ;
int     lctrksetndf( PTRTYPE trk, int ndf ) ;
int     lctrksetdedx( PTRTYPE trk, float dedx ) ;
int     lctrksetdedxerror( PTRTYPE trk, float dedxerr ) ;
int     lctrksetradiusofinnermosthit(  PTRTYPE trk , float r) ;
int     lctrkaddtrack( PTRTYPE trk, PTRTYPE track ) ;
int     lctrkaddhit( PTRTYPE trk, PTRTYPE hit ) ;

// fg: this method has no direct correspondence in the C++ API as there the vector is manipulated
// directly through it's interface via getSubdetectorHitNumbers
int     lctrksetsubdetectorhitnumbers( PTRTYPE trk, int* intv, const int nintv ) ;
                                                 

// now the fortran wrappers from cfortran.h              
extern "C"{
  
FCALLSCFUN0(CFORTRANPNTR, lctrkcreate, LCTRKCREATE, lctrkcreate ) 
FCALLSCFUN1(INT, lctrkdelete, LCTRKDELETE, lctrkdelete, CFORTRANPNTR ) 

FCALLSCFUN1(INT, lctrkid, LCTRKID, lctrkid, CFORTRANPNTR) 
FCALLSCFUN1(INT, lctrkgettype, LCTRKGETTYPE, lctrkgettype, CFORTRANPNTR) 
// FCALLSCFUN2(INT, lctrktesttype, LCTRKTESTTYPE,lctrktesttype, CFORTRANPNTR, INT ) 
FCALLSCFUN1(FLOAT, lctrkgetomega, LCTRKGETOMEGA, lctrkgetomega, CFORTRANPNTR) 
FCALLSCFUN1(FLOAT, lctrkgettanlambda, LCTRKGETTANLAMBDA, lctrkgettanlambda, CFORTRANPNTR) 
FCALLSCFUN1(FLOAT, lctrkgetphi, LCTRKGETPHI, lctrkgetphi, CFORTRANPNTR) 
FCALLSCFUN1(FLOAT, lctrkgetd0, LCTRKGETD0, lctrkgetd0, CFORTRANPNTR) 
FCALLSCFUN1(FLOAT, lctrkgetz0, LCTRKGETZ0, lctrkgetz0, CFORTRANPNTR) 
FCALLSCFUN2(INT, lctrkgetcovmatrix, LCTRKGETCOVMATRIX, lctrkgetcovmatrix, CFORTRANPNTR, FLOATV) 
FCALLSCFUN2(INT, lctrkgetreferencepoint, LCTRKGETREFERENCEPOINT, lctrkgetreferencepoint, CFORTRANPNTR, FLOATV) 
//FCALLSCFUN1(INT, lctrkisreferencepointpca, LCTRKISREFERENCEPOINTPCA, lctrkisreferencepointpca, CFORTRANPNTR) 
FCALLSCFUN1(FLOAT, lctrkgetchi2, LCTRKGETCHI2, lctrkgetchi2, CFORTRANPNTR) 
FCALLSCFUN1(INT, lctrkgetndf, LCTRKGETNDF, lctrkgetndf, CFORTRANPNTR) 
FCALLSCFUN1(FLOAT, lctrkgetdedx, LCTRKGETDEDX, lctrkgetdedx, CFORTRANPNTR) 
FCALLSCFUN1(FLOAT, lctrkgetdedxerror, LCTRKGETDEDXERROR, lctrkgetdedxerror, CFORTRANPNTR) 
FCALLSCFUN1(FLOAT, lctrkgetradiusofinnermosthit, LCTRKGETRADIUSOFINNERMOSTHIT, lctrkgetradiusofinnermosthit, CFORTRANPNTR) 
FCALLSCFUN3(INT, lctrkgetsubdetectorhitnumbers,  LCTRKGETSUBDETECTORHITNUMBERS, lctrkgetsubdetectorhitnumbers, CFORTRANPNTR, INTV, INTV) 
FCALLSCFUN1(CFORTRANPNTR, lctrkgettracks,  LCTRKGETTRACKS, lctrkgettracks, CFORTRANPNTR) 
FCALLSCFUN1(CFORTRANPNTR, lctrkgettrackerhits,  LCTRKGETTRACKERHITS, lctrkgettrackerhits, CFORTRANPNTR) 


FCALLSCFUN3(INT, lctrksettypebit, LCTRKSETTYPEBIT, lctrksettypebit, CFORTRANPNTR, INT, INT) 
FCALLSCFUN2(INT, lctrksetomega, LCTRKSETOMEGA, lctrksetomega, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lctrksettanlambda, LCTRKSETTANLAMBDA, lctrksettanlambda, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lctrksetphi, LCTRKSETPHI, lctrksetphi, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lctrksetd0, LCTRKSETD0, lctrksetd0, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lctrksetz0, LCTRKSETZ0, lctrksetz0, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lctrksetcovmatrix, LCTRKSETCOVMATRIX, lctrksetcovmatrix, CFORTRANPNTR, FLOATV) 
FCALLSCFUN2(INT, lctrksetreferencepoint, LCTRKSETREFERENCEPOINT, lctrksetreferencepoint, CFORTRANPNTR, FLOATV) 
//FCALLSCFUN2(INT,lctrksetisreferencepointpca, LCTRKSETISREFERENCEPOINTPCA, lctrksetisreferencepointpca, CFORTRANPNTR, INT) 
FCALLSCFUN2(INT, lctrksetchi2, LCTRKSETCHI2, lctrksetchi2, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lctrksetndf, LCTRKSETNDF, lctrksetndf, CFORTRANPNTR, INT) 
FCALLSCFUN2(INT, lctrksetdedx, LCTRKSETDEDX, lctrksetdedx, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lctrksetdedxerror, LCTRKSETDEDXERROR, lctrksetdedxerror, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lctrksetradiusofinnermosthit, LCTRKSETRADIUSOFINNERMOSTHIT, lctrksetradiusofinnermosthit, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lctrkaddtrack, LCTRKADDTRACK, lctrkaddtrack, CFORTRANPNTR, CFORTRANPNTR) 
FCALLSCFUN2(INT, lctrkaddhit, LCTRKADDHIT, lctrkaddhit, CFORTRANPNTR, CFORTRANPNTR) 

FCALLSCFUN3(INT, lctrksetsubdetectorhitnumbers, LCTRKSETSUBDETECTORHITNUMBERS, lctrksetsubdetectorhitnumbers, CFORTRANPNTR, INTV, INT ) 

}
  
           
