#include "SIO/LCSIO.h"

#include "SIO_functions.h"
#include "SIO_stream.h"
#include <cctype>
#include <cerrno>
#include "Exceptions.h"

#include <iostream>
#include <sstream>

using namespace EVENT ;

namespace SIO {

  const char* LCSIO::FILE_EXTENSION=".slcio" ;

  void LCSIO::checkVersion(int versionID){
    if ( SIO_VERSION_MAJOR( versionID ) < 1 && SIO_VERSION_MINOR(versionID) < 8)
      throw IO::IOException(" Old file versions ( < v00-08 ) no longer supported !") ;    
  }

  unsigned int LCSIO::read( SIO_stream* stream , std::string &str ) {
    // Read string len
    int strLen;
    unsigned int status = stream->read_data( &strLen  , 1  ) ;
    if( !( status & 1 ) ) return status ;
    str.resize( strLen );
    return stream->read_data( const_cast<char*>(str.c_str()), strLen );
  }

  unsigned int LCSIO::write( SIO_stream* stream , int i){
    return stream->write_data( &i , 1 );
  }
  unsigned int LCSIO::write( SIO_stream* stream , unsigned int i){
    return stream->write_data( &i , 1 );
  }

  // needed for LP64 architectures
#if defined(_LP64) || defined(__APPLE_CC__)
  unsigned int LCSIO::write( SIO_stream* stream , size_t i){
    return stream->write_data( &i , 1 );
  }
#endif

  unsigned int LCSIO::write( SIO_stream* stream , EVENT::long64 i){
    return stream->write_data( &i , 1 );
  }

  unsigned int LCSIO::write( SIO_stream* stream , float f){
    return stream->write_data( &f , 1 );
  }

  unsigned int LCSIO::write(SIO_stream* stream , const std::string& s) {
    int strLen = s.length() ;
    int status = stream->write_data( &strLen , 1 );
    if( !( status & 1 ) ) return status ;
    return stream->write_data( const_cast<char*>( s.c_str() ) , strLen );
  }
  
} // namespace 
 
