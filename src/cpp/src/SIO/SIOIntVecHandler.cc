#include "SIO/SIOIntVecHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/LCIntVec.h"

#include "SIO_functions.h"
#include "SIO_block.h"

using namespace EVENT ;


namespace SIO{
    
  unsigned int SIOIntVecHandler::read(SIO_stream* stream, 
				      LCObject** objP ){
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

    if( _vers > SIO_VERSION_ENCODE( 1, 2)   ) {
      SIO_PTAG( stream , vec ) ;
    }

    return ( SIO_BLOCK_SUCCESS ) ;
	
  }
    
    
  unsigned int SIOIntVecHandler::write(SIO_stream* stream, 
				       const LCObject* obj ){
    
    unsigned int status ; 

    const LCIntVec* vec = dynamic_cast<const LCIntVec*>(obj)  ;
    
    int nElements = vec->size() ;

    LCSIO_WRITE( stream, nElements ) ;

//     for(int i=0;i<nElements;i++){
//       LCSIO_WRITE( stream, (*vec)[i] ) ;
//       //cout <<  (*vec)[i] <<   ", " ; 
//     }    

    int*  v_0 = const_cast<int*> ( & (*vec)[0] ) ;

    SIO_DATA( stream , v_0 , nElements ) ;

//     SIO_DATA( stream , & ( (int) (*vec)[0] ) , nElements ) ;

    //    cout << endl ;

    SIO_PTAG( stream , vec ) ;
    
    return ( SIO_BLOCK_SUCCESS ) ;
    
  }
  
} // namespace
