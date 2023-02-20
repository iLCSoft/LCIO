#include "SIO/SIOLCParameters.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO {
    
  void SIOLCParameters::read( sio::read_device &device, EVENT::LCParameters& params, sio::version_type vers ) {
    int nIntParameters ;
    SIO_DATA( device , &nIntParameters , 1 ) ;
    for(int i=0; i< nIntParameters ; i++ ) {
      std::string key;
      SIO_SDATA( device,  key ) ; 
      int nInt  ;
      SIO_DATA( device , &nInt , 1 ) ;
      EVENT::IntVec intVec(nInt) ;
      for(int j=0; j< nInt ; j++ ) {
	       SIO_DATA( device , &intVec[j]  , 1 ) ;
      }
      params.setValues( key , intVec ) ;
    }
    int nFloatParameters ;
    SIO_DATA( device , &nFloatParameters , 1 ) ;
    for(int i=0; i< nFloatParameters ; i++ ) {
      std::string key;
      SIO_SDATA( device,  key ) ; 
      int nFloat  ;
      SIO_DATA( device , &nFloat , 1 ) ;
      EVENT::FloatVec floatVec(nFloat) ;
      for(int j=0; j< nFloat ; j++ ) {
	      SIO_DATA( device , &floatVec[j]  , 1 ) ;
      }
      params.setValues( key , floatVec ) ;
    }
    if( vers > SIO_VERSION_ENCODE( 2, 16 )   ) {

      int nDoubleParameters ;
      SIO_DATA( device , &nDoubleParameters , 1 ) ;
      for(int i=0; i< nDoubleParameters ; i++ ) {
	std::string key;
	SIO_SDATA( device,  key ) ;
	int nDouble  ;
	SIO_DATA( device , &nDouble , 1 ) ;
	EVENT::DoubleVec doubleVec(nDouble) ;
	for(int j=0; j< nDouble ; j++ ) {
	  SIO_DATA( device , &doubleVec[j]  , 1 ) ;
	}
	params.setValues( key , doubleVec ) ;
      }
    }
    int nStringParameters ;
    SIO_DATA( device , &nStringParameters , 1 ) ;
    for(int i=0; i< nStringParameters ; i++ ) {
      std::string key;
      SIO_SDATA( device,  key ) ; 
      int nString  ;
      SIO_DATA( device , &nString , 1 ) ;
      EVENT::StringVec stringVec(nString) ;
      for(int j=0; j< nString ; j++ ){
        std::string val;
        SIO_SDATA( device,  val ) ; 
      	stringVec[j] = val ;
      }
      params.setValues( key , stringVec ) ;
    }
  }
    
  void SIOLCParameters::write( sio::write_device &device, const EVENT::LCParameters& params ) {
    EVENT::StringVec intKeys ;
    int nIntParameters = params.getIntKeys( intKeys ).size() ;
    SIO_DATA( device , &nIntParameters , 1 ) ;
    for(int i=0; i< nIntParameters ; i++ ) {
      EVENT::IntVec intVec ;
      params.getIntVals(  intKeys[i], intVec ) ;
      int nInt  = intVec.size()  ;
	    SIO_SDATA( device, intKeys[i]  ) ;
	    SIO_DATA( device , &nInt , 1 ) ;
	      for(int j=0; j< nInt ; j++ ) {
	        SIO_SDATA( device, intVec[j]  ) ;
        }
    }
    EVENT::StringVec floatKeys ;
    int nFloatParameters = params.getFloatKeys( floatKeys ).size() ;
    SIO_DATA( device , &nFloatParameters , 1 ) ;
    for(int i=0; i< nFloatParameters ; i++ ) {
      EVENT::FloatVec floatVec ;
      params.getFloatVals(  floatKeys[i], floatVec ) ;
      int nFloat  = floatVec.size()  ;     // = params.getNFloat( floatKeys[i] ) ;
    	SIO_SDATA( device, floatKeys[i]  ) ;
    	SIO_DATA( device , &nFloat , 1 ) ;
    	for(int j=0; j< nFloat ; j++ ){
    	  SIO_SDATA( device, floatVec[j]  ) ;
    	}
    }
    EVENT::StringVec doubleKeys ;
    int nDoubleParameters = params.getDoubleKeys( doubleKeys ).size() ;
    SIO_DATA( device , &nDoubleParameters , 1 ) ;
    for(int i=0; i< nDoubleParameters ; i++ ) {
      EVENT::DoubleVec doubleVec ;
      params.getDoubleVals(  doubleKeys[i], doubleVec ) ;
      int nDouble  = doubleVec.size()  ;     // = params.getNDouble( doubleKeys[i] ) ;
      SIO_SDATA( device, doubleKeys[i]  ) ;
      SIO_DATA( device , &nDouble , 1 ) ;
      for(int j=0; j< nDouble ; j++ ){
	SIO_SDATA( device, doubleVec[j]  ) ;
      }
    }
    EVENT::StringVec stringKeys ;
    int nStringParameters = params.getStringKeys( stringKeys ).size() ;
    SIO_DATA( device , &nStringParameters , 1 ) ;
    for(int i=0; i< nStringParameters ; i++ ){
      EVENT::StringVec stringVec ;
      params.getStringVals(  stringKeys[i], stringVec ) ;
      int nString  = stringVec.size()  ;
    	SIO_SDATA( device, stringKeys[i]  ) ;
    	SIO_DATA( device , &nString , 1 ) ;
    	for(int j=0; j< nString ; j++ ){
    	  SIO_SDATA( device, stringVec[j]  ) ;
    	}
    }
  }
  
} // namespace
