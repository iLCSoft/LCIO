**********************************************************************
*                                                                    *
      PROGRAM SIMJOB                                                !*
*     SIMJOB                                                         *
*     A program to demonstrate the use of the low level              *
*     fortran interface to LCIO for writing LCIO files.              *
*                                                                    *
*     F.Gaede 09/10/2003                                             *
**********************************************************************
      implicit none
 
#include "lciof77api.inc"

c---- variables
      PTRTYPE runheader, event, writer, sthcol, mcpcol, schcol, cahcol
      PTRTYPE  hit, mcp, mcpm, mcpd, mcpdd
      integer status
      integer i, j, k, irun, nrun, iev, nev, itim, locf
      integer nhit, flag
      character*15 filename
      character*10 detectorname
      real mcpp(3), schpos(3), cahpos(3)
      double precision sthpos(3)

C----- some constants
      filename = 'f_simjob.slcio'
      detectorname = 'TeslaPPT'
      nev = 10
      nrun = 10
      nhit = 100


      writer = lcwrtcreate()
      status = lcwrtopen( writer, filename , LCIO_WRITE_NEW )

c ----- create a few runs
      do irun=0,nrun

c----- write a run header first
         runheader = lcrhdcreate()
         status = lcrhdsetrunnumber( runheader, irun )
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
            
            itim   = 20031009*1000+iev
            event = lcevtcreate()
            status = lcevtsetdetectorname( event, detectorname )
            status = lcevtsetrunnumber( event, irun ) 
            status = lcevtseteventnumber( event,  iev ) 
            status = lcevtsettimestamp( event, itim )

            mcpcol = lccolcreate( LCIO_MCPARTICLE )
            sthcol = lccolcreate( LCIO_SIMTRACKERHIT )
            schcol = lccolcreate( LCIO_SIMCALORIMETERHIT )
            cahcol = lccolcreate( LCIO_CALORIMETERHIT )

c-------- create some monte carlo particles

            do i=1,nhit/2
               mcp = lcmcpcreate()
               status = lcmcpsetpdg( mcp, 123 ) 
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
            
c-----     now add everything to the event
            status = lcevtaddcollection(event,mcpcol ,'MCParticle') 
            status = lcevtaddcollection(event,sthcol ,'MyTrackerHits') 
            status = lcevtaddcollection(event,schcol ,'MyCalHits') 
            status = lcevtaddcollection(event,cahcol ,'MyRealCalHits') 
            
            status = lcwrtwriteevent( writer , event )
            
            if (status .eq. LCIO_ERROR )  then
               print *, 'SIMJOB: error - can not write event', iev
               stop
            end if
            
c------- need to delete the event as we created it
            status = lcevtdelete( event )
         enddo
         status = lcrhddelete( runheader ) 
      enddo
      status = lcwrtclose( writer ) 
      write(*,*)
      write(*,*) "----------------------------------------------------"
      write(*,*) "simjob is finished"
      write(*,*) "----------------------------------------------------"
      write(*,*)

c---- now call anajob
      call anajob
      
C     
      end
***********************************************************
*
      subroutine anajob
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
      
      integer status, irun, ievent
      integer i, nactdet
      character*80 detname

      write(*,*)
      write(*,*) "----------------------------------------------------"
      write(*,*) "anajob is started"
      write(*,*) "----------------------------------------------------"
      write(*,*)

      reader = lcrdrcreate()
c      write(*,*) " created reader at " , reader
      
      status = lcrdropen( reader, 'f_simjob.slcio' )
      
      if( status.eq.LCIO_ERROR) then
         write(*,*) " could not open file f_simjob.slcio "
         goto 99
      else
         write(*,*) " opened file f_simjob.slcio "
      endif
      

c---- first loop over runs and print information
      do 
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


      enddo
 9    continue

      status = lcrdrclose( reader ) 

      status = lcrdropen( reader, 'f_simjob.slcio' )
      
      if( status.eq.0) then
         write(*,*) " could not reopen file f_simjob.slcio "
         goto 99
      else
         write(*,*) " reopened file simjob.slcio "
      endif


c     ---- event loop ------
      do 
         
         event = lcrdrreadnextevent( reader )
         if( event.eq.0 ) goto 11 
         
         status = lcdumpevent( event )


      enddo
 11   continue
c     ---- end event loop ------


      status = lcrdrclose( reader )
      write(*,*) " closed file simjob.sio - status : " , status

 99   continue
      end

