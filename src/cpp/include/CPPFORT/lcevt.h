
/**Header file for the f77-wrapper functions of the LCEventImpl class.
 * 
 * @author F. Gaede
 * @version Oct 10, 2003
 */

#include "cfortran.h"
#include "cpointer.h"

#include "deprecation.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

// the event interface
LCIO_DEPRECATED_CAPI PTRTYPE lcevtcreate() ;
LCIO_DEPRECATED_CAPI int     lcevtdelete( PTRTYPE event ) ;

LCIO_DEPRECATED_CAPI int lcevtgetrunnumber( PTRTYPE event ) ;
LCIO_DEPRECATED_CAPI int lcevtgeteventnumber( PTRTYPE event )  ;
LCIO_DEPRECATED_CAPI char* lcevtgetdetectorname( PTRTYPE event );

LCIO_DEPRECATED_CAPI long lcevtgettimestamp( PTRTYPE event ) ;

LCIO_DEPRECATED_CAPI PTRTYPE lcevtgetcollectionnames( PTRTYPE event ) ;
// PTRTYPE lcevtgettrelationnames( PTRTYPE event ) ;
LCIO_DEPRECATED_CAPI PTRTYPE lcevtgetcollection( PTRTYPE event, const char* colname ) ;
// PTRTYPE lcevtgetrelation( PTRTYPE event, const char* name ) ;

LCIO_DEPRECATED_CAPI int lcevtaddcollection(     PTRTYPE event, PTRTYPE collection , char* colname ) ;
LCIO_DEPRECATED_CAPI int lcevtremovecollection(  PTRTYPE event, char* name ) ;
// int lcevtaddrelation(  PTRTYPE event, PTRTYPE relation, char* name ) ;
// int lcevtremoverelation( PTRTYPE event, const char* name ) ;

LCIO_DEPRECATED_CAPI int lcevtsetrunnumber(    PTRTYPE event, int rn ) ;
LCIO_DEPRECATED_CAPI int lcevtseteventnumber(  PTRTYPE event, int en ) ;
LCIO_DEPRECATED_CAPI int lcevtsetdetectorname( PTRTYPE event, char* dn ) ;
LCIO_DEPRECATED_CAPI int lcevtsettimestamp(    PTRTYPE event, long ts ) ;

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

