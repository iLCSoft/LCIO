/**Header file for the f77-wrapper functions of the LCRelationNavigator class.
 * 
 * @author H. Vogt
 * @version sep 14, 2004
 */

#include "cfortran.h"
#include "cpointer.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

// the RelationNavigator interface
PTRTYPE lcrnvcreate( const char* fromType, const char* toType ) ;
int lcrnvdelete( PTRTYPE relation ) ;
PTRTYPE lcrnvcreatefromcollection( PTRTYPE collection ) ;

char* lcrnvgetfromtype( PTRTYPE relation ) ;
char* lcrnvgettotype( PTRTYPE relation ) ;
PTRTYPE lcrnvgetrelatedtoobjects( PTRTYPE relation, PTRTYPE object ) ;
PTRTYPE lcrnvgetrelatedfromobjects( PTRTYPE relation, PTRTYPE object ) ;
PTRTYPE lcrnvgetrelatedtoweights ( PTRTYPE relation, PTRTYPE object ) ;
PTRTYPE lcrnvgetrelatedfromweights ( PTRTYPE relation, PTRTYPE object ) ;

int lcrnvgaddrelation(PTRTYPE relation, PTRTYPE objectfrom, PTRTYPE objectto, float weight ) ;
int lcrnvgremoverelation(PTRTYPE relation, PTRTYPE objectfrom, PTRTYPE objectto ) ;
PTRTYPE lcrnvcreatecollection(PTRTYPE relation ) ;

// now the fortran wrappers from cfortran.h
extern "C"{
FCALLSCFUN2(CFORTRANPNTR, lcrnvcreate, LCRNVCREATE, lcrnvcreate, STRING, STRING ) 
FCALLSCFUN1(INT, lcrnvdelete, LCRNVDELETE, lcrnvdelete, CFORTRANPNTR ) 
FCALLSCFUN1(CFORTRANPNTR, lcrnvcreatefromcollection, LCRNVCREATEFROMCOLLECTION, lcrnvcreatefromcollection, CFORTRANPNTR ) 

FCALLSCFUN1(STRING, lcrnvgetfromtype, LCRNVGETFROMTYPE, lcrnvgetfromtype, CFORTRANPNTR ) 
FCALLSCFUN1(STRING, lcrnvgettotype, LCRNVGETTOTYPE, lcrnvgettotype, CFORTRANPNTR ) 
FCALLSCFUN2(CFORTRANPNTR, lcrnvgetrelatedtoobjects, LCRNVGETRELATEDTOOBJECTS, lcrnvgetrelatedtoobjects,
            CFORTRANPNTR, CFORTRANPNTR ) 
FCALLSCFUN2(CFORTRANPNTR, lcrnvgetrelatedfromobjects, LCRNVGETRELATEDFROMOBJECTS, lcrnvgetrelatedfromobjects,
            CFORTRANPNTR, CFORTRANPNTR ) 
FCALLSCFUN2(CFORTRANPNTR, lcrnvgetrelatedtoweights, LCRNVGETRELATEDTOWEIGHTS, lcrnvgetrelatedtoweights,
            CFORTRANPNTR, CFORTRANPNTR ) 
FCALLSCFUN2(CFORTRANPNTR, lcrnvgetrelatedfromweights, LCRNVGETRELATEDFROMWEIGHTS, lcrnvgetrelatedfromweights,
            CFORTRANPNTR, CFORTRANPNTR ) 

FCALLSCFUN4(INT, lcrnvgaddrelation, LCRNVGADDRELATION, lcrnvgaddrelation, 
            CFORTRANPNTR, CFORTRANPNTR, CFORTRANPNTR, FLOAT ) 
FCALLSCFUN3(INT, lcrnvgremoverelation, LCRNVGREMOVERELATION, lcrnvgremoverelation, 
            CFORTRANPNTR, CFORTRANPNTR, CFORTRANPNTR ) 
FCALLSCFUN1(CFORTRANPNTR, lcrnvcreatecollection, LCRNVCREATECOLLECTION, lcrnvcreatecollection, CFORTRANPNTR ) 

}
   
