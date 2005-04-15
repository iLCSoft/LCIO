/**Header file for additional f77-wrapper functions. 
 * These functions add some convenient methods to the low level interface.
 * Typically they allow to access all of the data of a particular object with
 * one call.
 * 
 * @modified by F.Gaede (combined convenient api in one file and renamed some functions)
 * 
 * @author H. Vogt
 * @version Sep 30, 2003
 * @modif   Mar 19, 2004 (PTRTYPE consistency)
 *
 */

#include "cfortran.h"
#include "cpointer.h"


//--------------- convenient method to open a list of files for reading (read-only)

/**Opens a list of files for reading (read-only).
 */
int lcrdropenchain( PTRTYPE reader, void* filenamesv, const int nfiles, const int nchfilename );

//--------------- convenient methods to read/write the run header

/**Write a run header to the specified writer with the given data.
 */
int lcwriterunheader( PTRTYPE writer, const int irun, const char* detname, const char* descr,
                         void* sdnamevec, const int nsubd, const int nchsubd) ;

/**Read the next run header and fills the return arguments with the data.
 */
PTRTYPE lcreadnextrunheader( PTRTYPE reader, int* irun, void* detname, void* descr,
                            void* sdnamevec, int* nsubd, const int nchsubd) ;

//---------------  convenient methods for the event interface

/**Set the complete event header data in the event.
 */
int lcseteventheader( PTRTYPE event, const int irun, const int ievent, 
		      const int timestamp, const char* detname );

/**Read the complete event header data from the event.
 */
int lcgeteventheader( PTRTYPE event, int* irun, int* ievent, int* timestamp, void* detname );

/**Dump the run header to the stdout 
 */
int lcdumprunheader( PTRTYPE runheader ) ;

/**Dump the event to the stdout - one line per collection.
 */
int lcdumpevent( PTRTYPE event ) ;

/**Detailed dump of all the data in the event to stdout.
 */
int lcdumpeventdetailed( PTRTYPE event ) ;

//---------------  convenient methods for the mcparticle interface


/**Return all the data of the specified MCParticle in the arguments.
 */
int lcgetmcparticledata( PTRTYPE mcparticle, int* pdg, int* genstatus, int* simstatus
			 , double* prodvtx, float* momentum, float* mass, float* charge, 
			 int* ndaughters ) ;


/**Fill the hepevt common block with the MCParicle data in the LCIO event.
 */
int lcio2hepevt( PTRTYPE event ) ;

/**Create an MCParticle collection from the hepevt common block and add it to the event.
 */
int hepevt2lcio( PTRTYPE event ) ;



//---------------  convenient methods for SimTrackerHits

/**Add a new SimTrackerHit with the given data to the collection.
 */
int lcaddsimtrackerhit( PTRTYPE col, int cellID, double* pos, float dEdx,
			    float time, PTRTYPE mcp ) ;

/**Return all the data from the specified SimTrackerHit in the arguments.
 */
int lcgetsimtrackerhit( PTRTYPE col, int i, int* cellID, double* pos, float* dEdx, 
			    float* time, PTRTYPE* mcp ) ;


//---------------  convenient methods for SimCalorimeterHits

/**Add a new SimCalorimeterHit with the given data to the collection.
 * Returns a pointer the new hit.
 */
PTRTYPE lcaddsimcalohit( PTRTYPE col, int cellID0, int cellID1, float energy, float* pos ) ;

/**Return the data of the specified SimCalorimeterHit in the arguments.
 */
PTRTYPE lcgetsimcalohit( PTRTYPE col, int i, int* cellID0, int* cellID1, float* energy,
			 float* pos ) ;

/**Return the specified contribution of a MCParticle to the hit in the arguments.
 */
int lcgetsimcalohitmccont( PTRTYPE hit, int i, PTRTYPE* mcp, float* energy, float* time, 
			     int* pdg ) ;


/**Create an object vector
*/
PTRTYPE lcobjectvectorcreate( PTRTYPE* ocjectv, const int nobjv ) ;

/**Create an LC int vector
*/
PTRTYPE lcintvectorcreate( int* intv, const int nintv ) ;

/**Create a LC float vector
*/
PTRTYPE lcfloatvectorcreate( float* floatv, const int nfloatv ) ;

/**Create a LC string vector
*/
PTRTYPE lcstringvectorcreate( void* stringv, const int nstringv, const int nchstringv) ;

/**Return the content of a LC int vector
*/
int lcgetintvector( PTRTYPE vector, int* intv, int* nintv ) ;

/**Return the content of a LC float vector
*/
int lcgetfloatvector( PTRTYPE vector, float* floatv, int* nfloatv ) ;

/**Return the content of a LC string vector
*/
int lcgetstringvector( PTRTYPE vector, void* stringv, int* nstringv, const int nchstringv) ;

                  
/**Create/Delete an int vector
*/
PTRTYPE intvectorcreate( int* intv, const int nintv ) ;
int intvectordelete( PTRTYPE vector ) ;

/**Create/Delete a float vector
*/
PTRTYPE floatvectorcreate( float* floatv, const int nfloatv ) ;
int floatvectordelete( PTRTYPE vector ) ;

/**Create/Delete a string vector
*/
PTRTYPE stringvectorcreate( void* stringv, const int nstringv, const int nchstringv) ;
int stringvectordelete( PTRTYPE vector ) ;
                  
/**Create/Delete a pointer vector
*/
PTRTYPE pointervectorcreate( PTRTYPE* pointerv, const int npointerv ) ;
int pointervectordelete( PTRTYPE vector ) ;


/**Return the content of an int vector
*/
int getintvector( PTRTYPE vector, int* intv, int* nintv ) ;

/**Return the content of a float vector
*/
int getfloatvector( PTRTYPE vector, float* floatv, int* nfloatv ) ;

/**Return the content of a string vector
*/
int getstringvector( PTRTYPE vector, void* stringv, int* nstringv, const int nchstringv) ;

/**Return the content of a pointer vector
*/
int getpointervector( PTRTYPE vector, PTRTYPE* pointerv, int* npointerv ) ;



//---------------  convenient methods for the interface nterface to store generic named parameters 
//---------------  of type int, float and string.

/**For the set methods:
*/
int lcsetparameters( const char* classname, PTRTYPE classp, const char* method,
			    const char* key, PTRTYPE vecp) ;


/**For the get methods:
*/
int lcgetparameters( const char* classname, PTRTYPE classp, const char* method,
			    const char* key, PTRTYPE vecp) ;


// now the fortran wrappers from cfortran.h
extern "C"{

#define lcrdropenchain_STRV_A2 NUM_ELEM_ARG(3)
FCALLSCFUN4(INT, lcrdropenchain, LCRDRDOPENCHAIN, lcrdropenchain,
            CFORTRANPNTR, STRINGV, INT, INT) 


#define lcwriterunheader_STRV_A5 NUM_ELEM_ARG(6)
FCALLSCFUN7(INT, lcwriterunheader, LCWRITERUNHEADER, lcwriterunheader, CFORTRANPNTR, INT, 
	    STRING, STRING, STRINGV, INT, INT) 
#define lcreadnextrunheader_STRV_A3 NUM_ELEMS(1)
#define lcreadnextrunheader_STRV_A4 NUM_ELEMS(1)
#define lcreadnextrunheader_STRV_A5 NUM_ELEM_ARG(6)
FCALLSCFUN7(CFORTRANPNTR, lcreadnextrunheader, LCREADNEXTRUNHEADER, lcreadnextrunheader, 
	    CFORTRANPNTR, INTV, PSTRINGV, PSTRINGV, PSTRINGV, INTV, INT) 


FCALLSCFUN5(INT, lcseteventheader, LCSETEVENTHEADER, lcseteventheader, CFORTRANPNTR, 
	    INT, INT, INT, STRING ) 
#define lcgeteventheader_STRV_A5 NUM_ELEMS(1)
FCALLSCFUN5(INT, lcgeteventheader, LCGETEVENTHEADER, lcgeteventheader, CFORTRANPNTR, INTV, 
	    INTV, INTV, PSTRINGV ) 
FCALLSCFUN1(INT, lcdumprunheader, LCDUMPRUNHEADER, lcdumprunheader, CFORTRANPNTR ) 
FCALLSCFUN1(INT, lcdumpevent, LCDUMPEVENT, lcdumpevent, CFORTRANPNTR ) 
FCALLSCFUN1(INT, lcdumpeventdetailed, LCDUMPEVENTDETAILED, lcdumpeventdetailed,CFORTRANPNTR)


FCALLSCFUN9(INT, lcgetmcparticledata, LCGETMCPARTICLEDATA, lcgetmcparticledata,
            CFORTRANPNTR , INTV , INTV , INTV, DOUBLEV , FLOATV , FLOATV , FLOATV , INTV ) 
FCALLSCFUN1(INT, lcio2hepevt, LCIO2HEPEVT, lcio2hepevt , CFORTRANPNTR ) 
FCALLSCFUN1(INT, hepevt2lcio, HEPEVT2LCIO, hepevt2lcio, CFORTRANPNTR ) 


FCALLSCFUN6(INT, lcaddsimtrackerhit , LCADDSIMTRACKERHIT , lcaddsimtrackerhit, 
            CFORTRANPNTR , INT , DOUBLEV , FLOAT , FLOAT , CFORTRANPNTR ) 

FCALLSCFUN7(INT, lcgetsimtrackerhit, LCGETSIMTRACKERHIT, lcgetsimtrackerhit, 
            CFORTRANPNTR , INT , INTV , DOUBLEV , FLOATV , FLOATV , CFORTRANPNTRV ) 


FCALLSCFUN5(CFORTRANPNTR, lcaddsimcalohit, LCADDSIMCALOHIT, lcaddsimcalohit,
            CFORTRANPNTR, INT , INT, FLOAT, FLOATV ) 
FCALLSCFUN6(CFORTRANPNTR , lcgetsimcalohit, LCGETSIMCALOHIT, lcgetsimcalohit,
            CFORTRANPNTR , INT , INTV ,  INTV , FLOATV , FLOATV ) 
FCALLSCFUN6(INT, lcgetsimcalohitmccont, LCGETSIMCALOHITMCCONT, lcgetsimcalohitmccont,
            CFORTRANPNTR , INT , CFORTRANPNTRV, FLOATV , FLOATV , INTV ) 

FCALLSCFUN2(CFORTRANPNTR, lcobjectvectorcreate, LCOBJECTVECTORCREATE , lcobjectvectorcreate,
            CFORTRANPNTRV, INT ) 
FCALLSCFUN2(CFORTRANPNTR, lcintvectorcreate, LCINTVECTORCREATE , lcintvectorcreate,
            INTV, INT ) 
FCALLSCFUN2(CFORTRANPNTR, lcfloatvectorcreate, LCFLOATVECTORCREATE , lcfloatvectorcreate,
            FLOATV, INT ) 
#define lcstringvectorcreate_STRV_A1 NUM_ELEM_ARG(2)
FCALLSCFUN3(CFORTRANPNTR, lcstringvectorcreate, LCSTRINGVECTORCREATE , lcstringvectorcreate,
            PSTRINGV, INT, INT ) 

FCALLSCFUN3(INT, lcgetintvector, LCGETINTVECTOR , lcgetintvector,
            CFORTRANPNTR , INTV, INTV ) 
FCALLSCFUN3(INT, lcgetfloatvector, LCGETFLOATVECTOR , lcgetfloatvector,
            CFORTRANPNTR , FLOATV, INTV ) 
#define lcgetstringvector_STRV_A2 NUM_ELEM_ARG(3)
FCALLSCFUN4(INT, lcgetstringvector, LCGETSTRINGVECTOR , lcgetstringvector,
            CFORTRANPNTR , PSTRINGV, INTV, INT ) 


FCALLSCFUN2(CFORTRANPNTR, intvectorcreate, INTVECTORCREATE , intvectorcreate,
            INTV, INT ) 
FCALLSCFUN1(INT, intvectordelete, INTVECTORDELETE, intvectordelete,
            CFORTRANPNTR) 
FCALLSCFUN2(CFORTRANPNTR, floatvectorcreate, FLOATVECTORCREATE , floatvectorcreate,
            FLOATV, INT ) 
FCALLSCFUN1(INT, floatvectordelete, FLOATVECTORDELETE, floatvectordelete,
            CFORTRANPNTR) 
#define stringvectorcreate_STRV_A1 NUM_ELEM_ARG(2)
FCALLSCFUN3(CFORTRANPNTR, stringvectorcreate, STRINGVECTORCREATE , stringvectorcreate,
            PSTRINGV, INT, INT ) 
FCALLSCFUN1(INT, stringvectordelete, STRINGVECTORDELETE, stringvectordelete,
            CFORTRANPNTR) 
FCALLSCFUN2(CFORTRANPNTR, pointervectorcreate, POINTERVECTORCREATE , pointervectorcreate,
            CFORTRANPNTRV, INT ) 
FCALLSCFUN1(INT, pointervectordelete, POINTERVECTORDELETE, pointervectordelete,
            CFORTRANPNTR) 

FCALLSCFUN3(INT, getintvector, GETINTVECTOR , getintvector,
            CFORTRANPNTR , INTV, INTV ) 
FCALLSCFUN3(INT, getfloatvector, GETFLOATVECTOR , getfloatvector,
            CFORTRANPNTR , FLOATV, INTV ) 
#define getstringvector_STRV_A2 NUM_ELEM_ARG(3)
FCALLSCFUN4(INT, getstringvector, GETSTRINGVECTOR , getstringvector,
            CFORTRANPNTR , PSTRINGV, INTV, INT ) 
FCALLSCFUN3(INT, getpointervector, GETPOINTERVECTOR , getpointervector,
            CFORTRANPNTR , CFORTRANPNTRV, INTV ) 


FCALLSCFUN5(INT, lcsetparameters, LCSETPARAMETERS, lcsetparameters,
            STRING, CFORTRANPNTR, STRING, STRING, CFORTRANPNTR ) 

FCALLSCFUN5(INT, lcgetparameters, LCGETPARAMETERS, lcgetparameters,
            STRING, CFORTRANPNTR, STRING, STRING, CFORTRANPNTR ) 

}

