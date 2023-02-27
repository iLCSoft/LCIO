/**Header file for the f77-wrapper functions of the Vertex Class.
 * 
 * @author engels
 * @version Aug 28, 2006
 */
#include "cfortran.h"
#include "cpointer.h"

#include "deprecation.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

LCIO_DEPRECATED_CAPI PTRTYPE lcvtxcreate() ;
LCIO_DEPRECATED_CAPI int     lcvtxdelete( PTRTYPE vtx ) ;

LCIO_DEPRECATED_CAPI int     lcvtxid( PTRTYPE vtx ) ;
LCIO_DEPRECATED_CAPI bool    lcvtxisprimary( PTRTYPE vtx ) ;
LCIO_DEPRECATED_CAPI char*   lcvtxgetalgorithmtype( PTRTYPE vtx ) ;
LCIO_DEPRECATED_CAPI float   lcvtxgetchi2( PTRTYPE vtx ) ;
LCIO_DEPRECATED_CAPI float   lcvtxgetprobability( PTRTYPE vtx ) ;
LCIO_DEPRECATED_CAPI int     lcvtxgetposition( PTRTYPE vtx, float* pos ) ;
LCIO_DEPRECATED_CAPI int     lcvtxgetcovmatrix( PTRTYPE vtx, float* cvmtx ) ;
LCIO_DEPRECATED_CAPI int     lcvtxgetparameters( PTRTYPE vtx, float* vec, int* nvec ) ;
LCIO_DEPRECATED_CAPI PTRTYPE lcvtxgetassociatedparticle( PTRTYPE vtx ) ;

LCIO_DEPRECATED_CAPI int     lcvtxsetprimary( PTRTYPE vtx, bool pri ) ;
LCIO_DEPRECATED_CAPI int     lcvtxsetalgorithmtype( PTRTYPE vtx, char* type ) ;
LCIO_DEPRECATED_CAPI int     lcvtxsetchi2( PTRTYPE vtx, float chi2 ) ;
LCIO_DEPRECATED_CAPI int     lcvtxsetprobability( PTRTYPE vtx, float prob ) ;
LCIO_DEPRECATED_CAPI int     lcvtxsetposition( PTRTYPE vtx, float* pos ) ;
LCIO_DEPRECATED_CAPI int     lcvtxsetcovmatrix( PTRTYPE vtx, float* cvmtx ) ;
LCIO_DEPRECATED_CAPI int     lcvtxaddparameter( PTRTYPE vtx, float param ) ;
LCIO_DEPRECATED_CAPI int     lcvtxsetassociatedparticle( PTRTYPE vtx, PTRTYPE rcp ) ;

// now the fortran wrappers from cfortran.h
extern "C"{
  
FCALLSCFUN0(CFORTRANPNTR, lcvtxcreate, LCVTXCREATE, lcvtxcreate ) 
FCALLSCFUN1(INT, lcvtxdelete, LCVTXDELETE, lcvtxdelete, CFORTRANPNTR ) 

FCALLSCFUN1(INT, lcvtxid, LCVTXID, lcvtxid, CFORTRANPNTR) 
FCALLSCFUN1(LOGICAL, lcvtxisprimary, LCVTXISPRIMARY, lcvtxisprimary, CFORTRANPNTR) 
FCALLSCFUN1(STRING, lcvtxgetalgorithmtype, LCVTXGETALGORITHMTYPE, lcvtxgetalgorithmtype, CFORTRANPNTR) 
FCALLSCFUN1(FLOAT, lcvtxgetchi2, LCVTXGETCHI2, lcvtxgetchi2, CFORTRANPNTR) 
FCALLSCFUN1(FLOAT, lcvtxgetprobability, LCVTXGETPROBABILITY, lcvtxgetprobability, CFORTRANPNTR) 
FCALLSCFUN2(INT, lcvtxgetposition, LCVTXGETPOSITION, lcvtxgetposition, CFORTRANPNTR, FLOATV) 
FCALLSCFUN2(INT, lcvtxgetcovmatrix, LCVTXGETCOVMATRIX, lcvtxgetcovmatrix, CFORTRANPNTR, FLOATV) 
FCALLSCFUN3(INT, lcvtxgetparameters, LCVTXGETPARAMETERS, lcvtxgetparameters, CFORTRANPNTR, FLOATV, INTV)
FCALLSCFUN1(CFORTRANPNTR, lcvtxgetassociatedparticle,  LCVTXGETASSOCIATEDPARTICLE, lcvtxgetassociatedparticle, CFORTRANPNTR) 

FCALLSCFUN2(INT, lcvtxsetprimary, LCVTXSETPRIMARY, lcvtxsetprimary, CFORTRANPNTR, LOGICAL) 
FCALLSCFUN2(INT, lcvtxsetalgorithmtype, LCVTXSETALGORITHMTYPE, lcvtxsetalgorithmtype, CFORTRANPNTR, STRING)
FCALLSCFUN2(INT, lcvtxsetchi2, LCVTXSETCHI2, lcvtxsetchi2, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lcvtxsetprobability, LCVTXSETPROBABILITY, lcvtxsetprobability, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lcvtxsetposition, LCVTXSETPOSITION, lcvtxsetposition, CFORTRANPNTR, FLOATV) 
FCALLSCFUN2(INT, lcvtxsetcovmatrix, LCVTXSETCOVMATRIX, lcvtxsetcovmatrix, CFORTRANPNTR, FLOATV) 
FCALLSCFUN2(INT, lcvtxaddparameter, LCVTXADDPARAMETER, lcvtxaddparameter, CFORTRANPNTR, FLOAT)
FCALLSCFUN2(INT, lcvtxsetassociatedparticle, LCVTXSETASSOCIATEDPARTICLE, lcvtxsetassociatedparticle, CFORTRANPNTR, CFORTRANPNTR ) 

}
  
           
