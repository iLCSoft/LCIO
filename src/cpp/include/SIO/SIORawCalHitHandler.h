#ifndef SIO_SIORAWCALHITHANDLER_H
#define SIO_SIORAWCALHITHANDLER_H 1

#include "SIO/SIOObjectHandler.h"

namespace SIO {

  /** Implementation of SIOObjectHandler to handle IO of RawCalorimeterHits.
   *
   * @author gaede
   * @version $Id: SIORawCalHitHandler.h,v 1.2 2005-04-15 08:37:42 gaede Exp $
   */
  class SIORawCalHitHandler : public SIOObjectHandler {
  public:
    /// Constructor
    SIORawCalHitHandler() ;

    /// Reads lcio objects from an SIO stream.
    void read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) override ;

    /// Writes lcio objects to an SIO stream.
    void write( sio::write_device& device, const EVENT::LCObject* obj ) override ;

    /// Factory method to create an object of the type of the collection
    EVENT::LCObject *create() const override ;
  }; // class
} // namespace

#endif /* ifndef hep_lcio_event_SIORAWCALHITHANDLER_H */
