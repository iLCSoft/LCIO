/**Header file for the f77-wrapper functions of the Vertex Class.
 * 
 * @author engels
 * @version Aug 28, 2006
 */
#include "cfortran.h"
#include "cpointer.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

PTRTYPE lcvtxcreate() ;
int     lcvtxdelete( PTRTYPE vtx ) ;

int     lcvtxid( PTRTYPE vtx ) ;
bool    lcvtxisprimary( PTRTYPE vtx ) ;
char*   lcvtxgetalgorithmtype( PTRTYPE vtx ) ;
float   lcvtxgetchi2( PTRTYPE vtx ) ;
float   lcvtxgetprobability( PTRTYPE vtx ) ;
int     lcvtxgetposition( PTRTYPE vtx, float* pos ) ;
int     lcvtxgetcovmatrix( PTRTYPE vtx, float* cvmtx ) ;
int     lcvtxgetparameters( PTRTYPE vtx, float* vec, int* nvec ) ;
PTRTYPE lcvtxgetassociatedparticle( PTRTYPE vtx ) ;

int     lcvtxsetprimary( PTRTYPE vtx, bool pri ) ;
int     lcvtxsetalgorithmtype( PTRTYPE vtx, char* type ) ;
int     lcvtxsetchi2( PTRTYPE vtx, float chi2 ) ;
int     lcvtxsetprobability( PTRTYPE vtx, float prob ) ;
int     lcvtxsetposition( PTRTYPE vtx, float* pos ) ;
int     lcvtxsetcovmatrix( PTRTYPE vtx, float* cvmtx ) ;
int     lcvtxaddparameter( PTRTYPE vtx, float param ) ;
int     lcvtxsetassociatedparticle( PTRTYPE vtx, PTRTYPE rcp ) ;


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
  
           
