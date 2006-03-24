**********************************************************************
*                                                                    *
      PROGRAM SIMJOB                                                !*
*     SIMJOB                                                         *
*     A program to demonstrate the use of the low level              *
*     fortran interface to LCIO for writing LCIO files.              *
*     it corresponts to lcio/src/cpp/src/EXAMPLE/simjob.cc           *
*                                                                    *
*     H. Vogt  06/15/2004                                            *
*     updated: 21/09/2004 (H.V.)                                     *
*                                                                    *
**********************************************************************

      call my_simjob
      end

      subroutine my_simjob
      implicit none
 
#include "lciof77api.inc"
#include "lciof77apiext.inc"

#define      RAND_MAX        2147483647
#define      INT_MAX         2147483647

c---- variables
      PTRTYPE runheader, event, writer, sthcol
      PTRTYPE mcpcol, schcol, cahcol, pvec, relnavcol
      PTRTYPE mom, hit, mcp, mcpm, mcpd, mcpdd, tpcHit, relnav
      PTRTYPE TPCVec, trkVec, extFVec, extIVec, extF, extI
      PTRTYPE helpp
      logical storeRawData
      integer status
      integer i, j, k, l, irun, nrun, iev, nev, itim, locf, nmcpart
      integer nhits, flag, extiv(2), tpcFlag, rawData(10), size, argc
      integer intvec(3)
      real    p0(3), mcpp(3), schpos(3), cahpos(3), rn, extfv(4)
      character*80 key, description
      character*30 filename
      character*11 detectorname
      character*7  ecalName,tpcName
      double precision sthpos(3), ep(3)

C----- some constants
      filename     = 'f_simjob.slcio'
      detectorname = 'D09TileHcal'
      ecalName     = 'ECAL007'
      tpcName      = 'TPC4711'

      nrun    = 10  !
      nev     = 10  ! events
      nmcpart = 10  ! mc particles per event
      nhits   = 50  ! calorimeter hits per event

* -----  Simple test program to demonstrate writing of data with lcio.

         argc = iargc()
         if ( argc .gt. 0 )  call getarg (1, filename)

c -----  loop over runs
      do  irun= 0,nrun-1

c-----   create sio writer
         writer = lcwrtcreate()
         if (irun .eq. 0)  then
           status = lcwrtopen( writer, filename , LCIO_WRITE_NEW )
         else
           status = lcwrtopen( writer, filename , LCIO_WRITE_APPEND )
         end if

* ****   NB: in order to test writing multiple files we create a new LCWriter
* ****   for every run even though we are in fact writing to one file only;
* ****   so for a simple job writing one file the
* ****   'createLCWriter/open' and 'close/delete' will be outside the run loop...


c-----   run header 
         runheader = lcrhdcreate()
         status = lcrhdsetrunnumber( runheader, irun )
         status = lcrhdsetdetectorname( runheader, detectorname  )
         write (description,'(a,i1,a)') " run: ", irun,
     $      " just for testing lcio  - no physics !" ;
         status = lcrhdsetdescription
     $            ( runheader, description(1:len_trim(description)) )

         status = lcrhdaddactivesubdetector( runheader, ecalName )
         status = lcrhdaddactivesubdetector( runheader, tpcName )
         
c-----   add some parameters to the run header
         description = 'simjob.cc'
         pvec        = stringvectorcreate( description, 1,
     &                   description(1:len_trim(description)) )
         status      = lcsetparameters( LCIO_LCRUNHEADER, runheader,
     &                   'setStringValues', 'SimulationProgram', pvec)
         do  i = 1,3
           intvec(i) = i
         end do
         pvec   = intvectorcreate(intvec, 3)
         status = lcsetparameters( LCIO_LCRUNHEADER, runheader,
     &              'setIntValues', 'SomeIndices', pvec)

         status = lcwrtwriterunheader( writer, runheader ) 

c-----  event loop 
         do  iev = 0,nev-1
            
c--------   we need to use the implementation classes here
            event  = lcevtcreate()
            status = lcevtsetrunnumber( event, irun ) 
            status = lcevtseteventnumber( event,  iev ) 
            status = lcevtsetdetectorname( event, detectorname )

c--------   create and add some mc particles
            mcpcol = lccolcreate( LCIO_MCPARTICLE )

c--------   debug only - don't write MCParticles to LCIO file
c-----      status = lccolsettransient( mcpcol , 1 ) 

c--------   debug only - add the same particle to more than one collection
c--------   LCCollectionVec* mcVec2 = new LCCollectionVec( LCIO::MCPARTICLE )  ;

            mom    = lcmcpcreate()
            status = lcmcpsetpdg( mom, 1 )
            p0(1)  = 0.
            p0(2)  = 0.
            p0(3)  = 1000.
            status = lcmcpsetmomentum( mom, p0 )
            status = lcmcpsetmass( mom, 3.01 )

            do j = 1,nmcpart
               mcp    = lcmcpcreate()
               status = lcmcpsetpdg( mcp , 1000*j )
               mcpp(1) = (j-1)*1.
               mcpp(2) = 4./1024.
               mcpp(3) = 8./1024.
               status = lcmcpsetmomentum( mcp, mcpp ) 
               status = lcmcpsetmass( mcp, .135 )

c--------      create and add some daughters
               do k = 1,3
                 mcpd    = lcmcpcreate()
                 status  = lcmcpsetpdg( mcpd, 1000*j + 100*k )
                 mcpp(1) = (k-1)*1.
                 mcpp(2) = 4.1
                 mcpp(3) = 8.1
                 status  = lcmcpsetmomentum( mcpd, mcpp )
                 status  = lcmcpsetmass( mcpd, .135 )
                 do l = 1,2
                    mcpdd   = lcmcpcreate()
                    status  = lcmcpsetpdg( mcpdd, 
     &                           1000*j + 100*k + 10*l )
                    mcpp(1) = (l-1)*1.
                    mcpp(2) = 0.41
                    mcpp(3) = 4.1
                    status  = lcmcpsetmomentum( mcpdd, mcpp )
                    status  = lcmcpsetmass( mcpdd, .135 )
                    ep(1)   = 1.111111
                    ep(2)   = 2.2222222
                    ep(3)   = 3.3333333
                    status  = lcmcpsetendpoint( mcpdd, ep )

                    flag    = 0
                    flag = ibset( flag, BITCreatedInSimulation )
                    flag = ibset( flag, BITBackscatter )
                    flag = ibset( flag, BITDecayedInTracker )
                    flag = ibclr( flag, BITDecayedInCalorimeter )
                    flag = ibclr( flag, BITLeftDetector )
                    flag = ibset( flag, BITStopped )

                    status  = lcmcpsetsimulatorstatus( mcpdd, flag )
                    status  = lcmcpaddparent( mcpdd , mcpd )
                    status  = lccoladdelement( mcpcol, mcpdd )

c--------           debug only - add the same particle to more than one collection
c--------           mcVec2->push_back( d2 )
                 end do
                 status  = lcmcpaddparent( mcpd, mcp)
                 status  = lccoladdelement( mcpcol, mcpd )
               end do
               status  = lcmcpaddparent( mcp, mom )
               status  = lccoladdelement( mcpcol, mcp )
            end do
            status  = lccoladdelement( mcpcol, mom )

c------     now add some calorimeter hits
            schcol = lccolcreate( LCIO_SIMCALORIMETERHIT )

c--------   set flag for long format (including position )
c--------   and PDG and cellid1
            flag = 0
            flag = ibset( flag, LCIO_CHBIT_LONG )
            flag = ibset( flag, LCIO_CHBIT_PDG )
            flag = ibset( flag, LCIO_CHBIT_ID1 )
            status = lccolsetflag( schcol, flag )

            do j = 1,nhits

               hit = lcschcreate()
               status = lcschsetenergy
     &                    ( hit, 3.1415*irand( 0 )/RAND_MAX)
               schpos(1) = 1.1*irand( 0 )/RAND_MAX
               schpos(2) = 2.2*irand( 0 )/RAND_MAX
               schpos(3) = 3.3*irand( 0 )/RAND_MAX
               status = lcschsetposition( hit, schpos )

               status = lcschsetcellid0( hit,  j -1 + 65335 )
               status = lcschsetcellid1( hit,  65535 )

               status = lccoladdelement( schcol, hit )

c--------      assign the hits randomly to MC particles
               rn     = .99999*irand( 0 )/RAND_MAX
               rn        = rn*nmcpart
               mcp    = lccolgetelementat( mcpcol, int( rn) + 1 )
               status = lcschaddmcparticlecontribution
     $                    ( hit, mcp, 0.314159, 0.1155 , 0 )  ! no pdg
            end do
               
c--------   data can be modified as long as is not not made persistent
            do j = 1,nhits
               hit    = lccolgetelementat( schcol, j )
               mcp    = lccolgetelementat( mcpcol, 1 )
               status = lcschaddmcparticlecontribution
     $                    ( hit, mcp, 0.1, 0., 0 )
            end do

c--------   and finally some tracker hits
c--------   with some user extensions (4 floats and 2 ints) per track:
c--------   we just need to create parallel collections of float and int vectors
            trkVec    = lccolcreate( LCIO_SIMTRACKERHIT )
            extFVec   = lccolcreate( LCIO_LCFLOATVEC )
            extIVec   = lccolcreate( LCIO_LCINTVEC )


            flag = 0
            flag = ibset( flag, LCIO_THBIT_MOMENTUM )
            status = lccolsetflag( trkVec, flag )

            do j = 1,nhits
               hit       = lcsthcreate()
               status    = lcsthsetdedx( hit, 30.e-9 )
               sthpos(1) = 1.1*irand( 0 )/RAND_MAX
               sthpos(2) = 2.2*irand( 0 )/RAND_MAX
               sthpos(3) = 3.3*irand( 0 )/RAND_MAX
               status    = lcsthsetposition( hit, sthpos )
               status    = lcsthsetmomentumxyz( hit, 1., 2., 3. )
               status    = lcsthsetpathlength( hit, .042 ) 

c--------      assign the hits randomly to MC particles
               rn        = .99999*irand( 0 )/RAND_MAX
               rn        = rn*nmcpart
               mcp       = lccolgetelementat( mcpcol, int( rn) + 1 )
               status    = lcsthsetmcparticle ( hit, mcp )

c--------      fill the extension vectors (4 floats, 2 ints)
               extfv(1)  = 3.14159
               do k = 1,3
                 extfv(k+1) = sthpos(k)*0.1
               end do
               extiv(1)  = 123456789
               extiv(2)  = int( rn )
               extF      = lcfloatvectorcreate( extfv, 4 )
               extI      = lcintvectorcreate( extiv, 2 )

c--------      add the hit and the extensions to their corresponding collections
               status    = lccoladdelement( trkVec, hit )
               status    = lccoladdelement( extFVec, extF )
               status    = lccoladdelement( extIVec, extI )
            end do


c--------   add all collections to the event
            status = lcevtaddcollection( event, mcpcol, 'MCParticle' )

            status = lcevtaddcollection( event, schcol, ecalName )
            status = lcevtaddcollection( event, trkVec, tpcName )
            status = lcevtaddcollection( 
     &                 event, extFVec, tpcName//'UserFloatExtension' )

            status = lcevtaddcollection(
     &                 event, extIVec, tpcName//'UserIntExtension' )

c--------   test: add a collection for one event only:
            if (irun .eq. 1 .and. iev .eq. 0)  then     ! first event of last run
              extFVec   = lccolcreate( LCIO_LCFLOATVEC )
              do  l = 1,4
                extfv(l) = l
              end do
              extF   = lcfloatvectorcreate( extfv, 4 )
              status = lccoladdelement( extFVec, extF )
              status = lcevtaddcollection(
     &                   event, extFVec, 'AdditionalExtension' )
            end if

c--------   even though this is a simjob we can store 'real data' objects :
c--------   --- for example we can store TPC hits ------------

            TPCVec = lccolcreate( LCIO_TPCHIT )

c--------   test new relation navigator object
            relNav = lcrnvcreate( LCIO_TPCHIT, LCIO_SIMTRACKERHIT )

            storeRawData = .true.
            flag = 0
            if (storeRawData)  flag = ibset( flag, LCIO_TPCBIT_RAW )
            status       = lccolsetflag ( TPCVec, flag )
            do  j = 1,nhits
              tpcHit     = lctphcreate()

c--------     test new relation navigator object
              helpp  = lccolgetelementat ( trkVec, j )
              status = lcrnvgaddrelation( relNav, tpcHit, helpp, 0.95 )

              status     = lctphsetcellid( tpcHit, j-1 )
              status     = lctphsettime( tpcHit, 0.1234567 )
              status     = lctphsetcharge( tpcHit, 3.14159 )
              status     = lctphsetquality( tpcHit, z'bad' )
              if (storeRawData)  then
                size       = ( dble(irand( 0 ) )/RAND_MAX )*10
                do  k = 1,size
                  rawData(k) =  
     &               int ( ( dble(irand( 0 ) )/RAND_MAX )*INT_MAX )
                end do
                status     = lctphsetrawdata( tpcHit, rawData, size )
              end if
              status     = lccoladdelement( TPCVec, tpcHit )
            end do
            status       = lcevtaddcollection(
     &                       event, TPCVec, 'TPCRawFADC' )
            relNavCol    = lcrnvcreatecollection ( relNav )
            status       = lcevtaddcollection(
     &                       event, relNavCol, 'TPCRawFADCMCTruth' )

c--------   --------------  all for TPC --------------------

c--------   write the event to the file
            status = lcwrtwriteevent( writer , event )
c--------   dump the event 
            status = lcdumpevent( event )

c--------   we created the event so we need to delete it ...
            status = lcevtdelete( event )

c----    event loop end
         end do

         status = lcrhddelete( runheader )

         status = lcwrtclose( writer )
         status = lcwrtdelete( writer )

c---- run loop end
      end do

      print *
      print *, 'created ', nrun, ' runs with ', nrun*nev, ' events'
      print *
      stop

C     
      end

