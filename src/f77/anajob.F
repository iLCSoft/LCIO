***********************************************************
*
      program anajob
*
*     A program to demonstrate the use of the
*     fortran interface to LCIO for reading LCIO files.
*
*     H. Vogt  30/09/2003
*     modified by F. Gaede 10/30/2003
***********************************************************
      implicit none

#include "lciof77api.inc"
#include "lciof77apiext.inc"

      PTRTYPE reader, event, runhdr, detnames, pcolnamev, pcol, pobject
      
      integer status, irun, ievent
      integer i, k, ii, nactdet, ncolname
      character*80 detname

      character*40 colname, colnamev(20)
      character*40 colname1, colname2
      integer nintv, nfloatv, nelem, ncolnames
      integer intv(10)
      real    floatv(10)

      reader = lcrdrcreate()
c      write(*,*) " created reader at " , reader
      
      status = lcrdropen( reader, 'simjob.slcio' )
      
      if( status.eq.LCIO_ERROR) then
         write(*,*) " could not open file simjob.slcio "
         write(*,*) " first one has to run simjob (c++ executable) "
         goto 99
      else
         write(*,*) " opened file simjob.slcio "
      endif
      

c---- first loop over runs and print information
      do  k = 1,10000
         runhdr = lcrdrreadnextrunheader( reader, LCIO_READ_ONLY )
         if( runhdr.eq.0 ) goto 9
         
         write(*,*) "  Run : ",lcrhdgetrunnumber( runHdr )
     $        ,lcrhdgetdetectorname( runHdr )
     $        ,lcrhdgetdescription( runHdr )
         
         write(*,*) " active subdetectors : "
         detnames = lcrhdgetactivesubdetectors( runHdr ) 
         nactdet = lcsvcgetlength( detnames  ) 
         do i=1,nactdet
            write(*,*) lcsvcgetstringat( detnames, i ) 
         enddo
c---- check string vector access of extended f77api
         ncolnames = 20
         status    = lcgetstringvector
     &               ( detnames, colnamev, ncolnames, len(colnamev(1)) )
         write(*,*) 'testing lcgetstringvector'
         write(*,*) 'subdetectors found: ', ncolnames
         write(*,*) (colnamev(i),i=1,ncolnames)

      enddo
 9    continue

      status = lcrdrclose( reader ) 

      write(*,*) "-----------------------------------------------------"
      status = lcrdropen( reader, 'simjob.slcio' )
      
      if( status.eq.0) then
         write(*,*) " could not reopen file simjob.slcio "
         goto 99
      else
         write(*,*) " reopened file simjob.slcio "
      endif


c     ---- event loop ------
      do k = 1,10000
         
         event = lcrdrreadnextevent( reader )
         if( event.eq.0 ) goto 11 

c        print only 1st event
         if (k .ne. 1)   go to 10

*        get collection names
         pcolnamev = lcevtgetcollectionnames( event )
         ncolname  = lcsvcgetlength( pcolnamev )
         print *, 'collection names found'
         do  i = 1,ncolname
           colname = lcsvcgetstringat(pcolnamev , i )
           print *,  colname
         end do

c        check access to vectors (int, float)
         colname1 = 'TPC4711UserIntExtension'
         colname2 = 'TPC4711UserFloatExtension'
         pcol    = lcevtgetcollection( event , colname2)
         nelem   = lccolgetnumberofelements( pcol )
         print *, 'testing lcgetfloatvector'
         print *, 'collection TPC4711UserFloatExtension has ',
     &            nelem, ' elements'
         do  i = 1,nelem
           pobject = lccolgetelementat( pcol, i )
           nfloatv = 10
           status  = lcgetfloatvector( pobject, floatv, nfloatv )
           print '(a,i3,a,i3,a,10f10.5)',
     &              'floatv ', i, ' has ', nfloatv, ' elements: ',
     &              (floatv(ii),ii=1,nfloatv)
         end do

         pcol    = lcevtgetcollection( event, colname1 )
         nelem   = lccolgetnumberofelements( pcol )
         print *, 'testing lcgetintvector'
         print *, 'collection TPC4711UserIntExtension has ', 
     &            nelem, ' elements'
         do  i = 1,nelem
           pobject = lccolgetelementat( pcol, i )
           nintv   = 10
           status  = lcgetintvector( pobject, intv, nintv )
           print '(a,i3,a,i3,a,10i10)', 
     &              'intv ', i, ' has ', nintv, ' elements: ',
     &              (intv(ii),ii=1,nintv)
         end do

         status = lcdumpevent( event )

 10     enddo
 11   continue
c     ---- end event loop ------


      status = lcrdrclose( reader )
      write(*,*) " closed file simjob.sio - status : " , status

 99   continue
      end

