
c#ifndef HEPEVT1_H
c#define HEPEVT1_H 1

#include "lciof77pointer.inc"

C *** HEPEVT1 commonblock.
      PTRTYPE          MCPOINTERV(NMXHEP)
      REAL             CHHEP(NMXHEP)
      COMMON /HEPEVT1/ MCPOINTERV, CHHEP
      SAVE   /HEPEVT1/

c#endif
