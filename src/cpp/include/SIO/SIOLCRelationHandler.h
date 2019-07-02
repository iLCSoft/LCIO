#ifndef SIO_SIOLCWGTRELATIONHANDLER_H
#define SIO_SIOLCWGTRELATIONHANDLER_H 1

#include "SIO/SIOObjectHandler.h"

namespace SIO {

  /** Implementation of SIOObjectHandler to handle IO of CalorimeterHits.
   *
   * @author gaede
   * @version $Id: SIOLCRelationHandler.h,v 1.2 2005-04-15 08:37:42 gaede Exp $
   */
  class SIOLCRelationHandler : public SIOObjectHandler {
  public:
    /// Constructor
    SIOLCRelationHandler() ;

    /// Reads lcio objects from an SIO stream.
    void read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) ;

    /// Writes lcio objects to an SIO stream.
    void write( sio::write_device& device, const EVENT::LCObject* obj ) ;

    /// Factory method to create an object of the type of the collection
    EVENT::LCObject *create() const ;
  }; // class
} // namespace

#endif /* ifndef hep_lcio_event_SIOLCWGTRELATIONHANDLER_H */
