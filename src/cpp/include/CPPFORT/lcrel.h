/**Header file for the f77-wrapper functions of the LCRelation class.
 * 
 * @author H. Vogt
 * @version Jun 23, 2004
 */

#include "cfortran.h"
#include "cpointer.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

// the relation interface
PTRTYPE lcrelcreate( const char* fromType, const char* toType) ;
int lcreldelete( PTRTYPE relation ) ;

int lcrelnumberofrelations( PTRTYPE relation, PTRTYPE object) ;
PTRTYPE lcrelgetrelation( PTRTYPE relation, PTRTYPE object, int i ) ;
float lcrelgetweight( PTRTYPE relation, PTRTYPE object, int i ) ;

int lcreladdrelation(PTRTYPE relation, PTRTYPE objectfrom, PTRTYPE objectto, float weight ) ;
int lcrelusecaching(PTRTYPE relation, bool val ) ;
int lcrelsettypes(PTRTYPE relation, const char* fromType, const char* toType ) ;

// now the fortran wrappers from cfortran.h
extern "C"{
FCALLSCFUN2(CFORTRANPNTR, lcrelcreate, LCRELCREATE, lcrelcreate, STRING, STRING ) ;
FCALLSCFUN1(INT, lcreldelete, LCRELDELETE, lcreldelete, CFORTRANPNTR ) ;

FCALLSCFUN2(INT, lcrelnumberofrelations, LCRELNUMBEROFRELATIONS, lcrelnumberofrelations, CFORTRANPNTR, CFORTRANPNTR ) ;
FCALLSCFUN3(CFORTRANPNTR, lcrelgetrelation, LCRELGETRELATION, lcrelgetrelation, CFORTRANPNTR, CFORTRANPNTR, INT ) ;
FCALLSCFUN3(FLOAT, lcrelgetweight, LCRELGETWEIGHT, lcrelgetweight, CFORTRANPNTR, CFORTRANPNTR, INT ) ;

FCALLSCFUN4(INT, lcreladdrelation, LCRELADDRELATION, lcreladdrelation, CFORTRANPNTR, CFORTRANPNTR, CFORTRANPNTR, FLOAT ) ;
FCALLSCFUN2(INT, lcrelusecaching, LCRELUSECACHING, lcrelusecaching, CFORTRANPNTR, LOGICAL ) ;
FCALLSCFUN3(INT, lcrelsettypes, LCRELSETTYPES, lcrelsettypes, CFORTRANPNTR, STRING, STRING ) ;

}
   
