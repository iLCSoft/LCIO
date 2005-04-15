
/**Header file for the f77-wrapper functions of the LCEventImpl class.
 * 
 * @author F. Gaede
 * @version Oct 10, 2003
 */

#include "cfortran.h"
#include "cpointer.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

// the event interface
PTRTYPE lcevtcreate() ;
int     lcevtdelete( PTRTYPE event ) ;

int lcevtgetrunnumber( PTRTYPE event ) ;
int lcevtgeteventnumber( PTRTYPE event )  ;
char* lcevtgetdetectorname( PTRTYPE event );

long lcevtgettimestamp( PTRTYPE event ) ;

PTRTYPE lcevtgetcollectionnames( PTRTYPE event ) ;
// PTRTYPE lcevtgettrelationnames( PTRTYPE event ) ;
PTRTYPE lcevtgetcollection( PTRTYPE event, const char* colname ) ;
// PTRTYPE lcevtgetrelation( PTRTYPE event, const char* name ) ;

int lcevtaddcollection(     PTRTYPE event, PTRTYPE collection , char* colname ) ;
int lcevtremovecollection(  PTRTYPE event, char* name ) ; 
// int lcevtaddrelation(  PTRTYPE event, PTRTYPE relation, char* name ) ;
// int lcevtremoverelation( PTRTYPE event, const char* name ) ;

int lcevtsetrunnumber(    PTRTYPE event, int rn ) ;
int lcevtseteventnumber(  PTRTYPE event, int en ) ;
int lcevtsetdetectorname( PTRTYPE event, char* dn ) ;
int lcevtsettimestamp(    PTRTYPE event, long ts ) ;

// now the fortran wrappers from cfortran.h
extern "C"{

FCALLSCFUN0(CFORTRANPNTR, lcevtcreate,LCEVTCREATE, lcevtcreate ) 
FCALLSCFUN1(INT, lcevtdelete, LCEVTDELETE, lcevtdelete, CFORTRANPNTR ) 

FCALLSCFUN1(INT, lcevtgetrunnumber, LCEVTGETRUNNUMBER,lcevtgetrunnumber, CFORTRANPNTR ) 
FCALLSCFUN1(INT, lcevtgeteventnumber, LCEVTGETEVENTNUMBER,lcevtgeteventnumber, CFORTRANPNTR ) 
FCALLSCFUN1(STRING, lcevtgetdetectorname, LCEVTGETDETECTORNAME, lcevtgetdetectorname, CFORTRANPNTR ) 
FCALLSCFUN1(LONG, lcevtgettimestamp, LCEVTGETTIMESTAMP,lcevtgettimestamp, CFORTRANPNTR ) 
FCALLSCFUN1(CFORTRANPNTR, lcevtgetcollectionnames, LCEVTGETCOLLECTIONNAMES, lcevtgetcollectionnames, CFORTRANPNTR ) 
// FCALLSCFUN1(CFORTRANPNTR, lcevtgettrelationnames, LCEVTGETTRELATIONNAMES, lcevtgettrelationnames, CFORTRANPNTR ) 
FCALLSCFUN2(CFORTRANPNTR, lcevtgetcollection, LCEVTGETCOLLECTION, lcevtgetcollection, CFORTRANPNTR, STRING ) 
// FCALLSCFUN2(CFORTRANPNTR, lcevtgetrelation, LCEVTGETRELATION, lcevtgetrelation, CFORTRANPNTR, STRING ) 

FCALLSCFUN3(INT, lcevtaddcollection,LCEVTADDCOLLECTION,lcevtaddcollection,CFORTRANPNTR, CFORTRANPNTR, STRING ) 
FCALLSCFUN2(INT, lcevtremovecollection, LCEVTREMOVECOLLECTION, lcevtremovecollection, CFORTRANPNTR, STRING ) 
// FCALLSCFUN3(INT, lcevtaddrelation, LCEVTADDRELATION, lcevtaddrelation, CFORTRANPNTR, CFORTRANPNTR, STRING ) 
// FCALLSCFUN2(INT, lcevtremoverelation, LCEVTREMOVERELATION, lcevtremoverelation, CFORTRANPNTR,STRING ) 

FCALLSCFUN2(INT, lcevtsetrunnumber, LCEVTSETRUNNUMBER, lcevtsetrunnumber, CFORTRANPNTR, INT ) 
FCALLSCFUN2(INT, lcevtseteventnumber, LCEVTSETEVENTNUMBER, lcevtseteventnumber, CFORTRANPNTR, INT ) 
FCALLSCFUN2(INT, lcevtsetdetectorname, LCEVTSETDETECTORNAME, lcevtsetdetectorname, CFORTRANPNTR, STRING ) 
FCALLSCFUN2(INT, lcevtsettimestamp, LCEVTSETTIMESTAMP, lcevtsettimestamp, CFORTRANPNTR, LONG ) 
}

