// -*- C++ -*-
/**Header file for the f77-wrapper functions to the C++ version of LCIO.
 * Here we define cfunctions the class SimTrackerHit methods/functions
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
int     LCSetSimTrackerHit( PTRTYPE col, int cellID, double* pos, float dEdx, float time, PTRTYPE mcp ) ;

int     LCGetSimTrackerHit( PTRTYPE col, int i, int* cellID, double* pos, float* dEdx, float* time, PTRTYPE* mcp ) ;

// now the fortran wrappers from cfortran.h
extern "C"{
FCALLSCFUN6(INT, LCSetSimTrackerHit , LCSETSIMTRACKERHIT , lcsetsimtrackerhit, 
            CFORTRANPNTR , INT , DOUBLEV , FLOAT , FLOAT , CFORTRANPNTR ) ;

FCALLSCFUN7(INT, LCGetSimTrackerHit, LCGETSIMTRACKERHIT, lcgetsimtrackerhit, 
            CFORTRANPNTR , INT , INTV , DOUBLEV , FLOATV , FLOATV , CFORTRANPNTRV ) ;
}

