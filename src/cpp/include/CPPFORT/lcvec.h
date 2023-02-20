/**Header file for the f77-wrapper functions for stl vectors.
 * 
 * @author F. Gaede
 * @version Nov 3, 2003
 */


#include "cfortran.h"
#include "cpointer.h"

#include "deprecation.h"

#include <string>
#include <vector>


typedef std::vector<std::string> LCStrVec ;
typedef std::vector<PTRTYPE> PointerVec ;


// define an interface to a LC string/int/float vector
LCIO_DEPRECATED_CAPI int   lcsvcgetlength(PTRTYPE strvec) ;
LCIO_DEPRECATED_CAPI char* lcsvcgetstringat(PTRTYPE strvec, int index) ;

LCIO_DEPRECATED_CAPI int   lcivcgetlength(PTRTYPE intvec) ;
LCIO_DEPRECATED_CAPI int   lcivcgetintat(PTRTYPE intvec, int index) ;

LCIO_DEPRECATED_CAPI int   lcfvcgetlength(PTRTYPE floatvec) ;
LCIO_DEPRECATED_CAPI float lcfvcgetfloatat(PTRTYPE floatvec, int index) ;

// define an interface to read a standard string/int/pointer/float vector
LCIO_DEPRECATED_CAPI int   stringvectorgetlength(PTRTYPE strvec) ;
LCIO_DEPRECATED_CAPI char* stringvectorgetelement(PTRTYPE strvec, int index) ;

LCIO_DEPRECATED_CAPI int   intvectorgetlength(PTRTYPE intvec) ;
LCIO_DEPRECATED_CAPI int   intvectorgetelement(PTRTYPE intvec, int index) ;

LCIO_DEPRECATED_CAPI int     pointervectorgetlength(PTRTYPE intvec) ;
LCIO_DEPRECATED_CAPI PTRTYPE pointervectorgetelement(PTRTYPE intvec, int index) ;

LCIO_DEPRECATED_CAPI int   floatvectorgetlength(PTRTYPE floatvec) ;
LCIO_DEPRECATED_CAPI float floatvectorgetelement(PTRTYPE floatvec, int index) ;

// now the fortran wrappers from cfortran.h
extern "C"{

FCALLSCFUN1(INT, lcsvcgetlength, LCSVCGETLENGTH, lcsvcgetlength, CFORTRANPNTR ) 
FCALLSCFUN2(STRING, lcsvcgetstringat, LCSVCGETSTRINGAT, lcsvcgetstringat, CFORTRANPNTR, INT ) 

FCALLSCFUN1(INT, lcivcgetlength, LCIVCGETLENGTH, lcivcgetlength, CFORTRANPNTR ) 
FCALLSCFUN2(INT, lcivcgetintat, LCIVCGETINTAT, lcivcgetintat, CFORTRANPNTR, INT ) 

FCALLSCFUN1(INT, lcfvcgetlength, LCFVCGETLENGTH, lcfvcgetlength, CFORTRANPNTR ) 
FCALLSCFUN2(FLOAT, lcfvcgetfloatat, LCFVCGETFLOATAT, lcfvcgetfloatat, CFORTRANPNTR, INT ) 


FCALLSCFUN1(INT, stringvectorgetlength, STRINGVECTORGETLENGTH, stringvectorgetlength, CFORTRANPNTR ) 
FCALLSCFUN2(STRING, stringvectorgetelement, STRINGVECTORGETELEMENT, stringvectorgetelement, CFORTRANPNTR, INT ) 

FCALLSCFUN1(INT, intvectorgetlength, INTVECTORGETLENGTH, intvectorgetlength, CFORTRANPNTR ) 
FCALLSCFUN2(INT, intvectorgetelement, INTVECTORGETELEMENT, intvectorgetelement, CFORTRANPNTR, INT ) 

FCALLSCFUN1(INT, pointervectorgetlength, POINTERVECTORGETLENGTH, pointervectorgetlength, CFORTRANPNTR ) 
FCALLSCFUN2(CFORTRANPNTR, pointervectorgetelement, POINTERVECTORGETELEMENT, pointervectorgetelement, CFORTRANPNTR, INT ) 

FCALLSCFUN1(INT, floatvectorgetlength, FLOATVECTORGETLENGTH, floatvectorgetlength, CFORTRANPNTR ) 
FCALLSCFUN2(FLOAT, floatvectorgetelement, FLOATVECTORGETELEMENT, floatvectorgetelement, CFORTRANPNTR, INT ) 

}

