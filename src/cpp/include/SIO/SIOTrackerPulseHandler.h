#ifndef SIO_SIOTrackerPulseHandler_H
#define SIO_SIOTrackerPulseHandler_H 1

#include "SIO/SIOObjectHandler.h"

namespace SIO {

/** Implementation of SIOObjectHandler to handle IO of TrackerPulses.
 *
 * @author gaede
 * @version $Id: SIOTrackerPulseHandler.h,v 1.1 2005-05-31 07:43:25 gaede Exp $
 */
  class SIOTrackerPulseHandler : public SIOObjectHandler {
  public:
    /// Constructor
    SIOTrackerPulseHandler() ;

    /// Reads lcio objects from an SIO stream.
    void read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) override ;

    /// Writes lcio objects to an SIO stream.
    void write( sio::write_device& device, const EVENT::LCObject* obj ) override ;

    /// Factory method to create an object of the type of the collection
    EVENT::LCObject *create() const override ;
  }; // class
} // namespace

#endif /* ifndef SIO_SIOTPCHITHANDLER_H */
