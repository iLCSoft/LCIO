/**Header file for the f77-wrapper functions of the ParticleID Class.
 * 
 * @author H. Vogt
 * @version May 10, 2004
 */
#include "cfortran.h"
#include "cpointer.h"

#include "deprecation.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

LCIO_DEPRECATED_CAPI PTRTYPE lcpidcreate() ;
LCIO_DEPRECATED_CAPI int     lcpiddelete( PTRTYPE pid ) ;

LCIO_DEPRECATED_CAPI int     lcpidid( PTRTYPE pid ) ;
LCIO_DEPRECATED_CAPI int     lcpidgettype( PTRTYPE pid ) ;
LCIO_DEPRECATED_CAPI int     lcpidgetpdg( PTRTYPE pid ) ;
LCIO_DEPRECATED_CAPI float   lcpidgetlikelihood( PTRTYPE pid ) ;

LCIO_DEPRECATED_CAPI int     lcpidgetalgorithmtype( PTRTYPE pid ) ;
LCIO_DEPRECATED_CAPI int     lcpidgetparameters( PTRTYPE pid, float* vec, int* nvec ) ;

LCIO_DEPRECATED_CAPI int     lcpidsettype( PTRTYPE pid, int type ) ;
LCIO_DEPRECATED_CAPI int     lcpidsetpdg( PTRTYPE pid, int pdg ) ;
LCIO_DEPRECATED_CAPI int     lcpidsetlikelihood( PTRTYPE pid, float logl ) ;

LCIO_DEPRECATED_CAPI int     lcpidsetalgorithmtype( PTRTYPE pid, int algo ) ;
LCIO_DEPRECATED_CAPI int     lcpidaddparameter( PTRTYPE pid, float param ) ;


// now the fortran wrappers from cfortran.h              
extern "C"{
  
FCALLSCFUN0(CFORTRANPNTR, lcpidcreate, LCPIDCREATE, lcpidcreate ) 
FCALLSCFUN1(INT, lcpiddelete, LCPIDDELETE, lcpiddelete, CFORTRANPNTR ) 

FCALLSCFUN1(INT, lcpidid, LCPIDID, lcpidid, CFORTRANPNTR) 
FCALLSCFUN1(INT, lcpidgettype, LCPIDGETTYPE, lcpidgettype, CFORTRANPNTR) 
FCALLSCFUN1(INT, lcpidgetpdg, LCPIDGETPDG, lcpidgetpdg, CFORTRANPNTR) 

FCALLSCFUN1(FLOAT, lcpidgetlikelihood, LCPIDGETLIKELIHOOD, lcpidgetlikelihood, CFORTRANPNTR) 


FCALLSCFUN1(INT, lcpidgetalgorithmtype, LCPIDGETALGORITHMTYPE, lcpidgetalgorithmtype, CFORTRANPNTR ) 
FCALLSCFUN3(INT, lcpidgetparameters, LCPIDGETPARAMETERS, lcpidgetparameters, CFORTRANPNTR, FLOATV, INTV) 

FCALLSCFUN2(INT, lcpidsettype, LCPIDSETTYPE, lcpidsettype, CFORTRANPNTR, INT) 
FCALLSCFUN2(INT, lcpidsetpdg, LCPIDSETPDG, lcpidsetpdg, CFORTRANPNTR, INT) 

FCALLSCFUN2(INT, lcpidsetlikelihood, LCPIDSETLIKELIHOOD, lcpidsetlikelihood, CFORTRANPNTR, FLOAT) 

FCALLSCFUN2(INT, lcpidsetalgorithmtype, LCPIDSETALGORITHMTYPE, lcpidsetalgorithmtype, CFORTRANPNTR, INT) 
FCALLSCFUN2(INT, lcpidaddparameter, LCPIDADDPARAMETER, lcpidaddparameter, CFORTRANPNTR, FLOAT) 

}
  
           
