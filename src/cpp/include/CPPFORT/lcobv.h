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
LCIO_DEPRECATED_CAPI int lcobvgetlength( PTRTYPE vector ) ;
LCIO_DEPRECATED_CAPI PTRTYPE lcobvgetobject( PTRTYPE vector, int index ) ;
LCIO_DEPRECATED_CAPI int lcobvgetobjectid( PTRTYPE vector, int index ) ;
LCIO_DEPRECATED_CAPI float lcobvgetweight( PTRTYPE vector, int index ) ;

// now the fortran wrappers from cfortran.h
extern "C"{
FCALLSCFUN1(INT, lcobvgetlength, LCOBVGETLENGTH, lcobvgetlength, 
            CFORTRANPNTR ) 
FCALLSCFUN2(CFORTRANPNTR, lcobvgetobject, LCOBVGETOBJECT, lcobvgetobject,
            CFORTRANPNTR, INT ) 
FCALLSCFUN2(INT, lcobvgetobjectid, LCOBVGETOBJECTID, lcobvgetobjectid,
            CFORTRANPNTR, INT )
FCALLSCFUN2(FLOAT, lcobvgetweight, LCOBVGETWEIGHT, lcobvgetweight,
            CFORTRANPNTR, INT )

}
   
