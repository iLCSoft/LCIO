#ifndef SIO_SIOTRACKHANDLER_H
#define SIO_SIOTRACKHANDLER_H 1

#include "SIO/SIOObjectHandler.h"

namespace SIO {

/** Implementation of SIOObjectHandler to handle IO of Tracks.
 *
 * @author gaede
 * @version $Id: SIOTrackHandler.h,v 1.4 2005-04-15 08:37:43 gaede Exp $
 */
  class SIOTrackHandler : public SIOObjectHandler {
  public:
    /// Constructor
    SIOTrackHandler() ;

    /// Reads lcio objects from an SIO stream.
    void read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) override ;

    /// Writes lcio objects to an SIO stream.
    void write( sio::write_device& device, const EVENT::LCObject* obj ) override ;

    /// Factory method to create an object of the type of the collection
    EVENT::LCObject *create() const override ;
  }; // class
} // namespace

#endif /* ifndef SIO_SIOTRACKHANDLER_H */
