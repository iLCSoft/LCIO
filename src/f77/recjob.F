
#define      RAND_MAX        2147483647        
#define      INT_MAX         2147483647

**********************************************************************
*                                                                    *
      PROGRAM RECJOB                                                !*
*     RECOJOB                                                        *
*     A program to demonstrate the use of the low level              *
*     fortran interface to LCIO for writing LCIO files.              *
*     (it is in closest correspondence to recjob.cc)                 *
*                                                                    *
*     H. Vogt  05/06/2004                                            *
*     updated: 21/09/2004 (H.V.)                                     *
*                                                                    *
**********************************************************************

      implicit none
 
      integer nargs, nFiles, i
      character*40 filenames(20),fileoname

      nFiles       = 1
      filenames(1) = 'f_recjob.slcio'
      filenames(2) = 'f_simjob.slcio'
      fileoname = filenames(1)

c---- read file names from command line arguments
      nargs = iargc()
      if (nargs .lt. 1)  then
ccc        print *, ' usage:  recjob <output-file>',
ccc     &           '<input-file1> [[input-file2],...]'
ccc        print *, ' filenames must have less then 40 characters'
ccc        print *, ' up to 20 filenames are accepted'
ccc        print *, ' no arguments given - using default'
ccc        print *
ccc      else if (nargs .gt. 20)  then
ccc        print *, ' usage: more than max = 20 file names given'
ccc        stop
      else
        do  i = 1, nargs
          call getarg (i, filenames(i))
        end do
        nFiles = nargs - 1
c---    make consistent with recjob.cc, if nargs > 0
        nFiles       = 1
        fileoname    = filenames(2)
        filenames(2) = filenames(1)
      end if

      call srand ( 1234 )

      call event_processor ( fileoname, filenames(2), nFiles )

      stop

      end

**********************************************************************
*                                                                    *
      subroutine event_processor ( fileoname, fileinames, nifiles ) !*
*                                                                    *
**********************************************************************

      implicit none

#include "recjob.inc"

      character*40 fileinames(*), fileoname
      integer      nifiles

      PTRTYPE      pstrv
      integer      lcrdreventprocessor

***   create LCStrvec of input file names
      pstrv = stringvectorcreate ( fileinames, 
     &                               nifiles, len (fileinames(1)) )

***   create writer, open output file
      writer = lcwrtcreate()
      status = lcwrtopen( writer, fileoname , LCIO_WRITE_NEW )
      if( status.eq.LCIO_ERROR) then
         write(*,*) ' could not open file ', 
     &              fileoname(1:lnblnk(fileoname))
         call exit (1)
      endif

***   create reader, open input file chain, start event loop
      ievent = 0
      status = lcrdreventprocessor( pstrv )
      
***   end of event processing, close I/O files
      ievent = ievent
      status = lcwrtclose ( writer )
      write(*,*) "----------------------------------------------------"
      write(*,*) " added collection: 'SomeClusters' and 'SomeTracks'"
      write(*,*) " to   ", ievent, " events"
      write(*,*) " and added one extra MCParticle to each event."
      write(*,*) "----------------------------------------------------"

      status = lcwrtdelete( writer )

      stop

      end


**********************************************************************
*                                                                    *
      subroutine processrunheader ( pheader )                       !*
*                                                                    *
**********************************************************************
*
      implicit none

#include "recjob.inc"

      PTRTYPE pheader

***   just copy run headers to the outputfile
      header = pheader
      status = lcwrtwriterunheader ( writer, header )
*
      end

**********************************************************************
*                                                                    *
      subroutine modifyrunheader ( pheader )                        !*
*                                                                    *
**********************************************************************
*
      implicit none

      PTRTYPE pheader

***   do nothing for runheader here

      end

**********************************************************************
*                                                                    *
      subroutine processevent ( pevent )                             !*
*                                                                    *
*     used for 'read only' access                                    *
*                                                                    *
**********************************************************************

      implicit none

#include "recjob.inc"

       PTRTYPE pevent

***   increase event counter, set event pointer
      eventi = pevent
      ievent = ievent + 1

***   create some tracks and add them to the event
      call tracks

***   create some clusters and add them to the event
      call clusters

***   add some vertices
      call vertices
      
**   add some reconstructed particles
      call reco_particles

      end

**********************************************************************
*                                                                    *
      subroutine tracks                                             !*
*                                                                    *
**********************************************************************

      implicit none

#include "recjob.inc"

      PTRTYPE helpp, intvpp, intvp, lcgetpointerfromaddress, pvec
      integer tpcflag, ntpchits, ntracks, cellid, trkflag, i, j, k
      integer ihit1, ihit2, ihit3
      integer ntracker, vtxindex, sitindex, tpcindex, intv(20), nintv
      integer typevalues
      real    cov(6), covt(15), refp(3)
      character*30 colname1, colname2, typenames, trackernames(3),
     &             relation
      double precision dpos(3), dhelp

***   in order to be able to point back to hits, we need to create
***   generic TrackerHits from the TPCHits first
      colname1 = 'TPCRawFADC'
      tpchitcol  = lcevtgetcollection( eventi , colname1 )
***   fg20040824 -> THE LOGIC IS REVERSED - NO NEED TO SET A BIT TO GET THE POINTER FLAG
ccc      tpcflag    = lccolgetflag ( tpchitcol )
ccc      tpcflag    = ibset ( tpcflag, LCIO_TPCBIT_PTR )
ccc      status     = lccolsetflag ( tpchitcol, tpcflag )

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

        status  = lctrhaddrawhit ( trhitp, tpchit )  ! store the original raw data hit
        status  = lctrhaddrawhit ( trhitp, tpchit )  ! for testing add the same raw hit twice

        do j = 1, 6
          cov(j)  = i
        end do
        status  = lctrhsetcovmatrix ( trhitp, cov )
        status  = lccoladdelement ( trackerhitcol, trhitp )
      end do

***   set the parameters to decode the type information in the collection
***   for the time being this has to be done manually
***   in the future we should provide a more convenient mechanism to
***   decode this sort of meta information
      typenames  = LCIO_TPCHIT
      typevalues = 1
      pvec       = stringvectorcreate( typenames, 1,
     &               len ( typenames ) )
      status     = lcsetparameters( LCIO_LCCOLLECTION, trackerhitcol,
     &               'setStringValues', 'TrackerHitTypeNames', pvec)
      pvec       = intvectorcreate( typevalues, 1 )
      status     = lcsetparameters( LCIO_LCCOLLECTION, trackerhitcol,
     &               'setIntValues', 'TrackerHitTypeValues', pvec)

      colname2 = 'TrackerHits'
      status   = lcevtaddcollection ( eventi, trackerhitcol, colname2 )
  
***   now the tracks
***   if we want to point back to the hits we need to set the flag
      trackcol = lccolcreate ( LCIO_TRACK )
      trkflag  = 0
      trkflag  = ibset ( trkflag, LCIO_TRBIT_HITS )
      status   = lccolsetflag ( trackcol, trkflag )

***   set the hit numbers and create string vector of tracker names
      ntracker = 3
      vtxindex = 1
      sitindex = 2
      tpcindex = 3
      ntracks  = 10
      trackernames(vtxindex) = 'VTX'
      trackernames(sitindex) = 'SIT'
      trackernames(tpcindex) = 'TPC'
      pvec        = stringvectorcreate( trackernames(1), ntracker,
     &                len ( trackernames(1) ) )

      do  i = 1, ntracks
        trkp    = lctrkcreate ()
        status  = lctrksettypebit ( trkp, 7, 1 )
        status  = lctrksetomega (  trkp, i*1.1 )
        status  = lctrksettanlambda ( trkp, real (i*m_pi/10.) )
        status  = lctrksetphi ( trkp, real (i*m_pi/5.) )
        status  = lctrksetd0 ( trkp, i*1. )
        status  = lctrksetz0 ( trkp, i*10. )
        status  = lctrksetchi2 ( trkp,  1.01 )
        status  = lctrksetndf ( trkp, 42 )
        status  = lctrksetradiusofinnermosthit ( trkp, 3.141592 )

        status  = lcsetparameters( LCIO_LCCOLLECTION, trackcol,
     &              'setStringValues', 'TrackSubdetectorNames', pvec )

        intv(vtxindex) = 12
        intv(sitindex) = 24
        intv(tpcindex) = 36
ccc       now check lctrksetsubdetectorhitnumbers, lctrkgetsubdetectorhitnumbers
ccc       if ( i .gt. 3 )  then
ccc         intv(i) = i*10 +i
ccc         ntracker = i
ccc       end if
        status  = lctrksetsubdetectorhitnumbers ( trkp, intv, ntracker )
ccc       nintv   = 6
ccc       status  = lctrkgetsubdetectorhitnumbers ( trkp, intv, nintv )
ccc       print *, nintv, ' Integers found'
ccc       do j = 1, nintv
ccc         print *, 'Integer at ',j,' is', intv(j)
ccc       end do

        status  = lctrksetdedx ( trkp,  3.14159 )
        status  = lctrksetdedxerror ( trkp,  42. )
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
        ihit1   = dhelp * irand( 0 )/RAND_MAX
        ihit2   = dhelp * irand( 0 )/RAND_MAX
        ihit3   = dhelp * irand( 0 )/RAND_MAX
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

      PTRTYPE  helpp, clusp, pvec, pstrv, pobj, vecp
      PTRTYPE  simhitsvp, simhitswp, calveccol, cconp
      integer  calflag, nsimhits, cluflag, nhits, nclus, type, ncluscol
      integer  relflag, ncalhits
      integer  ncalhit, ihit1, ihit2, ihit3
      integer  i, j
      real     posv(3), eposv(6), shape(6), energy
      double precision dpos(3), dhelp
      character*30 colname1, colname2, colname3, relation
      character*30 shapeparams(6)

***   create some clusters and add them to the event
      colname1  = 'ECAL007'
      simcalcol = lcevtgetcollection( eventi , colname1 )
      cluscol   = lccolcreate (  LCIO_CLUSTER )
      calhitcol = lccolcreate (  LCIO_CALORIMETERHIT )

***   include relations
      screlcol  = lccolcreate (  LCIO_LCRELATION )
      relation  = LCIO_CALORIMETERHIT
      pvec      = stringvectorcreate( relation, 1,
     &              len ( relation ) )
      status    = lcsetparameters(LCIO_LCCOLLECTION, screlcol,
     &              'setStringValues', 'RelationFromType', pvec )
      relation  = LCIO_SIMCALORIMETERHIT
      pvec      = stringvectorcreate( relation, 1,
     &              len ( relation ) )
      status    = lcsetparameters(LCIO_LCCOLLECTION, screlcol,
     &              'setStringValues', 'RelationToType', pvec )
      

***   in order to be able to point back to hits, we need to create
***   generic CalorimeterHits from the SimCalorimeterHits first
***   here we set the pointer flag bit that is needed to be able to point from
***   the generic Clusters to the 'raw data' CalorimeterHits
***   -> this should be done automatically in a future release
ccc      calflag = lccolgetflag ( calhitcol )
ccc      calflag = ibset ( calflag, LCIO_RCHBIT_PTR )
ccc      status  = lccolsetflag ( calhitcol, calflag )

      nsimhits = lccolgetnumberofelements ( simcalcol )
ccc         print *, '# of nsimhits: ',nsimhits
      do  i = 1, nsimhits
        calhitp     = lccahcreate ()
        simcalhitp  = lccolgetelementat ( simcalcol, i )
ccc         print '(a,z10)', 'pointer of simcalhit:', simcalhitp

        energy      = lcschgetenergy ( simcalhitp )
        status      = lccahsetenergy 
     &                  ( calhitp, lcschgetenergy ( simcalhitp ) )
        status      = lccahsetcellid0
     &                  ( calhitp, lcschgetcellid0 ( simcalhitp ) )
        status      = lcschgetposition ( simcalhitp, posv )
        status      = lccahsetposition ( calhitp, posv )
        do j = 1,3
          status      = lccoladdelement ( screlcol,
     &                    lcrelcreate( calhitp, simcalhitp, 0.5 ) )
        end do
        status      = lccoladdelement ( calhitcol, calhitp )
      end do

      colname2 = 'CalorimeterHits'
      status   = lcevtaddcollection ( eventi, calhitcol, colname2 )
      relflag  = 0
      relflag  = ibset ( relflag, LCIO_LCREL_WEIGHTED )
      status   = lccolsetflag ( screlcol, relflag )
      colname3 = 'CalorimeterHitsSimRel'
ccc      status   = lcevtaddrelation ( eventi, screlcol, colname3 )
      status   = lcevtaddcollection ( eventi, screlcol, colname3 )

      if (lcevtgeteventnumber ( eventi ) .eq. 0  .and.
     &    lcevtgetrunnumber ( eventi ) .eq. 0)  then

***   the following is some example code on how to access the relation
***   create a navigation object from a collection using different methods
        helpp    = lcrnvcreatefromcollection( screlcol )
        colname1 = lcrnvgetfromtype ( helpp )
        colname2 = lcrnvgettotype ( helpp )
        print *, 'Relation example for first event:  [' ,
     &            colname1(1:len_trim ( colname1 )), ' - ',
     &            colname2(1:len_trim ( colname2 )), '] '
        print *, 'Relation are given twice using different methods'
        ncalhits = lccolgetnumberofelements( calhitcol )
        do  i = 1, ncalhits
          calhitp     = lccolgetelementat ( calhitcol , i )
          simhitsvp = lcrnvgetrelatedtoobjects ( helpp, calhitp )
          simhitswp = lcrnvgetrelatedtoweights ( helpp, calhitp )

***       methods: via lcobv
          print '(a,z4,$)', '  relations for object ', lccahid (calhitp)
          do  j = 1, lcobvgetlength ( simhitsvp )
            print '(a,z8,a,z2,a,f3.1,a,$)', ' [',
     &               lcobvgetobject ( simhitsvp, j ),
     &               '] [',
     &               lcobvgetobjectid ( simhitsvp, j ),
     &               '] (',
     &               lcobvgetweight ( simhitswp, j ),
     &               ') '
          end do
          print *

***       methods: via stl vector and lcsch 
          print '(a,z4,$)', '  relations for object ', lccahid (calhitp)
          do  j = 1, pointervectorgetlength ( simhitsvp )
            pobj = pointervectorgetelement ( simhitsvp, j )
            print '(a,z8,a,z2,af3.1,a,$)', ' [',
     &               pobj,
     &               '] [',
     &               lcschid ( pobj ),
     &               '] (',
     &               floatvectorgetelement ( simhitswp, j ),
     &               ') '
          end do
          print *

        end do

***   add some calibration constants as generic user objects
        calveccol = lccolcreate ( LCIO_LCGENERICOBJECT )
        do  i = 1, ncalhits
          calhitp   = lccolgetelementat ( calhitcol , i )
           cconp    = lcgobcreate()
           status   = lcgobsetintval( cconp, 1, lccahid ( calhitp ) )
           status   = lcgobsetfloatval( cconp, 2, 1.*(i-1) )
           status   = lcgobsetfloatval( cconp, 3, 0.01*(i-1) )
           status   = lccoladdelement( calveccol, cconp )
        end do
        status      = lcevtaddcollection ( eventi, calveccol,
     &                  'Calibration' )

      end if

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
          status  = lcclusettypebit ( clusp, 1 , 1 )
          status  = lcclusettypebit ( clusp, 7 , 1 )
          status  = lcclusettypebit ( clusp, 11, 1 )

          status  = lcclusetenergy ( clusp, i*1.1)
          posv(1) = 12.
          posv(2) = 123456789.
          posv(3) = .0987654321
          status  = lcclusetposition ( clusp, posv )
          do j = 1, 6
            eposv(j) = j
          end do
          status  = lcclusetpositionerror ( clusp, eposv )
          status  = lcclusetitheta ( clusp, real (i*m_pi/10.) )
          status  = lcclusetiphi ( clusp, real (i*m_pi/5. ) )
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
                                              
***       set the cluster ashape variables
          shapeparams(1) = 'Shape_trans'
          shapeparams(2) = 'Shape_long'
          shapeparams(3) = 'Shape_axis_x'
          shapeparams(4) = 'Shape_axis_y'
          shapeparams(5) = 'Shape_axis_z'
          shapeparams(6) = 'Shape_quality'
          vecp   = stringvectorcreate( shapeparams, 6,
     &               len ( shapeparams(1) ) )
          status = lcsetparameters (LCIO_LCCOLLECTION, cluscol,
     &               'setStringValues', 'ClusterShapeParameters', vecp )
          vecp   = floatvectorcreate( shape, 6 )
          status = lcclusetshape ( clusp, vecp )
c          status  = lcclusetshape ( clusp, shape )
c          status  = lcclusetemweight ( clusp, .333)
c          status  = lcclusethadweight ( clusp, .333)
c          status  = lcclusetmuonweight ( clusp, .333)




***       add some random hits
          ncalhit = lccolgetnumberofelements ( calhitcol )

          dhelp   = dble ( ncalhit )
          ihit1   =  dhelp * irand( 0 )/RAND_MAX
          ihit2   =  dhelp * irand( 0 )/RAND_MAX
          ihit3   =  dhelp * irand( 0 )/RAND_MAX
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
      subroutine vertices                                           !*
*                                                                    *
**********************************************************************

      implicit none

#include "recjob.inc"

      PTRTYPE helpp, clusp
      integer i, j, k
      real    cov(6), pos(3)
      character*30 colname

      integer   nvtx
      parameter (nvtx=11)

***   add some vertices
      vtxcol = lccolcreate (  LCIO_VERTEX )
      do  i = 1, nvtx
        vtx  =  lcvtxcreate ()
        if ( i .eq. 1 )  then
          status   =  lcvtxsetprimary ( vtx, .true. )
        end if
        
        k=MOD(irand( 0 ), 7)
        if ( k .eq. 0 ) then
          status   =  lcvtxsetalgorithmtype(vtx, "ZvTop")
        else if ( k .eq. 1 ) then
          status   =  lcvtxsetalgorithmtype(vtx, "ZvKin")
        else if ( k .eq. 2 ) then
          status   =  lcvtxsetalgorithmtype(vtx, "42")
        else if ( k .eq. 3 ) then
          status   =  lcvtxsetalgorithmtype(vtx, "SimAnnealing")
        else if ( k .eq. 5 ) then
          status   =  lcvtxsetalgorithmtype(vtx, "_Test")
        end if
        
        status   =  lcvtxsetchi2 ( vtx, (i-1)*101.101 )
        status   =  lcvtxsetprobability ( vtx, 0.511*(i-1) )
        do  j = 1, 6
          cov(j) = j
        end do
        status   =  lcvtxsetcovmatrix ( vtx, cov )
        pos(1)  =  10.
        pos(2)  =  20.
        pos(3)  =  30.
        status   =  lcvtxsetposition ( vtx, pos )
        do  j = 1, 3
          status   = lcvtxaddparameter ( vtx, (j-1)*.1 )
        end do

***     add this vertex to collection
        status  =  lccoladdelement ( vtxcol, vtx )
        
      end do
      colname  =  'Vertices'
      status   =  lcevtaddcollection ( eventi, vtxcol, colname )
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
      character*30 colname
      double precision dpos(3), dhelp

      integer   nreccp
      parameter (nreccp=10)

***   add some reconstructed particles
      recopcol = lccolcreate (  LCIO_RECONSTRUCTEDPARTICLE )
      do  i = 1, nreccp
        recopp   =  lcrcpcreate ()
c        status   =  lcrcpsetprimary ( recopp, .true. )
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
        status   =  lcrcpsetreferencepoint (recopp, refp )

***     associate vertices
        vtx    =  lccolgetelementat ( vtxcol , i )
        status   =  lcrcpsetstartvertex ( recopp, vtx )
        vtx    =  lccolgetelementat ( vtxcol , i + 1 )
        status   =  lcvtxsetassociatedparticle ( vtx , recopp )

***     add some particle ids
        npid = 5
        do  j = 1, npid
          pidp     = lcpidcreate ()
          prob     = dble ( (j+1)/npid )
          status   = lcpidsetlikelihood (pidp, prob )
          status   = lcpidsettype ( pidp, j )
          status   = lcpidsetpdg ( pidp, -11 )


          status   = lcpidsetalgorithmtype ( pidp, 1 )
          do  k = 1, 3
            status   = lcpidaddparameter ( pidp, (k-1)*.1 )
          end do
          status   = lcrcpaddparticleid ( recopp, pidp )
        end do
        status   = lcrcpsetgoodnessofpid ( recopp, 0.7 )

***     some other particles
        if ( i .gt. 2 )  then
          recopp1 = lccolgetelementat ( recopcol , i - 1 )
          recopp2 = lccolgetelementat ( recopcol , i - 2 )
          status  = lcrcpaddparticle ( recopp, recopp1, 0.1 )
          status  = lcrcpaddparticle ( recopp, recopp2, 0.9 )
        end if


***     a track
        itrk    = dble ( lccolgetnumberofelements ( trackcol ) )*
     &                   irand( 0 )/RAND_MAX
        trkp    = lccolgetelementat ( trackcol, itrk + 1 )
        status  = lcrcpaddtrack ( recopp , trkp, 1.0 )

***     a cluster
        iclu    = dble ( lccolgetnumberofelements ( cluscol ) )*
     &                   irand( 0 )/RAND_MAX
        clusp   = lccolgetelementat ( cluscol, iclu + 1 )
        status  = lcrcpaddcluster ( recopp , clusp, 1.0 )

***     and finally an MCParticle
        mcpcol  = lcevtgetcollection( eventi , LCIO_MCPARTICLE )
        imcp    = dble ( lccolgetnumberofelements ( mcpcol ) )*
     &                   irand( 0 )/RAND_MAX
        mcpp    = lccolgetelementat ( mcpcol, imcp + 1 )
c        status  = lcrcpaddmcparticle ( recopp , mcpp, 0.5 )


***     add this reconstructed particle to collection
        status  =  lccoladdelement ( recopcol, recopp )
        
      end do
      colname  =  'ReconstructedParticle'
      status   =  lcevtaddcollection ( eventi, recopcol, colname )
*
      end


**********************************************************************
*                                                                    *
      subroutine modifyevent ( pevent )                             !*
*                                                                    *
**********************************************************************

      implicit none

#include "recjob.inc"

      PTRTYPE pevent
      integer nmcp, i

***   here we can modify existing objects that have been read from a stream:
      eventi  = pevent
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

***   now dump event and write
      call output_event

      end

**********************************************************************
*                                                                    *
      subroutine output_event                                       !*
*                                                                    *
**********************************************************************
*
      implicit none

#include "recjob.inc"

***   dump event
      status = lcdumpevent ( eventi )

***   write event
      status = lcwrtwriteevent ( writer, eventi )
*
      end

