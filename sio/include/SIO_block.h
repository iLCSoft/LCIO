// ----------------------------------------------------------------------------
// CVS $Id: SIO_block.h,v 1.2 2003-03-17 12:54:57 gaede Exp $
// ----------------------------------------------------------------------------
// => Controller for an SIO block.                                
// ----------------------------------------------------------------------------
//
// General Description:
//
// Each SIO_block controls one block type.                
//
// ----------------------------------------------------------------------------

#ifndef SIO_BLOCK_H
#define SIO_BLOCK_H 1

// -- std headers
#include <string>
#include <memory>

// -- sio headers
#include <SIO_definitions.h>


/* START DEPRECATED */
class SIO_stream;

class SIO_block
{
public:
    SIO_block( const char* );
    virtual ~SIO_block();

    std::string*           getName();
    virtual unsigned int   xfer( SIO_stream*, SIO_operation, unsigned int ) = 0;
    virtual unsigned int   version() = 0;

private:
    std::string            name;

};

/* END DEPRECATED */

namespace sio {
  
  // forward declarations
  class stream;
  
  /**
   *  @brief  block class
   *
   *  Base class for SIO block serialization
   */
  class block {
  public:
    /**
     *  @brief  Constructor
     * 
     *  @param  name the block name 
     */
    block(const std::string &name);
    
    /**
     *  @brief  Destructor
     */
    virtual ~block();

    /**
     *  @brief  Get the block name
     */
    const std::string &name() const;

    /**
     *  @brief  Read/write the block to the target stream 
     *  
     *  @param  s the input/output stream to read/write in/out
     *  @param  op specify the read or write operation
     *  @param  v the block version
     */
    virtual unsigned int xfer(stream *s, SIO_operation op, unsigned int v) = 0;
    
    /**
     *  @brief  Get the block version
     */
    virtual unsigned int version() const = 0;
  
  private:
    /// The block name
    std::string _name;
  };

  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  
  inline block::block(const std::string &name) :
    _name(name) {
    /* nop */
  }
  
  //----------------------------------------------------------------------------
  
  inline block::~block() {
    /* nop */
  }
  
  //----------------------------------------------------------------------------
  
  inline const std::string &block::name() const {
    return _name;
  }
  
}

#endif
