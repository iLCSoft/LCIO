// -*- C++ -*-
/**Header file for the f77-wrapper functions to the C++ version of LCIO.
 * Here we define cfunctions for the class LCFactory methods/functions
 * to create a LCWriter object
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

// factory methods
PTRTYPE LCCreateWriter() ;
int     LCDeleteWriter( PTRTYPE writer ) ;

// the writer interface
int   LCWriterOpen( PTRTYPE writer, const char* filename ) ;
int   LCWriterClose( PTRTYPE writer ) ;

// now the fortran wrappers from cfortran.h
extern "C"{
FCALLSCFUN0(CFORTRANPNTR, LCCreateWriter, LCCREATEWRITER, lccreatewriter ) ;
FCALLSCFUN1(INT, LCDeleteWriter, LCDELETEWRITER, lcdeletewriter, CFORTRANPNTR ) ;

FCALLSCFUN2(INT, LCWriterOpen, LCWRITEROPEN, lcwriteropen, CFORTRANPNTR, STRING ) ;
FCALLSCFUN1(INT, LCWriterClose, LCWRITERCLOSE, lcwriterclose, CFORTRANPNTR ) ;
}

