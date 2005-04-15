/**Header file for the f77-wrapper functions of the LCWriter class.
 * 
 * @author F. Gaede
 * @version Oct 10, 2003
 */

#include "cfortran.h"
#include "cpointer.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

PTRTYPE lcwrtcreate() ;
int     lcwrtdelete( PTRTYPE writer ) ;

// the writer interface
int   lcwrtopen( PTRTYPE writer, const char* filename , int writeMode ) ;
int   lcwrtclose( PTRTYPE writer ) ;

int   lcwrtwriterunheader( PTRTYPE writer, PTRTYPE header) ;
int   lcwrtwriteevent( PTRTYPE writer, PTRTYPE event) ;


// now the fortran wrappers from cfortran.h
extern "C"{
FCALLSCFUN0(CFORTRANPNTR, lcwrtcreate, LCWRTCREATE, lcwrtcreate ) 
FCALLSCFUN1(INT, lcwrtdelete, LCWRTDELETE, lcwrtdelete, CFORTRANPNTR ) 

FCALLSCFUN3(INT, lcwrtopen, LCWRTOPEN, lcwrtopen, CFORTRANPNTR, STRING ,INT ) 
FCALLSCFUN1(INT, lcwrtclose, LCWRTCLOSE, lcwrtclose, CFORTRANPNTR ) 

FCALLSCFUN2(INT, lcwrtwriterunheader,LCWRTWRITERUNHEADER,lcwrtwriterunheader,CFORTRANPNTR,CFORTRANPNTR ) 
FCALLSCFUN2(INT, lcwrtwriteevent,LCWRTWRITEEVENT,lcwrtwriteevent,CFORTRANPNTR,CFORTRANPNTR ) 

}

