#include "SIO/SIOStrVecHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/LCStrVec.h"
#include "SIO_functions.h"
#include "SIO_block.h"
#include "SIO/LCSIO.h"

#include <iostream>
#include <cstring>

using namespace std ;
using namespace EVENT ;


namespace SIO{
    
  unsigned int SIOStrVecHandler::read(SIO_stream* stream,
				      LCObject** objP){
    unsigned int status ; 
	
    // create a new object :
    LCStrVec* vec  = new LCStrVec ;
    *objP = vec ;
	
    int nElements ;
    SIO_DATA( stream ,  &(nElements) , 1  ) ;

    //    char* x = new char*[]( nElements ) ;
    for(int i=0;i<nElements;i++){
      char* x ;
      int len ;
      LCSIO_READ_LEN( stream , &x , &len) ;

      //      vec->push_back( x ) ;
      vec->resize( vec->size() + 1 ) ;
      vec->back().resize(len) ;
      memcpy( &(vec->back()[0]) , x , len ) ;

    }

    if( _vers > SIO_VERSION_ENCODE( 1, 2)   ) {
      SIO_PTAG( stream , vec ) ;
    }
    return ( SIO_BLOCK_SUCCESS ) ;
	
  }
    
    
  unsigned int SIOStrVecHandler::write(SIO_stream* stream,
				       const LCObject* obj){
    
    unsigned int status ; 

    const LCStrVec* vec = dynamic_cast<const LCStrVec*>(obj)  ;
    
    int nElements = vec->size() ;

    LCSIO_WRITE( stream, nElements ) ;
    for(int i=0;i<nElements;i++){

      LCSIO_WRITE( stream, (*vec)[i] ) ;

//      std::cout <<  (*vec)[i] <<   ", " ;
    }    
//      std::cout << std::endl ;


    SIO_PTAG( stream , vec ) ;
    
    return ( SIO_BLOCK_SUCCESS ) ;
    
  }
  
} // namespace
