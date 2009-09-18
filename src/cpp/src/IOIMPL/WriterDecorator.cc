#include "WriterDecorator.h"

#include "SIO/LCSIO.h"
#include "SIO/SIOWriter.h"

#include "RIO/RIOWriter.h"



namespace IOIMPL{

  
  void WriterDecorator::initialize(const std::string& filename){
    
    if( _wrt != 0 )
      return  ;      // already initialized 

    // check whether filename ends on .slcio or .rlcio
    if( filename.rfind( RIO_FILE_EXTENSION ) + strlen( RIO_FILE_EXTENSION ) == filename.length() ) { 
      
      _wrt =  new RIO::RIOWriter ;
    }
    else if( filename.rfind( SIO::LCSIO::FILE_EXTENSION ) + strlen( SIO::LCSIO::FILE_EXTENSION ) == filename.length() ) { 
      
      _wrt = new SIO::SIOWriter ;
    }
    else{
      
      _wrt =  new SIO::SIOWriter ;  // default 
    }
    
    
    if( _compressionLevel != -1 ) 

      _wrt->setCompressionLevel( _compressionLevel )  ;

  }

}
