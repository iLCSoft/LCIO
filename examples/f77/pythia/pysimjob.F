 
**********************************************************************
*                                                                    *
      PROGRAM PYSIMJOB                                              !*
*     PYSIMJOB                                                       *
*     A program to demonstrate the use of LCIO                       *
*     including the StdHep COMMON for MCParticle info                *
*     as a check that the program works as intended.                 *
*                                                                    *
*     The StdHep COMMON is filled here by                            *
*     reading the file pythia.data written using a modified          *
*     version of LCWRITE including particle charges.                 *
*                                                                    *
*     TO produce the file pythia.data use the script pythiahepevt.sh *
*                                                                    *
*     H. Vogt 30/09/2003                                             *
**********************************************************************
      implicit none
 
#include "lciof77api.inc"
#include "lciof77apiext.inc"

      PTRTYPE writer, reader, event
      PTRTYPE header, col, mcparticle
      integer status

      character*80 filename
      character*80 detname, comment, subdet

      integer linfile, lfilename, getstring, lenocc
      integer i, j, k, irun, iev, itim, locf

C *** Subdetector arrays definitions (must be consistent)- be very carfully !!!
C     usage in lcwriterunheader (brdets, ndets, chlen) 
C     and lcreadnextrunheader (subdets, nsubd(io), chlenout)

      integer len
      integer ndets
      parameter (ndets=15)
ccc      integer chlen
ccc      parameter (chlen=4)
      character*4 brdets(ndets), chtmp
      data brdets /'vtx', 'itc', 'ftd', 'sit', 'tpc', 'bcal', 'ecal',
     &             'coil', 'yoke', 'lat',  'mask', 'bpip', 'fch', 
     &             'quad', 'hall'/

      integer nsubd, nsubdio
      parameter (nsubd=50)
ccc      integer chlenout
ccc      parameter (chlenout=4)
      character*4 subdets(nsubd)

C
C *** Local definitions
      character*80 infile
      integer      lunin, ieorun
      integer      length

C *** Filename of LCIO file
      filename = 'pysimjob.slcio'
      lfilename = lenocc( filename )

C *** for skipping pass 1 - go to 100
ccc      go  to  100
C
C============================================================================
C *** PASS1 PASS1 PASS1
C============================================================================

      filename = 'pysimjob.slcio'
      lfilename = lenocc( filename )

C *** Open LCIO StdHep input file
      infile = 'pythia.data'
      linfile = lenocc ( infile )
      lunin  = 1

      print *, ' '
      print '(4a)' , 'PYSIMJOB: first pass - read file ''' ,
     &                infile(1:lenocc(infile)) , 
     &                ''' and write LCIO file ''', 
     &                filename(1:lfilename) ,''''
      print '(a)' , '**************************************************'
      open (unit=lunin, status='old',
     &      file=infile, form='formatted', err= 900)

c *** Set charge vector for hepevt common to zero
      do i = 1,nmxhep
        chhep(i) = 0.
      end do

C *** Open LCIO output
      writer = lcwrtcreate()
      status = lcwrtopen( writer, filename, LCIO_WRITE_NEW )
      if (status .eq. 0)  then
        print *, 'PYSIMJOB: error - lcio file could not be opened'
        stop
      else
        print '(a,a)', 'PYSIMJOB: file opened: ', filename(1:lfilename)
      end if

C *** Create, fill, and write Runheader
      irun   = 9999
      status = lcwriterunheader( 
     &           writer , irun , 'Tesla Detector' ,
     &           'Pythia generated events' , 
     &           brdets , ndets , len( brdets(1) ) )

C
C *** Loop over events to be generated.
C
      itim   = 20030923
      iev    = 0
10    continue

C *** Read one pythia event written by LCWRITE into common hepevt
      call readstdhep ( lunin, ieorun )
      if (ieorun .eq. 0)  then
C ***   Set event # in common hepevt
        iev    = iev + 1
        nevhep = iev
cc        CALL LCLIST

C ***   Create event
        event = lcevtcreate()
        
C ***   Write event header 
        status = lcseteventheader( event , irun , iev , itim , 
     &                             'Tesla Detector' )

C ***   Convert hepevt common content to LCIO MCParticle collection
        status = hepevt2lcio( event )

        IF (status .eq. 0)  THEN
          print *, 'PYSIMJOB: error - can not write hepevt common'
     &           , ' for event ', iev
        END IF

C ***    Check on MCParticle collection
         col = lcevtgetcollection ( event , LCIO_MCPARTICLE )

         if (col .ne. 0)  then

           status = lcio2hepevt( event )
           if (status .eq. 0)  then
             print '(a,a,i5)'
     &             , 'PYSIMJOB: error - can not set '
     &             , 'hepevt common content for event ', i
             stop
           endif

           status = lccolgetnumberofelements ( col )
           print '(a,i5,a,z8,a,i5,a)' ,
     &           'Event ', iev , ' at pointer ' , event ,
     &           ' has a MCParticle collection with ', status ,
     &           ' elements'
           do  j  = 1,status
             mcparticle = lccolgetelementat( col, j )
           enddo
         else
             print '(a)', 'no MCParticle collection found'
         endif

C ***   Write this event to lcio output file
        status = lcwrtwriteevent( writer , event )
        if (status .eq. 0)  then
          print *, 'PYSIMJOB: error - can not write event', iev
          stop
        else
        end if

C ***   Delete event
        status = lcevtdelete( event )
      end if
      if (ieorun .eq. 0)  go  to  10
C
C *** EOF on input file seen, close files, delete writer
C

      status = lcwrtclose( writer )
      print '(a,a,a,i2)', ' PYSIMJOB: file ', filename(1:lfilename)
     &                  , ' closed, status = ' , status
      status = lcwrtdelete( writer )
      close ( unit=lunin )

C============================================================================
C *** PASS2 PASS2 PASS2
C============================================================================
100   continue

      filename = 'pysimjob.slcio'
      lfilename = lenocc( filename )

      print *, ' '
      print '(3a)' , 'PYSIMJOB: second pass - read file ''' ,
     &               filename(:lfilename), ''' and dump events read'
      print '(a)' , '**************************************************'

C *** now reopen the output as input
      reader = lcrdrcreate()
      status = lcrdropen( reader, filename )
      if( status.eq.0) then
         print '(a,a,a)', 'PYSIMJOB: error - can not open file '''
     &                  , filename(1:lfilename), ''' for reading'
         stop
      else
         print '(a,a)', 'PYSIMJOB: opened file: ', filename(1:lfilename)
      endif

C *** Read Runheader info
      irun    = 0
      detname = ' '
      comment = ' '
      do i = 1,nsubd
        subdets(i) = ' '
      enddo

C     nsubdio is I/O argument !
      nsubdio = nsubd 
      header = lcreadnextrunheader( reader,
     &           irun, detname, comment , 
     &           subdets , nsubdio, len( subdets(1) ) )

      print '(/a)', "run header info"
      print '(a,i5)', "  run: ", IRUN
      print '(a,a)',  "  det: ", detname(1:lenocc(detname))
      print '(a,a)',  "  comment: ", comment(1:lenocc(comment))
                       
      print '(a)', "subdetectors:"
      do  i = 1,nsubdio
        print '(a,a)', "  ", subdets(i)
      end do
      print '(a,/)', "run header info finished"

C *** Event loop **************************************************************
      do  i = 1,1000000
         event = lcrdrreadnextevent( reader , LCIO_READ_ONLY )
         if( event.eq.0 ) goto 110

C ***    Get event header
         irun    = 0
         iev     = 0
         itim    = 0
         detname = ' '
         status  = lcgeteventheader( event , irun , iev , itim ,
     &                              detname )
        
         print '(/,a)' , '*********************************************'
         print '(a,z8)', 'event pointer for this event is: ', event
         print '(a,i5,a,i5,a,a)', "  run: ",irun,", evt: ",iev
     &         , ", det: ",detname(1:lenocc(detname))
         print '(a)', '************************************************'

C ***    Check on MCParticle collection
         col = lcevtgetcollection ( event , 'MCParticle'//char(0) )
         if (col .ne. 0)  then
           status = lcio2hepevt( event )
           if (status .eq. 0)  then
             print '(a,a,i5)'
     &             , 'PYSIMJOB: error - can not set '
     &             , 'hepevt common content for event ', i
           endif
           status = lccolgetnumberofelements ( col )
           print '(a,i5,a)', 'MCParticle collection with ', status
     &           , ' elements'
           do  j  = 1,status
             mcparticle = lccolgetelementat ( col, j )
           enddo
         else
             print '(a)', 'no MCParticle collection found'
         endif

C ***    extended printout for the first 3 events
         if (i .lt. 4)  then
C          List content of StdHep common
           call lclist
C          and all MCParticle pointers
           print '(a)', "all pointers of MCParticles;"
           print '(10z8)', (mcpointerv(j),j=1,nhep)
         endif
      enddo

C *** End of event loop *******************************************************
110   continue

C *** EOF on input, close and delete reader and stop
      print *, 'eof on input file ', filename(1:lfilename)
      status = lcrdrclose( reader )
      print '(a,a,a,i2)', ' PYSIMJOB: file ', filename(1:lfilename)
     &                  , ' closed, status = ' , status
      status = lcrdrdelete( reader )
      print *, 'Job finished !'
      stop

C *** Open error for pythia.data input file
900   continue
      print '(3a)', 'PYSIMJOB: file ', infile(1:linfile),
     &              ' does not exist'
      print '(a)',  'Use ''pythiahepevt.sh'' to create this file'
      stop
C
      end
*  
      SUBROUTINE READSTDHEP (LUN, IEORUN)
*****************************************************************************
*****************************************************************************
*
*  Reading of events written by LCWRITE 
*
*  Arguments:
*    LUN       logical unit number for input file
*    IEORUN    flag for EOF on input file
*
*****************************************************************************
*****************************************************************************
*

      IMPLICIT NONE

#include "hepevt0.inc"
#include "hepevt1.inc"

      INTEGER LUN, IEORUN
      INTEGER I, J, K, L, ICHECK
      INTEGER PYCHGE

* Read one event
      IEORUN = 0
      READ (LUN, 10, IOSTAT=ICHECK) NHEP

      DO I = 1 , NHEP
        READ (LUN, 12, IOSTAT=ICHECK, END=999) ISTHEP(I),
     +        IDHEP(I), (JMOHEP(J,I),J=1,2), (JDAHEP(L,I),L=1,2)
        READ (LUN, 11, IOSTAT=ICHECK, END=999) (PHEP(J,I),J=1,5)
        READ (LUN, 11, IOSTAT=ICHECK, END=999) (VHEP(L,I),L=1,4),
     +                                         CHHEP(I)

c$$$        write(*,1234) i,ISTHEP(I),IDHEP(I),(JMOHEP(J,I),J=1,2),
c$$$     $       (JDAHEP(L,I),L=1,2)
      END DO

c$$$ 1234 FORMAT(I3,I5,I9,4I5)

      RETURN
*
  999 CONTINUE
      PRINT *, 'EOF on HEPEVT input on unit', LUN
      IEORUN = 1
      RETURN
*
 10   FORMAT(1I5)
 11   FORMAT(10F17.10)
 12   FORMAT(I9,I9,4I5)
*
      END
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
*              ( provided by Hywel Phillips (H.T.Phillips@rl.ac.uk))
*
*
*****************************************************************************
*****************************************************************************

      IMPLICIT NONE

#include "hepevt0.inc"
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
     &       '         Vx         Vy         Vz       time')

 12   FORMAT(1X,I5,I4,I6,4I6,5F10.3,4E11.3)
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
