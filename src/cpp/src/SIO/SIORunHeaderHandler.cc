#include "SIO/SIORunHeaderHandler.h"
#include "SIO/SIOLCParameters.h"
#include "SIO/LCSIO.h"

#include "EVENT/LCRunHeader.h"
#include "EVENT/LCIO.h"

#include "SIO_functions.h"

#include <iostream>


using namespace EVENT ; // for LCIO object
using namespace IOIMPL ;

namespace SIO  {


  SIORunHeaderHandler::SIORunHeaderHandler(const std::string& name) : 
    SIO_block( name.c_str() ),
    _rhP(0) {
  }

  SIORunHeaderHandler::SIORunHeaderHandler(const std::string& name, IOIMPL::LCRunHeaderIOImpl** aRhP) : 
    SIO_block( name.c_str() ),
    _rhP( aRhP ) {
    
    *_rhP = 0 ;
  }


  SIORunHeaderHandler::~SIORunHeaderHandler(){
//     if (*_rhP != 0 )  delete *_rhP ;
  }

  
  void SIORunHeaderHandler::setRunHeader(const LCRunHeader* rh ){
    _hdr = rh ;
  }
  void SIORunHeaderHandler::setRunHeaderPtr( LCRunHeaderIOImpl** hdrP ) {
    _rhP= hdrP ;
  } 


  unsigned int SIORunHeaderHandler::xfer( SIO_stream* stream, SIO_operation op, 
					  unsigned int versionID){
  
    LCSIO::checkVersion(versionID) ;
    
    unsigned int status ; // needed by the SIO_DATA macro
  
    if( op == SIO_OP_READ ){ 

      if(!_rhP) return LCIO::ERROR ;  // in read mode we need an address for the pointer

      // delete the old run header object 
      // -> for every handler there will only be one RunHeader object at any given time
      if (*_rhP != 0 )  delete *_rhP ;
      *_rhP = new IOIMPL::LCRunHeaderIOImpl ;

      // for the run header we read all the data into temporary variables
      // as the data is mostly strings that need temporaries anyhow ...
      int rnTmp ;
      SIO_DATA( stream ,  &rnTmp  , 1  ) ;
      (*_rhP)->setRunNumber( rnTmp    );

      char* detNameTmp ; 
      LCSIO_READ( stream,  &detNameTmp ) ; 
      (*_rhP)->setDetectorName( detNameTmp  )  ;

      char* descTmp ; 
      LCSIO_READ( stream,  &descTmp ) ; 
      (*_rhP)->setDescription( descTmp  )  ;
      
      // read  active sub detector names
      int nSDN ;
      SIO_DATA( stream ,  &nSDN , 1 ) ;
      for( int i=0; i<nSDN ; i++ ){

	char* sdnTmp ; 
	LCSIO_READ( stream,  &sdnTmp ) ; 

	(*_rhP)->addActiveSubdetector( sdnTmp ) ;
      }

      // read parameters
      if( versionID > SIO_VERSION_ENCODE( 1, 1)   ) 
	SIOLCParameters::read( stream ,  (*_rhP)->parameters() , versionID) ;
      
    }  else if( op == SIO_OP_WRITE ){ 
    
      if( _hdr ){

	LCSIO_WRITE( stream, _hdr->getRunNumber() ) ; 
	LCSIO_WRITE( stream, _hdr->getDetectorName() ) ;
	LCSIO_WRITE( stream, _hdr->getDescription() ) ;
	
	// now write list of  active sub detector names
	const std::vector<std::string>* strVec = _hdr->getActiveSubdetectors() ;
	
	int nSDN = strVec->size() ;
	SIO_DATA( stream, &nSDN, 1 ) ;
	
	for( std::vector<std::string>::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
	  LCSIO_WRITE( stream, *name ) ;
	} 
	
	// write parameters
	if( version() > SIO_VERSION_ENCODE( 1, 1) ) 
	  SIOLCParameters::write( stream , _hdr->getParameters() ) ;

	

      } else {
	std::cout << " SIORunHeaderHandler::xfer : run header pointer not set !  " << std::endl ;
	return LCIO::ERROR ;
      }
    }
    
    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
  unsigned int   SIORunHeaderHandler::version(){
    
    int version = SIO_VERSION_ENCODE( LCIO::MAJORVERSION, LCIO::MINORVERSION ) ;
    return version ;
    
  }
  
}
