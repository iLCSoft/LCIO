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
int lcmcpgetnumberofparents( PTRTYPE mcparticle )  ;
PTRTYPE lcmcpgetparent( PTRTYPE mcparticle , int i ) ;
// PTRTYPE lcmcpgetsecondparent( PTRTYPE mcparticle ) ;
PTRTYPE lcmcpgetdaughter( PTRTYPE mcparticle, int i ) ;
int lcmcpgetendpoint(  PTRTYPE mcparticle, double* ep) ;
int lcmcpgetnumberofdaughters( PTRTYPE mcparticle )  ;
int lcmcpgetpdg( PTRTYPE mcparticle )  ;
int lcmcpgetgeneratorstatus( PTRTYPE mcparticle )  ;
int lcmcpgetsimulatorstatus( PTRTYPE mcparticle )  ;
int lcmcpgetvertex( PTRTYPE mcparticle, double* vtx )  ;
float lcmcpgettime( PTRTYPE mcparticle )  ;
int lcmcpgetmomentum( PTRTYPE mcparticle, double* p)  ;
double lcmcpgetmass( PTRTYPE mcparticle )  ;
float lcmcpgetcharge( PTRTYPE mcparticle )  ;
double lcmcpgetenergy( PTRTYPE mcparticle )  ;
int lcmcpgetspin( PTRTYPE mcparticle, float* spin ) ;
int lcmcpgetcolorflow( PTRTYPE mcparticle, int* cflow ) ;

int lcmcpaddparent(  PTRTYPE mcparticle, PTRTYPE parent ) ;
// int lcmcpsetsecondparent(  PTRTYPE mcparticle, PTRTYPE parent ) ;
// int lcmcpadddaughter(  PTRTYPE mcparticle, PTRTYPE daughter ) ;
int lcmcpsetpdg( PTRTYPE mcparticle, int pdg ) ;
int lcmcpsetgeneratorstatus( PTRTYPE mcparticle, int status ) ;
int lcmcpsetsimulatorstatus( PTRTYPE mcparticle, int status ) ;
int lcmcpsetvertex( PTRTYPE mcparticle, double vtx[3] ) ;
int lcmcpsetendpoint( PTRTYPE mcparticle, double pnt[3] ) ;
int lcmcpsetmomentum( PTRTYPE mcparticle,  float p[3] );
int lcmcpsetmass( PTRTYPE mcparticle, float m) ;
int lcmcpsetcharge( PTRTYPE mcparticle, float c ) ;
int lcmcpsetspin( PTRTYPE mcparticle, float spin[3] ) ;
int lcmcpsetcolorflow( PTRTYPE mcparticle, int cflow[2] ) ;

// now the fortran wrappers from cfortran.h
extern "C"{
FCALLSCFUN0(CFORTRANPNTR, lcmcpcreate,LCMCPCREATE, lcmcpcreate ) 
FCALLSCFUN1(INT, lcmcpdelete, LCMCPDELETE, lcmcpdelete, CFORTRANPNTR ) 

FCALLSCFUN1(INT, lcmcpgetnumberofparents, LCMCPGETNUMBEROFPARENTS, lcmcpgetnumberofparents, CFORTRANPNTR ) 
FCALLSCFUN2(CFORTRANPNTR, lcmcpgetparent, LCMCPGETPARENT, lcmcpgetparent, CFORTRANPNTR , INT) 
// FCALLSCFUN1(CFORTRANPNTR, lcmcpgetsecondparent, LCMCPGETSECONDPARENT, lcmcpgetsecondparent, CFORTRANPNTR ) 
FCALLSCFUN2(CFORTRANPNTR, lcmcpgetdaughter,lcmcpgetdaughter,lcmcpgetdaughter, CFORTRANPNTR, INT ) 
FCALLSCFUN2(INT, lcmcpgetendpoint,LCMCPGETENDPOINT,lcmcpgetendpoint, CFORTRANPNTR, DOUBLEV ) 
FCALLSCFUN1(INT, lcmcpgetnumberofdaughters, LCMCPGETNUMBEROFDAUGHTERS, lcmcpgetnumberofdaughters, CFORTRANPNTR ) 
FCALLSCFUN1(INT, lcmcpgetpdg,LCMCPGETPDG, lcmcpgetpdg, CFORTRANPNTR ) 
FCALLSCFUN1(INT, lcmcpgetgeneratorstatus, LCMCPGETGENERATORSTATUS, lcmcpgetgeneratorstatus, CFORTRANPNTR ) 
FCALLSCFUN1(INT, lcmcpgetsimulatorstatus, LCMCPGETSIMULATORSTATUS, lcmcpgetsimulatorstatus, CFORTRANPNTR ) 
FCALLSCFUN2(INT, lcmcpgetvertex,LCMCPGETVERTEX,lcmcpgetvertex, CFORTRANPNTR, DOUBLEV ) 
FCALLSCFUN1(FLOAT, lcmcpgettime, LCMCPGETTIME, lcmcpgettime, CFORTRANPNTR ) 
FCALLSCFUN2(INT, lcmcpgetmomentum,LCMCPGETMOMENTUM,lcmcpgetmomentum, CFORTRANPNTR, DOUBLEV )
FCALLSCFUN1(DOUBLE, lcmcpgetmass, LCMCPGETMASS, lcmcpgetmass, CFORTRANPNTR )
FCALLSCFUN1(FLOAT, lcmcpgetcharge,LCMCPGETCHARGE,lcmcpgetcharge, CFORTRANPNTR ) 
FCALLSCFUN1(DOUBLE, lcmcpgetenergy, LCMCPGETENERGY, lcmcpgetenergy, CFORTRANPNTR )
FCALLSCFUN2(INT, lcmcpgetspin, LCMCPGETSPIN, lcmcpgetspin, CFORTRANPNTR, FLOATV)
FCALLSCFUN2(INT, lcmcpgetcolorflow, LCMCPGETCOLORFLOW, lcmcpgetcolorflow, CFORTRANPNTR, INTV)


FCALLSCFUN2(INT, lcmcpaddparent,LCMCPADDPARENT,lcmcpaddparent, CFORTRANPNTR, CFORTRANPNTR ) 
// FCALLSCFUN2(INT, lcmcpsetsecondparent,LCMCPSETSECONDPARENT,lcmcpsetsecondparent, CFORTRANPNTR, CFORTRANPNTR ) 
// FCALLSCFUN2(INT, lcmcpadddaughter,LCMCPADDDAUGHTER,lcmcpadddaughter, CFORTRANPNTR, CFORTRANPNTR ) 
FCALLSCFUN2(INT, lcmcpsetpdg,LCMCPSETPDG,lcmcpsetpdg, CFORTRANPNTR, INT ) 
FCALLSCFUN2(INT, lcmcpsetgeneratorstatus,LCMCPSETGENERATORSTATUS,lcmcpsetgeneratorstatus, CFORTRANPNTR, INT ) 
FCALLSCFUN2(INT, lcmcpsetsimulatorstatus,LCMCPSETSIMULATORSTATUS,lcmcpsetsimulatorstatus, CFORTRANPNTR, INT ) 
FCALLSCFUN2(INT, lcmcpsetvertex,LCMCPSETVERTEX,lcmcpsetvertex, CFORTRANPNTR, DOUBLEV ) 
FCALLSCFUN2(INT, lcmcpsetendpoint,LCMCPSETENDPOINT,lcmcpsetendpoint, CFORTRANPNTR, DOUBLEV ) 
FCALLSCFUN2(INT, lcmcpsetmomentum,LCMCPSETMOMENTUM,lcmcpsetmomentum, CFORTRANPNTR, FLOATV ) 
FCALLSCFUN2(INT, lcmcpsetmass,LCMCPSETMASS,lcmcpsetmass, CFORTRANPNTR, FLOAT ) 
FCALLSCFUN2(INT, lcmcpsetcharge, LCMCPSETCHARGE, lcmcpsetcharge, CFORTRANPNTR, FLOAT ) 
FCALLSCFUN2(INT, lcmcpsetspin, LCMCPSETSPIN, lcmcpsetspin, CFORTRANPNTR, FLOATV)
FCALLSCFUN2(INT, lcmcpsetcolorflow, LCMCPSETCOLORFLOW, lcmcpsetcolorflow, CFORTRANPNTR, INTV)
}

