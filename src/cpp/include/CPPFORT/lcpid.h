/**Header file for the f77-wrapper functions of the ParticleID Class.
 * 
 * @author H. Vogt
 * @version May 10, 2004
 */
#include "cfortran.h"
#include "cpointer.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

PTRTYPE lcpidcreate() ;
int     lcpiddelete( PTRTYPE pid ) ;

int     lcpidid( PTRTYPE pid ) ;
int     lcpidgettypeid( PTRTYPE pid ) ;
float   lcpidgetprobability( PTRTYPE pid ) ;
char*   lcpidgetidentifier( PTRTYPE pid ) ;
int     lcpidgetparameters( PTRTYPE pid, float* vec, int* nvec ) ;

int     lcpidsettypeid( PTRTYPE pid, int type ) ;
int     lcpidsetprobability( PTRTYPE pid, float prob ) ;
int     lcpidsetidentifier( PTRTYPE pid, char* ident ) ;
int     lcpidaddparameter( PTRTYPE pid, float param ) ;


// now the fortran wrappers from cfortran.h              
extern "C"{
  
FCALLSCFUN0(CFORTRANPNTR, lcpidcreate, LCPIDCREATE, lcpidcreate ) ;
FCALLSCFUN1(INT, lcpiddelete, LCPIDDELETE, lcpiddelete, CFORTRANPNTR ) ;

FCALLSCFUN1(INT, lcpidid, LCPIDID, lcpidid, CFORTRANPNTR) ;
FCALLSCFUN1(INT, lcpidgettypeid, LCPIDGETTYPEID, lcpidgettypeid, CFORTRANPNTR) ;
FCALLSCFUN1(FLOAT, lcpidgetprobability, LCPIDGETPROBABILITY, lcpidgetprobability, CFORTRANPNTR) ;
FCALLSCFUN1(STRING, lcpidgetidentifier, LCPIDGETIDENTIFIER, lcpidgetidentifier, CFORTRANPNTR ) ;
FCALLSCFUN3(INT, lcpidgetparameters, LCPIDGETPARAMETERS, lcpidgetparameters, CFORTRANPNTR, FLOATV, INTV) ;

FCALLSCFUN2(INT, lcpidsettypeid, LCPIDSETTYPEID, lcpidsettypeid, CFORTRANPNTR, INT) ;
FCALLSCFUN2(INT, lcpidsetprobability, LCPIDSETPROBABILITY, lcpidsetprobability, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN2(INT, lcpidsetidentifier, LCPIDSETIDENTIFIER, lcpidsetidentifier, CFORTRANPNTR, STRING) ;
FCALLSCFUN2(INT, lcpidaddparameter, LCPIDADDPARAMETER, lcpidaddparameter, CFORTRANPNTR, FLOAT) ;

}
  
           
