/**Header file for the f77-wrapper functions of the TPCHitImpl class.
 * 
 * @author F. Gaede
 * @version Oct 10, 2003
 */
#include "cfortran.h"
#include "cpointer.h"

#include "deprecation.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

LCIO_DEPRECATED_CAPI PTRTYPE lctphcreate() ;
LCIO_DEPRECATED_CAPI int lctphdelete( PTRTYPE hit ) ;

LCIO_DEPRECATED_CAPI int lctphid( PTRTYPE hit )  ;
LCIO_DEPRECATED_CAPI int lctphgetcellid( PTRTYPE hit )  ;
LCIO_DEPRECATED_CAPI float lctphgettime( PTRTYPE hit )  ;
LCIO_DEPRECATED_CAPI float lctphgcharge( PTRTYPE hit )  ;
LCIO_DEPRECATED_CAPI int lctphgetquality( PTRTYPE hit )  ;

LCIO_DEPRECATED_CAPI int lctphgetnrawdatawords( PTRTYPE hit )  ;
LCIO_DEPRECATED_CAPI int lctphgetrawdataword( PTRTYPE hit, int i)  ;

LCIO_DEPRECATED_CAPI int lctphsetcellid( PTRTYPE hit, int id ) ;
LCIO_DEPRECATED_CAPI int lctphsettime( PTRTYPE hit, float t );
LCIO_DEPRECATED_CAPI int lctphsetcharge( PTRTYPE hit, float c );
LCIO_DEPRECATED_CAPI int lctphsetquality( PTRTYPE hit, int q );
LCIO_DEPRECATED_CAPI int lctphsetrawdata( PTRTYPE hit, int* rawData, int size ) ;
//int lctphinitrawdataarray( PTRTYPE hit, int size ) ;

// now the fortran wrappers from cfortran.h
extern "C"{
  
FCALLSCFUN0(CFORTRANPNTR, lctphcreate, LCTPHCREATE, lctphcreate ) 
FCALLSCFUN1(INT, lctphdelete, LCTPHDELETE, lctphdelete, CFORTRANPNTR ) 

FCALLSCFUN1(INT, lctphid, LCTPHID, lctphdid, CFORTRANPNTR ) 
FCALLSCFUN1(INT, lctphgetcellid,LCTPHGETCELLID,lctphgetcellid,CFORTRANPNTR) 
FCALLSCFUN1(FLOAT, lctphgettime,LCTPHGETTIME,lctphgettime,CFORTRANPNTR) 
FCALLSCFUN1(FLOAT, lctphgcharge,LCTPHGCHARGE,lctphgcharge,CFORTRANPNTR) 
FCALLSCFUN1(INT, lctphgetquality,LCTPHGETQUALITY,lctphgetquality,CFORTRANPNTR) 

FCALLSCFUN1(INT, lctphgetnrawdatawords,LCTPHGETNRAWDATAWORDS,lctphgetnrawdatawords,CFORTRANPNTR) 
FCALLSCFUN2(INT, lctphgetrawdataword,LCTPHGETRAWDATAWORD,lctphgetrawdataword,CFORTRANPNTR,INT) 

FCALLSCFUN2(INT, lctphsetcellid, LCTPHSETCELLID, lctphsetcellid, CFORTRANPNTR, INT ) 
FCALLSCFUN2(INT, lctphsettime, LCTPHSETTIME, lctphsettime, CFORTRANPNTR, FLOAT ) 
FCALLSCFUN2(INT, lctphsetcharge, LCTPHSETCHARGE, lctphsetcharge, CFORTRANPNTR, FLOAT ) 
FCALLSCFUN2(INT, lctphsetquality, LCTPHSETQUALITY, lctphsetquality, CFORTRANPNTR, INT ) 
FCALLSCFUN3(INT, lctphsetrawdata, LCTPHSETRAWDATA, lctphsetrawdata, CFORTRANPNTR, INTV, INT ) 
  //FCALLSCFUN2(INT, lctphinitrawdataarray, LCTPHINITRAWDATAARRAY, lctphinitrawdataarray, CFORTRANPNTR, INT ) 

}

