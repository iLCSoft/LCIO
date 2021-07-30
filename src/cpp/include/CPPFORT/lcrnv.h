/**Header file for the f77-wrapper functions of the LCRelationNavigator class.
 * 
 * @author H. Vogt
 * @version sep 14, 2004
 */

#include "cfortran.h"
#include "cpointer.h"

#include "deprecation.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

// the RelationNavigator interface
LCIO_DEPRECATED_CAPI PTRTYPE lcrnvcreate( const char* fromType, const char* toType ) ;
LCIO_DEPRECATED_CAPI int lcrnvdelete( PTRTYPE relation ) ;
LCIO_DEPRECATED_CAPI PTRTYPE lcrnvcreatefromcollection( PTRTYPE collection ) ;

LCIO_DEPRECATED_CAPI char* lcrnvgetfromtype( PTRTYPE relation ) ;
LCIO_DEPRECATED_CAPI char* lcrnvgettotype( PTRTYPE relation ) ;
LCIO_DEPRECATED_CAPI PTRTYPE lcrnvgetrelatedtoobjects( PTRTYPE relation, PTRTYPE object ) ;
LCIO_DEPRECATED_CAPI PTRTYPE lcrnvgetrelatedfromobjects( PTRTYPE relation, PTRTYPE object ) ;
LCIO_DEPRECATED_CAPI PTRTYPE lcrnvgetrelatedtoweights ( PTRTYPE relation, PTRTYPE object ) ;
LCIO_DEPRECATED_CAPI PTRTYPE lcrnvgetrelatedfromweights ( PTRTYPE relation, PTRTYPE object ) ;

LCIO_DEPRECATED_CAPI int lcrnvgaddrelation(PTRTYPE relation, PTRTYPE objectfrom, PTRTYPE objectto, float weight ) ;
LCIO_DEPRECATED_CAPI int lcrnvgremoverelation(PTRTYPE relation, PTRTYPE objectfrom, PTRTYPE objectto ) ;
LCIO_DEPRECATED_CAPI PTRTYPE lcrnvcreatecollection(PTRTYPE relation ) ;

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
   
