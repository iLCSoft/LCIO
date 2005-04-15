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
PTRTYPE lcrelcreate0() ;
PTRTYPE lcrelcreate( PTRTYPE objectfrom, PTRTYPE objectto, float weight ) ;
int lcreldelete( PTRTYPE relation ) ;

int lcrelid(PTRTYPE relation) ;
PTRTYPE lcrelgetfrom( PTRTYPE relation ) ;
PTRTYPE lcrelgetto( PTRTYPE relation ) ;
float lcrelgetweight( PTRTYPE relation ) ;
               
int lcrelsetfrom(PTRTYPE relation, PTRTYPE object ) ;
int lcrelsetto(PTRTYPE relation, PTRTYPE object ) ;
int lcrelsetweight(PTRTYPE relation,float weight ) ;

// now the fortran wrappers from cfortran.h
extern "C"{
FCALLSCFUN0(CFORTRANPNTR, lcrelcreate0, LCRELCREATE0, lcrelcreate0 ) 
FCALLSCFUN3(CFORTRANPNTR, lcrelcreate, LCRELCREATE, lcrelcreate, CFORTRANPNTR, CFORTRANPNTR, FLOAT ) 
FCALLSCFUN1(INT, lcreldelete, LCRELDELETE, lcreldelete, CFORTRANPNTR ) 

FCALLSCFUN1(INT, lcrelid, LCRELID, lcrelid, CFORTRANPNTR ) 
FCALLSCFUN1(CFORTRANPNTR, lcrelgetfrom, LCRELGETFROM, lcrelgetfrom, CFORTRANPNTR ) 
FCALLSCFUN1(CFORTRANPNTR, lcrelgetto, LCRELGETTO, lcrelgetto, CFORTRANPNTR ) 
FCALLSCFUN1(FLOAT, lcrelgetweight, LCRELGETWEIGHT, lcrelgetweight, CFORTRANPNTR ) 

FCALLSCFUN2(INT, lcrelsetfrom, LCRELSETFROM, lcrelsetfrom, CFORTRANPNTR, CFORTRANPNTR ) 
FCALLSCFUN2(INT, lcrelsetto, LCRELSETTO, lcrelsetto, CFORTRANPNTR, CFORTRANPNTR ) 
FCALLSCFUN2(INT, lcrelsetweight, LCRELSETWEIGHT, lcrelsetweight, CFORTRANPNTR, FLOAT ) 

}
   
