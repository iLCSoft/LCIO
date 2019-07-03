#ifndef SIO_SIORUNHEADERHANDLER_H
#define SIO_SIORUNHEADERHANDLER_H 1

// -- lcio headers
#include "EVENT/LCRunHeader.h"
#include "IOIMPL/LCRunHeaderIOImpl.h"

// -- sio headers
#include <sio/block.h>

namespace SIO {


/** Handler for LCRunHeader/LCRunHeaderImpl objects.
 *
 * @author gaede
 * @version $Id: SIORunHeaderHandler.h,v 1.8 2005-04-15 08:37:43 gaede Exp $
 * fg 20030609 using data interface for writing
 */
  class SIORunHeaderHandler : public sio::block {
  public:
    SIORunHeaderHandler(const SIORunHeaderHandler&) = delete ;
    SIORunHeaderHandler& operator=(const SIORunHeaderHandler&) = delete ;
    ~SIORunHeaderHandler() = default ;

    /// Constructor
    SIORunHeaderHandler() ;

    /// Set the run header to read/write
    void setRunHeader( EVENT::LCRunHeader* hdr ) ;

    // from sio::block
    void read( sio::read_device &device, sio::version_type vers ) override ;
    void write( sio::write_device &device ) override ;

  private:
    /// Run header for reading/writing
    EVENT::LCRunHeader        *_runHeader {nullptr} ;
  }; // class

} // namespace

#endif /* ifndef SIO_SIORUNHEADERHANDLER_H */
