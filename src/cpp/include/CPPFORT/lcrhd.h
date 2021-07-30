/**Header file for the f77-wrapper functions of the LCEventImpl class.
 * 
 * @author F. Gaede
 * @version Oct 11, 2003
 */

#include "cfortran.h"
#include "cpointer.h"

#include "deprecation.h"


// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

LCIO_DEPRECATED_CAPI PTRTYPE lcrhdcreate() ;
LCIO_DEPRECATED_CAPI int lcrhddelete( PTRTYPE runHeader ) ;
LCIO_DEPRECATED_CAPI int lcrhdgetrunnumber( PTRTYPE runHeader )  ;

LCIO_DEPRECATED_CAPI char* lcrhdgetdetectorname( PTRTYPE runHeader  ) ;
LCIO_DEPRECATED_CAPI char* lcrhdgetdescription( PTRTYPE runHeader )  ;

LCIO_DEPRECATED_CAPI PTRTYPE lcrhdgetactivesubdetectors(PTRTYPE runHeader) ;

LCIO_DEPRECATED_CAPI int lcrhdsetrunnumber( PTRTYPE runHeader, int rn) ;
LCIO_DEPRECATED_CAPI int lcrhdsetdetectorname( PTRTYPE runHeader, const char* dn) ;
LCIO_DEPRECATED_CAPI int lcrhdsetdescription( PTRTYPE runHeader, const char* dsc) ;
LCIO_DEPRECATED_CAPI int lcrhdaddactivesubdetector( PTRTYPE runHeader, const char* adn) ;


// now the fortran wrappers from cfortran.h
extern "C"{
FCALLSCFUN0(CFORTRANPNTR, lcrhdcreate,LCRHDCREATE, lcrhdcreate ) 
FCALLSCFUN1(INT, lcrhddelete, LCRHDDELETE, lcrhddelete, CFORTRANPNTR ) 

FCALLSCFUN1(INT, lcrhdgetrunnumber, LCRHDGETRUNNUMBER, lcrhdgetrunnumber, CFORTRANPNTR ) 
FCALLSCFUN1(STRING, lcrhdgetdetectorname, LCRHDGETDETECTORNAME, lcrhdgetdetectorname, CFORTRANPNTR ) 
FCALLSCFUN1(STRING, lcrhdgetdescription, LCRHDGETDESCRIPTION, lcrhdgetdescription, CFORTRANPNTR ) 

FCALLSCFUN1(CFORTRANPNTR, lcrhdgetactivesubdetectors, LCRHDGETACTIVESUBDETECTORS, 
	    lcrhdgetactivesubdetectors, CFORTRANPNTR ) 

FCALLSCFUN2(INT, lcrhdsetrunnumber, LCRHDSETRUNNUMBER, lcrhdsetrunnumber, CFORTRANPNTR, INT ) 
FCALLSCFUN2(INT, lcrhdsetdetectorname, LCRHDSETDETECTORNAME,lcrhdsetdetectorname, CFORTRANPNTR, STRING ) 
FCALLSCFUN2(INT, lcrhdsetdescription, LCRHDSETDESCRIPTION,lcrhdsetdescription, CFORTRANPNTR, STRING ) 
FCALLSCFUN2(INT, lcrhdaddactivesubdetector, LCRHDADDACTIVESUBDETECTOR,lcrhdaddactivesubdetector, CFORTRANPNTR, STRING ) 
}

