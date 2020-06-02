#pragma once

// -- sio headers
#include <sio/definitions.h>

// -- std headers
#include <string>

namespace sio {
  
  class read_device ;
  class write_device ;
  
  /**
   *  @brief  block class.
   *
   *  Implements to reading (writing) of record blocks from (to) a buffer.
   *  Blocks are versioned and the version is written out in the block header,
   *  so that on read operation, the version can be use to perform a proper
   *  decoding of data.
   */
  class block {
  public:
    /// No default constructor
    block() = delete ;
    /// No copy constructor
    block( const block& ) = delete ;
    /// No move constructor
    block( block&& ) = delete ;
    /// No copy assignement
    block& operator=( const block& ) = delete ;
    /// No move assignement
    block& operator=( block&& ) = delete ;
    /// Default destructor
    virtual ~block() = default ;

    /**
     *  @brief  Constructor
     *  
     *  @param  nam the block name
     *  @param  vers the block version
     */
    block( const std::string &nam, sio::version_type vers ) ;
    
    /**
     *  @brief  Get the block name
     */
    const std::string &name() const noexcept ;

    /**
     *  @brief  Get the block version
     */
    sio::version_type version() const noexcept ;
    
    /**
     *  @brief  Perform decoding of block buffer using the device
     *  
     *  @param  device the read device containing the block buffer
     *  @param  vers the block version read out from the stream
     */
    virtual void read( sio::read_device &device, sio::version_type vers ) = 0 ;
    
    /**
     *  @brief  Perform encoding of block data in a buffer using the device
     *  
     *  @param  device the write device containing the block buffer
     */
    virtual void write( sio::write_device &device ) = 0 ;
        
  private:
    ///< The block version
    const sio::version_type            _version ;
    ///< The block name
    const std::string                  _name ;
  };
  
}