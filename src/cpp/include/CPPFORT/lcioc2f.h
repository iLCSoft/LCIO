// -*- C++ -*-
/**Header file for the f77-wrapper functions to the C++ version of LCIO.
 * Here we define one cfunction for every class method/function
 * taking an integer/long argument for a pointer to the class (base class type).
 * Then using cfortran.h we define fortran callable names for these functions.
 * 
 * @author gaede
 * @version Mar 13, 2003
 */


#include "cfortran.h"

// define the length of pointers - this has to made machine independent ...
typedef int PTRTYPE ;
#define CFORTRANPNTR INT


// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

// factory methods
PTRTYPE lfactCreateLCReader() ;
PTRTYPE lfactCreateLCWriter() ;

// the reader interface
int  lrdrOpen( PTRTYPE reader,const char* filename ) ;
PTRTYPE lrdrReadNextEvent( PTRTYPE reader ) ;
int  lrdrClose( PTRTYPE reader) ;

// the event interface
int levtGetRunNumber( PTRTYPE event ) ;
int levtGetEventNumber( PTRTYPE event ) ;
char* levtGetDetectorName( PTRTYPE event ) ;


// now the fortran wrappers from cfortran.h
extern "C"{

FCALLSCFUN0(CFORTRANPNTR, lfactCreateLCReader, LFACTCREATELCREADER, lfactcreatelcreader ) ;
FCALLSCFUN0(CFORTRANPNTR, lfactCreateLCWriter, LFACTCREATELCWRITER, lfactcreatelcwriter ) ;

FCALLSCFUN2(INT, lrdrOpen, LRDROPEN, lrdropen, CFORTRANPNTR, STRING ) ;
FCALLSCFUN1(CFORTRANPNTR, lrdrReadNextEvent, LRDRREADNEXTEVENT, lrdrreadnextevent, CFORTRANPNTR ) ;
FCALLSCFUN1(INT, lrdrClose, LRDRCLOSE, lrdrclose, CFORTRANPNTR ) ;

FCALLSCFUN1(INT, levtGetRunNumber,LEVTGETRUNNUMBER, levtgetrunnumber, CFORTRANPNTR ) ;
FCALLSCFUN1(INT, levtGetEventNumber,LEVTGETEVENTNUMBER, levtgeteventnumber, CFORTRANPNTR ) ;
FCALLSCFUN1(STRING, levtGetDetectorName,LEVTGETDETECTORNAME, levtgetdetectorname, CFORTRANPNTR ) ;

}
