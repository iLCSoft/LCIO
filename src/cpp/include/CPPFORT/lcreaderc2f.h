// -*- C++ -*-
/**Header file for the f77-wrapper functions to the C++ version of LCIO.
 * Here we define cfunctions for the class LCFactory methods/functions
 * to create a LCReader object
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
PTRTYPE LCCreateReader() ;
int     LCDeleteReader( PTRTYPE reader ) ;

// the reader interface
int   LCReaderOpen( PTRTYPE reader,const char* filename ) ;
int   LCReaderClose( PTRTYPE reader ) ;

// now the fortran wrappers from cfortran.h
extern "C"{
FCALLSCFUN0(CFORTRANPNTR, LCCreateReader, LCCREATEREADER, lccreatereader ) ;
FCALLSCFUN1(INT, LCDeleteReader, LCDELETEREADER, lcdeletereader, CFORTRANPNTR ) ;

FCALLSCFUN2(INT, LCReaderOpen, LCREADEROPEN, lcreaderopen,  CFORTRANPNTR, STRING ) ;
FCALLSCFUN1(INT, LCReaderClose, LCREADERCLOSE, lcreaderclose, CFORTRANPNTR ) ;
}

