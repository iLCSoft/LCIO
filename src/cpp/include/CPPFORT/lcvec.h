/**Header file for the f77-wrapper functions for stl vectors.
 * 
 * @author F. Gaede
 * @version Nov 3, 2003
 */

#include "cfortran.h"
#include "cpointer.h"

#include <string>
#include <vector>


typedef std::vector<std::string> LCStrVec ;
typedef std::vector<PTRTYPE> PointerVec ;


// define an interface to a LC string/int/float vector
int   lcsvcgetlength(PTRTYPE strvec) ;
char* lcsvcgetstringat(PTRTYPE strvec, int index) ;

int   lcivcgetlength(PTRTYPE intvec) ;
int   lcivcgetintat(PTRTYPE intvec, int index) ;

int   lcfvcgetlength(PTRTYPE floatvec) ;
float lcfvcgetfloatat(PTRTYPE floatvec, int index) ;

// define an interface to read a standard string/int/pointer/float vector
int   stringvectorgetlength(PTRTYPE strvec) ;
char* stringvectorgetelement(PTRTYPE strvec, int index) ;

int   intvectorgetlength(PTRTYPE intvec) ;
int   intvectorgetelement(PTRTYPE intvec, int index) ;

int     pointervectorgetlength(PTRTYPE intvec) ;
PTRTYPE pointervectorgetelement(PTRTYPE intvec, int index) ;

int   floatvectorgetlength(PTRTYPE floatvec) ;
float floatvectorgetelement(PTRTYPE floatvec, int index) ;

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

