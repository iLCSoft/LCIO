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

int     lcrcpgettype( PTRTYPE rcp ) ;
bool    lcrcpisprimary( PTRTYPE rcp ) ;
int     lcrcpgetmomentum( PTRTYPE rcp, float* p ) ;
float   lcrcpgetenergy( PTRTYPE rcp ) ;
int     lcrcpgetcovmatrix( PTRTYPE rcp, float* cvmtx ) ;
float   lcrcpgetmass( PTRTYPE rcp ) ;
float   lcrcpgetcharge( PTRTYPE rcp ) ;
int     lcrcpgetreferencepoint( PTRTYPE rcp, float* refpoint ) ;
PTRTYPE lcrcpgetparticleids( PTRTYPE rcp ) ;
PTRTYPE lcrcpgetparticles( PTRTYPE rcp ) ;
int     lcrcpgetparticleweights( PTRTYPE rcp, float* weights, int* nweights ) ;
PTRTYPE lcrcpgetclusters( PTRTYPE rcp ) ;
int     lcrcpgetclusterweights( PTRTYPE rcp, float* weights, int* nweights ) ;
PTRTYPE lcrcpgettracks( PTRTYPE rcp ) ;
int     lcrcpgettrackweights( PTRTYPE rcp, float* weights, int* nweights ) ;
PTRTYPE lcrcpgetmcparticles( PTRTYPE rcp ) ;
int     lcrcpgetmcparticleweights( PTRTYPE rcp, float* weights, int* nweights ) ;

int     lcrcpsettype( PTRTYPE rcp, int type ) ;
int     lcrcpsetprimary( PTRTYPE rcp, bool lprimary ) ;
int     lcrcpsetmomentum( PTRTYPE rcp, float* p ) ;
int     lcrcpsetenergy( PTRTYPE rcp, float energy ) ;
int     lcrcpsetcovmatrix( PTRTYPE rcp, float* cvmtx ) ;
int     lcrcpsetmass( PTRTYPE rcp, float xmass) ;
int     lcrcpsetcharge( PTRTYPE rcp, float charge ) ;
int     lcrcpsetreferencepoint( PTRTYPE rcp, float* refpoint ) ;
int     lcrcpaddparticleid( PTRTYPE rcp, PTRTYPE id ) ;
int     lcrcpaddparticle( PTRTYPE rcp, PTRTYPE particle, float weigth ) ;
int     lcrcpaddcluster( PTRTYPE rcp, PTRTYPE clus, float weigth ) ;
int     lcrcpaddtrack( PTRTYPE rcp, PTRTYPE track, float weigth ) ;
int     lcrcpaddmcparticle( PTRTYPE rcp, PTRTYPE mcp, float weigth ) ;
                                                  

// now the fortran wrappers from cfortran.h
extern "C"{
  
FCALLSCFUN0(CFORTRANPNTR, lcrcpcreate, LCRCPCREATE, lcrcpcreate ) ;
FCALLSCFUN1(INT, lcrcpdelete, LCRCPDELETE, lcrcpdelete, CFORTRANPNTR ) ;

FCALLSCFUN1(INT, lcrcpgettype, LCRCPGETTYPE, lcrcpgettype, CFORTRANPNTR) ;
FCALLSCFUN1(LOGICAL, lcrcpisprimary, LCRCPISPRIMARY, lcrcpisprimary, CFORTRANPNTR) ;
FCALLSCFUN2(INT, lcrcpgetmomentum, LCRCPGETMOMENTUM, lcrcpgetmomentum, CFORTRANPNTR, FLOATV) ;
FCALLSCFUN1(FLOAT, lcrcpgetenergy, LCRCPGETENERGY, lcrcpgetenergy, CFORTRANPNTR) ;
FCALLSCFUN2(INT, lcrcpgetcovmatrix, LCRCPGETCOVMATRIX, lcrcpgetcovmatrix, CFORTRANPNTR, FLOATV) ;
FCALLSCFUN1(FLOAT, lcrcpgetmass, LCRCPGETMASS, lcrcpgetmass, CFORTRANPNTR) ;
FCALLSCFUN1(FLOAT, lcrcpgetcharge, LCRCPGETCHARGE, lcrcpgetcharge, CFORTRANPNTR) ;
FCALLSCFUN2(INT, lcrcpgetreferencepoint, LCRCPGETREFERENCEPOINT, lcrcpgetreferencepoint, CFORTRANPNTR, FLOATV) ;
FCALLSCFUN1(CFORTRANPNTR, lcrcpgetparticleids, LCRCPGETPARTICLEIDS, lcrcpgetparticleids, CFORTRANPNTR) ;
FCALLSCFUN1(CFORTRANPNTR, lcrcpgetparticles,  LCRCPGETPARTICLES, lcrcpgetparticles, CFORTRANPNTR) ;
FCALLSCFUN3(INT, lcrcpgetparticleweights, LCRCPGETPARTICLEWEIGHTS, lcrcpgetparticleweights, CFORTRANPNTR, FLOATV, INTV) ;
FCALLSCFUN1(CFORTRANPNTR, lcrcpgetclusters, LCRCPGETCLUSTERS, lcrcpgetclusters, CFORTRANPNTR) ;
FCALLSCFUN3(INT, lcrcpgetclusterweights, LCRCPGETCLUSTERWEIGHTS, lcrcpgetclusterweights, CFORTRANPNTR, FLOATV, INTV) ;
FCALLSCFUN1(CFORTRANPNTR, lcrcpgettracks, LCRCPGETTRACKS, lcrcpgettracks, CFORTRANPNTR) ;
FCALLSCFUN3(INT, lcrcpgettrackweights, LCRCPGETTRACKWEIGHTS, lcrcpgettrackweights, CFORTRANPNTR, FLOATV, INTV) ;
FCALLSCFUN1(CFORTRANPNTR, lcrcpgetmcparticles, LCRCPGETMCPARTICLES, lcrcpgetmcparticles, CFORTRANPNTR) ;
FCALLSCFUN3(INT, lcrcpgetmcparticleweights, LCRCPGETMCPARTICLEWEIGHTS, lcrcpgetmcparticleweights, 
            CFORTRANPNTR, FLOATV, INTV) ;

FCALLSCFUN2(INT, lcrcpsettype, LCRCPSETTYPE, lcrcpsettype, CFORTRANPNTR, INT) ;
FCALLSCFUN2(INT, lcrcpsetprimary, LCRCPSETPRIMARY, lcrcpsetprimary, CFORTRANPNTR, LOGICAL) ;
FCALLSCFUN2(INT, lcrcpsetmomentum, LCRCPSETMOMENTUM, lcrcpsetmomentum, CFORTRANPNTR, FLOATV) ;
FCALLSCFUN2(INT, lcrcpsetenergy, LCRCPSETENERGY, lcrcpsetenergy, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN2(INT, lcrcpsetcovmatrix, LCRCPSETCOVMATRIX, lcrcpsetcovmatrix, CFORTRANPNTR, FLOATV) ;
FCALLSCFUN2(INT, lcrcpsetmass, LCRCPSETMASS, lcrcpsetmass, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN2(INT, lcrcpsetcharge, LCRCPSETCHARGE, lcrcpsetcharge, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN2(INT, lcrcpsetreferencepoint, LCRCPSETREFERENCEPOINT, lcrcpsetreferencepoint, CFORTRANPNTR, FLOATV) ;
FCALLSCFUN2(INT, lcrcpaddparticleid, LCRCPADDPARTICLEID, lcrcpaddparticleid, CFORTRANPNTR, CFORTRANPNTR) ;
FCALLSCFUN3(INT, lcrcpaddparticle, LCRCPADDPARTICLE, lcrcpaddparticle, CFORTRANPNTR, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN3(INT, lcrcpaddcluster, LCRCPADDCLUSTER, lcrcpaddcluster, CFORTRANPNTR, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN3(INT, lcrcpaddtrack, LCRCPADDTRACK, lcrcpaddtrack, CFORTRANPNTR, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN3(INT, lcrcpaddmcparticle, LCRCPADDMCPARTICLE, lcrcpaddmcparticle, CFORTRANPNTR, CFORTRANPNTR, FLOAT) ;

}
  
           
