#ifndef SIO_SIOCOLLECTIONHANDLER_H
#define SIO_SIOCOLLECTIONHANDLER_H 1

// -- std headers
#include <string>
#include <memory>

// -- lcio headers
#include "EVENT/LCCollection.h"

// -- sio headers
#include <sio/block.h>

namespace SIO {

  class SIOObjectHandler ;

  /** Handler for LCCollection/LCCollectionIOVec objects for SIO.
   *
   * @author gaede
   * @version $Id: SIOCollectionHandler.h,v 1.8 2005-04-15 08:37:42 gaede Exp $
   */
  class SIOCollectionHandler : public sio::block {
  public:
    SIOCollectionHandler() = delete ;
    SIOCollectionHandler(const SIOCollectionHandler&) = delete;
    SIOCollectionHandler& operator=(const SIOCollectionHandler&) = delete ;
    ~SIOCollectionHandler() = default ;

  public:
    /// Constructor with collection (block) name and object handler
    SIOCollectionHandler( const std::string& name, std::shared_ptr<SIOObjectHandler> handler ) ;

    /// Get the collection type
    const std::string &type() const ;

    /// Set the collection to read/write
    void setCollection( EVENT::LCCollection *col ) ;

    // from sio::block
    void read( sio::read_device &device, sio::version_type vers ) override ;
    void write( sio::write_device &device ) override ;

  private:
    /// The collection to read/write
    EVENT::LCCollection                *_collection {nullptr} ;
    /// The object handler for reading/writing
    std::shared_ptr<SIOObjectHandler>   _handler {nullptr} ;
  };

} // namespace
#endif /* ifndef SIO_SIOCOLLECTIONHANDLER_H */
