***********************************************************
*
      program anajob
*
*     A program to demonstrate the use of the
*     fortran interface to LCIO for reading LCIO files.
*
*     H. Vogt  30/09/2003
*     modified by F. Gaede 10/30/2003
*     Bug corrected by H. Vogt  03/19/2004
*
***********************************************************
      implicit none

#include "lciof77api.inc"
#include "lciof77apiext.inc"

      PTRTYPE reader, event, runhdr, detnames 
      PTRTYPE pcolnamev, pcol, pobject
      
      integer nargs, nFiles, nEvents
      integer status, irun, ievent
      integer i, k, ii, nactdet, ncolname
      character*80 detname, descr

      character*40 colname, colnamev(20), filenames(20)
      character*40 colname1, colname2
      integer nintv, nfloatv, nelem, ncolnames
      integer intv(10)
      real    floatv(10)


c---- read file names from command line (only argument)
      nargs = iargc()
      if (nargs .lt. 1)  then
        print *, ' usage:  anajob <input-file1> [[input-file2],...]'
        stop
      else if (nargs .gt. 20)  then
        print *, ' usage: more than max = 20 file names given'
        stop
      end if
      do  i = 1, nargs
        call getarg (i, filenames(i))
      end do
      nFiles = nargs

      reader = lcrdrcreate()

c---- first we read the run information

c---- for reading from one file only use sth. like:
c---- filename = 'recjob.slcio'
c---- status = lcrdropen( reader, filename )
      status = lcrdropenchain( reader, filenames(1), nFiles,
     &           len(filenames(1)) )
      
      if( status .eq. LCIO_ERROR ) then
         write(*,*) ' could not open files '
         stop
      else
         write(*,*) 'will open and read from files: '
         write(*,'(5x,a)')
     &              ( filenames(i)(1:lnblnk(filenames(i)) ),i=1,nFiles)
      endif
      

c---- loop over all run headers
    1 continue
      runhdr = lcrdrreadnextrunheader( reader, LCIO_READ_ONLY )
      if( runhdr .eq. 0 )  go to 10
         
       status = lcdumprunheader( runhdr )
ccc      detname = lcrhdgetdetectorname( runHdr )
ccc      descr   = lcrhdgetdescription( runHdr )
ccc      write(*,'(a,i2,4a)') '  Run :', lcrhdgetrunnumber( runHdr ),
ccc     &                     ' - ', detname(1:lnblnk(detname)),
ccc     &                     ':  ', descr(1:lnblnk(descr))
         
c---- this is additional compared to anajob.cc
ccc      write(*,*) " active subdetectors : "
ccc      detnames = lcrhdgetactivesubdetectors( runHdr )
ccc      nactdet = lcsvcgetlength( detnames  )
ccc      do i=1,nactdet
ccc        write(*,*) lcsvcgetstringat( detnames, i )
ccc      end do
c---- check string vector access of extended f77api (additional compared to anajob.cc)
ccc      ncolnames = 20
ccc      status    = lcgetstringvector
ccc     &               ( detnames, colnamev, ncolnames, len(colnamev(1)) )
ccc      write(*,*) 'testing lcgetstringvector'
ccc      write(*,*) 'subdetectors found: ', ncolnames
ccc      write(*,*) (colnamev(i),i=1,ncolnames)

        go  to  1

   10 continue

      status = lcrdrclose( reader ) 

c---- now loop over the file(s) again and dump event data

      status = lcrdropenchain( reader, filenames(1), nFiles,
     &           len(filenames(1)) )
      write(*,*)
      if ( status .eq. 0 ) then
        write(*,*) " could not reopen files "
        stop
      else
         write(*,*) 'will reopen and read from files: '
         write(*,'(5x,a)')
     &              ( filenames(i)(1:lnblnk(filenames(i)) ),i=1,nFiles)
      endif

      nEvents = 0


c---- ---fg: debugging only:
c      status = lcrdrskipnevents(  reader, 97 ) 

c---- ---- event loop ------

   20 continue
      event = lcrdrreadnextevent( reader, LCIO_READ_ONLY )
      if( event .eq. 0 )  go to 30

      status = lcdumpevent( event )
      nEvents = nEvents + 1
        go  to  20

c----    the following is scipped but may be used for methods tests
c        print only 1st event

         if (k .ne. 1)   go to 110

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

  110    continue

c---- -------- end of event loop -----------
   30 continue
      write(*,*)
      write(*,*) '', nEvents, ' events read from files: '
      write(*,'(5x,a)') 
     &              ( filenames(i)(1:lnblnk(filenames(i)) ),i=1,nFiles)



      status = lcrdrclose( reader )
      status = lcrdrdelete( reader )

      end

