
* include file for recojob
* ************************

#include "lciof77api.inc"


      PTRTYPE reader, writer
      PTRTYPE eventi, evento, header
      PTRTYPE mcpcol, mcpp
      PTRTYPE tpchitcol, trackerhitcol, trhitp, tpchit, trackcol, trkp
      PTRTYPE simcalcol, cluscol, calhitcol, simcalhitp, calhitp
      PTRTYPE recopcol, recopp, pidp, recopp1, recopp2
      PTRTYPE screlp

      common /pointers/ 
     &        reader, writer
     &,       eventi, evento, header
     &,       mcpcol, mcpp
     &,       tpchitcol, trackerhitcol, trhitp, tpchit, trackcol, trkp
     &,       simcalcol, cluscol, calhitcol, simcalhitp, calhitp
     &,       recopcol, recopp, pidp, recopp1, recopp2
     &,       ievent

      integer status, ievent

      double precision M_PI
      parameter       (M_PI=3.14159265358979323846)

