// -*- C++ -*-
/**Header file for the f77-wrapper functions to the C++ version of LCIO.
 * Here we define cfunctions for the class LCCollectionVec methods/functions
 * taking an integer/long argument for a pointer to the class (base class type).
 * Then using cfortran.h we define fortran callable names for these functions.
 * 
 * @author H. Vogt
 * @version Sep 30, 2003
 */


#include "cfortran.h"
#include "cpointer.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux


// the collection interface
PTRTYPE LCCreateCollectionVec( const char* colname ) ;
PTRTYPE LCAddElement( PTRTYPE collection, PTRTYPE  object ) ;

PTRTYPE LCGetElement( PTRTYPE collection, int nelement ) ;
int     LCGetNumberOfElements( PTRTYPE collection ) ;
char*   LCGetTypeName( PTRTYPE collection ) ;
char*   LCGetName( const char* mytypename ) ;

// now the fortran wrappers from cfortran.h
extern "C"{
FCALLSCFUN1(CFORTRANPNTR, LCCreateCollectionVec, LCCREATECOLLECTIONVEC, lccreatecollectionvec, STRING ) ;
FCALLSCFUN2(INT, LCAddElement, LCADDELEMENT, lcaddelement, CFORTRANPNTR, CFORTRANPNTR ) ;

FCALLSCFUN2(CFORTRANPNTR, LCGetElement, LCGETELEMENT, lcgetelement, CFORTRANPNTR,  INT ) ;
FCALLSCFUN1(INT, LCGetNumberOfElements, LCGETNUMBEROFELEMENTS, lcgetnumberofelements, CFORTRANPNTR ) ;
FCALLSCFUN1(STRING, LCGetTypeName, LCGETTYPENAME, lcgettypename, CFORTRANPNTR ) ;
FCALLSCFUN1(STRING, LCGetName, LCGETNAME, lcgetname, STRING ) ;
}
   
