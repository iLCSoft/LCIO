// -*- C++ -*-
/**Header file for the f77-wrapper functions to the C++ version of LCIO.
 * Here we define cfunctions for the event class methods/functions
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
PTRTYPE LCCreateEvent() ;
int     LCDeleteEvent( PTRTYPE event ) ;
int     LCSetEventHeader( PTRTYPE event, const int irun, const int ievent, const int timestamp, const char* detname );
int     LCAddCollection( PTRTYPE event, PTRTYPE collection , char* colname ) ;
int     LCWriteEvent(PTRTYPE writer, PTRTYPE event ) ;

PTRTYPE LCReadNextEvent( PTRTYPE reader ) ;
int     LCGetEventHeader(PTRTYPE event, int* irun, int* ievent, int* timestamp, void* detname );
PTRTYPE LCGetCollection(  PTRTYPE event, const char* colname ) ;
int     LCDumpEvent( PTRTYPE event ) ;
int     LCDumpEventDetailed( PTRTYPE event ) ;

// now the fortran wrappers from cfortran.h
extern "C"{
FCALLSCFUN0(CFORTRANPNTR, LCCreateEvent, LCCREATEEVENT, lccreateevent ) ;
FCALLSCFUN1(INT, LCDeleteEvent, LCDELETEEVENT, lcdeleteevent, CFORTRANPNTR ) ;
FCALLSCFUN5(INT, LCSetEventHeader, LCSETEVENTHEADER, lcseteventheader, CFORTRANPNTR, INT, INT, INT, STRING ) ;
FCALLSCFUN3(INT, LCAddCollection, LCADDCOLLECTION, lcaddcollection, CFORTRANPNTR, CFORTRANPNTR, STRING ) ;
FCALLSCFUN2(INT, LCWriteEvent, LCWRITEEVENT, lcwriteevent, CFORTRANPNTR, CFORTRANPNTR ) ;

FCALLSCFUN1(INT, LCReadNextEvent, LCREADNEXTEVENT, lcreadnextevent, CFORTRANPNTR ) ;
#define lcgeteventheader_STRV_A5 NUM_ELEMS(1)
FCALLSCFUN5(INT, LCGetEventHeader, LCGETEVENTHEADER, lcgeteventheader, CFORTRANPNTR, INTV, INTV, INTV, PSTRINGV ) ;
FCALLSCFUN2(CFORTRANPNTR, LCGetCollection, LCGETCOLLECTION, lcgetcollection, CFORTRANPNTR,  STRING ) ;
FCALLSCFUN1(INT, LCDumpEvent, LCDUMPEVENT, lcdumpevent, CFORTRANPNTR ) ;
FCALLSCFUN1(INT, LCDumpEventDetailed, LCDUMPEVENTDETAILED, lcdumpeventdetailed, CFORTRANPNTR ) ;
}

