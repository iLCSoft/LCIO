/**Header file for the f77-wrapper functions of the LCCollectionVec class.
 * 
 * @author F. Gaede
 * @version Oct 10, 2003
 */
#include "cfortran.h"
#include "cpointer.h"

#include "deprecation.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

// the collection interface
LCIO_DEPRECATED_CAPI PTRTYPE lccolcreate( const char* colname ) ;
LCIO_DEPRECATED_CAPI int lccoldelete( PTRTYPE collection ) ;

LCIO_DEPRECATED_CAPI int lccolgetnumberofelements( PTRTYPE collection ) ;
LCIO_DEPRECATED_CAPI char* lccolgettypename( PTRTYPE collection ) ;
LCIO_DEPRECATED_CAPI PTRTYPE lccolgetelementat( PTRTYPE collection, int index ) ;
LCIO_DEPRECATED_CAPI int lccolgetflag(PTRTYPE collection)  ;

LCIO_DEPRECATED_CAPI bool lccolistransient(PTRTYPE collection) ;
LCIO_DEPRECATED_CAPI int lccolsettransient(PTRTYPE collection, bool value) ;

LCIO_DEPRECATED_CAPI bool lccolisdefault(PTRTYPE collection) ;
LCIO_DEPRECATED_CAPI int lccolsetdefault(PTRTYPE collection, bool value) ;

LCIO_DEPRECATED_CAPI int lccolsetflag(PTRTYPE collection, int flag) ;
LCIO_DEPRECATED_CAPI int lccoladdelement(PTRTYPE collection, PTRTYPE object) ;
LCIO_DEPRECATED_CAPI int lccolremoveelementat(PTRTYPE collection, int i)  ;

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
   
