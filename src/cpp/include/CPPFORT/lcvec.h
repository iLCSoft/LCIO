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


// define an interface to read a string vector
int lcsvcgetlength(PTRTYPE strvec) ;
char* lcsvcgetstringat(PTRTYPE strvec, int index) ;

int lcivcgetlength(PTRTYPE intvec) ;
int lcivcgetintat(PTRTYPE intvec, int index) ;

int lcfvcgetlength(PTRTYPE floatvec) ;
float lcfvcgetfloatat(PTRTYPE floatvec, int index) ;

FCALLSCFUN1(INT, lcsvcgetlength, LCSVCGETLENGTH, lcsvcgetlength, CFORTRANPNTR ) ;
FCALLSCFUN2(STRING, lcsvcgetstringat, LCSVCGETSTRINGAT, lcsvcgetstringat, CFORTRANPNTR, INT ) ;

FCALLSCFUN1(INT, lcivcgetlength, LCIVCGETLENGTH, lcivcgetlength, CFORTRANPNTR ) ;
FCALLSCFUN2(INT, lcivcgetintat, LCIVCGETINTAT, lcivcgetintat, CFORTRANPNTR, INT ) ;

FCALLSCFUN1(INT, lcfvcgetlength, LCFVCGETLENGTH, lcfvcgetlength, CFORTRANPNTR ) ;
FCALLSCFUN2(FLOAT, lcfvcgetfloatat, LCFVCGETFLOATAT, lcfvcgetfloatat, CFORTRANPNTR, INT ) ;

