c**********************************************************
c
      program pyanajob
c
c Trivial test program to demonstrate the use of LCIO 
c from Fortran
c 
c H. Vogt  30/09/2003
c**********************************************************
      implicit none

#include "hepevt.inc"
#include "hepevt1.inc"

C This may be architecture dependent
#define PTRTYPE INTEGER

      PTRTYPE lccreatereader, reader
      PTRTYPE lcreadnextevent, event
      PTRTYPE lcgetcollection, col
      PTRTYPE lcgetelement, mcparticlev(1000)
      PTRTYPE lcgetsimcalohit, hit
      PTRTYPE          pmcp

      integer lcreaderopen, lcreaderclose, status
      integer lcgeteventheader, irun, nrun, nrunmax,
     &        ievt, ievent, itim
      character*100 lcgettypename, coltypename
      integer lcgetnumberofelements, lcgetsimtrackerhit
      integer lcgethepevt
      integer lcdumpevent, lcdumpeventdetailed
      integer lcgetsimcalohitnmcp, nmcpc, lcgetsimcalohitcmcp
      integer lcgetmcparticlecont, hepevtst, ndaughters

      integer          i, j, k, length, lenocc
      integer          ncols, ncolvec

      integer          cellid, cellid0, cellid1, pdg
      real             dedx, time, energy, posv(3), momv(3), mass
      double precision dposv(3)

      integer      linfile
      character*80 infile
      character*20 detname
      parameter    (ncols=7)
      character*40 colnames(ncols)
      data         colnames /'MCParticle',
     &             'ECAL007', 'TPC4711', 'TPC4711UserFloatExtension',
     &             'TPC4711UserIntExtension', 'AdditionalExtension',
     &             'TPCRawFADC'                                       /


      infile  = 'pysimjob.slcio'
      linfile = lenocc( infile )

      reader  = lccreatereader()
      write(*,*) ' created reader at ' , reader

c     ---- run loop ------
      nrunmax = 5
      do irun = 1,nrunmax

      status = lcreaderopen( reader, infile )

      if( status.eq.0) then
         print '(2a)', ' could not open file ', infile(1:linfile)
         return
      else
         print '(2a)', ' opened file ', infile(1:linfile)
      endif

c     ---- event loop ------
      do   ievt = 1,100000
         event = lcreadnextevent( reader )
         if( event.eq.0 ) goto 10

          print '(a,z8)', ' event pointer: ',  event
                           
         status = lcgeteventheader( event , nrun , ievent , itim ,
     &                              detname )


         write(*,*) "  run: ",nrun,", evt: ",ievent,
     &              " det: ",detname(1:lenocc(detname))

C ***    Check on collections  for first event of each run
         if (ievent .eq. irun)  then
           status = lcdumpevent( event )
           print '(a,/,/)' , '****************************************'
           do i = 1,ncols
             col = lcgetcollection ( event ,
     &               colnames(i)(1:lenocc(colnames(i)))//char(0) )
             if (col .ne. 0)  then
               ncolvec = lcgetnumberofelements ( col )
               coltypename = lcgettypename ( col )
               print '(2a,i5,2a)', colnames(i)(1:lenocc(colnames(i))),
     &                             ' collection with ', ncolvec,
     &                             ' elements of type ',
     &                             coltypename(1:lenocc(coltypename))

               if (coltypename(1:lenocc(coltypename)) .eq.
     &             'MCParticle')  then
                 print '(a)' , 'MCParticle: pointer - content'
                 do  j  = 1,ncolvec
                    mcparticlev(j) = lcgetelement ( col, j )
                    status  = lcgetmcparticlecont ( mcparticlev(j) ,
     &                pdg , hepevtst , dposv, momv , mass , ndaughters)
                    print '(z10,3h - ,2i6,3d12.5,4e10.3,i3)' ,
     &                mcparticlev(j) ,
     &                pdg , hepevtst , dposv , momv , mass , ndaughters
                 enddo
                 print '(a,a)' , 'MCParticle pointers summary: '
                 print '(8z10)'  , (mcparticlev(k),k=1,ncolvec)

c ***            for MCParticle file the HEPEVT common and print
                 status = lcgethepevt( event )
                 if (status .eq. 0)  then
                   print '(a,a,i5)'
     &                 , 'PYSIMJOB: error - can not set '
     &                 , 'hepevt common content for event ', i
                 else
                   call lclist
                 endif

               endif


               if (coltypename(1:lenocc(coltypename)) .eq. 
     &             'SimTrackerHit')  then
                 print '(a)' , 'collection elements have:'
                 do j = 1,ncolvec
                   status = lcgetsimtrackerhit
     &               ( col , j , cellid , dposv , dedx , time , pmcp )
                   print '(i3,3d12.5,2e10.3,z10)',
     &               cellid , dposv , dedx , time , pmcp
                 enddo
               endif

               if (coltypename(1:lenocc(coltypename)) .eq. 
     &             'SimCalorimeterHit')  then
                 print '(a)' , 'collection elements have:'
                 do j = 1,ncolvec
                   hit = lcgetsimcalohit
     &               ( col , j , cellid0 , cellid1 , energy, posv )
                   print '(2i11,4e10.3)',
     &               cellid0 , cellid1 , energy, posv
                   nmcpc = lcgetsimcalohitnmcp( hit )
                   print '(a)' , '     particle contributions  have:'
                   do k = 1,nmcpc
                     status = lcgetsimcalohitcmcp
     &                 ( hit , k , pmcp , energy , time , pdg )
                     print '(5x,z10,2e10.3,i6)',
     &                  pmcp , energy , time , pdg
                   enddo
                 enddo
               endif

             else
               print '(3a)', 'no ', colnames(i)(1:lenocc(colnames(i))),
     &                     ' collection found'
             endif
           enddo

         endif

         
      enddo
c     ---- end event loop ------

10    continue
      status = lcreaderclose( reader )
      print '(3a,i1)', ' closed file ', infile(1:linfile),
     &              ' - status : ', status
      print '(a)',  '*************************************************'
      enddo
c     ---- end run loop ------
*
      end
*
      SUBROUTINE LCLIST
*****************************************************************************
*****************************************************************************
*
*     SUBROUTINE LCLIST(LUN)
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

#include "hepevt.inc" 
#include "hepevt1.inc"

      INTEGER LUN,I,J,K,L,M
*
*   write event data
*
      LUN = 6
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
     &       '        Vx        Vy        Vz      time')

 12   FORMAT(1X,I5,I4,I6,4I6,5F10.3,4E10.2)
      RETURN

 999  WRITE(6,*) 'LCLIST: Error writing to file on unit ',LUN
      STOP
      END
*
*****************************************************************************
*****************************************************************************
*
*  Utility package taken from cernlib 
*
*****************************************************************************
*****************************************************************************
*
      INTEGER FUNCTION LENOCC (CHV)
C
C CERN PROGLIB# M507    LENOCC          .VERSION KERNFOR  4.21  890323
C ORIG. March 85, A.Petrilli, re-write 21/02/89, JZ
C
C-    Find last non-blank character in CHV

      CHARACTER    CHV*(*)

      N = LEN(CHV)

      DO 17  JJ= N,1,-1
      IF (CHV(JJ:JJ).NE.' ') GO TO 99
   17 CONTINUE
      JJ = 0

   99 LENOCC = JJ
      RETURN
      END
*
