/**Header file for the f77-wrapper functions of the Cluster Class.
 * 
 * @author H. Vogt
 * @version May 10, 2004
 */
#include "cfortran.h"
#include "cpointer.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

PTRTYPE lcclucreate() ;
int     lccludelete( PTRTYPE clu ) ;

int     lccluid( PTRTYPE clu ) ;
int     lcclugettype( PTRTYPE clu ) ;
// int     lcclutesttype( PTRTYPE clu , int bit ) ;

float   lcclugetenergy( PTRTYPE clu ) ;
int     lcclugetposition( PTRTYPE clu, float* pos ) ;
int     lcclugetpositionerror( PTRTYPE clu, float* poserr ) ;
float   lcclugetitheta( PTRTYPE clu ) ;
float   lcclugetiphi( PTRTYPE clu ) ;
int     lcclugetdirectionerror( PTRTYPE clu, float* direrr ) ;

// int     lcclugetshape( PTRTYPE clu, float* shape ) ;
// int     lcclugetparticletype( PTRTYPE clu, float* weights) ;

PTRTYPE lcclugetshape( PTRTYPE clu ) ;

PTRTYPE lcclugetparticleids( PTRTYPE clu ) ;
PTRTYPE lcclugetclusters( PTRTYPE clu ) ;
PTRTYPE lcclugetcalorimeterhits( PTRTYPE clu ) ;
PTRTYPE lcclugetsubdetectorenergies( PTRTYPE clu ) ;

int     lcclusettypebit( PTRTYPE clu, int index, int val ) ;
int     lcclusetenergy( PTRTYPE clu, float energy ) ;
int     lcclusetposition( PTRTYPE clu, float* refpoint ) ;
int     lcclusetpositionerror( PTRTYPE clu, float* poserr ) ;
int     lcclusetitheta( PTRTYPE clu, float theta ) ;
int     lcclusetiphi( PTRTYPE clu, float phi ) ;
int     lcclusetdirectionerror( PTRTYPE clu, float* direrr ) ;
int     lcclusetshape( PTRTYPE clu, PTRTYPE pshapevec ) ;
int     lccluaddparticleid( PTRTYPE clu, PTRTYPE pid ) ;
int     lccluaddcluster( PTRTYPE clu, PTRTYPE clus ) ;
int     lccluaddhit( PTRTYPE clu, PTRTYPE calohit, float weigth ) ;

// fg: these methods have no direct correspondence in the C++ API as there the vector is manipulated
// directly through the interface
int     lcclugethitcontributions( PTRTYPE clu, float* ener, int* nener ) ;
int     lcclusetsubdetectorenergies( PTRTYPE cluster, float* floatv, const int nfloatv ) ;

// now the fortran wrappers from cfortran.h
extern "C"{
  
FCALLSCFUN0(CFORTRANPNTR, lcclucreate, LCCLUCREATE, lcclucreate ) 
FCALLSCFUN1(INT, lccludelete, LCCLUDELETE, lccludelete, CFORTRANPNTR ) 

FCALLSCFUN1(INT, lccluid, LCCLUID, lccluid, CFORTRANPNTR) 
FCALLSCFUN1(INT, lcclugettype, LCCLUGETTYPE, lcclugettype, CFORTRANPNTR) 
// FCALLSCFUN2(INT, lcclutesttype, LCCLUTESTTYPE,lcclutesttype, CFORTRANPNTR, INT ) 
FCALLSCFUN1(FLOAT, lcclugetenergy, LCCLUGETENERGY, lcclugetenergy, CFORTRANPNTR)
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
  
