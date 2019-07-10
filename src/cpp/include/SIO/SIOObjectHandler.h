#ifndef SIO_SIOOBJECTHANDLER_H
#define SIO_SIOOBJECTHANDLER_H 1

#include "EVENT/LCObject.h"
#include "EVENT/LCCollection.h"

#include <sio/definitions.h>

namespace SIO {

/**  Interface for all lcio object SIO-handlers, has to be implemented
 * for all event entities (hits, tracks, clusters,...).
 *
 * @author gaede
 * @version $Id: SIOObjectHandler.h,v 1.10 2008-11-27 16:53:35 engels Exp $
 */
class SIOObjectHandler {
public:
  /// Constructor with collection type
  SIOObjectHandler( const std::string &colType ) ;

  /// Default destructor
  virtual ~SIOObjectHandler() = default ;

  /// Get the collection flag
  unsigned int flag() const ;

  /// Get the collection type associated to the object handler (see EVENT/LCIO.h)
  const std::string &collectionType() const ;

  /// Init collection reading
  virtual void initReading( sio::read_device &device, EVENT::LCCollection *collection, sio::version_type vers ) ;

  /// Init collection writing
  virtual void initWriting( sio::write_device &device, EVENT::LCCollection *collection ) ;

  /// Reads lcio objects from an SIO stream.
  virtual void read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) = 0 ;

  /// Writes lcio objects to an SIO stream.
  virtual void write( sio::write_device& device, const EVENT::LCObject* obj ) = 0 ;

  /// Factory method to create an object of the type of the collection
  virtual EVENT::LCObject *create() const = 0 ;

 protected:
   /// The collection type associated to LCIO object
   const std::string     _collectionType ;
   /// The collection flag
   unsigned int          _flag {0} ;
}; // class

} // namespace
#endif /* ifndef hep_lcio_event_SIOOBJECTHANDLER_H */
