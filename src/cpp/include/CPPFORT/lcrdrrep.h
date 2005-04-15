
/**Header file for the f77-wrapper function of the RunEventProcessor class.
 * 
 * @author H. Vogt
 * @version Jun 22, 2004
 */

#include "cfortran.h"
#include "cpointer.h"

#include "lcio.h" 

#include "IO/LCWriter.h"
#include "IO/LCReader.h"
#include "IO/LCEventListener.h"
#include "IO/LCRunListener.h"
#include "IOIMPL/LCFactory.h"

#include "EVENT/LCIO.h"
#include "EVENT/LCStrVec.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCEventImpl.h"


//--------------- convenient method to open a file and process it by the RunEventProcessor class
/**Opens a file for reading, register run and event listener, and process the input stream
 */
int lcrdreventprocessor( PTRTYPE filenamevec ) ;

// now the fortran wrappers from cfortran.h
extern "C"{
FCALLSCFUN1(INT, lcrdreventprocessor, LCRDREVENTPROCESSOR, lcrdreventprocessor, CFORTRANPNTR ) 
}

