/**Header file for the f77-wrapper functions of the Cluster Class.
 * 
 * @author H. Vogt
 * @version May 10, 2004
 */
#include "cfortran.h"
#include "cpointer.h"

#include "deprecation.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

LCIO_DEPRECATED_CAPI PTRTYPE lcclucreate() ;
LCIO_DEPRECATED_CAPI int     lccludelete( PTRTYPE clu ) ;

LCIO_DEPRECATED_CAPI int     lccluid( PTRTYPE clu ) ;
LCIO_DEPRECATED_CAPI int     lcclugettype( PTRTYPE clu ) ;
// int     lcclutesttype( PTRTYPE clu , int bit ) ;

LCIO_DEPRECATED_CAPI float   lcclugetenergy( PTRTYPE clu ) ;
LCIO_DEPRECATED_CAPI float   lcclugetenergyerr( PTRTYPE clu ) ;
LCIO_DEPRECATED_CAPI int     lcclugetposition( PTRTYPE clu, float* pos ) ;
LCIO_DEPRECATED_CAPI int     lcclugetpositionerror( PTRTYPE clu, float* poserr ) ;
LCIO_DEPRECATED_CAPI float   lcclugetitheta( PTRTYPE clu ) ;
LCIO_DEPRECATED_CAPI float   lcclugetiphi( PTRTYPE clu ) ;
LCIO_DEPRECATED_CAPI int     lcclugetdirectionerror( PTRTYPE clu, float* direrr ) ;

// int     lcclugetshape( PTRTYPE clu, float* shape ) ;
// int     lcclugetparticletype( PTRTYPE clu, float* weights) ;

LCIO_DEPRECATED_CAPI PTRTYPE lcclugetshape( PTRTYPE clu ) ;

LCIO_DEPRECATED_CAPI PTRTYPE lcclugetparticleids( PTRTYPE clu ) ;
LCIO_DEPRECATED_CAPI PTRTYPE lcclugetclusters( PTRTYPE clu ) ;
LCIO_DEPRECATED_CAPI PTRTYPE lcclugetcalorimeterhits( PTRTYPE clu ) ;
LCIO_DEPRECATED_CAPI PTRTYPE lcclugetsubdetectorenergies( PTRTYPE clu ) ;

LCIO_DEPRECATED_CAPI int     lcclusettypebit( PTRTYPE clu, int index, int val ) ;
LCIO_DEPRECATED_CAPI int     lcclusetenergy( PTRTYPE clu, float energy ) ;
LCIO_DEPRECATED_CAPI int     lcclusetenergyerr( PTRTYPE clu, float enerr ) ;
LCIO_DEPRECATED_CAPI int     lcclusetposition( PTRTYPE clu, float* refpoint ) ;
LCIO_DEPRECATED_CAPI int     lcclusetpositionerror( PTRTYPE clu, float* poserr ) ;
LCIO_DEPRECATED_CAPI int     lcclusetitheta( PTRTYPE clu, float theta ) ;
LCIO_DEPRECATED_CAPI int     lcclusetiphi( PTRTYPE clu, float phi ) ;
LCIO_DEPRECATED_CAPI int     lcclusetdirectionerror( PTRTYPE clu, float* direrr ) ;
LCIO_DEPRECATED_CAPI int     lcclusetshape( PTRTYPE clu, PTRTYPE pshapevec ) ;
LCIO_DEPRECATED_CAPI int     lccluaddparticleid( PTRTYPE clu, PTRTYPE pid ) ;
LCIO_DEPRECATED_CAPI int     lccluaddcluster( PTRTYPE clu, PTRTYPE clus ) ;
LCIO_DEPRECATED_CAPI int     lccluaddhit( PTRTYPE clu, PTRTYPE calohit, float weigth ) ;

// fg: these methods have no direct correspondence in the C++ API as there the vector is manipulated
// directly through the interface
LCIO_DEPRECATED_CAPI int     lcclugethitcontributions( PTRTYPE clu, float* ener, int* nener ) ;
LCIO_DEPRECATED_CAPI int     lcclusetsubdetectorenergies( PTRTYPE cluster, float* floatv, const int nfloatv ) ;

// now the fortran wrappers from cfortran.h
extern "C"{
  
FCALLSCFUN0(CFORTRANPNTR, lcclucreate, LCCLUCREATE, lcclucreate ) 
FCALLSCFUN1(INT, lccludelete, LCCLUDELETE, lccludelete, CFORTRANPNTR ) 

FCALLSCFUN1(INT, lccluid, LCCLUID, lccluid, CFORTRANPNTR) 
FCALLSCFUN1(INT, lcclugettype, LCCLUGETTYPE, lcclugettype, CFORTRANPNTR) 
// FCALLSCFUN2(INT, lcclutesttype, LCCLUTESTTYPE,lcclutesttype, CFORTRANPNTR, INT ) 
FCALLSCFUN1(FLOAT, lcclugetenergy, LCCLUGETENERGY, lcclugetenergy, CFORTRANPNTR)
FCALLSCFUN1(FLOAT, lcclugetenergyerr, LCCLUGETENERGYERR, lcclugetenergyerr, CFORTRANPNTR)
FCALLSCFUN2(INT, lcclugetposition, LCCLUGETPOSITION, lcclugetposition, CFORTRANPNTR, FLOATV) 
FCALLSCFUN2(INT, lcclugetpositionerror, LCCLUGETPOSITIONERROR, lcclugetpositionerror, CFORTRANPNTR, FLOATV) 
FCALLSCFUN1(FLOAT, lcclugetitheta, LCCLUGETITHETA, lcclugetitheta, CFORTRANPNTR) 
FCALLSCFUN1(FLOAT, lcclugetiphi, LCCLUGETIPHI, lcclugetiphi, CFORTRANPNTR) 
FCALLSCFUN2(INT, lcclugetdirectionerror, LCCLUGETDIRECTIONERROR, lcclugetdirectionerror, CFORTRANPNTR, FLOATV) 
FCALLSCFUN1(CFORTRANPNTR, lcclugetshape, LCCLUGETSHAPE, lcclugetshape, CFORTRANPNTR ) 
FCALLSCFUN1(CFORTRANPNTR, lcclugetparticleids, LCCLUGETPARTICLEIDS, lcclugetparticleids, CFORTRANPNTR ) 

FCALLSCFUN1(CFORTRANPNTR, lcclugetclusters, LCCLUGETCLUSTERS, lcclugetclusters, CFORTRANPNTR) 
FCALLSCFUN1(CFORTRANPNTR, lcclugetcalorimeterhits, LCCLUGETCALORIMETERHITS, lcclugetcalorimeterhits, CFORTRANPNTR) 
FCALLSCFUN1(CFORTRANPNTR, lcclugetsubdetectorenergies, LCCLUGETSUBDETECTORENERGIES, lcclugetsubdetectorenergies, CFORTRANPNTR) 
FCALLSCFUN3(INT, lcclugethitcontributions, LCCLUGETHITCONTRIBUTIONS, lcclugethitcontributions, CFORTRANPNTR, FLOATV, INTV) 

FCALLSCFUN3(INT, lcclusettypebit, LCCLUSETTYPEBIT, lcclusettypebit, CFORTRANPNTR, INT, INT) 
FCALLSCFUN2(INT, lcclusetenergy, LCCLUSETENERGY, lcclusetenergy, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lcclusetenergyerr, LCCLUSETENERGYERR, lcclusetenergyerr, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lcclusetposition, LCCLUSETPOSITION, lcclusetposition, CFORTRANPNTR, FLOATV) 
FCALLSCFUN2(INT, lcclusetpositionerror, LCCLUSETPOSITIONERROR, lcclusetpositionerror, CFORTRANPNTR, FLOATV) 
FCALLSCFUN2(INT, lcclusetitheta, LCCLUSETITHETA, lcclusetitheta, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lcclusetiphi, LCCLUSETIPHI, lcclusetiphi, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lcclusetdirectionerror, LCCLUSETDIRECTIONERROR, lcclusetdirectionerror, CFORTRANPNTR, FLOATV) 
FCALLSCFUN2(INT, lcclusetshape, LCCLUSETSHAPE, lcclusetshape, CFORTRANPNTR, CFORTRANPNTR ) 
// FCALLSCFUN2(INT, lcclusetemweight, LCCLUSETEMWEIGHT, lcclusetemweight, CFORTRANPNTR, FLOAT) 
// FCALLSCFUN2(INT, lcclusethadweight, LCCLUSETHADWEIGHT, lcclusethadweight, CFORTRANPNTR, FLOAT) 
// FCALLSCFUN2(INT, lcclusetmuonweight, LCCLUSETMUONWEIGHT, lcclusetmuonweight, CFORTRANPNTR, FLOAT) 

FCALLSCFUN2(INT, lccluaddparticleid, LCCLUADDPARTICLEID, lccluaddparticleid, CFORTRANPNTR, CFORTRANPNTR) 
FCALLSCFUN2(INT, lccluaddcluster, LCCLUADDCLUSTER, lccluaddcluster, CFORTRANPNTR, CFORTRANPNTR) 
FCALLSCFUN3(INT, lccluaddhit, LCCLUADDHIT, lccluaddhit, CFORTRANPNTR, CFORTRANPNTR, FLOAT) 
FCALLSCFUN3(INT, lcclusetsubdetectorenergies, LCCLUSETSUBDETECTORENERGIES, lcclusetsubdetectorenergies, CFORTRANPNTR, FLOATV, INT ) 

}
  
