/**Header file for the f77-wrapper functions of the Track Class.
 * 
 * @author H. Vogt
 * @version May 10, 2004
 */
#include "cfortran.h"
#include "cpointer.h"

#include "deprecation.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

LCIO_DEPRECATED_CAPI PTRTYPE lctrkcreate() ;
LCIO_DEPRECATED_CAPI int     lctrkdelete( PTRTYPE trk ) ;

LCIO_DEPRECATED_CAPI int     lctrkid( PTRTYPE trk ) ;
LCIO_DEPRECATED_CAPI int     lctrkgettype( PTRTYPE trk ) ;
// int     lctrktesttype( PTRTYPE trk , int bit ) ;

LCIO_DEPRECATED_CAPI float   lctrkgetd0( PTRTYPE trk ) ;
LCIO_DEPRECATED_CAPI float   lctrkgetphi( PTRTYPE trk ) ;
LCIO_DEPRECATED_CAPI float   lctrkgetomega( PTRTYPE trk ) ;
LCIO_DEPRECATED_CAPI float   lctrkgetz0( PTRTYPE trk ) ;
LCIO_DEPRECATED_CAPI float   lctrkgettanlambda( PTRTYPE trk ) ;

LCIO_DEPRECATED_CAPI int     lctrkgetcovmatrix( PTRTYPE trk, float* cvmtx ) ;
LCIO_DEPRECATED_CAPI int     lctrkgetreferencepoint( PTRTYPE trk, float* refpoint ) ;
//int     lctrkisreferencepointpca( PTRTYPE trk ) ;
LCIO_DEPRECATED_CAPI float   lctrkgetchi2( PTRTYPE trk ) ;
LCIO_DEPRECATED_CAPI int     lctrkgetndf( PTRTYPE trk ) ;
LCIO_DEPRECATED_CAPI float   lctrkgetdedx( PTRTYPE trk ) ;
LCIO_DEPRECATED_CAPI float   lctrkgetdedxerror( PTRTYPE trk ) ;
LCIO_DEPRECATED_CAPI float   lctrkgetradiusofinnermosthit(  PTRTYPE trk ) ;
LCIO_DEPRECATED_CAPI int     lctrkgetsubdetectorhitnumbers( PTRTYPE trk, int* intv, int* nintv) ;
LCIO_DEPRECATED_CAPI PTRTYPE lctrkgettracks( PTRTYPE trk ) ;
LCIO_DEPRECATED_CAPI PTRTYPE lctrkgettrackerhits( PTRTYPE trk ) ;

LCIO_DEPRECATED_CAPI int     lctrksettypebit( PTRTYPE trk, int index, int val ) ;
LCIO_DEPRECATED_CAPI int     lctrksetomega( PTRTYPE trk, float omega ) ;
LCIO_DEPRECATED_CAPI int     lctrksettanlambda( PTRTYPE trk, float tanlambda ) ;
LCIO_DEPRECATED_CAPI int     lctrksetphi( PTRTYPE trk, float phi ) ;
LCIO_DEPRECATED_CAPI int     lctrksetd0( PTRTYPE trk, float d0 ) ;
LCIO_DEPRECATED_CAPI int     lctrksetz0( PTRTYPE trk, float z0 ) ;
LCIO_DEPRECATED_CAPI int     lctrksetcovmatrix( PTRTYPE trk, float* cvmtx ) ;
LCIO_DEPRECATED_CAPI int     lctrksetreferencepoint( PTRTYPE trk, float* refpoint ) ;
//int     lctrksetisreferencepointpca( PTRTYPE trk , int val) ;
LCIO_DEPRECATED_CAPI int     lctrksetchi2( PTRTYPE trk, float chi2 ) ;
LCIO_DEPRECATED_CAPI int     lctrksetndf( PTRTYPE trk, int ndf ) ;
LCIO_DEPRECATED_CAPI int     lctrksetdedx( PTRTYPE trk, float dedx ) ;
LCIO_DEPRECATED_CAPI int     lctrksetdedxerror( PTRTYPE trk, float dedxerr ) ;
LCIO_DEPRECATED_CAPI int     lctrksetradiusofinnermosthit(  PTRTYPE trk , float r) ;
LCIO_DEPRECATED_CAPI int     lctrkaddtrack( PTRTYPE trk, PTRTYPE track ) ;
LCIO_DEPRECATED_CAPI int     lctrkaddhit( PTRTYPE trk, PTRTYPE hit ) ;

// fg: this method has no direct correspondence in the C++ API as there the vector is manipulated
// directly through it's interface via getSubdetectorHitNumbers
LCIO_DEPRECATED_CAPI int     lctrksetsubdetectorhitnumbers( PTRTYPE trk, int* intv, const int nintv ) ;
                                                 

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
  
           
