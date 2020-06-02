// -- sio headers
#include <sio/memcpy.h>

namespace sio {

  void memcpy::reverse_copy( const sio::byte *const from, sio::byte * dest, std::size_t size, std::size_t count ) {
    for( std::size_t s=0 ; s<size ; s++ ) {
      for( std::size_t c=0 ; c<count ; c++ ) {
        dest [ (size-1) - s + c*size ] = from [ s + c*size ] ;
      }
    }
  }

  //--------------------------------------------------------------------------

  void memcpy::copy( const sio::byte *const from, sio::byte *dest, std::size_t size, std::size_t count ) {
  #ifdef SIO_BIG_ENDIAN
    std::memcpy( dest, from, size * count ) ;
  #else
    sio::memcpy::reverse_copy( from, dest, size, count ) ;
  #endif
  }

}
