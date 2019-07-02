#ifndef SIO_SIOCALHITHANDLER_H
#define SIO_SIOCALHITHANDLER_H 1

#include "SIO/SIOObjectHandler.h"

namespace SIO {

  /** Implementation of SIOObjectHandler to handle IO of CalorimeterHits.
   *
   * @author gaede
   * @version $Id: SIOCalHitHandler.h,v 1.7 2005-04-15 08:37:42 gaede Exp $
   */
  class SIOCalHitHandler : public SIOObjectHandler {
  public:
    /// Constructor
    SIOCalHitHandler() ;

    /// Reads lcio objects from an SIO stream.
    void read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) override ;

    /// Writes lcio objects to an SIO stream.
    void write( sio::write_device& device, const EVENT::LCObject* obj ) override ;

    /// Factory method to create an object of the type of the collection
    EVENT::LCObject *create() const override ;
  }; // class
} // namespace

#endif /* ifndef hep_lcio_event_SIOCALHITHANDLER_H */
