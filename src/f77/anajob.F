c**********************************************************
c
      program anajob
c
c Trivial test program to demonstrate the use of LCIO 
c from Fortran
c gaede 03/13/2003
c modified H. Vogt  30/09/2003
c**********************************************************
      implicit none

C This may be architecture dependent
#define PTRTYPE INTEGER

      PTRTYPE lccreatereader, reader
      PTRTYPE lcreadnextevent, event
      PTRTYPE lcgetcollection, col
      PTRTYPE lcgetelement, mcparticlev(500)
      PTRTYPE lcgetsimcalohit, hit
      PTRTYPE pmcp

      integer lcreaderopen, lcreaderclose, status
      integer lcgeteventheader, irun, ievent, itim 
      integer lcgetnumberofelements, lcgetsimtrackerhit
      integer lcdumpevent, lcdumpeventdetailed
      integer lcgetsimcalohitnmcp, nmcpc, lcgetsimcalohitcmcp
      integer lcgetmcparticlecont, hepevtst, ndaughters

      character*100 lcgettypename, coltypename
      character*20  detname

      integer          i, j, k, length, lenocc
      integer          ncols, ncolvec
      integer          cellid, cellid0, cellid1, pdg
      real             dedx, time, energy, posv(3), momv(3), mass
      double precision dposv(3)

      parameter (ncols=7)
      character*40 colnames(ncols)
      data colnames /'MCParticle',
     &               'ECAL007', 'TPC4711', 'TPC4711UserFloatExtension',
     &               'TPC4711UserIntExtension', 'AdditionalExtension',
     &               'TPCRawFADC'                               /

      reader = lccreatereader()
      write(*,*) " created reader at " , reader


      status = lcreaderopen( reader, 'simjob.slcio' )

      if( status.eq.0) then
         write(*,*) " could not open file simjob.slcio "
         goto 99
      else
         write(*,*) " opened file simjob.slcio "
      endif

c     ---- event loop ------
      do 10
         event = lcreadnextevent( reader )
         if( event.eq.0 ) goto 11 

         write(*,*) " read event : " , event
                           
         status = lcgeteventheader( event , irun , ievent , itim ,
     &                              detname )


         write(*,*) "  run: ",irun,", evt: ",ievent,
     &              " det: ",detname(1:lenocc(detname))

C ***    Check on collections  for first event of last run
         if (irun .eq. 9 .and. ievent .eq. 0)  then
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
         
 10   continue
 11   continue
c     ---- end event loop ------


      status = lcreaderclose( reader )
      write(*,*) " closed file simjob.sio - status : " , status

 99   continue
      end

      INTEGER FUNCTION LENOCC (CHV)
C
C CERN PROGLIB# M507    LENOCC          .VERSION KERNFOR  4.21  890323
C ORIG. March 85, A.Petrilli, re-write 21/02/89, JZ
C
C-    Find last non-blank character in CHV

      CHARACTER    CHV*(*)

      N = LEN(CHV)

      DO 17  JJ= N,1,-1
      IF ((CHV(JJ:JJ).NE.' ').AND.(CHV(JJ:JJ).NE.CHAR(0))) GO TO 99
   17 CONTINUE
      JJ = 0

   99 LENOCC = JJ
      RETURN
      END
*
