/**Header file for the f77-wrapper functions of the TPCHitImpl class.
 * 
 * @author F. Gaede
 * @version Oct 10, 2003
 */
#include "cfortran.h"
#include "cpointer.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

PTRTYPE lctphcreate() ;
int lctphdelete( PTRTYPE hit ) ;

int lctphid( PTRTYPE hit )  ;
int lctphgetcellid( PTRTYPE hit )  ;
float lctphgettime( PTRTYPE hit )  ;
float lctphgcharge( PTRTYPE hit )  ;
int lctphgetquality( PTRTYPE hit )  ;

int lctphgetnrawdatawords( PTRTYPE hit )  ;
int lctphgetrawdataword( PTRTYPE hit, int i)  ;

int lctphsetcellid( PTRTYPE hit, int id ) ;
int lctphsettime( PTRTYPE hit, float t );
int lctphsetcharge( PTRTYPE hit, float c );
int lctphsetquality( PTRTYPE hit, int q );
int lctphsetrawdata( PTRTYPE hit, int* rawData, int size ) ;
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

