#include "SIO/SIORunHeaderHandler.h"
#include "SIO/LCSIO.h"

#include "EVENT/LCRunHeader.h"
#include "EVENT/LCIO.h"
#include "IMPL/LCRunHeaderImpl.h"

#include "SIO_functions.h"

#include <iostream>

using namespace EVENT ;
using namespace IMPL ;

namespace SIO  {


  SIORunHeaderHandler::SIORunHeaderHandler(const std::string& name) : 
    SIO_block( name.c_str() ),
    _rhP(0) {
  }

  SIORunHeaderHandler::SIORunHeaderHandler(const std::string& name, IMPL::LCRunHeaderImpl** aRhP) : 
    SIO_block( name.c_str() ),
    _rhP( aRhP ) {
  }


  SIORunHeaderHandler::~SIORunHeaderHandler(){
  }

  
  void SIORunHeaderHandler::setRunHeader(const LCRunHeader* rh ){
    _hdr = rh ;
  }


  unsigned int SIORunHeaderHandler::xfer( SIO_stream* stream, SIO_operation op, 
					  unsigned int versionID){
  
    unsigned int status ; // needed by the SIO_DATA macro
  
    if( op == SIO_OP_READ ){ 

      // for the run header we read all the data into temporary variables
      // as the data is mostly strings that needd temporaries anyhow ...
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

      
    }  else if( op == SIO_OP_WRITE ){ 
    
      if( _hdr ){

	LCSIO_WRITE( stream, _hdr->getRunNumber() ) ; 
	LCSIO_WRITE( stream, _hdr->getDetectorName() ) ;
	LCSIO_WRITE( stream, _hdr->getDescription() ) ;
	
	// now write list of  active sub detector names
	const StringVec* strVec = _hdr->getActiveSubdetectors() ;
	
	int nSDN = strVec->size() ;
	SIO_DATA( stream, &nSDN, 1 ) ;
	
	for( StringVec::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
	  LCSIO_WRITE( stream, *name ) ;
	} 
	
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
  
};
