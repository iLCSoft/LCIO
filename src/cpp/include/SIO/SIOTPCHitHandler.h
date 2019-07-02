#ifndef SIO_SIOTPCHITHANDLER_H
#define SIO_SIOTPCHITHANDLER_H 1

#include "SIO/SIOObjectHandler.h"

namespace SIO {

/** Implementation of SIOObjectHandler to handle IO of TPCHits.
 *
 * @author gaede
 * @version $Id: SIOTPCHitHandler.h,v 1.4 2005-04-15 08:37:43 gaede Exp $
 */
  class SIOTPCHitHandler : public SIOObjectHandler {
  public:
    /// Constructor
    SIOTPCHitHandler() ;

    /// Reads lcio objects from an SIO stream.
    void read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) override ;

    /// Writes lcio objects to an SIO stream.
    void write( sio::write_device& device, const EVENT::LCObject* obj ) override ;

    /// Factory method to create an object of the type of the collection
    EVENT::LCObject *create() const override ;
  }; // class
} // namespace

#endif /* ifndef SIO_SIOTPCHITHANDLER_H */
