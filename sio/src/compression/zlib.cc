#include <sio/compression/zlib.h>

// -- sio headers
#include <sio/buffer.h>
#include <sio/exception.h>

// -- zlib headers
#include <zlib.h>

// -- std headers
#include <sstream>

namespace sio {

  void zlib_compression::set_level( int level ) {
    if(level < 0) {
      _level = Z_DEFAULT_COMPRESSION;
    }
    else if(level > 9) {
      _level = 9;
    }
    else {
      _level = level;
    }
  }
  
  //--------------------------------------------------------------------------

  int zlib_compression::level() const {
    return _level ;
  }

  //--------------------------------------------------------------------------

  void zlib_compression::uncompress( const buffer_span &inbuf, buffer &outbuf ) {
    if( not inbuf.valid() ) {
      SIO_THROW( sio::error_code::invalid_argument, "Buffer is not valid" ) ;
    }
    // zlib uncompress requires to pass an address of uLongf type ...
    uLongf outsize = outbuf.size() ;
    auto zstat = ::uncompress( (Bytef*)outbuf.data(), &outsize, (const Bytef*)inbuf.data(), inbuf.size() ) ;
    if( Z_OK != zstat ) {
      std::stringstream ss ;
      ss << "Zlib uncompression failed with status " << zstat ;
      SIO_THROW( sio::error_code::compress_error, ss.str() ) ;
    }
    SIO_DEBUG( "ZLIB uncompress OK!" ) ;
  }

  //--------------------------------------------------------------------------

  void zlib_compression::compress( const buffer_span &inbuf, buffer &outbuf ) {
    if( not inbuf.valid() ) {
      SIO_THROW( sio::error_code::invalid_argument, "Buffer is not valid" ) ;
    }
    // comp_bound is a first estimate of the compressed size.
    // After compression, the real output size is returned,
    // this is why the buffer is resized after calling compress2().
    auto comp_bound = ::compressBound( inbuf.size() ) ;
    if( outbuf.size() < comp_bound ) {
      outbuf.resize( comp_bound ) ;
    }
    auto zstat = ::compress2( reinterpret_cast<Bytef*>(outbuf.data()), &comp_bound, reinterpret_cast<const Bytef*>(inbuf.data()), inbuf.size(), _level ) ;
    if( Z_OK != zstat ) {
      std::stringstream ss ;
      ss << "Zlib compression failed with status " << zstat ;
      SIO_THROW( sio::error_code::compress_error, ss.str() ) ;
    }
    outbuf.resize( comp_bound ) ;
    SIO_DEBUG( "ZLIB compress OK!" ) ;
  }

}
