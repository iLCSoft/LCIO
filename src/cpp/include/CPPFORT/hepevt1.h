// -*- C++ -*-
  /**StdHep fortran interface to the COMMON /HEPEVT1/ 
   * used for MCParticle pointers and charges
   * 
   * @author Vogt
   * @version Sep 30, 2003
   */

/*
** HEPEVT1 COMMON block for MCParticle pointers
C ------------------------------------------------------------------
C
      integer NMXHEP
      parameter (NMXHEP=4000)
      common /HEPEVT1/ MCPOINTERV(NMXHEP), MCCHARGEV(NMXHEP)
      integer          MCPOINTERV
      real             MCCHARGEV
      save /HEPEVT1/
C...      MCPOINTERV   - vector containing MCParticle pointer
C...      MCCHARGEV    - vector containing MCParticle charge
C
C -------------------------------------------------------------------
C
*/


#include "cpointer.h"

#define NMXHEP 4000
struct hepevt1_t {
PTRTYPE mcpointerv[NMXHEP]; /* The MCParticle pointer */
float   mcchargev[NMXHEP];  /* The MCParticle charge */
};

/*   prototypes */
extern "C" 
{
extern struct hepevt1_t hepevt1_;
}

#define FTNhep1 hepevt1_

