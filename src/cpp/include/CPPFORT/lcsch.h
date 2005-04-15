/**Header file for the f77-wrapper functions of the SimCalorimeterHitImpl class.
 * 
 * @author F. Gaede
 * @version Oct 11, 2003
 */

#include "cfortran.h"
#include "cpointer.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

PTRTYPE lcschcreate() ;
int lcschdelete( PTRTYPE simcalhit ) ;

int lcschid( PTRTYPE simcalhit ) ;
int lcschgetcellid0( PTRTYPE simcalhit )  ;
int lcschgetcellid1( PTRTYPE simcalhit )  ;
float lcschgetenergy( PTRTYPE simcalhit )  ;
int lcschgetposition( PTRTYPE simcalhit, float * )  ;
int lcschgetnmcparticles( PTRTYPE simcalhit )  ;
int lcschgetnmccontributions( PTRTYPE simcalhit )  ;

PTRTYPE lcschgetparticlecont( PTRTYPE simcalhit, int i)  ;
float lcschgetenergycont( PTRTYPE simcalhit, int i)  ;
float lcschgettimecont( PTRTYPE simcalhit, int i)  ;
int lcschgetpdgcont( PTRTYPE simcalhit, int i)  ;

int lcschsetcellid0( PTRTYPE simcalhit, int id0) ;
int lcschsetcellid1( PTRTYPE simcalhit, int id1) ;
int lcschsetenergy( PTRTYPE simcalhit, float en) ;
int lcschsetposition( PTRTYPE simcalhit, float pos[3])  ;
int lcschaddmcparticlecontribution(  PTRTYPE simcalhit, PTRTYPE mcparticle, float en,float t, int pdg ) ; 

// now the fortran wrappers from cfortran.h
extern "C"{
FCALLSCFUN0(CFORTRANPNTR, lcschcreate,LCSCHCREATE, lcschcreate ) 
FCALLSCFUN1(INT, lcschdelete, LCSCHDELETE, lcschdelete, CFORTRANPNTR ) 

FCALLSCFUN1(INT, lcschid, LCSCHID, lcschid, CFORTRANPNTR ) 
FCALLSCFUN1(INT, lcschgetcellid0, LCSCHGETCELLID0, lcschgetcellid0, CFORTRANPNTR ) 
FCALLSCFUN1(INT, lcschgetcellid1, LCSCHGETCELLID1, lcschgetcellid1, CFORTRANPNTR ) 
FCALLSCFUN1(FLOAT, lcschgetenergy, LCSCHGETENERGY, lcschgetenergy, CFORTRANPNTR ) 
FCALLSCFUN2(INT, lcschgetposition,LCSCHGETPOSITION,lcschgetposition, CFORTRANPNTR, FLOATV ) 
FCALLSCFUN1(INT, lcschgetnmcparticles,LCSCHGETNMCPARTICLES,lcschgetnmcparticles, CFORTRANPNTR ) 
FCALLSCFUN1(INT, lcschgetnmccontributions,LCSCHGETNMCCONTRIBUTIONS,lcschgetnmccontributions, CFORTRANPNTR ) 

FCALLSCFUN2(CFORTRANPNTR, lcschgetparticlecont,LCSCHGETPARTICLECONT,lcschgetparticlecont,CFORTRANPNTR,INT ) 
FCALLSCFUN2(FLOAT, lcschgetenergycont,LCSCHGETENERGYCONT,lcschgetenergycont,CFORTRANPNTR, INT ) 
FCALLSCFUN2(FLOAT, lcschgettimecont,LCSCHGETTIMECONT,lcschgettimecont,CFORTRANPNTR,INT ) 

FCALLSCFUN2(INT, lcschsetcellid0,LCSCHSETCELLID0,lcschsetcellid0, CFORTRANPNTR, INT ) 
FCALLSCFUN2(INT, lcschsetcellid1,LCSCHSETCELLID1,lcschsetcellid1, CFORTRANPNTR, INT ) 
FCALLSCFUN2(INT, lcschsetenergy,LCSCHSETENERGY,lcschsetenergy, CFORTRANPNTR, FLOAT ) 
FCALLSCFUN2(INT, lcschsetposition,LCSCHSETPOSITION,lcschsetposition, CFORTRANPNTR, FLOATV ) 
FCALLSCFUN5(INT, lcschaddmcparticlecontribution,LCSCHADDMCPARTICLECONTRIBUTION,lcschaddmcparticlecontribution, CFORTRANPNTR, CFORTRANPNTR, FLOAT, FLOAT, INT ) 

}

