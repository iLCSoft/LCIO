
/**Header file for the f77-wrapper functions of the LCReader class.
 * 
 * @author F. Gaede
 * @version Oct 10, 2003
 */

#include "cfortran.h"
#include "cpointer.h"

#include "deprecation.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

LCIO_DEPRECATED_CAPI PTRTYPE lcrdrcreate() ;
LCIO_DEPRECATED_CAPI int     lcrdrdelete( PTRTYPE reader ) ;

LCIO_DEPRECATED_CAPI int   lcrdropen( PTRTYPE reader, const char* filename ) ;
LCIO_DEPRECATED_CAPI int   lcrdrclose( PTRTYPE reader ) ;
LCIO_DEPRECATED_CAPI int   lcrdrreadstream( PTRTYPE reader, int maxRecord ) ;

LCIO_DEPRECATED_CAPI PTRTYPE lcrdrreadnextrunheader(PTRTYPE reader, int accessMode) ;
LCIO_DEPRECATED_CAPI PTRTYPE lcrdrreadnextevent(PTRTYPE reader, int accessMode) ;
LCIO_DEPRECATED_CAPI PTRTYPE lcrdrreadevent(PTRTYPE reader, int runNumber, int evtNumber );

LCIO_DEPRECATED_CAPI int  lcrdrskipnevents( PTRTYPE reader, int n ) ;

extern "C"{

FCALLSCFUN0(CFORTRANPNTR, lcrdrcreate, LCRDRCREATE, lcrdrcreate ) 
FCALLSCFUN1(INT, lcrdrdelete, LCRDRDELETE, lcrdrdelete, CFORTRANPNTR ) 

FCALLSCFUN2(INT, lcrdropen, LCRDROPEN, lcrdropen,  CFORTRANPNTR, STRING ) 
FCALLSCFUN1(INT, lcrdrclose, LCRDRCLOSE, lcrdrclose, CFORTRANPNTR ) 
FCALLSCFUN2(INT, lcrdrreadstream, LCRDRREADSTREAM, lcrdrreadstream, CFORTRANPNTR, INT ) 

FCALLSCFUN2(CFORTRANPNTR,lcrdrreadnextrunheader,LCRDRREADNEXTRUNHEADER,lcrdrreadnextrunheader,CFORTRANPNTR,INT)
FCALLSCFUN2(CFORTRANPNTR,lcrdrreadnextevent,LCRDRREADNEXTEVENT,lcrdrreadnextevent,CFORTRANPNTR,INT)
FCALLSCFUN3(CFORTRANPNTR,lcrdrreadevent,LCRDRREADEVENT,lcrdrreadevent,CFORTRANPNTR,INT,INT)

FCALLSCFUN2(INT, lcrdrskipnevents, LCRDRSKIPNEVENTS, lcrdrskipnevents, CFORTRANPNTR, INT ) 

}

