#ifndef SIO_SIOIndexHandler_H
#define SIO_SIOIndexHandler_H 1

// -- lcio headers
#include "SIO/RunEventMap.h"

// -- sio headers
#include "sio/block.h"

// -- std headers
#include <memory>

namespace SIO {

/** Handler for LCIOrandomAccess and LCIOIndex objects/blocks.
 *
 * @author gaede
 * @version $Id: SIOIndexHandler.h,v 1.2 2010-06-22 13:49:54 gaede Exp $
 */
  class SIOIndexHandler : public sio::block {
    typedef RunEvent::long64 long64 ;
  public:
    SIOIndexHandler(const SIOIndexHandler&) = delete;
    SIOIndexHandler& operator=(const SIOIndexHandler&) = delete ;
    ~SIOIndexHandler() = default ;

    /// Constructor
    SIOIndexHandler() ;

    /// Set the run / event map to read or write
    void setRunEventMap( std::shared_ptr<RunEventMap> remap ) ;

    /// Get the run / event map
    std::shared_ptr<RunEventMap> runEventMap() const ;

    // from sio::block
    void read( sio::read_device &device, sio::version_type vers ) override ;
    void write( sio::write_device &device ) override ;

  private:
    /// The run / event map to read/write
    std::shared_ptr<RunEventMap>  _runEventMap {nullptr} ;
  }; // class

} // namespace

#endif /* ifndef SIO_SIORUNHEADERHANDLER_H */
