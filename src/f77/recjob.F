**********************************************************************
*                                                                    *
      PROGRAM RECJOB                                                !*
*     RECOJOB                                                        *
*     A program to demonstrate the use of the low level              *
*     fortran interface to LCIO for writing LCIO files.              *
*     (it is in closest correspondence to recjob.cc)                 *
*                                                                    *
*     H. Vogt  05/06/2004                                            *
*                                                                    *
**********************************************************************

      implicit none
 
      integer nargs
      character*30 fileiname, fileoname


C----- some constants

      fileiname = 'f_simjob.slcio'
      fileoname = 'f_recjob.slcio'

      call srand ( 1234 )

      nargs = iargc()
      if (nargs .gt. 0)  call getarg (1, fileiname)
      if (nargs .gt. 1)  call getarg (2, fileoname)
      print *, 'RECJOB: the filenames used for I/O are: ',
     &          fileiname(1:lnblnk(fileiname)), ', ', 
     &          fileoname(1:lnblnk(fileoname))


      call event_processor ( fileiname, fileoname )

      stop

      end

**********************************************************************
*                                                                    *
      subroutine event_processor ( fileiname, fileoname )           !*
*                                                                    *
**********************************************************************

      implicit none

#include "recjob.inc"

      character*30 fileiname, fileoname

***   create reader, open input file
      reader = lcrdrcreate()
      
      status = lcrdropen( reader, fileiname )
      
      if( status.eq.LCIO_ERROR) then
         write(*,*) ' could not open file ', 
     &              fileiname(1:lnblnk(fileiname))
         call exit (1)
      else
ccc      write(*,*) ' opened file ', fileiname(1:lnblnk(fileiname))
      endif

***   create writer, open output file
      writer = lcwrtcreate()
      status = lcwrtopen( writer, fileoname , LCIO_WRITE_NEW )
      if( status.eq.LCIO_ERROR) then
         write(*,*) ' could not open file ', 
     &              fileoname(1:lnblnk(fileiname))
         call exit (1)
      endif

***   process run header
      call modify_run_header

***   event processing
      do ievent = 1,1000000
        eventi = lcrdrreadnextevent( reader, LCIO_UPDATE )
        if( eventi .eq. 0 )  go to 100
        call modify_event ( ievent )
        call process_event ( ievent )
      end do

***   end of event processing, close I/O files
 100  continue
      ievent = ievent - 1
      status = lcwrtclose ( writer )
      write(*,*) "----------------------------------------------------"
      write(*,*) " added collection: 'SomeClusters' and 'SomeTracks'"
      write(*,*) " to   ", ievent, " events"
      write(*,*) " and added one extra MCParticle to each event."
      write(*,*) "----------------------------------------------------"

      status = lcrdrclose( reader )
      write(*,*) 'closed file ', fileiname(1:lnblnk(fileiname)),
     &           ' -  status : ' , status


      status = lcwrtdelete( writer )
      status = lcrdrdelete( reader )

      end

**********************************************************************
*                                                                    *
      subroutine modify_event ( ievent )                           !*
*                                                                    *
**********************************************************************

      implicit none

#include "recjob.inc"

      integer nmcp, i
***   here we can modify existing objects that have been read from a stream:
      mcpcol  = lcevtgetcollection( eventi , LCIO_MCPARTICLE )
      nmcp    = lccolgetnumberofelements ( mcpcol )
      do  i = 1, nmcp
        mcpp    = lccolgetelementat ( mcpcol, i )
        status  = lcmcpsetpdg ( mcpp, 1234 )
      end do

***   or we could add sth. to existing collections
      mcpp    = lcmcpcreate ()
      status  = lcmcpsetpdg ( mcpp, 1234 )
      status  = lcmcpaddparent ( mcpp, lccolgetelementat ( mcpcol, 1 ) )
      status  = lccoladdelement ( mcpcol, mcpp )

      end

**********************************************************************
*                                                                    *
      subroutine process_event ( ievent )                           !*
*                                                                    *
**********************************************************************

***   create some tracks and add them to the event
      call tracks

***   create some clusters and add them to the event
      call clusters

***   add some reconstructed particles
      call reco_particles

***   dump event and write
      call output_event

      end

**********************************************************************
*                                                                    *
      subroutine tracks                                             !*
*                                                                    *
**********************************************************************

      implicit none

#include "recjob.inc"

      PTRTYPE helpp
      integer tpcflag, ntpchits, ntracks, cellid, trkflag, i, j
      integer ihit1, ihit2, ihit3
      real    cov(6), covt(15), refp(3)
      character*30 colname1, colname2
      double precision dpos(3), dhelp

***   in order to be able to point back to hits, we need to create
***   generic TrackerHits from the TPCHits first
      colname1 = 'TPCRawFADC'
      tpchitcol  = lcevtgetcollection( eventi , colname1 )
      tpcflag    = lccolgetflag ( tpchitcol )
      tpcflag    = ibset ( tpcflag, LCIO_TPCBIT_PTR )
      status     = lccolsetflag ( tpchitcol, tpcflag )

      trackerhitcol = lccolcreate ( LCIO_TRACKERHIT )
      ntpchits = lccolgetnumberofelements ( tpchitcol )

      do  i = 1, ntpchits
        trhitp  = lctrhcreate ()
        tpchit  = lccolgetelementat ( tpchitcol, i)
        status  = lctrhsetdedx ( trhitp, lctphgcharge ( tpchit ) )
        status  = lctrhsettime ( trhitp, lctphgettime ( tpchit ) )
        cellid  = lctphgetcellid ( tpchit )
        dpos(1) = iand ( cellid, 'ff'x )
        dpos(2) = ishft ( iand ( cellid, 'ff00'x ) , -8 )
        dpos(3) = ishft ( iand ( cellid, 'ff0000'x ) , -16 )
        status  = lctrhsetposition ( trhitp, dpos )
        status  = lctrhsettpchit ( trhitp, tpchit )
        do j = 1, 6
          cov(j)  = i
        end do
        status  = lctrhsetcovmatrix ( trhitp, cov )
        status  = lccoladdelement ( trackerhitcol, trhitp )
      end do
      colname2 = 'TrackerHits'
      status   = lcevtaddcollection ( eventi, trackerhitcol, colname2 )
  
***   now the tracks
***   if we want to point back to the hits we need to set the flag
      trackcol = lccolcreate ( LCIO_TRACK )
      trkflag  = ibset ( trkflag, LCIO_TRBIT_HITS )
      status   = lccolsetflag ( trackcol, trkflag )

      ntracks  = 10
      do  i = 1, ntracks
        trkp    = lctrkcreate ()
        status  = lctrksettypebit ( trkp, 7 )
        status  = lctrksetomega (  trkp, i*1.1 )
        status  = lctrksettanlambda ( trkp, i*m_pi/10. )
        status  = lctrksetphi ( trkp, i*m_pi/5. )
        status  = lctrksetd0 ( trkp, i*1. )
        status  = lctrksetz0 ( trkp, i*10. )
        status  = lctrksetchi2 (trkp,  1.01 )
        status  = lctrksetdedx (trkp,  3.14159 )
        status  = lctrksetdedxerror (trkp,  42. )
        do j = 1,15
          covt(j) = j
        end do
        status  = lctrksetcovmatrix (trkp,  covt )
        refp(1) = 12.
        refp(2) = 123456789.
        refp(3) = .0987654321
        status  = lctrksetreferencepoint (trkp,  refp )

***     add some random hits  (c++ counts from 0, fortran api from 1)
        dhelp   = dble ( lccolgetnumberofelements ( trackerhitcol ))
        ihit1   = dhelp * rand ( 0 )
        ihit2   = dhelp * rand ( 0 )
        ihit3   = dhelp * rand ( 0 )
        helpp   = lccolgetelementat ( trackerhitcol , ihit1 + 1 )
        status  = lctrkaddhit ( trkp, helpp )
        helpp   = lccolgetelementat ( trackerhitcol , ihit2 + 1 )
        status  = lctrkaddhit ( trkp, helpp )
        helpp   = lccolgetelementat ( trackerhitcol , ihit3 + 1 )
        status  = lctrkaddhit ( trkp, helpp )

***     add tracks that where used to create this track
        j       = lccolgetnumberofelements ( trackcol )
        if ( j .gt. 1 )  then
          status= lctrkaddtrack (trkp, lccolgetelementat(trackcol, j))
          status= lctrkaddtrack (trkp, lccolgetelementat(trackcol, j-1))
        end if
        status  =  lccoladdelement ( trackcol , trkp )
      end do

      colname2 = 'SomeTracks'
      status  =  lcevtaddcollection ( eventi, trackcol, colname2 )
*
      end

**********************************************************************
*                                                                    *
      subroutine clusters                                           !*
*                                                                    *
**********************************************************************

      implicit none

#include "recjob.inc"

#define  COMBINED      3
#define  CHARGED       2

      PTRTYPE  helpp, clusp
      integer  calflag, nsimhits, cluflag, nhits, nclus, type, ncluscol
      integer  ncalhit, ihit1, ihit2, ihit3
      integer  i, j
      real     posv(3), eposv(6), shape(6), energy
      double precision dpos(3), dhelp
      character*30 colname1, colname2

***   create some clusters and add them to the event
      colname1  = 'ECAL007'
      simcalcol = lcevtgetcollection( eventi , colname1 )
      cluscol   = lccolcreate (  LCIO_CLUSTER )
      calhitcol = lccolcreate (  LCIO_CALORIMETERHIT )

***   in order to be able to point back to hits, we need to create
***   generic CalorimeterHits from the SimCalorimeterHits first
***   here we set the pointer flag bit that is needed to be able to point from
***   the generic Clusters to the 'raw data' CalorimeterHits
***   -> this should be done automatically in a future release
      calflag = lccolgetflag ( calhitcol )
      calflag = ibset ( calflag, LCIO_RCHBIT_PTR )
      status  = lccolsetflag ( calhitcol, calflag )

      nsimhits = lccolgetnumberofelements ( simcalcol )
ccc         print *, '# of nsimhits: ',nsimhits
      do  i = 1, nsimhits
        calhitp     = lccahcreate ()
        simcalhitp  = lccolgetelementat ( simcalcol, i )
        energy      = lcschgetenergy ( simcalhitp )
ccc         print *, 'energy: ', lcschgetenergy ( simcalhitp )
ccc         print *, 'energy: ', energy
ccc         stop
        status      = lccahsetenergy 
     &                  ( calhitp, lcschgetenergy ( simcalhitp ) )
        status      = lccahsetcellid0
     &                  ( calhitp, lcschgetcellid0 ( simcalhitp ) )
        status      = lcschgetposition ( simcalhitp, posv )
        status      = lccahsetposition ( calhitp, posv )
        status      = lccoladdelement ( calhitcol, calhitp )
      end do
      colname2 = 'CalorimeterHits'
      status   = lcevtaddcollection ( eventi, calhitcol, colname2 )

***   if we want to point back to the hits we need to set the flag
      cluflag = 0
      cluflag = ibset ( cluflag, LCIO_CLBIT_HITS )
      status  = lccolsetflag ( cluscol, cluflag )
      if ( calhitcol .ne. 0 )  then
        nhits     = lccolgetnumberofelements ( calhitcol )
        nclus     = nhits/10
        do  i = 1, nclus
          clusp   = lcclucreate ()
***   set type bits to "Cluster::COMBINED << 16 | Cluster::CHARGED"
ccc          type    = ior (ishft ( COMBINED, 16), CHARGED ) ! obsolete
ccc          status  = lcclusettype ( clusp, type )          ! obsolete
          status  = lcclusettypebit ( clusp, 1 )
          status  = lcclusettypebit ( clusp, 7 )
          status  = lcclusettypebit ( clusp, 11 )

          status  = lcclusetenergy ( clusp, i*1.1)
          posv(1) = 12.
          posv(2) = 123456789.
          posv(3) = .0987654321
          status  = lcclusetposition ( clusp, posv )
          do j = 1, 6
            eposv(j) = j
          end do
          status  = lcclusetpositionerror ( clusp, eposv )
          status  = lcclusettheta ( clusp, i*m_pi/10. )
          status  = lcclugetphi ( clusp, i*m_pi/5. )
          do j = 1, 6
            if ( j .lt. 4 )  then
              eposv(j) = j
            else
              eposv(j) = 0
            end if
          end do
          status  = lcclusetdirectionerror ( clusp, eposv )
          do j = 1, 6
            if ( j .lt. 4 )  then
              shape(j) = j
            else
              shape(j) = 7 - j
            end if
          end do
          status  = lcclusetshape ( clusp, shape )
          status  = lcclusetemweight ( clusp, .333)
          status  = lcclusethadweight ( clusp, .333)
          status  = lcclusetmuonweight ( clusp, .333)

***       add some random hits
          ncalhit = lccolgetnumberofelements ( calhitcol )
          dhelp   = dble ( ncalhit*rand ( 0 ) )
          ihit1   =  int ( dble ( ncalhit) * rand ( 0 ) )
          ihit2   =  int ( dble ( ncalhit) * rand ( 0 ) )
          ihit3   =  int ( dble ( ncalhit) * rand ( 0 ) )
          helpp   = lccolgetelementat ( calhitcol, ihit1 + 1 )
          status  = lccluaddhit ( clusp, helpp , 1.)
          helpp   = lccolgetelementat ( calhitcol, ihit2 + 1 )
          status  = lccluaddhit ( clusp, helpp , 2. )
          helpp   = lccolgetelementat ( calhitcol, ihit3 + 1 )
          status  = lccluaddhit ( clusp, helpp , 3. )

***       add clusters that where used to create this cluster
          ncluscol = lccolgetnumberofelements ( cluscol )
          if ( ncluscol .gt. 1 )  then
            helpp   = lccolgetelementat ( cluscol, ncluscol )
            status  = lccluaddcluster ( clusp, helpp )
            helpp   = lccolgetelementat ( cluscol, ncluscol - 1 )
            status  = lccluaddcluster ( clusp, helpp )
          end if
          status  =  lccoladdelement ( cluscol, clusp )
        end do
      end if
      colname2 =  'SomeClusters'
      status   =  lcevtaddcollection ( eventi, cluscol, colname2 )
*
      end

**********************************************************************
*                                                                    *
      subroutine reco_particles                                     !*
*                                                                    *
**********************************************************************

      implicit none

#include "recjob.inc"

#define  SINGLE        1

      PTRTYPE helpp, clusp
      integer i, j, k
      integer ihit1, ihit2, ihit3, npid, itrk, iclu, imcp
      real    mom(3), cov(10), refp(3), prob
      character*30 colname1, colname2
      character*40 ident
      double precision dpos(3), dhelp

      integer   nreccp
      parameter (nreccp=10)

***   create some reconstructed particles
      recopcol = lccolcreate (  LCIO_RECONSTRUCTEDPARTICLE )
      do  i = 1, nreccp
        recopp   =  lcrcpcreate ()
        status   =  lcrcpsetprimary ( recopp, .true. )
        status   =  lcrcpsettype ( recopp, SINGLE )
        mom(1)   =  1.1
        mom(2)   =  2.2
        mom(3)   =  3.3
        status   =  lcrcpsetmomentum ( recopp, mom )
        status   =  lcrcpsetenergy ( recopp, (i-1)*101.101 )
        do  j = 1, 10
          cov(j) = j
        end do
        status   =  lcrcpsetcovmatrix ( recopp, cov )
        status   =  lcrcpsetmass ( recopp, 0.511*(i-1) )
        status   =  lcrcpsetcharge ( recopp, -2./3. )
        refp(1)  =  10.
        refp(2)  =  20.
        refp(3)  =  30.

***     add some particle ids
        npid = 5
        do  j = 1, npid
          pidp     = lcpidcreate ()
          prob     = dble ( (j+1)/npid )
          status   = lcpidsetprobability (pidp, prob )
          status   = lcpidsettypeid ( pidp, -11 )
          ident    = 'recojob-RunEventProcessor'
          status   = lcpidsetidentifier ( pidp, ident )
          do  k = 1, 3
            status   = lcpidaddparameter ( pidp, (k-1)*.1 )
          end do
          status   = lcrcpaddparticleid ( recopp, pidp )
        end do

***     some other particles
        if ( i .gt. 2 )  then
          recopp1 = lccolgetelementat ( recopcol , i - 1 )
          recopp2 = lccolgetelementat ( recopcol , i - 2 )
          status  = lcrcpaddparticle ( recopp, recopp1, 0.1 )
          status  = lcrcpaddparticle ( recopp, recopp2, 0.9 )
        end if

***     a track
        itrk    = dble ( lccolgetnumberofelements ( trackcol )*rand(0) )
        trkp    = lccolgetelementat ( trackcol, itrk + 1 )
        status  = lcrcpaddtrack ( recopp , trkp, 1.0 )

***     a cluster
        iclu    = dble ( lccolgetnumberofelements ( cluscol )*rand(0) )
        clusp   = lccolgetelementat ( cluscol, iclu + 1 )
        status  = lcrcpaddcluster ( recopp , clusp )

***     and finally an MCParticle
        mcpcol  = lcevtgetcollection( eventi , LCIO_MCPARTICLE )
        imcp    = dble ( lccolgetnumberofelements ( mcpcol )*rand(0) )
        mcpp    = lccolgetelementat ( mcpcol, imcp + 1 )
        status  = lcrcpaddmcparticle ( recopp , mcpp, 0.5 )

***     add this reconstructed particle to collection
        status  =  lccoladdelement ( recopcol, recopp )
        
      end do
      colname2 =  'ReconstructedParticle'
      status   =  lcevtaddcollection ( eventi, recopcol, colname2 )
*
      end

**********************************************************************
*                                                                    *
      subroutine modify_run_header                                  !*
*                                                                    *
**********************************************************************
*
      implicit none

#include "recjob.inc"

***   just copy run headers to the outputfile
      header = lcrdrreadnextrunheader ( reader, LCIO_READ_ONLY )
      status = lcwrtwriterunheader ( writer, header )
*
      end

**********************************************************************
*                                                                    *
      subroutine output_event                                       !*
*                                                                    *
**********************************************************************
*
      implicit none

#include "recjob.inc"
#include "lciof77apiext.inc"

***   dump event
      status = lcdumpevent ( eventi )

***   write event
      status = lcwrtwriteevent ( writer, eventi )
*
      end

