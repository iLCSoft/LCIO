// -*- C++ -*-
  /**StdHep fortran interface to the COMMON /HEPEVT/ 
   * used for MCParticle data
   * 
   * @author Vogt
   * @version Sep 30, 2003
   */

/*
** Basic HEPEVT COMMON block 
C ------------------------------------------------------------------
C
      integer NMXHEP
      parameter (NMXHEP=4000)
      common/HEPEVT/NEVHEP,NHEP,ISTHEP(NMXHEP),IDHEP(NMXHEP),
     &JMOHEP(2,NMXHEP),JDAHEP(2,NMXHEP),PHEP(5,NMXHEP),VHEP(4,NMXHEP)
      integer NEVHEP,NHEP,ISTHEP,IDHEP,JMOHEP,JDAHEP
      double precision PHEP,VHEP
      save /HEPEVT/
C...      NEVHEP       - event number
C...      NHEP         - number of entries in this event
C...      ISTHEP(..)   - status code
C...      IDHEP(..)    - particle ID, P.D.G. standard
C...      JMOHEP(1,..) - position of mother particle in list
C...      JMOHEP(2,..) - position of second mother particle in list
C...      JDAHEP(1,..) - position of first daughter in list
C...      JDAHEP(2,..) - position of last daughter in list
C...      PHEP(1,..)   - x momentum in GeV/c
C...      PHEP(2,..)   - y momentum in GeV/c
C...      PHEP(3,..)   - z momentum in GeV/c
C...      PHEP(4,..)   - energy in GeV
C...      PHEP(5,..)   - mass in GeV/c**2
C...      VHEP(1,..)   - x vertex position in mm
C...      VHEP(2,..)   - y vertex position in mm
C...      VHEP(3,..)   - z vertex position in mm
C...      VHEP(4,..)   - production time in mm/c
C
C -------------------------------------------------------------------
C
*/

/*  note that to avoid alignment problems, structures and common blocks
    should be in the order: double precision, real, integer.
*/

#define NMXHEP 4000
struct hepevt_t {
int nevhep;		/* The event number */
int nhep;		/* The number of entries in this event */
int isthep[NMXHEP]; 	/* The Particle id */
int idhep[NMXHEP];      /* The particle id */
int jmohep[NMXHEP][2];    /* The position of the mother particle */
int jdahep[NMXHEP][2];    /* Position of the first daughter... */
double phep[NMXHEP][5];    /* 4-Momentum, mass */
double vhep[NMXHEP][4];    /* Vertex information */
};

/*   prototypes */
extern "C" 
{
extern struct hepevt_t hepevt_;
}

#define FTNhep hepevt_

