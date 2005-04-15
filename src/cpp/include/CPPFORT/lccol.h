/**Header file for the f77-wrapper functions of the LCCollectionVec class.
 * 
 * @author F. Gaede
 * @version Oct 10, 2003
 */
#include "cfortran.h"
#include "cpointer.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

// the collection interface
PTRTYPE lccolcreate( const char* colname ) ;
int lccoldelete( PTRTYPE collection ) ;

int lccolgetnumberofelements( PTRTYPE collection ) ;
char* lccolgettypename( PTRTYPE collection ) ;
PTRTYPE lccolgetelementat( PTRTYPE collection, int index ) ;
int lccolgetflag(PTRTYPE collection)  ;

bool lccolistransient(PTRTYPE collection) ;
int lccolsettransient(PTRTYPE collection, bool value) ;

bool lccolisdefault(PTRTYPE collection) ;
int lccolsetdefault(PTRTYPE collection, bool value) ;

int lccolsetflag(PTRTYPE collection, int flag) ;
int lccoladdelement(PTRTYPE collection, PTRTYPE object) ;
int lccolremoveelementat(PTRTYPE collection, int i)  ;

// now the fortran wrappers from cfortran.h
extern "C"{
FCALLSCFUN1(CFORTRANPNTR, lccolcreate, LCCOLCREATE, lccolcreate, STRING ) 
FCALLSCFUN1(INT, lccoldelete, LCCOLDELETE, lccoldelete, CFORTRANPNTR ) 

FCALLSCFUN1(INT, lccolgetnumberofelements,LCCOLGETNUMBEROFELEMENTS,lccolgetnumberofelements,CFORTRANPNTR) 
FCALLSCFUN1(STRING, lccolgettypename, LCCOLGETTYPENAME, lccolgettypename, CFORTRANPNTR ) 
FCALLSCFUN2(CFORTRANPNTR, lccolgetelementat, LCCOLGETELEMENTAT, lccolgetelementat, CFORTRANPNTR,  INT ) 
FCALLSCFUN1(INT, lccolgetflag, LCCOLGETFLAG, lccolgetflag, CFORTRANPNTR ) 

FCALLSCFUN1(LOGICAL, lccolistransient, LCCOLISTRANSIENT, lccolistransient, CFORTRANPNTR ) 
FCALLSCFUN2(INT, lccolsettransient, LCCOLSETTRANSIENT, lccolsettransient, CFORTRANPNTR, LOGICAL ) 

FCALLSCFUN1(LOGICAL, lccolisdefault, LCCOLISDEFAULT, lccolisdefault, CFORTRANPNTR ) 
FCALLSCFUN2(INT, lccolsetdefault, LCCOLSETDEFAULT, lccolsetdefault, CFORTRANPNTR, LOGICAL ) 

FCALLSCFUN2(INT, lccolsetflag, LCCOLSETFLAG, lccolsetflag, CFORTRANPNTR, INT ) 
FCALLSCFUN2(INT, lccoladdelement, LCCOLADDELEMENT, lccoladdelement, CFORTRANPNTR, CFORTRANPNTR ) 
FCALLSCFUN2(INT, lccolremoveelementat, LCCOLREMOVEELEMENTAT, lccolremoveelementat, CFORTRANPNTR, INT ) 


}
   
