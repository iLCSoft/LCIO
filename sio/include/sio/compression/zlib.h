#pragma once

// -- sio headers
#include <sio/definitions.h>

namespace sio {
  
  class buffer ;
  class buffer_span ;
  
  class zlib_compression {
  public:
    /// Default constructor
    zlib_compression() = default ;
    /// Default destructor
    ~zlib_compression() = default ;
    
    /**
     *  @brief  Set the compression level.
     *          - Z_DEFAULT_COMPRESSION: default zlib compression level
     *          - -1: no commpression
     *          - [1-9] various levels
     *          Note that above 9, the level is to 9
     *          
     *  @param  level the compression level to use
     */
    void set_level( int level ) ;
    
    /**
     *  @brief  Get the compression level
     */
    int level() const ;

    /**
     *  @brief  Uncompress the buffer and return a new buffer (reference).
     *          The uncpmpressed buffer must have been resized correctly 
     *          before calling this function.
     * 
     *  @param  inbuf the input buffer to uncompress
     *  @param  outbuf the uncompressed buffer to receive
     */
    void uncompress( const buffer_span &inbuf, buffer &outbuf ) ;
    
    /**
     *  @brief  Compress the buffer and return a new buffer
     * 
     *  @param  inbuf the input buffer to compress
     *  @param  outbuf the output buffer to receive
     */
    void compress( const buffer_span &inbuf, buffer &outbuf ) ;
    
  private:
    ///< The compression level (on compress)
    int               _level {0} ;
  };

}

