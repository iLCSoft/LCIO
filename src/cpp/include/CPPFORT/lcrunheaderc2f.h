// -*- C++ -*-
/**Header file for the f77-wrapper functions to the C++ version of LCIO.
 * Here we define cfunctions for the class LCRunHeader methods/functions
 * taking an integer/long argument for a pointer to the class (base class type).
 * Then using cfortran.h we define fortran callable names for these functions.
 * 
 * @author H. Vogt
 * @version Sep 30, 2003
 */


#include "cfortran.h"
#include "cpointer.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux


// the event interface
int     LCWriteRunHeader(PTRTYPE writer, const int irun, const char* detname, const char* descr,
                         void* sdnamevec, const int nsubd, const int nchsubd) ;
PTRTYPE LCReadNextRunHeader(PTRTYPE reader, int* irun, void* detname, void* descr,
                            void* sdnamevec, int* nsubd, const int nchsubd) ;

// now the fortran wrappers from cfortran.h
extern "C"{
#define lcwriterunheader_STRV_A5 NUM_ELEM_ARG(6)
FCALLSCFUN7(INT, LCWriteRunHeader, LCWRITERUNHEADER, lcwriterunheader, CFORTRANPNTR, INT, STRING, STRING,
            STRINGV, INT, INT) ;
#define lcreadnextrunheader_STRV_A3 NUM_ELEMS(1)
#define lcreadnextrunheader_STRV_A4 NUM_ELEMS(1)
#define lcreadnextrunheader_STRV_A5 NUM_ELEM_ARG(6)
FCALLSCFUN7(CFORTRANPNTR, LCReadNextRunHeader, LCREADNEXTRUNHEADER, lcreadnextrunheader, CFORTRANPNTR,
            INTV, PSTRINGV, PSTRINGV, PSTRINGV, INTV, INT) ;
}

