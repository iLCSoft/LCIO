#ifndef SIO_SIOTRACKERHITZCYLINDERHANDLER_H
#define SIO_SIOTRACKERHITZCYLINDERHANDLER_H 1

#include "SIO/SIOObjectHandler.h"

namespace SIO {

/** Implementation of SIOObjectHandler to handle IO of TrackerHitZCylinder.
 *
 * @author engels
 * @version $Id:$
 */
  class SIOTrackerHitZCylinderHandler : public SIOObjectHandler {
  public:
    /// Constructor
    SIOTrackerHitZCylinderHandler() ;

    /// Reads lcio objects from an SIO stream.
    void read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) override ;

    /// Writes lcio objects to an SIO stream.
    void write( sio::write_device& device, const EVENT::LCObject* obj ) override ;

    /// Factory method to create an object of the type of the collection
    EVENT::LCObject *create() const override ;
  }; // class
} // namespace

#endif /* ifndef SIO_SIOTRACKERHITZCYLINDERHANDLER_H */
