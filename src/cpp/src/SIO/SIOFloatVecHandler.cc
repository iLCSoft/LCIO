#include "SIO/SIOFloatVecHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/LCFloatVec.h"

#include "SIO_functions.h"
#include "SIO_block.h"

using namespace EVENT ;


namespace SIO{
    
  unsigned int SIOFloatVecHandler::read(SIO_stream* stream, 
				      LCObject** objP ){
    unsigned int status ; 
	
    // create a new object :
    LCFloatVec* vec  = new LCFloatVec ;
    *objP = vec ;
	
    int nElements ;
    SIO_DATA( stream ,  &(nElements) , 1  ) ;

    //    float* x = new float[]( nElements ) ;
    for(int i=0;i<nElements;i++){
      float x ;
      SIO_DATA( stream , &x , 1 ) ;
      vec->push_back( x ) ;
    }

    if( _vers > SIO_VERSION_ENCODE( 1, 2)   ) {
      SIO_PTAG( stream , vec ) ;
    }
    
    return ( SIO_BLOCK_SUCCESS ) ;
	
  }
    
    
  unsigned int SIOFloatVecHandler::write(SIO_stream* stream, 
				       const LCObject* obj ){
    
    unsigned int status ; 

    const LCFloatVec* vec = dynamic_cast<const LCFloatVec*>(obj)  ;
    
    int nElements = vec->size() ;

    //    cout << " >>>>> writing  LCFloatVec - size : " << nElements << ": " ;
    LCSIO_WRITE( stream, nElements ) ;
    for(int i=0;i<nElements;i++){
      LCSIO_WRITE( stream, (*vec)[i] ) ;
      //cout <<  (*vec)[i] <<   ", " ; 
    }    
    //    cout << endl ;

    SIO_PTAG( stream , vec ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
    
  }
  
} // namespace
