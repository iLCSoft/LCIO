**********************************************************************
*                                                                    *
      PROGRAM SIMJOB                                                !*
*     SIMJOB                                                         *
*     A program to demonstrate the use of the low level              *
*     fortran interface to LCIO                                      *
*                                                                    *
*     F.Gaede 09/10/2003                                             *
**********************************************************************
      implicit none
 
C This may be architecture dependent
#define PTRTYPE integer


c---- LCIO low level interface --------------------
c----- the LCReader interface
      PTRTYPE lcrdrcreate
      integer lcrdrdelete, lcrdropen, lcrdrclose
      PTRTYPE lcrdrreadnextrunheader
      PTRTYPE lcrdrreadnextevent
      PTRTYPE lcrdrreadevent
c-----the LCWriter interface
      PTRTYPE lcwrtcreate
      integer lcwrtdelete, lcwrtopen, lcwrtclose
      integer lcwrtwriterunheader, lcwrtwriteevent
c-----the LCRunHeader interface
      PTRTYPE lcrhdcreate
      integer lcrhddelete, lcrhdgetrunnumber
      character*80 lcrhdgetdetectorname
      character*1024 lcrhdgetdescription
      integer lcrhdsetrunnumber, lcrhdsetdetectorname
      integer lcrhdsetdescription, lcrhdaddactivesubdetector
c-----the LCEvent interface
      PTRTYPE lcevtcreate, lcevtgetcollection
      integer lcevtdelete
      integer lcevtgetrunnumber, lcevtgeteventnumber
      character*80 lcevtgetdetectorname
      integer*8 lcevtgettimestamp
      integer lcevtaddcollection, lcevtremovecollection
      integer lcevtsetrunnumber, lcevtseteventnumber
      integer lcevtsetdetectorname, lcevtsettimestamp
c-----the LCCollection interface
      PTRTYPE lccolcreate, lccolgetelementat
      integer lccoldelete, lccolgetnumberofelements
      character*80 lccolgettypename
      integer lccolgetflag, lccolsetflag, lccoladdelement
      integer lccolremoveelementat
c-----the MCParticle interface
      PTRTYPE lcmcpcreate, lcmcpgetparent, lcmcpgetsecondparent
      PTRTYPE lcmcpgetdaughter 
      integer lcmcpdelete, lcmcpgetendpoint, lcmcpgetnumberofdaughters
      integer lcmcpgetpdg, lcmcpgethepevtstatus 
      integer lcmcpgetvertex, lcmcpgetmomentum
      real lcmcpgetmass, lcmcpgetcharge, lcmcpgetenergy
      integer lcmcpsetparent, lcmcpsetsecondparent, lcmcpadddaughter 
      integer lcmcpsetpdg, lcmcpsethepevtstatus, lcmcpsetvertex 
      integer lcmcpsetendpoint, lcmcpsetmomentum, lcmcpsetmass 
      integer lcmcpsetcharge
c-----the SimTrackerHit interface
      PTRTYPE lcsthcreate, lcsthgetmcparticle
      integer lcsthdelete, lcsthgetcellid
      double precision lcsthgetposition
      real lcsthgetdedx, lcsthgettime
      integer lcsthsetcellid, lcsthsetposition, lcsthsetdedx   
      integer lcsthsettime, lcsthsetmcparticle
c-----the SimCalorimeterHit interface
      PTRTYPE lcschcreate, lcschgetparticlecont
      integer lcschdelete, lcschgetcellid0, lcschgetcellid1  
      real lcschgetenergy, lcschgetenergycont, lcschgettimecont
      integer lcschgetposition, lcschgetnmcparticles
      integer lcschgetpdgcont
      integer  lcschsetcellid0, lcschsetcellid1, lcschsetenergy
      integer  lcschsetposition, lcschaddmcparticlecontribution
c-----the CalorimeterHit interface
      PTRTYPE lccahcreate
      integer lccahdelete, lccahgetcellid0, lccahgetcellid1  
      real lccahgetenergy
      integer lccahgetposition
      integer  lccahsetcellid0, lccahsetcellid1, lccahsetenergy
      integer  lccahsetposition
c---- end of LCIO low level interface --------------------
      


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
      status = lcwrtopen( writer, filename , 0 )

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
            
            mcpcol = lccolcreate( 'MCParticle' )
            sthcol = lccolcreate( 'SimTrackerHit' )
            schcol = lccolcreate( 'SimCalorimeterHit' )
            cahcol = lccolcreate( 'CalorimeterHit' )

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

                  status = lcmcpsetparent( mcpd , mcp ) 
                  status = lcmcpadddaughter( mcp, mcpd ) 

                  status = lccoladdelement( mcpcol, mcpd ) 

                  do k=1,3
                     mcpdd = lcmcpcreate()
                     status = lcmcpsetpdg( mcpdd , 1234567 ) 
                     mcpp(1) = k*1.
                     mcpp(2) = k*2.
                     mcpp(3) = k*3.
                     status = lcmcpsetmomentum( mcpdd, mcpp ) 
                     
                     status = lcmcpsetparent( mcpdd , mcpd ) 
                     status = lcmcpadddaughter( mcpd, mcpdd ) 
                     
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
            

            status = lcevtaddcollection(event,mcpcol ,'MCParticles') 
            status = lcevtaddcollection(event,sthcol ,'MyTrackerHits') 
            status = lcevtaddcollection(event,schcol ,'MyCalHits') 
            status = lcevtaddcollection(event,cahcol ,'MyRealCalHits') 
            
            status = lcwrtwriteevent( writer , event )
            
            if (status .eq. 0)  then
               print *, 'SIMJOB: error - can not write event', iev
               stop
            end if
            
c------- need to delete the event as we created it
            status = lcevtdelete( event )
         enddo
      enddo
      status = lcwrtclose( writer ) 
      
C     
      end
