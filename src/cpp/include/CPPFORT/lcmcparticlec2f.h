// -*- C++ -*-
/**Header file for the f77-wrapper functions to the C++ version of LCIO.
 * Here we define cfunctions for class MCParticle methods/functions
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

int   LCGetMCParticleCont ( PTRTYPE mcparticle, int* pdg, int* hepevtstatus, double* prodvtx,
                            float* momentum, float* mass, int* ndaughters ) ;

// now the fortran wrappers from cfortran.h
extern "C"{
FCALLSCFUN7(INT, LCGetMCParticleCont, LCGETMCPARTICLECONT, lcgetmcparticlecont,
            CFORTRANPNTR , INTV , INTV , DOUBLEV , FLOATV , FLOATV , INTV ) ;
}

