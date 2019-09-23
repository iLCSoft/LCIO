#ifndef SIO_SIOEVENTHANDLER_H
#define SIO_SIOEVENTHANDLER_H 1

// -- std headers
#include <string>
#include <memory>

// -- lcio headers
#include "EVENT/LCEvent.h"
#include "IOIMPL/LCEventIOImpl.h"

// -- sio headers
#include <sio/block.h>

namespace SIO {


/** Handler for LCEvent/LCEventIOImpl objects.
 *
 * @author gaede
 * @version $Id: SIOEventHandler.h,v 1.8 2005-04-15 08:37:42 gaede Exp $
 */
  class SIOEventHandler : public sio::block {
  public:
    SIOEventHandler(const SIOEventHandler&) = delete;
    SIOEventHandler& operator=(const SIOEventHandler&) = delete ;
    ~SIOEventHandler() = default ;

    /// Constructor
    SIOEventHandler() ;

    // from sio::block
    void read( sio::read_device &device, sio::version_type vers ) override ;
    void write( sio::write_device &device ) override ;

    /// Set the event to read/write
    void setEvent( EVENT::LCEvent* evt ) ;
    
    /// Set the collection to read or write only
    void setCollectionNames( const std::set<std::string>& colnames ) ;

  private:
    static constexpr const char *SubsetPostfix = "_References" ;

  private:
    // Event pointer for reading/writing
    EVENT::LCEvent             *_event {nullptr} ;
    /// List of collection to read only
    std::set<std::string>       _colSubSet {} ;
  }; // class

} // namespace

#endif /* ifndef SIO_SIOEVENTHANDLER_H */
