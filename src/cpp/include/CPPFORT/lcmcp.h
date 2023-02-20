/**Header file for the f77-wrapper functions of the LCMCParticleImpl class.
 * 
 * @author F. Gaede
 * @version Oct 11, 2003
 */
#include "cfortran.h"
#include "cpointer.h"

#include "deprecation.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

LCIO_DEPRECATED_CAPI PTRTYPE lcmcpcreate() ;
LCIO_DEPRECATED_CAPI int lcmcpdelete( PTRTYPE mcparticle ) ;
LCIO_DEPRECATED_CAPI int lcmcpgetnumberofparents( PTRTYPE mcparticle )  ;
LCIO_DEPRECATED_CAPI PTRTYPE lcmcpgetparent( PTRTYPE mcparticle , int i ) ;
// PTRTYPE lcmcpgetsecondparent( PTRTYPE mcparticle ) ;
LCIO_DEPRECATED_CAPI PTRTYPE lcmcpgetdaughter( PTRTYPE mcparticle, int i ) ;
LCIO_DEPRECATED_CAPI int lcmcpgetendpoint(  PTRTYPE mcparticle, double* ep) ;
LCIO_DEPRECATED_CAPI int lcmcpgetnumberofdaughters( PTRTYPE mcparticle )  ;
LCIO_DEPRECATED_CAPI int lcmcpgetpdg( PTRTYPE mcparticle )  ;
LCIO_DEPRECATED_CAPI int lcmcpgetgeneratorstatus( PTRTYPE mcparticle )  ;
LCIO_DEPRECATED_CAPI int lcmcpgetsimulatorstatus( PTRTYPE mcparticle )  ;
LCIO_DEPRECATED_CAPI int lcmcpgetvertex( PTRTYPE mcparticle, double* vtx )  ;
LCIO_DEPRECATED_CAPI float lcmcpgettime( PTRTYPE mcparticle )  ;
LCIO_DEPRECATED_CAPI int lcmcpgetmomentum( PTRTYPE mcparticle, double* p)  ;
LCIO_DEPRECATED_CAPI double lcmcpgetmass( PTRTYPE mcparticle )  ;
LCIO_DEPRECATED_CAPI float lcmcpgetcharge( PTRTYPE mcparticle )  ;
LCIO_DEPRECATED_CAPI double lcmcpgetenergy( PTRTYPE mcparticle )  ;
LCIO_DEPRECATED_CAPI int lcmcpgetspin( PTRTYPE mcparticle, float* spin ) ;
LCIO_DEPRECATED_CAPI int lcmcpgetcolorflow( PTRTYPE mcparticle, int* cflow ) ;

LCIO_DEPRECATED_CAPI int lcmcpaddparent(  PTRTYPE mcparticle, PTRTYPE parent ) ;
// int lcmcpsetsecondparent(  PTRTYPE mcparticle, PTRTYPE parent ) ;
// int lcmcpadddaughter(  PTRTYPE mcparticle, PTRTYPE daughter ) ;
LCIO_DEPRECATED_CAPI int lcmcpsetpdg( PTRTYPE mcparticle, int pdg ) ;
LCIO_DEPRECATED_CAPI int lcmcpsetgeneratorstatus( PTRTYPE mcparticle, int status ) ;
LCIO_DEPRECATED_CAPI int lcmcpsetsimulatorstatus( PTRTYPE mcparticle, int status ) ;
LCIO_DEPRECATED_CAPI int lcmcpsetvertex( PTRTYPE mcparticle, double vtx[3] ) ;
LCIO_DEPRECATED_CAPI int lcmcpsetendpoint( PTRTYPE mcparticle, double pnt[3] ) ;
LCIO_DEPRECATED_CAPI int lcmcpsetmomentum( PTRTYPE mcparticle,  float p[3] );
LCIO_DEPRECATED_CAPI int lcmcpsetmass( PTRTYPE mcparticle, float m) ;
LCIO_DEPRECATED_CAPI int lcmcpsetcharge( PTRTYPE mcparticle, float c ) ;
LCIO_DEPRECATED_CAPI int lcmcpsetspin( PTRTYPE mcparticle, float spin[3] ) ;
LCIO_DEPRECATED_CAPI int lcmcpsetcolorflow( PTRTYPE mcparticle, int cflow[2] ) ;

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

