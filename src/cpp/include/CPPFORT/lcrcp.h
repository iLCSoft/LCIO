/**Header file for the f77-wrapper functions of the ReconstructedParticle Class.
 * 
 * @author H. Vogt
 * @version May 10, 2004
 */
#include "cfortran.h"
#include "cpointer.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

PTRTYPE lcrcpcreate() ;
int     lcrcpdelete( PTRTYPE rcp ) ;

int     lcrcpid( PTRTYPE rcp ) ;
int     lcrcpgettype( PTRTYPE rcp ) ;
bool    lcrcpiscompound( PTRTYPE rcp ) ;
int     lcrcpgetmomentum( PTRTYPE rcp, float* p ) ;
float   lcrcpgetenergy( PTRTYPE rcp ) ;
int     lcrcpgetcovmatrix( PTRTYPE rcp, float* cvmtx ) ;
float   lcrcpgetmass( PTRTYPE rcp ) ;
float   lcrcpgetcharge( PTRTYPE rcp ) ;
int     lcrcpgetreferencepoint( PTRTYPE rcp, float* refpoint ) ;
PTRTYPE lcrcpgetparticleids( PTRTYPE rcp ) ;
float   lcrcpgetgoodnessofpid( PTRTYPE pid ) ;
PTRTYPE lcrcpgetparticles( PTRTYPE rcp ) ;
// int     lcrcpgetparticleweights( PTRTYPE rcp, float* weights, int* nweights ) ;
PTRTYPE lcrcpgetclusters( PTRTYPE rcp ) ;
// int     lcrcpgetclusterweights( PTRTYPE rcp, float* weights, int* nweights ) ;
PTRTYPE lcrcpgettracks( PTRTYPE rcp ) ;
// int     lcrcpgettrackweights( PTRTYPE rcp, float* weights, int* nweights ) ;
// PTRTYPE lcrcpgetmcparticles( PTRTYPE rcp ) ;
// int     lcrcpgetmcparticleweights( PTRTYPE rcp, float* weights, int* nweights ) ;
PTRTYPE lcrcpgetstartvertex( PTRTYPE rcp ) ;
PTRTYPE lcrcpgetendvertex( PTRTYPE rcp ) ;


int     lcrcpsettype( PTRTYPE rcp, int type ) ;
int     lcrcpsetcompound( PTRTYPE rcp, bool lcompound ) ;
int     lcrcpsetmomentum( PTRTYPE rcp, float* p ) ;
int     lcrcpsetenergy( PTRTYPE rcp, float energy ) ;
int     lcrcpsetcovmatrix( PTRTYPE rcp, float* cvmtx ) ;
int     lcrcpsetmass( PTRTYPE rcp, float xmass) ;
int     lcrcpsetcharge( PTRTYPE rcp, float charge ) ;
int     lcrcpsetreferencepoint( PTRTYPE rcp, float* refpoint ) ;
int     lcrcpaddparticleid( PTRTYPE rcp, PTRTYPE id ) ;
int     lcrcpsetgoodnessofpid( PTRTYPE pid, float good ) ;
int     lcrcpaddparticle( PTRTYPE rcp, PTRTYPE particle ) ;
int     lcrcpaddcluster( PTRTYPE rcp, PTRTYPE clus ) ;
int     lcrcpaddtrack( PTRTYPE rcp, PTRTYPE track) ;
// int     lcrcpaddmcparticle( PTRTYPE rcp, PTRTYPE mcp, float weigth ) ;
int     lcrcpsetstartvertex( PTRTYPE rcp, PTRTYPE vtx ) ;
                                                  

// now the fortran wrappers from cfortran.h
extern "C"{
  
FCALLSCFUN0(CFORTRANPNTR, lcrcpcreate, LCRCPCREATE, lcrcpcreate ) 
FCALLSCFUN1(INT, lcrcpdelete, LCRCPDELETE, lcrcpdelete, CFORTRANPNTR ) 

FCALLSCFUN1(INT, lcrcpid, LCRCPID, lcrcpid, CFORTRANPNTR) 
FCALLSCFUN1(INT, lcrcpgettype, LCRCPGETTYPE, lcrcpgettype, CFORTRANPNTR) 
FCALLSCFUN1(LOGICAL, lcrcpiscompound, LCRCPISCOMPOUND, lcrcpiscompound, CFORTRANPNTR) 
FCALLSCFUN2(INT, lcrcpgetmomentum, LCRCPGETMOMENTUM, lcrcpgetmomentum, CFORTRANPNTR, FLOATV) 
FCALLSCFUN1(FLOAT, lcrcpgetenergy, LCRCPGETENERGY, lcrcpgetenergy, CFORTRANPNTR) 
FCALLSCFUN2(INT, lcrcpgetcovmatrix, LCRCPGETCOVMATRIX, lcrcpgetcovmatrix, CFORTRANPNTR, FLOATV) 
FCALLSCFUN1(FLOAT, lcrcpgetmass, LCRCPGETMASS, lcrcpgetmass, CFORTRANPNTR) 
FCALLSCFUN1(FLOAT, lcrcpgetcharge, LCRCPGETCHARGE, lcrcpgetcharge, CFORTRANPNTR) 
FCALLSCFUN2(INT, lcrcpgetreferencepoint, LCRCPGETREFERENCEPOINT, lcrcpgetreferencepoint, CFORTRANPNTR, FLOATV) 
FCALLSCFUN1(CFORTRANPNTR, lcrcpgetparticleids, LCRCPGETPARTICLEIDS, lcrcpgetparticleids, CFORTRANPNTR) 
FCALLSCFUN1(FLOAT, lcrcpgetgoodnessofpid, LCRCPGETGOODNESSOFPID, lcrcpgetgoodnessofpid, CFORTRANPNTR) 
FCALLSCFUN1(CFORTRANPNTR, lcrcpgetparticles,  LCRCPGETPARTICLES, lcrcpgetparticles, CFORTRANPNTR) 
// FCALLSCFUN3(INT, lcrcpgetparticleweights, LCRCPGETPARTICLEWEIGHTS, lcrcpgetparticleweights, CFORTRANPNTR, FLOATV, INTV) 
FCALLSCFUN1(CFORTRANPNTR, lcrcpgetclusters, LCRCPGETCLUSTERS, lcrcpgetclusters, CFORTRANPNTR) 
// FCALLSCFUN3(INT, lcrcpgetclusterweights, LCRCPGETCLUSTERWEIGHTS, lcrcpgetclusterweights, CFORTRANPNTR, FLOATV, INTV) 
FCALLSCFUN1(CFORTRANPNTR, lcrcpgettracks, LCRCPGETTRACKS, lcrcpgettracks, CFORTRANPNTR) 
// FCALLSCFUN3(INT, lcrcpgettrackweights, LCRCPGETTRACKWEIGHTS, lcrcpgettrackweights, CFORTRANPNTR, FLOATV, INTV) 
// FCALLSCFUN1(CFORTRANPNTR, lcrcpgetmcparticles, LCRCPGETMCPARTICLES, lcrcpgetmcparticles, CFORTRANPNTR) 
// FCALLSCFUN3(INT, lcrcpgetmcparticleweights, LCRCPGETMCPARTICLEWEIGHTS, lcrcpgetmcparticleweights, 
//             CFORTRANPNTR, FLOATV, INTV) 
FCALLSCFUN1(CFORTRANPNTR, lcrcpgetstartvertex,  LCRCPGETSTARTVERTEX, lcrcpgetstartvertex, CFORTRANPNTR)
FCALLSCFUN1(CFORTRANPNTR, lcrcpgetendvertex,  LCRCPGETENDVERTEX, lcrcpgetendvertex, CFORTRANPNTR)


FCALLSCFUN2(INT, lcrcpsettype, LCRCPSETTYPE, lcrcpsettype, CFORTRANPNTR, INT) 
// FCALLSCFUN2(INT, lcrcpsetcompound, LCRCPSETCOMPOUND, lcrcpsetcompound, CFORTRANPNTR, LOGICAL) 
FCALLSCFUN2(INT, lcrcpsetmomentum, LCRCPSETMOMENTUM, lcrcpsetmomentum, CFORTRANPNTR, FLOATV) 
FCALLSCFUN2(INT, lcrcpsetenergy, LCRCPSETENERGY, lcrcpsetenergy, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lcrcpsetcovmatrix, LCRCPSETCOVMATRIX, lcrcpsetcovmatrix, CFORTRANPNTR, FLOATV) 
FCALLSCFUN2(INT, lcrcpsetmass, LCRCPSETMASS, lcrcpsetmass, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lcrcpsetcharge, LCRCPSETCHARGE, lcrcpsetcharge, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lcrcpsetreferencepoint, LCRCPSETREFERENCEPOINT, lcrcpsetreferencepoint, CFORTRANPNTR, FLOATV) 
FCALLSCFUN2(INT, lcrcpaddparticleid, LCRCPADDPARTICLEID, lcrcpaddparticleid, CFORTRANPNTR, CFORTRANPNTR) 
FCALLSCFUN2(INT, lcrcpsetgoodnessofpid, LCRCPSETGOODNESSOFPID, lcrcpsetgoodnessofpid, CFORTRANPNTR, FLOAT) 

FCALLSCFUN2(INT, lcrcpaddparticle, LCRCPADDPARTICLE, lcrcpaddparticle, CFORTRANPNTR, CFORTRANPNTR ) 
FCALLSCFUN2(INT, lcrcpaddcluster, LCRCPADDCLUSTER, lcrcpaddcluster, CFORTRANPNTR, CFORTRANPNTR) 
FCALLSCFUN2(INT, lcrcpaddtrack, LCRCPADDTRACK, lcrcpaddtrack, CFORTRANPNTR, CFORTRANPNTR) 
// FCALLSCFUN3(INT, lcrcpaddmcparticle, LCRCPADDMCPARTICLE, lcrcpaddmcparticle, CFORTRANPNTR, CFORTRANPNTR, FLOAT) 
FCALLSCFUN2(INT, lcrcpsetstartvertex, LCRCPSETSTARTVERTEX, lcrcpsetstartvertex, CFORTRANPNTR, CFORTRANPNTR )
}
  
           
