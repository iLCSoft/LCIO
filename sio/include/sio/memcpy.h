#pragma once

// -- sio headers
#include <sio/definitions.h>

// -- std headers
#include <cstring>  // std::memcpy, std::size_t

namespace sio {

  /**
   *  @brief  sizeof_helper struct.
   *          Helper structure to hardcode the size of some types.
   *          This is there to ensure backward compatibility with
   *          the old SIO implementation
   */
  template <typename T>
  struct sizeof_helper {
    static constexpr std::size_t size = sizeof(T) ;
  };

#define SIO_FORCE_SIZEOF( TYPE, SIZE ) \
  template <> \
  struct sizeof_helper<TYPE> { \
    static constexpr std::size_t size = SIZE ; \
  }

  SIO_FORCE_SIZEOF( char, 1 ) ;
  SIO_FORCE_SIZEOF( unsigned char, 1 ) ;
  SIO_FORCE_SIZEOF( short, 2 ) ;
  SIO_FORCE_SIZEOF( unsigned short, 2 ) ;
  SIO_FORCE_SIZEOF( int , 4 ) ;
  SIO_FORCE_SIZEOF( unsigned int , 4 ) ;
#if defined(_AIX)      ||  defined(__alpha__) || defined(__i386__)  || defined(__sparc__) || defined(__APPLE_CC__) || defined(_LP64)
  SIO_FORCE_SIZEOF( long long , 8 ) ;
  SIO_FORCE_SIZEOF( unsigned long long , 8 ) ;
#else
  SIO_FORCE_SIZEOF( __int64 , 8 ) ;
  SIO_FORCE_SIZEOF( unsigned __int64 , 8 ) ;
#endif
  SIO_FORCE_SIZEOF( float , 4 ) ;
  SIO_FORCE_SIZEOF( double , 8 ) ;

#undef SIO_FORCE_SIZEOF

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  /**
   *  @brief  memcpy class
   *
   *  Perform raw data copy and deals with plateform endian-ness
   */
  class memcpy {
  public:
    // static API only
    memcpy() = delete ;

    /**
     *  @brief  Perform a reverse byte copy
     *
     *  @param  from the input bytes address to copy
     *  @param  dest the output destination of copied bytes
     *  @param  size the size of the element in the bytes
     *  @param  count the number of elements to copy
     */
    static void reverse_copy( const sio::byte *const from, sio::byte *dest, std::size_t size, std::size_t count ) ;

    /**
     *  @brief  Perform a byte array copy
     *
     *  @param  from the input bytes address to copy
     *  @param  dest the output destination of copied bytes
     *  @param  size the size of the element in the bytes
     *  @param  count the number of elements to copy
     */
    static void copy( const sio::byte *const from, sio::byte *dest, std::size_t size, std::size_t count ) ;

    /**
     *  @brief  Template overload of raw copy (see above) for writing.
     *          The size of the template parameter is evaluated using the
     *          helper structure sizeof_helper.
     *
     *  @param  from the array to copy
     *  @param  dest the destination byte pointer
     *  @param  count the number of elements to copy
     */
    template <typename T>
    static void write( const T *const from, sio::byte *dest, std::size_t count ) ;

    /**
     *  @brief  Template overload of raw copy (see above) for reading.
     *          The size of the template parameter is evaluated using the
     *          helper structure sizeof_helper.
     *
     *  @param  from the bytes to copy
     *  @param  dest the destination array
     *  @param  count the number of elements to copy
     */
    template <typename T>
    static void read( const sio::byte *const from, T *dest, std::size_t count ) ;
  };

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  template <typename T>
  inline void memcpy::write( const T *const from, sio::byte *dest, std::size_t count ) {
    sio::memcpy::copy( reinterpret_cast<const sio::byte*>(from), dest, sizeof_helper<T>::size, count ) ;
  }

  //--------------------------------------------------------------------------

  template <typename T>
  inline void memcpy::read( const sio::byte *const from, T *dest, std::size_t count ) {
    sio::memcpy::copy( from, reinterpret_cast<sio::byte*>(dest), sizeof_helper<T>::size, count ) ;
  }

}
