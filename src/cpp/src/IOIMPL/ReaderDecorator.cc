#include "ReaderDecorator.h"

#include "SIO/LCSIO.h"
#include "SIO/SIOReader.h"

#include "RIO/RIOReader.h"



namespace IOIMPL{

  
  void ReaderDecorator::initialize(const std::string& filename){
    
    if( _rdr != 0 )
      return  ;      // already initialized 

    // check whether filename ends on .slcio or .rlcio
    if( filename.rfind( RIO_FILE_EXTENSION ) + strlen( RIO_FILE_EXTENSION ) == filename.length() ) { 
      
      _rdr =  new RIO::RIOReader(_lcReaderFlag ) ;
    }
    else if( filename.rfind( SIO::LCSIO::FILE_EXTENSION ) 
	     + strlen( SIO::LCSIO::FILE_EXTENSION ) == filename.length() ) { 
      
      _rdr = new SIO::SIOReader(_lcReaderFlag ) ;
    }
    else{
      
      _rdr =  new SIO::SIOReader(_lcReaderFlag ) ;  // default 
    }


  }
  
}
