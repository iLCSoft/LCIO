#include "SIO/SIOIntVecHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/LCIntVec.h"

#include "SIO_functions.h"
#include "SIO_block.h"

using namespace EVENT ;


namespace SIO{
    
  unsigned int SIOIntVecHandler::read(SIO_stream* stream, 
				      LCObject** objP,
				      unsigned int flag,
				      unsigned int vers ){
    unsigned int status ; 
	
    // create a new object :
    LCIntVec* vec  = new LCIntVec ;
    *objP = vec ;
	
    int nElements ;
    SIO_DATA( stream ,  &(nElements) , 1  ) ;

    //    int* x = new int[]( nElements ) ;
    for(int i=0;i<nElements;i++){
      int x ;
      SIO_DATA( stream , &x , 1 ) ;
      vec->push_back( x ) ;
    }

    return ( SIO_BLOCK_SUCCESS ) ;
	
  }
    
    
  unsigned int SIOIntVecHandler::write(SIO_stream* stream, 
				       const LCObject* obj,
				       unsigned int flag ){
    
    unsigned int status ; 

    const LCIntVec* vec = dynamic_cast<const LCIntVec*>(obj)  ;
    
    int nElements = vec->size() ;

    //    cout << " >>>>> writing  LCIntVec - size : " << nElements << ": " ;
    LCSIO_WRITE( stream, nElements ) ;
    for(int i=0;i<nElements;i++){
      LCSIO_WRITE( stream, (*vec)[i] ) ;
      //cout <<  (*vec)[i] <<   ", " ; 
    }    
    //    cout << endl ;

    return ( SIO_BLOCK_SUCCESS ) ;
    
  }
  
}; // namespace
