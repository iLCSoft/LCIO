/**Header file for the f77-wrapper functions of the LCMCParticleImpl class.
 * 
 * @author F. Gaede
 * @version Oct 11, 2003
 */
#include "cfortran.h"
#include "cpointer.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

PTRTYPE lcmcpcreate() ;
int lcmcpdelete( PTRTYPE mcparticle ) ;
PTRTYPE lcmcpgetparent( PTRTYPE mcparticle ) ;
PTRTYPE lcmcpgetsecondparent( PTRTYPE mcparticle ) ;
PTRTYPE lcmcpgetdaughter( PTRTYPE mcparticle, int i ) ;
int lcmcpgetendpoint(  PTRTYPE mcparticle, double* ep) ;
int lcmcpgetnumberofdaughters( PTRTYPE mcparticle )  ;
int lcmcpgetpdg( PTRTYPE mcparticle )  ;
int lcmcpgethepevtstatus( PTRTYPE mcparticle )  ;
int lcmcpgetvertex( PTRTYPE mcparticle, double* vtx )  ;
int lcmcpgetmomentum( PTRTYPE mcparticle, float* p)  ;
float lcmcpgetmass( PTRTYPE mcparticle )  ;
float lcmcpgetcharge( PTRTYPE mcparticle )  ;
float lcmcpgetenergy( PTRTYPE mcparticle )  ;

int lcmcpsetparent(  PTRTYPE mcparticle, PTRTYPE parent ) ;
int lcmcpsetsecondparent(  PTRTYPE mcparticle, PTRTYPE parent ) ;
int lcmcpadddaughter(  PTRTYPE mcparticle, PTRTYPE daughter ) ;
int lcmcpsetpdg( PTRTYPE mcparticle, int pdg ) ;
int lcmcpsethepevtstatus( PTRTYPE mcparticle, int status ) ;
int lcmcpsetvertex( PTRTYPE mcparticle, double vtx[3] ) ;
int lcmcpsetendpoint( PTRTYPE mcparticle, double pnt[3] ) ;
int lcmcpsetmomentum( PTRTYPE mcparticle,  float p[3] );
int lcmcpsetmass( PTRTYPE mcparticle, float m) ;
int lcmcpsetcharge( PTRTYPE mcparticle, float c ) ;


// now the fortran wrappers from cfortran.h
extern "C"{
FCALLSCFUN0(CFORTRANPNTR, lcmcpcreate,LCMCPCREATE, lcmcpcreate ) ;
FCALLSCFUN1(INT, lcmcpdelete, LCMCPDELETE, lcmcpdelete, CFORTRANPNTR ) ;

FCALLSCFUN1(CFORTRANPNTR, lcmcpgetparent, LCMCPGETPARENT, lcmcpgetparent, CFORTRANPNTR ) ;
FCALLSCFUN1(CFORTRANPNTR, lcmcpgetsecondparent, LCMCPGETSECONDPARENT, lcmcpgetsecondparent, CFORTRANPNTR ) ;
FCALLSCFUN2(CFORTRANPNTR, lcmcpgetdaughter,lcmcpgetdaughter,lcmcpgetdaughter, CFORTRANPNTR, INT ) ;
FCALLSCFUN2(INT, lcmcpgetendpoint,LCMCPGETENDPOINT,lcmcpgetendpoint, CFORTRANPNTR, DOUBLEV ) ;
FCALLSCFUN1(INT, lcmcpgetnumberofdaughters, LCMCPGETNUMBEROFDAUGHTERS, lcmcpgetnumberofdaughters, CFORTRANPNTR ) ;
FCALLSCFUN1(INT, lcmcpgetpdg,LCMCPGETPDG, lcmcpgetpdg, CFORTRANPNTR ) ;
FCALLSCFUN1(INT, lcmcpgethepevtstatus, LCMCPGETHEPEVTSTATUS, lcmcpgethepevtstatus, CFORTRANPNTR ) ;
FCALLSCFUN2(INT, lcmcpgetvertex,LCMCPGETVERTEX,lcmcpgetvertex, CFORTRANPNTR, DOUBLEV ) ;
FCALLSCFUN2(INT, lcmcpgetmomentum,LCMCPGETMOMENTUM,lcmcpgetmomentum, CFORTRANPNTR, FLOATV ) ;
FCALLSCFUN1(FLOAT, lcmcpgetmass, LCMCPGETMASS, lcmcpgetmass, CFORTRANPNTR ) ;
FCALLSCFUN1(FLOAT, lcmcpgetcharge,LCMCPGETCHARGE,lcmcpgetcharge, CFORTRANPNTR ) ;
FCALLSCFUN1(FLOAT, lcmcpgetenergy, LCMCPGETENERGY, lcmcpgetenergy, CFORTRANPNTR ) ;

FCALLSCFUN2(INT, lcmcpsetparent,LCMCPSETPARENT,lcmcpsetparent, CFORTRANPNTR, CFORTRANPNTR ) ;
FCALLSCFUN2(INT, lcmcpsetsecondparent,LCMCPSETSECONDPARENT,lcmcpsetsecondparent, CFORTRANPNTR, CFORTRANPNTR ) ;
FCALLSCFUN2(INT, lcmcpadddaughter,LCMCPADDDAUGHTER,lcmcpadddaughter, CFORTRANPNTR, CFORTRANPNTR ) ;
FCALLSCFUN2(INT, lcmcpsetpdg,LCMCPSETPDG,lcmcpsetpdg, CFORTRANPNTR, INT ) ;
FCALLSCFUN2(INT, lcmcpsethepevtstatus,LCMCPSETHEPEVTSTATUS,lcmcpsethepevtstatus, CFORTRANPNTR, INT ) ;
FCALLSCFUN2(INT, lcmcpsetvertex,LCMCPSETVERTEX,lcmcpsetvertex, CFORTRANPNTR, DOUBLEV ) ;
FCALLSCFUN2(INT, lcmcpsetmomentum,LCMCPSETMOMENTUM,lcmcpsetmomentum, CFORTRANPNTR, FLOATV ) ;
FCALLSCFUN2(INT, lcmcpsetmass,LCMCPSETMASS,lcmcpsetmass, CFORTRANPNTR, FLOAT ) ;
FCALLSCFUN2(INT, lcmcpsetcharge, LCMCPSETCHARGE, lcmcpsetcharge, CFORTRANPNTR, FLOAT ) ;
}

