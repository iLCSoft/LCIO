// -*- C++ -*-
/**Header file for the f77-wrapper functions to the C++ version of LCIO.
 * Here we define cfunctions for the class SimCalorimeterHit methods/functions
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


PTRTYPE LCSetSimCaloHit( int cellID0, int cellID1, float energy, float* pos ) ;
int     LCSetSimCaloHitCmc(PTRTYPE hit, PTRTYPE mcp, float energy, float time, int pdg ) ;
PTRTYPE LCGetSimCaloHit( PTRTYPE col, int i, int* cellID0, int* cellID1, float* energy, float* pos ) ;
int     LCGetSimCaloHitNmcp( PTRTYPE hit ) ;
int     LCGetSimCaloHitCmcp( PTRTYPE hit, int i, PTRTYPE* mcp, float* energy, float* time, int* pdg ) ;

// now the fortran wrappers from cfortran.h
extern "C"{
FCALLSCFUN4(CFORTRANPNTR, LCSetSimCaloHit, LCSETSIMCALOHIT, lcsetsimcalohit,
            INT , INT, FLOAT, FLOATV ) ;
FCALLSCFUN5(INT, LCSetSimCaloHitCmc, LCSETSIMCALOHITCMC, lcsetsimcalohitcmc,
            CFORTRANPNTR , CFORTRANPNTR , FLOAT ,FLOAT , INT ) ;
FCALLSCFUN6(CFORTRANPNTR , LCGetSimCaloHit, LCGETSIMCALOHIT, lcgetsimcalohit,
            CFORTRANPNTR , INT , INTV ,  INTV , FLOATV , FLOATV ) ;
FCALLSCFUN1(INT, LCGetSimCaloHitNmcp, LCGETSIMCALOHITNMCP, lcgetsimcalohitnmcp,
            CFORTRANPNTR ) ;
FCALLSCFUN6(INT, LCGetSimCaloHitCmcp, LCGETSIMCALOHITCMCP, lcgetsimcalohitcmcp,
            CFORTRANPNTR , INT , CFORTRANPNTRV, FLOATV , FLOATV , INTV ) ;
}

