#include "SIO/SIOLCParameters.h"

#include "SIO/LCSIO.h"

#include "SIO_functions.h"
#include "SIO_block.h"

using namespace EVENT ;


namespace SIO{
    
  unsigned int SIOLCParameters::read(SIO_stream* stream, 
				     LCParameters& params,  
				     unsigned int vers) {

    unsigned int status ; 
	
	
    int nIntParameters ;
    SIO_DATA( stream , &nIntParameters , 1 ) ;

    for(int i=0; i< nIntParameters ; i++ ){

      char* keyTmp ; 
      LCSIO_READ( stream,  &keyTmp ) ; 
      std::string key( keyTmp ) ;

      int nInt  ;
      SIO_DATA( stream , &nInt , 1 ) ;
      IntVec intVec(nInt) ;

      for(int j=0; j< nInt ; j++ ){
	  SIO_DATA( stream , &intVec[j]  , 1 ) ;
      }
      params.setValues( key , intVec ) ;
    }

    int nFloatParameters ;
    SIO_DATA( stream , &nFloatParameters , 1 ) ;

    for(int i=0; i< nFloatParameters ; i++ ){

      char* keyTmp ; 
      LCSIO_READ( stream,  &keyTmp ) ; 
      std::string key( keyTmp ) ;

      int nFloat  ;
      SIO_DATA( stream , &nFloat , 1 ) ;
      FloatVec floatVec(nFloat) ;

      for(int j=0; j< nFloat ; j++ ){
	  SIO_DATA( stream , &floatVec[j]  , 1 ) ;
      }
      params.setValues( key , floatVec ) ;
    }

    int nStringParameters ;
    SIO_DATA( stream , &nStringParameters , 1 ) ;

    for(int i=0; i< nStringParameters ; i++ ){

      char* keyTmp ; 
      LCSIO_READ( stream,  &keyTmp ) ; 
      std::string key( keyTmp ) ;

      int nString  ;
      SIO_DATA( stream , &nString , 1 ) ;
      StringVec stringVec(nString) ;

      for(int j=0; j< nString ; j++ ){
	char* valTmp ; 
	LCSIO_READ( stream,  &valTmp ) ; 
	stringVec[j] = valTmp ;
      }
      params.setValues( key , stringVec ) ;
    }





    return ( SIO_BLOCK_SUCCESS ) ;
	
  }
    
    
  unsigned int SIOLCParameters::write(SIO_stream* stream, 
				      const LCParameters& params) {
    
    unsigned int status ; 

    StringVec intKeys ;
    int nIntParameters = params.getIntKeys( intKeys ).size() ;

    SIO_DATA( stream , &nIntParameters , 1 ) ;

    for(int i=0; i< nIntParameters ; i++ ){

      IntVec intVec ;

      params.getIntVals(  intKeys[i], intVec ) ;

      int nInt  = intVec.size()  ;     // = params.getNInt( intKeys[i] ) ;

//       if( nInt > 0 ){ 

	LCSIO_WRITE( stream, intKeys[i]  ) ;
	SIO_DATA( stream , &nInt , 1 ) ;


	for(int j=0; j< nInt ; j++ ){
	  LCSIO_WRITE( stream, intVec[j]  ) ;
 	}
//       }
    }

    StringVec floatKeys ;
    int nFloatParameters = params.getFloatKeys( floatKeys ).size() ;
    
    SIO_DATA( stream , &nFloatParameters , 1 ) ;

    for(int i=0; i< nFloatParameters ; i++ ){

      FloatVec floatVec ;

      params.getFloatVals(  floatKeys[i], floatVec ) ;

      int nFloat  = floatVec.size()  ;     // = params.getNFloat( floatKeys[i] ) ;

//       if( nFloat > 0 ){ 

	LCSIO_WRITE( stream, floatKeys[i]  ) ;
	SIO_DATA( stream , &nFloat , 1 ) ;


	for(int j=0; j< nFloat ; j++ ){
	  LCSIO_WRITE( stream, floatVec[j]  ) ;
	}
//       }
    }
    
    StringVec stringKeys ;
    int nStringParameters = params.getStringKeys( stringKeys ).size() ;
    
    SIO_DATA( stream , &nStringParameters , 1 ) ;
    
    for(int i=0; i< nStringParameters ; i++ ){
      
      StringVec stringVec ;
      
      params.getStringVals(  stringKeys[i], stringVec ) ;
      
      int nString  = stringVec.size()  ;     // = params.getNString( stringKeys[i] ) ;
      
//       if( nString > 0 ){ 
	
	LCSIO_WRITE( stream, stringKeys[i]  ) ;
	SIO_DATA( stream , &nString , 1 ) ;
	
	
	for(int j=0; j< nString ; j++ ){
	  LCSIO_WRITE( stream, stringVec[j]  ) ;
	}
//       }
    }


//     int nElements = vec->size() ;
    
//     //    cout << " >>>>> writing  LCParameters - size : " << nElements << ": " ;
//     LCSIO_WRITE( stream, nElements ) ;
//     for(int i=0;i<nElements;i++){
//       LCSIO_WRITE( stream, (*vec)[i] ) ;
//       //cout <<  (*vec)[i] <<   ", " ; 
//     }    
//     //    cout << endl ;

    return ( SIO_BLOCK_SUCCESS ) ;
    
  }
  
} // namespace
