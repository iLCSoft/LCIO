// -*- C++ -*-
/**Header file for the f77-wrapper functions to the C++ version of LCIO.
 * StdHep fortran interface to the COMMON /HEPEVT/
 * Here we define cfunctions for class HEPEVT methods/functions
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

int   LCSetHepEvt( PTRTYPE event ) ;
int   LCGetHepEvt( PTRTYPE event ) ;

// now the fortran wrappers from cfortran.h
extern "C"{

FCALLSCFUN1(INT, LCSetHepEvt, LCSETHEPEVT, lcsethepevt, CFORTRANPNTR ) ;
FCALLSCFUN1(INT, LCGetHepEvt, LCGETHEPEVT, lcgethepevt, CFORTRANPNTR ) ;
}
