**********************************************************************
*                                                                    *
      PROGRAM SIMJOB_CHAIN                                           !*
*                                                                    *
*     A program to demonstrate the use of the                        *
*     low level and the extended level                               *  
*     fortran interface to LCIO for writing LCIO files.              *
*                                                                    *
*     F.Gaede 09/10/2003                                             *
*     H.Vogt  14/06/2004  (modified)                                 *
**********************************************************************
      implicit none

#include "lciof77api.inc"
#include "lciof77apiext.inc"

      PTRTYPE reader, writer

      integer ifile,  nfiles, status, jj
      parameter (nfiles=4)
      character*8  filename
      character*16 filenamei(nfiles)
      character*1  chnum

c------  write several slcio output files
      filename = "f_simjob"
      do ifile = 1,nfiles
        write (chnum,'(i1)') ifile
        filenamei(ifile) = filename//chnum//'.slcio'
        call simjob( ifile, filenamei(ifile) )
      end do

c------  read a chain of slcio files and analyze then (1 pass only)
      reader = lcrdrcreate()
c      write(*,*) " created reader at " , reader
      
      status = lcrdropenchain( reader, filenamei(1), nfiles, 
     &                         len(filenamei(1)) )
      
      if( status.eq.LCIO_ERROR) then
         write(*,*) " could not open file chain:"
         write(*,*) (filenamei(jj),jj=1,nfiles)
         goto 99
      else
         write(*,*) " opened file chain is:" 
         write(*,*) (filenamei(jj),jj=1,nfiles)
      endif

      call anajob ( reader )

      status = lcrdrclose( reader )
      write(*,*) " closed file simjob.sio - status : " , status
*
   99 continue
      stop
*
      end
 
**********************************************************************
*                                                                    *
      subroutine simjob ( ipass, filename )                         !*
*                                                                    *
*     A program to demonstrate the use of the low level              *
*     fortran interface to LCIO for writing LCIO files.              *
*                                                                    *
*     F.Gaede 09/10/2003                                             *
**********************************************************************
      implicit none
 
#include "lciof77api.inc"
#include "lciof77apiext.inc"

c---- variables
      PTRTYPE runheader, event, writer, sthcol, mcpcol, schcol, cahcol
      PTRTYPE stringvcol, floatvcol, intvcol, pstringv, pfloatv, pintv
      PTRTYPE  hit, mcp, mcpm, mcpd, mcpdd
      PTRTYPE  mcpv(50), pointerv(50), ppointerv
      integer npointers
      integer status
      integer i, j, k, irun, nrun, iev, nev, itimv(8), itim, locf
      integer nhit, flag
      character*10 dum1, dum2, dum3

      integer ipass
      character*16 filename
      character*10 detectorname
      real mcpp(3), schpos(3), cahpos(3)
      double precision sthpos(3)
      character*4 strvnames(10)
      integer      intvvals(10)
      real         realvvals(10)

      data strvnames /'ABCD', 'BCDE', 'CDEF', 'DEFG', 'EFGH',
     &                'abcd', 'bcde', 'cdef', 'defg', 'efgh'  /
      data intvvals  / 1234, 2345, 3456, 4567, 5678, 0, 0, 0, 0, 0 /
      data realvvals / 123.4, 234.5, 345.6, 456.7, 567.8,
     &                 0., 0., 0., 0., 0. /

C----- some constants
      detectorname = 'TeslaPPT'
      nev = 10
      nrun = 5
      nhit = 100

      print *, 'filename is: ', filename

      writer = lcwrtcreate()
      status = lcwrtopen( writer, filename , LCIO_WRITE_NEW )

c ----- create a few runs
      do irun=1,nrun

c----- write a run header first (run number is: ipass*10 +irun)
         runheader = lcrhdcreate()
         status = lcrhdsetrunnumber( runheader, irun + ipass*10 )
         status = lcrhdsetdetectorname( runheader, detectorname  )
         status = lcrhdsetdescription( runheader,
     $        ' simple test file for f77 interface - no physiscs' )

         status = lcrhdaddactivesubdetector( runheader, 'VXD')
         status = lcrhdaddactivesubdetector( runheader, 'TPC')
         status = lcrhdaddactivesubdetector( runheader, 'HCal')
         status = lcrhdaddactivesubdetector( runheader, 'HCalPPT')
         
         status =  lcwrtwriterunheader( writer, runheader ) 

c-----  event loop 
         do iev = 1,nev
            
            event = lcevtcreate()
            status = lcevtsetdetectorname( event, detectorname )
            status = lcevtsetrunnumber( event, irun ) 
            status = lcevtseteventnumber( event,  iev ) 

c           get a time stamp
            call date_and_time (dum1, dum2, dum3, itimv)
            itim  = (itimv(5)*100+itimv(6))*100+itimv(7)
            status = lcevtsettimestamp( event, itim )

            mcpcol = lccolcreate( LCIO_MCPARTICLE )
            sthcol = lccolcreate( LCIO_SIMTRACKERHIT )
            schcol = lccolcreate( LCIO_SIMCALORIMETERHIT )
            cahcol = lccolcreate( LCIO_CALORIMETERHIT )

            pstringv   = lcstringvectorcreate ( strvnames, 10, 4 )
            pfloatv    = lcfloatvectorcreate ( realvvals, 10 )
            pintv      = lcintvectorcreate ( intvvals, 10 )

c-------- create some monte carlo particles

            do i = 1,nhit/2
               mcp     = lcmcpcreate()
               mcpv(i) = mcp
               status  = lcmcpsetpdg( mcp, 123 )
               mcpp(1) = i*1.
               mcpp(2) = i*2.
               mcpp(3) = i*3.
               status = lcmcpsetmomentum( mcp, mcpp ) 

               status = lccoladdelement( mcpcol, mcp ) 

               do j=1,2
                  mcpd = lcmcpcreate()
                  status = lcmcpsetpdg( mcpd , 12345 ) 
                  mcpp(1) = j*1.
                  mcpp(2) = j*2.
                  mcpp(3) = j*3.
                  status = lcmcpsetmomentum( mcpd, mcpp ) 

                  status = lcmcpaddparent( mcpd , mcp ) 


                  status = lccoladdelement( mcpcol, mcpd ) 

                  do k=1,3
                     mcpdd = lcmcpcreate()
                     status = lcmcpsetpdg( mcpdd , 1234567 ) 
                     mcpp(1) = k*1.
                     mcpp(2) = k*2.
                     mcpp(3) = k*3.
                     status = lcmcpsetmomentum( mcpdd, mcpp ) 
                     
                     status = lcmcpaddparent( mcpdd , mcpd ) 
                     
                     status = lccoladdelement( mcpcol, mcpdd ) 
                  enddo
               enddo
            enddo

c------   simulated tracker hits

c--------   set the flag bits to store position and pdg for simulated calorimeter hits
            flag = 2**31 + 2**28
            status = lccolsetflag( schcol, flag )

            do k=1,nhit

               hit = lcsthcreate() 
               status = lcsthsetcellid( hit,  314159 ) 
               sthpos(1) = 1.
               sthpos(2) = 2.
               sthpos(3) = 3.
               status = lcsthsetposition( hit, sthpos )

c ---            assign a MCParticle to each hit
               mcp = lccolgetelementat( mcpcol, k ) 
               status = lcsthsetmcparticle( hit, mcp ) 

               status = lccoladdelement( sthcol, hit ) 

            enddo


c---- simulated calorimeter hits
            do k=1,nhit

               hit = lcschcreate() 
               status = lcschsetcellid0( hit,  314159 ) 
               status = lcschsetcellid1( hit,  314159*2 ) 
               schpos(1) = 1.
               schpos(2) = 2.
               schpos(3) = 3.
               status = lcschsetposition( hit, schpos )

c ---            add energy from 2 MCParticles to each hit

               mcp = lccolgetelementat( mcpcol, k ) 
               status = lcschaddmcparticlecontribution
     $              ( hit, mcp, 0.001, 0. , 1234 ) 

               mcp = lccolgetelementat( mcpcol, k+1 ) 
               status = lcschaddmcparticlecontribution
     $              ( hit, mcp, 0.007, 0. , 4711 ) 

               status = lccoladdelement( schcol, hit ) 
            enddo


c---- real data calorimeter hits

c ----      set the flag bits to store position for calorimeter hits
            flag = 2**31
            status = lccolsetflag( cahcol, flag )

            do k=1,nhit
               hit = lccahcreate() 
               status = lccahsetcellid0( hit,  314159 ) 
               status = lccahsetcellid1( hit,  314159*2 ) 
               cahpos(1) = 11.
               cahpos(2) = 22.
               cahpos(3) = 33.
               status = lccahsetposition( hit, cahpos )
               status = lccahsetenergy( hit, 2.*k ) 

               status = lccoladdelement( cahcol, hit ) 
            enddo

c-----     now we will check the stl pointer vector
           if (irun .eq. 1 .and. iev .eq. 1)  then
             print *, 'pointers going into the pointer vector'
#if defined(__ia64__)
             print '(5z20)', (mcpv(i), i=1,nhit/2)
#else
             print '(10z10)', (mcpv(i), i=1,nhit/2)
#endif
             ppointerv = pointervectorcreate( mcpv, nhit/2 )
             npointers = pointervectorgetlength( ppointerv )
             status = getpointervector( ppointerv, pointerv, npointers)
             print *, 'pointers got from the pointer vector'
#if defined(__ia64__)
             print '(5z20)', (pointerv(i), i=1,npointers)
#else
             print '(10z10)', (pointerv(i), i=1,npointers)
#endif
             status = pointervectordelete( ppointerv )
           end if

            
c-----     now add everything to the event
            status = lcevtaddcollection(event,mcpcol ,'MCParticle') 
            status = lcevtaddcollection(event,sthcol ,'MyTrackerHits') 
            status = lcevtaddcollection(event,schcol ,'MyCalHits') 
            status = lcevtaddcollection(event,cahcol ,'MyRealCalHits') 

            stringvcol = lccolcreate( LCIO_LCSTRVEC )
            floatvcol  = lccolcreate( LCIO_LCFLOATVEC )
            intvcol    = lccolcreate( LCIO_LCINTVEC )

            status = lccoladdelement( stringvcol, pstringv )
            status = lccoladdelement( floatvcol, pfloatv )
            status = lccoladdelement( intvcol, pintv )

            status = lcevtaddcollection(event,stringvcol,'MyStringVec')
            status = lcevtaddcollection(event,floatvcol,'MyFloatVec')
            status = lcevtaddcollection(event,intvcol,'MyIntVec')
            
            status = lcwrtwriteevent( writer , event )

            if (status .eq. LCIO_ERROR )  then
               print *, 'SIMJOB: error - can not write event', iev
               stop
            end if
            
c------- need to delete the event as we created it
            status = lcevtdelete( event )

         enddo
      enddo
      status = lcwrtclose( writer ) 
      status = lcwrtdelete ( writer)
      write(*,*)
      write(*,*) "----------------------------------------------------"
      write(*,*) "simjob is finished"
      write(*,*) "----------------------------------------------------"
      write(*,*)

C     
      end

***********************************************************
*
      subroutine anajob ( reader )
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

      PTRTYPE reader, event, runhdr, detnames
      PTRTYPE pcol1, pcol2, pcol3, pobject1, pobject2, pobject3
      
      character*16 filename
      integer status, irun, ievent
      integer i, nactdet, ii
      character*80 detname

      character*40 colname1, colname2, colname3
      integer      nfloatv, nintv, nstrv
      integer      intv(20)
      real         floatv(20)
      character*10 strv(20)

      write(*,*)
      write(*,*) "----------------------------------------------------"
      write(*,*) "anajob is started"
      write(*,*) "----------------------------------------------------"
      write(*,*)




      do 

c---- look for the run header and print information
         runhdr = lcrdrreadnextrunheader( reader, LCIO_READ_ONLY )
         if( runhdr.eq.0 ) goto 99
         
         write(*,*) "  Run : ",lcrhdgetrunnumber( runHdr )
     $        ,lcrhdgetdetectorname( runHdr )
     $        ,lcrhdgetdescription( runHdr )
         
         write(*,*) " active subdetectors : "
         detnames = lcrhdgetactivesubdetectors( runHdr ) 
         nactdet = lcsvcgetlength( detnames  ) 
         do i=1,nactdet
            write(*,*) lcsvcgetstringat( detnames, i ) 
         enddo

c---- read first event of this run and print floatvec and intvec collection
         event = lcrdrreadnextevent( reader, LCIO_READ_ONLY )
         colname1 = 'MyFloatVec'
         colname2 = 'MyIntVec'
         colname3 = 'MyStringVec'
         pcol1    = lcevtgetcollection( event , colname1 )
         pcol2    = lcevtgetcollection( event , colname2 )
         pcol3    = lcevtgetcollection( event , colname3 )
         pobject1 = lccolgetelementat( pcol1, 1 )
         pobject2 = lccolgetelementat( pcol2, 1 )
         pobject3 = lccolgetelementat( pcol3, 1 )

         nfloatv  = 20
         status   = lcgetfloatvector( pobject1, floatv, nfloatv )
         print '(a,i3,a,i3,a,10f10.5)',
     &              'floatv ', i, ' has ', nfloatv, ' elements: ',
     &              (floatv(ii),ii=1,nfloatv)
         nintv    = 20
         status   = lcgetintvector( pobject2, intv, nintv )
         print '(a,i3,a,i3,a,10i10)',
     &              'intv ', i, ' has ', nintv, ' elements: ',
     &              (intv(ii),ii=1,nintv)
         nstrv    = 20
         status   = lcgetstringvector
     &                ( pobject3, strv, nstrv, len( strv(1) ) )
         print '(a,i3,a,i3,a,10a10)',
     &              'strv ', i, ' has ', nstrv, ' elements: ',
     &              (strv(ii),ii=1,nstrv)

      end do
         
c     ---- end loop ------

 99   continue
      end

