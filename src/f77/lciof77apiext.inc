*************************************************************
*  Include file for the convenient functions of the
*  Fortran interface to LCIO.
*  For documentation purpose the C style function declarations 
*  are provided.
* 
*  F.Gaede
*  10/30/2003
*************************************************************

#include "lciof77pointer.inc"

c --------------- convenient method to open a list of files for reading (read-only)

*     Opens a list of files for reading (read-only).
*
*     int lcrdropenchain( PTRTYPE reader, 
*                       void* filenamesv, const int nfiles, const int nchfilename );
      integer lcrdropenchain

c --------------- convenient methods to read/write the run header

*     Write a run header to the specified writer with the given data.
*     
*     int lcwriterunheader(PTRTYPE writer,int irun, const char* detname, const char* descr,
*                         void* sdnamevec, const int nsubd, const int nchsubd) ;
      integer lcwriterunheader
      
*     Read the next run header and fills the return arguments with the data.
*
*     PTRTYPE lcreadnextrunheader(PTRTYPE reader, int* irun, void* detname, void* descr,
*     void* sdnamevec, int* nsubd, const int nchsubd) ;

      PTRTYPE lcreadnextrunheader

c ---------------  convenient methods for the event interface

*     Set the complete event header data in the event.
*     
*     int lcseteventheader( PTRTYPE event, const int irun, const int ievent, 
*     const int timestamp, const char* detname );

      integer lcseteventheader

*     Read the complete event header data from the event.
*     
*     int lcgeteventheader(PTRTYPE event, int* irun, int* ievent, int* timestamp, void* detname );

      integer lcgeteventheader

*     Dump the run header to the stdout
*      
*     int lcdumprunheader( PTRTYPE runheader ) ;

      integer lcdumprunheader

*     Dump the event to the stdout - one line per collection.
*      
*     int lcdumpevent( PTRTYPE event ) ;

      integer lcdumpevent

*     Detailed dump of all the data in the event to stdout.
*     
*     int lcdumpeventdetailed( PTRTYPE event ) ;

      integer lcdumpeventdetailed

c ---------------  convenient methods for the mcparticle interface


*     Return all the data of the specified MCParticle in the arguments.
*     
*     int lcgetmcparticledata( PTRTYPE mcparticle, int* pdg, int* hepevtstatus, double* prodvtx,
*     float* momentum, float* mass, float* charge, int* ndaughters ) ;

      integer lcgetmcparticledata
      
*     Fill the hepevt common block with the MCParicle data in the LCIO event.
*     
*     int lcio2hepevt( PTRTYPE event ) ;
      
      integer lcio2hepevt

*     Create an MCParticle collection from the hepevt common block and add it to the event.
*     
*     int hepevt2lcio( PTRTYPE event ) ;

      integer hepevt2lcio
      

c ---------------  convenient methods for SimTrackerHits

*     Add a new SimTrackerHit with the given data to the collection.
*     
*     int lcaddsimtrackerhit( PTRTYPE col, int cellID, double* pos, float dEdx,
*     float time, PTRTYPE mcp ) ;

      integer lcaddsimtrackerhit

*     Return all the data from the specified SimTrackerHit in the arguments.
*     
*     int lcgetsimtrackerhit( PTRTYPE col, int i, int* cellID, double* pos, float* dEdx, 
*     float* time, PTRTYPE* mcp ) ;

      integer lcgetsimtrackerhit

c ---------------  convenient methods for SimCalorimeterHits

*     Add a new SimCalorimeterHit with the given data to the collection.
*     Returns a pointer the new hit.
*     
*     PTRTYPE lcaddsimcalohit( PTRTYPE col, int cellID0, int cellID1, float energy, float* pos ) ;

      PTRTYPE lcaddsimcalohit

*     Return the data of the specified SimCalorimeterHit in the arguments.
*     
*     int lcgetsimcalohit( PTRTYPE col, int i, int* cellID0, int* cellID1, float* energy, 
*     float* pos ) ;

      PTRTYPE lcgetsimcalohit

*     Return the specified contribution of a MCParticle to the hit in the arguments.
*     
*     int lcgetsimcalohitmccont( PTRTYPE hit, int i, PTRTYPE* mcp, float* energy, float* time, 
*     int* pdg ) ;

      integer lcgetsimcalohitmccont


c ---------------  convenient methods for the vector interface

*     Create a complete LC vector of Integers
*
*     PTRTYPE lcintvectorcreate ( int* intv, const int nintv )

      PTRTYPE lcintvectorcreate

*     Create a complete LC vector of Floats
*
*     PTRTYPE lcfloatvectorcreate ( float* floatv , const int nfloatv )

      PTRTYPE lcfloatvectorcreate

*     Create a complete LC vector of Strings
*
*     PTRTYPE lcstringvectorcreate ( void* stringv, const int nstringv, const int nchstringv)

      PTRTYPE lcstringvectorcreate

*     Get the complete LC vector of Integers
*     
*     int lcgetintvector( PTRTYPE vector, int* intv, int* nintv )

      integer lcgetintvector

*     Get the complete LC vector of Floats
*     
*     int lcgetfloatvector( PTRTYPE vector, float* floatv , int* nfloatv )

      integer lcgetfloatvector

*     Get the complete LC vector of Strings
*     
*     int lcgetstringvector( PTRTYPE vector, void* stringv, int* nstringv, const int nchstringv) 

      integer lcgetstringvector


*     Create/Delete a complete vector of Integers
*
*     PTRTYPE intvectorcreate ( int* intv, const int nintv )
*     int intvectordelete( PTRTYPE vector )

      PTRTYPE intvectorcreate
      integer intvectordelete

*     Createe/Delete a complete vector of Floats
*
*     PTRTYPE floatvectorcreate ( float* floatv , const int nfloatv )
*     int floatvectordelete( PTRTYPE vector )

      PTRTYPE floatvectorcreate
      integer floatvectordelete

*     Createe/Delete a complete vector of Strings
*
*     PTRTYPE stringvectorcreate ( void* stringv, const int nstringv, const int nchstringv)
*     int stringvectordelete( PTRTYPE vector )

      PTRTYPE stringvectorcreate
      integer stringvectordelete

*     Createe/Delete a complete vector of Pointers
*
*     PTRTYPE pointervectorcreate( PTRTYPE* pointerv, const int npointerv )
*     int pointervectordelete( PTRTYPE vector )

      PTRTYPE pointervectorcreate
      integer pointervectordelete


*     Get the complete vector of Integers
*     
*     int getintvector( PTRTYPE vector, int* intv, int* nintv )

      integer getintvector

*     Get the complete vector of Floats
*     
*     int getfloatvector( PTRTYPE vector, float* floatv , int* nfloatv )

      integer getfloatvector

*     Get the complete vector of Strings
*     
*     int getstringvector( PTRTYPE vector, void* stringv, int* nstringv, const int nchstringv) 

      integer getstringvector

*     Get the complete vector of Pointers
*     
*     int getpointervector( PTRTYPE vector, PTRTYPE* pointerv, int* npointerv )

      integer getpointervector


*     Set methods for LCParameters
*
*     int lcsetparameters ( const char* classname, PTRTYPE classp, const char* method, const char* key, PTRTYPE vecp)

      integer lcsetparameters

*     Get methods for LCParameters
*
*     int lcgetparameters ( const char* classname, PTRTYPE classp, const char* method, const char* key, PTRTYPE vecp)

      integer lcgetparameters

c----  need to define the HEPEVT common block
#include "hepevt0.inc"
#include "hepevt1.inc"

