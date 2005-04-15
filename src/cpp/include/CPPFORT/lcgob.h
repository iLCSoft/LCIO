/**Header file for the f77-wrapper functions of the LCGenericObject class.
 * 
 * @author H. Vogte
 * @version Sep 15, 2004
 */
#include "cfortran.h"
#include "cpointer.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

// the genericobject interface
PTRTYPE lcgobcreate() ;
PTRTYPE lcgobcreatefixed( int nint, int nfloat, int ndouble ) ;
int     lcgobdelete( PTRTYPE genericobject ) ;

int     lcgobid( PTRTYPE genericobject ) ;
int     lcgobgetnint( PTRTYPE genericobject ) ;
int     lcgobgetnfloat( PTRTYPE genericobject ) ;
int     lcgobgetndouble( PTRTYPE genericobject ) ;
int     lcgobgetintval( PTRTYPE genericobject, int index ) ;
float   lcgobgetfloatval( PTRTYPE genericobject, int index ) ;
double  lcgobgetdoubleval( PTRTYPE genericobject, int index ) ;

int     lcgobsetintval( PTRTYPE genericobject, int index, int value) ;
int     lcgobsetfloatval( PTRTYPE genericobject, int index, float value) ;
int     lcgobsetdoubleval( PTRTYPE genericobject, int index, double value) ;

bool    lcgobisfixedsize(PTRTYPE genericobject) ;
char*   lcgobgettypename(PTRTYPE genericobject) ;
char*   lcgobgetdatadescription(PTRTYPE genericobject) ;

// now the fortran wrappers from cfortran.h
extern "C"{
FCALLSCFUN0(CFORTRANPNTR, lcgobcreate, LCGOBCREATE, lcgobcreate ) 
FCALLSCFUN3(CFORTRANPNTR, lcgobcreatefixed, LCGOBCREATEFIXED, lcgobcreatefixed, INT, INT, INT ) 
FCALLSCFUN1(INT, lcgobdelete, LCGOBDELETE, lcgobdelete, CFORTRANPNTR ) 

FCALLSCFUN1(INT, lcgobid, LCGOBID, lcgobid, CFORTRANPNTR ) 
FCALLSCFUN1(INT, lcgobgetnint, LCGOBGETNINT, lcgobgetnint, CFORTRANPNTR ) 
FCALLSCFUN1(INT, lcgobgetnfloat, LCGOBGETNFLOAT, lcgobgetnfloat, CFORTRANPNTR ) 
FCALLSCFUN1(INT, lcgobgetndouble, LCGOBGETNDOUBLE, lcgobgetndouble, CFORTRANPNTR ) 
FCALLSCFUN2(INT, lcgobgetintval, LCGOBGETINTVAL, lcgobgetintval, CFORTRANPNTR, INT ) 
FCALLSCFUN2(FLOAT, lcgobgetfloatval, LCGOBGETFLOATVAL, lcgobgetfloatval, CFORTRANPNTR, INT ) 
FCALLSCFUN2(DOUBLE, lcgobgetdoubleval, LCGOBGETDOUBLEVAL, lcgobgetdoubleval, CFORTRANPNTR, INT ) 

FCALLSCFUN3(INT, lcgobsetintval, LCGOBSETINTVAL, lcgobsetintval, CFORTRANPNTR, INT, INT ) 
FCALLSCFUN3(INT, lcgobsetfloatval, LCGOBSETFLOATVAL, lcgobsetfloatval, CFORTRANPNTR, INT, FLOAT ) 
FCALLSCFUN3(INT, lcgobsetdoubleval, LCGOBSETDOUBLEVAL, lcgobsetdoubleval, CFORTRANPNTR, INT, DOUBLE ) 

FCALLSCFUN1(LOGICAL, lcgobisfixedsize, LCGOBISFIXEDSIZE, lcgobisfixedsize, CFORTRANPNTR )
FCALLSCFUN1(STRING, lcgobgettypename, LCGOBGETTYPENAME, lcgobgettypename, CFORTRANPNTR ) 
FCALLSCFUN1(STRING, lcgobgetdatadescription, LCGOBGETDATADESCRIPTION, lcgobgetdatadescription, CFORTRANPNTR ) 
}
   
