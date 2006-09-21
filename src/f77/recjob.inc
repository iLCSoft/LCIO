
* include file for recojob
* ************************

#include "lciof77api.inc"
#include "lciof77apiext.inc"


      PTRTYPE reader, writer
      PTRTYPE eventi, evento, header
      PTRTYPE mcpcol, mcpp
      PTRTYPE tpchitcol, trackerhitcol, trhitp, tpchit, trackcol, trkp
      PTRTYPE simcalcol, cluscol, calhitcol, simcalhitp, calhitp
      PTRTYPE recopcol, recopp, pidp, recopp1, recopp2
      PTRTYPE vtxcol, vtx

***   temporary pointers used in several routines
      PTRTYPE screlp, screlcol

      common /pointers/ 
     &        reader, writer
     &,       eventi, evento, header
     &,       mcpcol, mcpp
     &,       tpchitcol, trackerhitcol, trhitp, tpchit, trackcol, trkp
     &,       simcalcol, cluscol, calhitcol, simcalhitp, calhitp
     &,       recopcol, recopp, pidp, recopp1, recopp2
     &,       vtxcol, vtx
     &,       ievent

      integer status, ievent

      double precision M_PI
      parameter       (M_PI=3.14159265358979323846)

