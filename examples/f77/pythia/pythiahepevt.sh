#!/bin/sh
################### script to create a hepevt output ascii file ###############
#                                                                             #
#                   output file: $LCIO/bin/pythia.data                        #
#                   needs cernlib 2002 or higher                              #
#                                                                             #
###############################################################################

rm -rf /tmp/pyhepevt.f /tmp/pyhepevt.bin pythia.log pythia.data
cat > /tmp/pyhepevt.f << EOF
 
**********************************************************************
 
C     PYHEPEVT
C     A simple program to write a hepevt output ascii file
 
      PROGRAM PYHEPEVT
 
C *** PYTHIA commonblocks.

C...This file contains an almost complete listing of all PYTHIA 
C...commonblocks, and additionally some recommended other 
C...declarations. You may copy this to the top of your
C...mina program and then eliminate unnecessary parts.

C-----------------------------------------------------------------
 
C...All real arithmetic in double precision.
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
C...Three Pythia functions return integers, so need declaring.
      INTEGER PYK,PYCHGE,PYCOMP
C...Parameter statement to help give large particle numbers
C...(left- and righthanded SUSY, technicolor, excited fermions,
C...extra dimensions).
      PARAMETER (KSUSY1=1000000,KSUSY2=2000000,KTECHN=3000000,
     &KEXCIT=4000000,KDIMEN=5000000)

C...EXTERNAL statement links PYDATA on most machines.
      EXTERNAL PYDATA

C...Commonblocks.
C...The event record.
      COMMON/PYJETS/N,NPAD,K(4000,5),P(4000,5),V(4000,5)
C...Parameters.
      COMMON/PYDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
C...Particle properties + some flavour parameters.
      COMMON/PYDAT2/KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)
C...Decay information.
      COMMON/PYDAT3/MDCY(500,3),MDME(8000,2),BRAT(8000),KFDP(8000,5)
C...Particle names
      COMMON/PYDAT4/CHAF(500,2)
      CHARACTER CHAF*16
C...Random number generator information.
      COMMON/PYDATR/MRPY(6),RRPY(100)
C...Selection of hard scattering subprocesses.
      COMMON/PYSUBS/MSEL,MSELPD,MSUB(500),KFIN(2,-40:40),CKIN(200)
C...Parameters. 
      COMMON/PYPARS/MSTP(200),PARP(200),MSTI(200),PARI(200)
C...Internal variables.
      COMMON/PYINT1/MINT(400),VINT(400)
C...Process information.
      COMMON/PYINT2/ISET(500),KFPR(500,2),COEF(500,20),ICOL(40,4,2)
C...Parton distributions and cross sections.
      COMMON/PYINT3/XSFX(2,-40:40),ISIG(1000,3),SIGH(1000)
C...Resonance width and secondary decay treatment.
      COMMON/PYINT4/MWID(500),WIDS(500,5)
C...Generation and cross section statistics.
      COMMON/PYINT5/NGENPD,NGEN(0:500,3),XSEC(0:500,3)
C...Process names.
      COMMON/PYINT6/PROC(0:500)
      CHARACTER PROC*28
C...Total cross sections.
      COMMON/PYINT7/SIGT(0:6,0:6,0:5)
C...Photon parton distributions: total and valence only.
      COMMON/PYINT8/XPVMD(-6:6),XPANL(-6:6),XPANH(-6:6),XPBEH(-6:6), 
     &XPDIR(-6:6) 
      COMMON/PYINT9/VXPVMD(-6:6),VXPANL(-6:6),VXPANH(-6:6),VXPDGM(-6:6) 
C...Supersymmetry parameters.
      COMMON/PYMSSM/IMSS(0:99),RMSS(0:99)
C...Supersymmetry mixing matrices.
      COMMON/PYSSMT/ZMIX(4,4),UMIX(2,2),VMIX(2,2),SMZ(4),SMW(2),
     &SFMIX(16,4),ZMIXI(4,4),UMIXI(2,2),VMIXI(2,2)
C...R-parity-violating couplings in supersymmetry.
      COMMON/PYMSRV/RVLAM(3,3,3), RVLAMP(3,3,3), RVLAMB(3,3,3)
C...Internal parameters for R-parity-violating processes.
      COMMON/PYRVNV/AB(2,16,2),RMS(0:3),RES(6,5),IDR,IDR2,DCMASS,KFR(3)
      COMMON/PYRVPM/RM(0:3),A(2),B(2),RESM(2),RESW(2),MFLAG
      LOGICAL MFLAG
C...Parameters for Gauss integration of supersymmetric widths.
      COMMON/PYINTS/XXM(20)
      COMMON/PYG2DX/X1
C...Histogram information.
      COMMON/PYBINS/IHIST(4),INDX(1000),BIN(20000)
C...HEPEVT commonblock.
      PARAMETER (NMXHEP=4000)
      COMMON/HEPEVT/NEVHEP,NHEP,ISTHEP(NMXHEP),IDHEP(NMXHEP),
     &JMOHEP(2,NMXHEP),JDAHEP(2,NMXHEP),PHEP(5,NMXHEP),VHEP(4,NMXHEP)
      DOUBLE PRECISION PHEP,VHEP
C...User process initialization commonblock.
      INTEGER MAXPUP
      PARAMETER (MAXPUP=100)
      INTEGER IDBMUP,PDFGUP,PDFSUP,IDWTUP,NPRUP,LPRUP
      DOUBLE PRECISION EBMUP,XSECUP,XERRUP,XMAXUP
      COMMON/HEPRUP/IDBMUP(2),EBMUP(2),PDFGUP(2),PDFSUP(2),
     &IDWTUP,NPRUP,XSECUP(MAXPUP),XERRUP(MAXPUP),XMAXUP(MAXPUP),
     &LPRUP(MAXPUP)
C...User process event common block.
      INTEGER MAXNUP
      PARAMETER (MAXNUP=500)
      INTEGER NUP,IDPRUP,IDUP,ISTUP,MOTHUP,ICOLUP
      DOUBLE PRECISION XWGTUP,SCALUP,AQEDUP,AQCDUP,PUP,VTIMUP,SPINUP
      COMMON/HEPEUP/NUP,IDPRUP,XWGTUP,SCALUP,AQEDUP,AQCDUP,IDUP(MAXNUP),
     &ISTUP(MAXNUP),MOTHUP(2,MAXNUP),ICOLUP(2,MAXNUP),PUP(5,MAXNUP),
     &VTIMUP(MAXNUP),SPINUP(MAXNUP)

C *** HEPEVTCH commonblock.
      REAL CHHEP(NMXHEP)
      COMMON /HEPEVTCH/ CHHEP
      SAVE /HEPEVTCH/


      LOGICAL IFLIST
      CHARACTER*80 FILENAME
 
C *** Set flag for listing hepevt common content
      IFLIST  = .TRUE.

C *** Open ascii file for output and list if requited
      LUNOUT = 1
      FILENAME = 'pythia.data'
      OPEN (UNIT=LUNOUT, STATUS='NEW', 
     &      FILE=FILENAME, FORM='FORMATTED')
*
      IF (IFLIST)  THEN
        LUNLIST = 2
        FILENAME = 'pythiahepevt.log'
        OPEN (UNIT=LUNLIST, STATUS='NEW',
     &        FILE=FILENAME, FORM='FORMATTED')
      END IF
 
C *** Take values from example_MyPythiaOnlyToHepMC.cxx
      MSEL = 0
      MSUB(20) = 1
      MRPY(1) = 55122

C *** Tell Pythia not to write multiple copies of particles in event record.
      MSTP(128) = 2

C *** Set the top mass
      PMAS(6,1) = 175.

C *** Call pythia initialization
      ECMS = 14000.
      CALL PYINIT( "CMS", "p", "p", ECMS )
 

C *** Loop over events to be generated.
      NEVENTS = 20
      DO  IEV = 1,NEVENTS

C ***   Generate one pythia event
        CALL PYEVNT

C ***   Debugging option (activate statement below)
ccc        CALL PYLIST (2)

C ***   Fill hepevt common with pythia output, set event number
        CALL PYHEPC ( 1 )
        NEVHEP = IEV

C ***   fill hepevtch common with charge using PYTHIA utility PYCHGE
        DO I = 1,NHEP
          CHHEP(I) = PYCHGE (IDHEP(I))/3.
        END DO


C ***   List hepevt common content if requested
        IF (IFLIST)  CALL LCLIST ( LUNLIST )

C ***   Write hepevt to file 
        NEVHEP = IEV
        CALL LCWRITE ( LUNOUT )


      END DO
 
C *** List statistics for each process type.
      CALL PYSTAT ( 1 )

C *** Close output file(s)
      IF (IFLIST) CLOSE ( LUNLIST )
      CLOSE ( LUNOUT )
      print '(a,a,a,i2)', 'PYHEPEVT: hepevt output ascii file closed'
*
      END
*  
      SUBROUTINE LCLIST (LUN)
*****************************************************************************
*
*     SUBROUTINE LCLIST (LUN)
*
*     Purpose: List HEPEVT common 
*              to printer in Linear Collider workshop formatted form
*
*     NOTES: * All UNSTABLE particles with an average proper lifetime of
*              less than 10 mm should be decayed by the generator; others
*              (eg K0, Lambda0) should be left to the simulation.
*              This can be achieved in JETSET using MSTJ(22)=2 with the
*              default setting of PARJ(71)
*
*            * If you are using PYTHIA/JETSET remember to fill the
*              HEPEVT commons with CALL LUHEPC(1)
*
*            * If you are using PYTHIA 6.1 you need PYHEPC(1)
*
*            * You must ensure that the HEPEVT commons are DOUBLE PRECISION
*              (In JETSET 7.4 they are single precision by default)
*
*            * The event number (NEVHEP) is not written to file; the
*              simulation will take each event individually
*
*            * For SPARTICLE codes, see
*              http://d0lblt.lbl.gov/wwwpdg/mc_numbers.htm
*
*            * For the specifications of the HEPEVT commons see the LEP1
*              Yellow Report or the PYTHIA/JETSET manual
*
*            * The spin part (SHEP) of the commons is not written to file
*
*
*     Output:  to printer
*
*     Called:  Per event
*
*     Author:  Harald Vogt (hvogt@ifh.de) taken from LCWRITE
*     Author:  Hywel Phillips (H.T.Phillips@rl.ac.uk)
*
*
*****************************************************************************
*****************************************************************************

      IMPLICIT NONE
* switched to NMXHEP = 4000 for compatibility with PYTHIA 6.1
c M.Dobbs 04.2000 NMXHEP = 2000 for compatibility with PYTHIA 5.7
      INTEGER NMXHEP
      PARAMETER (NMXHEP = 4000)
      INTEGER NEVHEP, NHEP, ISTHEP(NMXHEP), IDHEP(NMXHEP),
     &     JMOHEP(2,NMXHEP), JDAHEP(2,NMXHEP)
      DOUBLE PRECISION PHEP(5,NMXHEP), VHEP(4,NMXHEP)
      COMMON /HEPEVT/ NEVHEP, NHEP, ISTHEP, IDHEP, JMOHEP, JDAHEP,
     &     PHEP, VHEP

* The spin part of the HEPEVT commons which is not written
      DOUBLE PRECISION SHEP(4,NMXHEP)
      COMMON /HEPSPN/ SHEP


      INTEGER LUN,I,J,K,L,M
*
*   write event data
*
      IF (NHEP.GT.NMXHEP) THEN
        WRITE(6,*) 'LCLIST: Error- NHEP ',NHEP,' is too big'
        WRITE(6,*) 'LCLIST: Event not written to file'
        RETURN
      ENDIF

      WRITE(LUN,1) NEVHEP
 1    FORMAT(' event #:',I5)
      WRITE(LUN,10) NHEP
      DO I=1,NHEP
        WRITE(LUN,12) I,ISTHEP(I),IDHEP(I),(JMOHEP(J,I),J=1,2),
     &  (JDAHEP(K,I),K=1,2),
     &  (PHEP(L,I),L=1,5),
     &  (VHEP(M,I),M=1,4)
      ENDDO

 10   FORMAT(' # of particles in this event:',1I5,/,
     &       '     I sts   pID 1st_m 2nd_m',
     &       ' 1st_d lst_d        Px        Py        Pz',
     &       '         E      mass',
     &       '         Vx         Vy         Vz       time')

 12   FORMAT(1X,I5,I4,I6,4I6,5F10.3,4E11.3)
      RETURN

 999  WRITE(6,*) 'LCLIST: Error writing to file on unit ',LUN
      STOP
      END

      SUBROUTINE LCWRITE(LUN)
*****************************************************************************
*
*     SUBROUTINE LCWRITE(LUN)
*
*     Purpose: Write HEPEVT common to output file
*              on unit LUN in Linear Collider workshop formatted form
*
*     NOTES: * All UNSTABLE particles with an average proper lifetime of
*              less than 10 mm should be decayed by the generator; others
*              (eg K0, Lambda0) should be left to the simulation.
*              This can be achieved in JETSET using MSTJ(22)=2 with the
*              default setting of PARJ(71)
*
*            * If you are using PYTHIA/JETSET remember to fill the
*              HEPEVT commons with CALL LUHEPC(1)
*
*            * If you are using PYTHIA 6.1 you need PYHEPC(1)
*
*            * You must ensure that the HEPEVT commons are DOUBLE PRECISION
*              (In JETSET 7.4 they are single precision by default)
*
*            * The event number (NEVHEP) is not written to file; the
*              simulation will take each event individually
*
*            * For SPARTICLE codes, see
*              http://d0lblt.lbl.gov/wwwpdg/mc_numbers.htm
*
*            * For the specifications of the HEPEVT commons see the LEP1
*              Yellow Report or the PYTHIA/JETSET manual
*
*            * The spin part (SHEP) of the commons is not written to file
*
*            * The output is slightly modified to include the charge
*              (obtained by the PYTHIA utility routine PYCHGE)
*
*
*     Input:   LUN - output file INTEGER
*
*     Output:  to file
*
*     Called:  Per event
*
*     Author:  Hywel Phillips (H.T.Phillips@rl.ac.uk)
*
*
*****************************************************************************
*****************************************************************************

      IMPLICIT NONE
* switched to NMXHEP = 4000 for compatibility with PYTHIA 6.1
c M.Dobbs 04.2000 NMXHEP = 2000 for compatibility with PYTHIA 5.7
      INTEGER NMXHEP
      PARAMETER (NMXHEP = 4000)
      INTEGER NEVHEP, NHEP, ISTHEP(NMXHEP), IDHEP(NMXHEP),
     &     JMOHEP(2,NMXHEP), JDAHEP(2,NMXHEP)
      DOUBLE PRECISION PHEP(5,NMXHEP), VHEP(4,NMXHEP)
      COMMON /HEPEVT/ NEVHEP, NHEP, ISTHEP, IDHEP, JMOHEP, JDAHEP,
     &     PHEP, VHEP

* The spin part of the HEPEVT commons which is not written
      DOUBLE PRECISION SHEP(4,NMXHEP)
      COMMON /HEPSPN/ SHEP

* The HEPEVTCH commonblock.
      REAL CHHEP(NMXHEP)
      COMMON /HEPEVTCH/ CHHEP
      SAVE /HEPEVTCH/

      INTEGER LUN,I,J,K
*
*   write event data
*

      IF (NHEP.GT.NMXHEP) THEN
        WRITE(6,*) 'LCWRITE: Error- NHEP ',NHEP,' is too big'
        WRITE(6,*) 'LCWRITE: Event not written to file'
        RETURN
      ENDIF

      WRITE(LUN,10) NHEP
      DO I=1,NHEP
        WRITE(LUN,12,ERR=999) ISTHEP(I),IDHEP(I),(JMOHEP(J,I),J=1,2),
     &  (JDAHEP(K,I),K=1,2)
        WRITE(LUN,11,ERR=999) (PHEP(J,I),J=1,5)

*       slightly modified to include charge instead of useless vhep(5,i)
        WRITE(LUN,11,ERR=999) (VHEP(K,I),K=1,4),CHHEP(I)
      ENDDO

 10   FORMAT(1I5)
 11   FORMAT(10F17.10)
 12   FORMAT(I9,I9,4I5)
      RETURN

 999  WRITE(6,*) 'LCWRITE: Error writing to file on unit ',LUN
      STOP
      END

EOF

# make the life simpler for pythia (based on cernlib 2002/2003)
# requires that the location of the cernlib script is in $PATH
PYTHIALIB=`cernlib pythia6205 pdflib804 lapack3 mathlib`

# echo g77 -o /tmp/pyhepevt.bin /tmp/pyhepevt.f  ${PYTHIALIB}
g77 -o /tmp/pyhepevt.bin /tmp/pyhepevt.f  ${PYTHIALIB}

# and run
rm -f pythia.data pythiahepevt.log pyhepevt.log
/tmp/pyhepevt.bin > pyhepevt.log
# if logging is not wanted for this task activate next line
#rm pythiahepevt.log pyhepevt.log

