/**Header file for the f77-wrapper functions of the ReconstructedParticle Class.
 * 
 * @author H. Vogt
 * @version May 10, 2004
 */
#include "cfortran.h"
#include "cpointer.h"

#include "deprecation.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

LCIO_DEPRECATED_CAPI PTRTYPE lcrcpcreate() ;
LCIO_DEPRECATED_CAPI int     lcrcpdelete( PTRTYPE rcp ) ;

LCIO_DEPRECATED_CAPI int     lcrcpid( PTRTYPE rcp ) ;
LCIO_DEPRECATED_CAPI int     lcrcpgettype( PTRTYPE rcp ) ;
LCIO_DEPRECATED_CAPI bool    lcrcpiscompound( PTRTYPE rcp ) ;
LCIO_DEPRECATED_CAPI int     lcrcpgetmomentum( PTRTYPE rcp, float* p ) ;
LCIO_DEPRECATED_CAPI float   lcrcpgetenergy( PTRTYPE rcp ) ;
LCIO_DEPRECATED_CAPI int     lcrcpgetcovmatrix( PTRTYPE rcp, float* cvmtx ) ;
LCIO_DEPRECATED_CAPI float   lcrcpgetmass( PTRTYPE rcp ) ;
LCIO_DEPRECATED_CAPI float   lcrcpgetcharge( PTRTYPE rcp ) ;
LCIO_DEPRECATED_CAPI int     lcrcpgetreferencepoint( PTRTYPE rcp, float* refpoint ) ;
LCIO_DEPRECATED_CAPI PTRTYPE lcrcpgetparticleids( PTRTYPE rcp ) ;
LCIO_DEPRECATED_CAPI float   lcrcpgetgoodnessofpid( PTRTYPE pid ) ;
LCIO_DEPRECATED_CAPI PTRTYPE lcrcpgetparticles( PTRTYPE rcp ) ;
// int     lcrcpgetparticleweights( PTRTYPE rcp, float* weights, int* nweights ) ;
LCIO_DEPRECATED_CAPI PTRTYPE lcrcpgetclusters( PTRTYPE rcp ) ;
// int     lcrcpgetclusterweights( PTRTYPE rcp, float* weights, int* nweights ) ;
LCIO_DEPRECATED_CAPI PTRTYPE lcrcpgettracks( PTRTYPE rcp ) ;
// int     lcrcpgettrackweights( PTRTYPE rcp, float* weights, int* nweights ) ;
// PTRTYPE lcrcpgetmcparticles( PTRTYPE rcp ) ;
// int     lcrcpgetmcparticleweights( PTRTYPE rcp, float* weights, int* nweights ) ;
LCIO_DEPRECATED_CAPI PTRTYPE lcrcpgetstartvertex( PTRTYPE rcp ) ;
LCIO_DEPRECATED_CAPI PTRTYPE lcrcpgetendvertex( PTRTYPE rcp ) ;


LCIO_DEPRECATED_CAPI int     lcrcpsettype( PTRTYPE rcp, int type ) ;
LCIO_DEPRECATED_CAPI int     lcrcpsetcompound( PTRTYPE rcp, bool lcompound ) ;
LCIO_DEPRECATED_CAPI int     lcrcpsetmomentum( PTRTYPE rcp, float* p ) ;
LCIO_DEPRECATED_CAPI int     lcrcpsetenergy( PTRTYPE rcp, float energy ) ;
LCIO_DEPRECATED_CAPI int     lcrcpsetcovmatrix( PTRTYPE rcp, float* cvmtx ) ;
LCIO_DEPRECATED_CAPI int     lcrcpsetmass( PTRTYPE rcp, float xmass) ;
LCIO_DEPRECATED_CAPI int     lcrcpsetcharge( PTRTYPE rcp, float charge ) ;
LCIO_DEPRECATED_CAPI int     lcrcpsetreferencepoint( PTRTYPE rcp, float* refpoint ) ;
LCIO_DEPRECATED_CAPI int     lcrcpaddparticleid( PTRTYPE rcp, PTRTYPE id ) ;
LCIO_DEPRECATED_CAPI int     lcrcpsetgoodnessofpid( PTRTYPE pid, float good ) ;
LCIO_DEPRECATED_CAPI int     lcrcpaddparticle( PTRTYPE rcp, PTRTYPE particle ) ;
LCIO_DEPRECATED_CAPI int     lcrcpaddcluster( PTRTYPE rcp, PTRTYPE clus ) ;
LCIO_DEPRECATED_CAPI int     lcrcpaddtrack( PTRTYPE rcp, PTRTYPE track) ;
// int     lcrcpaddmcparticle( PTRTYPE rcp, PTRTYPE mcp, float weigth ) ;
LCIO_DEPRECATED_CAPI int     lcrcpsetstartvertex( PTRTYPE rcp, PTRTYPE vtx ) ;
                                                  

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
  
           
