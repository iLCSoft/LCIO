/**Header file for the f77-wrapper functions of the CalorimeterHitImpl class.
 * 
 * @author F. Gaede
 * @version Oct 11, 2003
 */

#include "cfortran.h"
#include "cpointer.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

PTRTYPE lccahcreate() ;
int lccahdelete( PTRTYPE simcalhit ) ;
int lccahid( PTRTYPE simcalhit ) ;

int lccahgetcellid0( PTRTYPE simcalhit )  ;
int lccahgetcellid1( PTRTYPE simcalhit )  ;
float lccahgetenergy( PTRTYPE simcalhit )  ;
float lccahgettime( PTRTYPE simcalhit )  ;
int lccahgetposition( PTRTYPE simcalhit, float * )  ;

int lccahsetcellid0( PTRTYPE simcalhit, int id0) ;
int lccahsetcellid1( PTRTYPE simcalhit, int id1) ;
int lccahsetenergy( PTRTYPE simcalhit, float en) ;
int lccahsettime( PTRTYPE simcalhit, float time) ;
int lccahsetposition( PTRTYPE simcalhit, float pos[3])  ;


// now the fortran wrappers from cfortran.h
extern "C"{
FCALLSCFUN0(CFORTRANPNTR, lccahcreate,LCCAHCREATE, lccahcreate ) ;
FCALLSCFUN1(INT, lccahdelete, LCCAHDELETE, lccahdelete, CFORTRANPNTR ) ;
FCALLSCFUN1(INT, lccahid, LCCAHID, lccahid, CFORTRANPNTR ) ;

FCALLSCFUN1(INT, lccahgetcellid0, LCCAHGETCELLID0, lccahgetcellid0, CFORTRANPNTR ) ;
FCALLSCFUN1(INT, lccahgetcellid1, LCCAHGETCELLID1, lccahgetcellid1, CFORTRANPNTR ) ;
FCALLSCFUN1(FLOAT, lccahgetenergy, LCCAHGETENERGY, lccahgetenergy, CFORTRANPNTR ) ;
FCALLSCFUN1(FLOAT, lccahgettime, LCCAHGETTIME, lccahgettime, CFORTRANPNTR ) ;
FCALLSCFUN2(INT, lccahgetposition,LCCAHGETPOSITION,lccahgetposition, CFORTRANPNTR, FLOATV ) ;

FCALLSCFUN2(INT, lccahsetcellid0,LCCAHSETCELLID0,lccahsetcellid0, CFORTRANPNTR, INT ) ;
FCALLSCFUN2(INT, lccahsetcellid1,LCCAHSETCELLID1,lccahsetcellid1, CFORTRANPNTR, INT ) ;
FCALLSCFUN2(INT, lccahsetenergy,LCCAHSETENERGY,lccahsetenergy, CFORTRANPNTR, FLOAT ) ;
FCALLSCFUN2(INT, lccahsettime,LCCAHSETTIME,lccahsettime, CFORTRANPNTR, FLOAT ) ;
FCALLSCFUN2(INT, lccahsetposition,LCCAHSETPOSITION,lccahsetposition, CFORTRANPNTR, FLOATV ) ;
}

