****************************************************
*  Include file for the low level Fortran interface 
*  of LCIO. There is on function for each method of
*  al the relevenat classes in LCIO. Please include
*  this file in your Fortran program and use the
*  C++ API documentation and the naming convention
*  described in the users manual to find the
*  fortran function you need.
*
*  F.Gaede
*  10/30/2003
*  H. Vogt
*  05/06/2004  (reconstruction part added)
*
***************************************************

#include "lciof77pointer.inc"


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
      PTRTYPE lcrhdgetactivesubdetectors
      integer lcrhddelete, lcrhdgetrunnumber
      character*80 lcrhdgetdetectorname
      character*1024 lcrhdgetdescription
      integer lcrhdsetrunnumber, lcrhdsetdetectorname
      integer lcrhdsetdescription, lcrhdaddactivesubdetector

c-----the LCStrVec,LCIntVec,LCFloatVec interface
      integer lcsvcgetlength,lcivcgetlength,lcfvcgetlength
      character*80 lcsvcgetstringat
      integer lcsvcgetintat
      real    lcsvcgetfloatgat

c-----the LCEvent interface
      PTRTYPE lcevtcreate, lcevtgetcollection, lcevtgetcollectionnames
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
      PTRTYPE lcmcpcreate
      integer lcmcpgetnumberofparents, lcmcpgetnumberofdaughters
      PTRTYPE lcmcpgetparent, lcmcpgetdaughter 
      integer lcmcpdelete, lcmcpgetendpoint
      integer lcmcpgetpdg, lcmcpgetgeneratorstatus
      integer lcmcpgetsimulatorstatus
      integer lcmcpgetvertex, lcmcpgetmomentum
      real    lcmcpgetmass, lcmcpgetcharge, lcmcpgetenergy
      integer lcmcpaddparent, lcmcpadddaughter 
      integer lcmcpsetpdg, lcmcpsetgeneratorstatus
      integer lcmcpsetsimulatorstatus, lcmcpsetvertex 
      integer lcmcpsetendpoint, lcmcpsetmomentum, lcmcpsetmass 
      integer lcmcpsetcharge

c-----the SimTrackerHit interface
      PTRTYPE lcsthcreate, lcsthgetmcparticle
      integer lcsthdelete, lcsthgetcellid
      real    lcsthgetdedx, lcsthgettime
      double precision lcsthgetposition
      integer lcsthsetcellid, lcsthsetposition, lcsthsetdedx   
      integer lcsthsettime, lcsthsetmcparticle

c-----the TPCHitImpl interface
      PTRTYPE lctphcreate
      integer lctphdelete, lctphgetcellid, lctphgetquality
      integer lctphgetnrawdatawords, lctphgetrawdataword
      real    lctphgettime, lctphgcharge
      integer lctphsetcellid, lctphsettime, lctphsetcharge
      integer lctphsetquality, lctphsetrawdata


c-----the SimCalorimeterHit interface
      PTRTYPE lcschcreate, lcschgetparticlecont
      integer lcschdelete, lcschgetcellid0, lcschgetcellid1  
      real    lcschgetenergy, lcschgetenergycont, lcschgettimecont
      integer lcschgetposition, lcschgetnmcparticles
      integer lcschgetpdgcont
      integer lcschsetcellid0, lcschsetcellid1, lcschsetenergy
      integer lcschsetposition, lcschaddmcparticlecontribution

c-----the CalorimeterHit interface
      PTRTYPE lccahcreate
      integer lccahdelete, lccahgetcellid0, lccahgetcellid1  
      real    lccahgetenergy
      integer lccahgetposition
      integer lccahsetcellid0, lccahsetcellid1, lccahsetenergy
      integer lccahsetposition

c-----the Cluster interface
      PTRTYPE lcclucreate, lcclugetclusters, lcclugetcalorimeterhits
      integer lccludelete, lcclugetid, lcclugettype
      integer lcclugetposition
      integer lcclugetpositionerror, lcclugetdirectionerror
      integer lcclugetshape, lcclugetparticletype
      integer lcclugethitcontributions
      real    lcclugetenergy, lcclugettheta, lcclugetphi
      integer lcclusettypebit, lcclusetenergy, lcclusetposition
      integer lcclusetpositionerror,lcclusettheta, lcclusetphi
      integer lcclusetdirectionerror, lcclusetshape, lcclusetemweight
      integer lcclusethadweight, lcclusetmuonweight, lccluaddcluster
      integer lccluaddhit

c-----the ParticleID interface
      PTRTYPE lcpidcreate
      integer lcpiddelete, lcpidid, lcpidgettypeid, lcpidgetparameters
      real    lcpidgetprobability
      character*80 lcpidgetidentifier
      integer lcpidsettypeid, lcpidsetprobability, lcpidsetidentifier
      integer lcpidaddparameter

c-----the ReconstructedParticle interface
      PTRTYPE lcrcpcreate, lcrcpgetparticleids, lcrcpgetparticles
      PTRTYPE lcrcpgetclusters, lcrcpgettracks, lcrcpgetmcparticles
      integer lcrcpgettype, lcrcpgetmomentum, lcrcpgetcovmatrix
      integer lcrcpgetreferencepoint, lcrcpgetparticleweights
      integer lcrcpgetclusterweights, lcrcpgettrackweights
      integer lcrcpgetmcparticleweights
      real    lcrcpgetenergy, lcrcpgetmass, lcrcpgetcharge
      logical lcrcpisprimary
      integer lcrcpsettype, lcrcpsetprimary, lcrcpsetmomentum
      integer lcrcpsetenergy, lcrcpsetcovmatrix, lcrcpsetmass
      integer lcrcpsetcharge, lcrcpsetreferencepoint, lcrcpaddparticleid
      integer lcrcpaddparticle, lcrcpaddcluster, lcrcpaddtrack
      integer lcrcpaddmcparticle

c-----the TrackerHit interface
      PTRTYPE lctrhcreate, lctrhgetrawdatahit
      integer lctrhdelete, lctrhid, lctrhgetposition, lctrhgetcovmatrix
      real    lctrhgetdedx, lctrhgettime
      character*80 lctrhgettype
      integer lctrhsetposition, lctrhsetcovmatrix, lctrhsetdedx
      integer lctrhsettime, lctrhsettpchit

c-----the Track interface
      PTRTYPE lctrkcreate, lctrkgettracks, lctrkgettrackerhits
      integer lctrkdelete, lctrkid, lctrkgettype, lctrkgetcovmatrix
      integer lctrkgetreferencepoint, lctrkgetisreferencepointpca
      real    lctrkgetomega, lctrkgetanlambda, lctrkgetphi, lctrkgetd0
      real    lctrkgetz0, lctrkgetchi2, lctrkgetdedx, lctrkgetdedxerror
      integer lctrkgetndf
      integer lctrksettypebit, lctrksetomega, lctrksettanlambda
      integer lctrksetphi, lctrksetd0, lctrksetz0, lctrksetcovmatrix
      integer lctrksetreferencepoint, lctrksetchi2, lctrksetdedx
      integer lctrksetisreferencepointpca, lctrksetndf
      integer lctrksetdedxerror, lctrkaddtrack, lctrkaddhit

c---- constants as defined in EVENT::LCIO

      integer LCIO_CHBIT_LONG
      integer LCIO_CHBIT_BARREL
      integer LCIO_CHBIT_ID1
      integer LCIO_CHBIT_PDG
      integer LCIO_RCHBIT_LONG
      integer LCIO_RCHBIT_BARREL
      integer LCIO_RCHBIT_ID1
      integer LCIO_RCHBIT_PTR
      integer LCIO_THBIT_BARREL
      integer LCIO_TRBIT_HITS
      integer LCIO_CLBIT_HITS
      integer LCIO_TPCBIT_RAW
      integer LCIO_TPCBIT_PTR
      integer LCIO_READ_ONLY
      integer LCIO_UPDATE
      integer LCIO_ERROR
      integer LCIO_SUCCESS
      integer LCIO_WRITE_NEW
      integer LCIO_WRITE_APPEND

      parameter (LCIO_CHBIT_LONG=31, LCIO_CHBIT_BARREL=30,
     &           LCIO_CHBIT_PDG=28)
      parameter (LCIO_RCHBIT_LONG=31, LCIO_RCHBIT_BARREL=30,
     &           LCIO_RCHBIT_ID1=29, LCIO_RCHBIT_PTR=28)
      parameter (LCIO_THBIT_BARREL=31)
      parameter (LCIO_TRBIT_HITS=31)
      parameter (LCIO_CLBIT_HITS=31)
      parameter (LCIO_TPCBIT_RAW=31, LCIO_TPCBIT_PTR=30)
      parameter (LCIO_READ_ONLY=0, LCIO_UPDATE=1)
      parameter (LCIO_ERROR=0, LCIO_SUCCESS=1)
      parameter (LCIO_WRITE_NEW=0, LCIO_WRITE_APPEND =1)

      character*32 LCIO_LCEVENT 
      character*32 LCIO_LCRUNHEADER 
      character*32 LCIO_MCPARTICLE  
      character*32 LCIO_SIMCALORIMETERHIT 
      character*32 LCIO_CALORIMETERHIT 
      character*32 LCIO_SIMTRACKERHIT  
      character*32 LCIO_TPCHIT  
      character*32 LCIO_TRACKERHIT
      character*32 LCIO_LCFLOATVEC  
      character*32 LCIO_LCINTVEC  
      character*32 LCIO_TRACK
      character*32 LCIO_CLUSTER
      character*32 LCIO_RECONSTRUCTEDPARTICLE

      data LCIO_LCEVENT /'LCEvent'/
      data LCIO_LCRUNHEADER /'LCRunHeader'/
      data LCIO_MCPARTICLE /'MCParticle'/ 
      data LCIO_SIMCALORIMETERHIT /'SimCalorimeterHit'/
      data LCIO_CALORIMETERHIT /'CalorimeterHit'/
      data LCIO_SIMTRACKERHIT /'SimTrackerHit'/ 
      data LCIO_TPCHIT /'TPCHit'/
      data LCIO_TRACKERHIT /'TrackerHit'/
      data LCIO_LCFLOATVEC /'LCFloatVec'/ 
      data LCIO_LCINTVEC /'LCIntVec'/ 
      data LCIO_TRACK /'Track'/
      data LCIO_CLUSTER /'Cluster'/
      data LCIO_RECONSTRUCTEDPARTICLE /'ReconstructedParticle'/


