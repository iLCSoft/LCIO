/**Header file for the f77-wrapper functions of the LCEventImpl class.
 * 
 * @author F. Gaede
 * @version Oct 11, 2003
 */

#include "cfortran.h"
#include "cpointer.h"


// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

PTRTYPE lcrhdcreate() ;
int lcrhddelete( PTRTYPE runHeader ) ;
int lcrhdgetrunnumber( PTRTYPE runHeader )  ;

char* lcrhdgetdetectorname( PTRTYPE runHeader  ) ;
char* lcrhdgetdescription( PTRTYPE runHeader )  ;

PTRTYPE lcrhdgetactivesubdetectors(PTRTYPE runHeader) ;

int lcrhdsetrunnumber( PTRTYPE runHeader, int rn) ;
int lcrhdsetdetectorname( PTRTYPE runHeader, const char* dn) ;
int lcrhdsetdescription( PTRTYPE runHeader, const char* dsc) ;
int lcrhdaddactivesubdetector( PTRTYPE runHeader, const char* adn) ;


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

