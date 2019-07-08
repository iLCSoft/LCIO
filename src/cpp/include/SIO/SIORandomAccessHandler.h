#ifndef SIO_SIORandomAccessHandler_H
#define SIO_SIORandomAccessHandler_H 1

// -- sio headers
#include "sio/block.h"

// -- std headers
#include <memory>

namespace SIO {

  class LCIORandomAccess ;

/** Handler for LCIOrandomAccess and LCIOIndex objects/blocks.
 *
 * @author gaede
 * @version $Id: SIORandomAccessHandler.h,v 1.2 2010-06-22 13:49:54 gaede Exp $
 */
  class SIORandomAccessHandler : public sio::block {
  public:
    ~SIORandomAccessHandler() = default ;
    SIORandomAccessHandler(const SIORandomAccessHandler&) = delete;
    SIORandomAccessHandler& operator=(const SIORandomAccessHandler&) = delete ;

    /// Constructor
    SIORandomAccessHandler() ;

    /// Set the random access object to read or write
    void setRandomAccess( std::shared_ptr<LCIORandomAccess> ra ) ;

    /// Get the random access object to read or write
    std::shared_ptr<LCIORandomAccess> randomAccess() const ;

    // from sio::block
    void read( sio::read_device &device, sio::version_type vers ) override ;
    void write( sio::write_device &device ) override ;

  private:
    /// The random access object to read / write
    std::shared_ptr<LCIORandomAccess> _randomAccess {nullptr} ;
  }; // class

} // namespace

#endif /* ifndef SIO_SIORUNHEADERHANDLER_H */
