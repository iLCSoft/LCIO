#ifndef SIO_SIOPARTICLEHANDLER_H
#define SIO_SIOPARTICLEHANDLER_H 1

#include "SIO/SIOObjectHandler.h"

#include "EVENT/LCEvent.h"

namespace SIO {



  /** Implementation of SIOObjectHandler to handle IO of MCParticles.
   *
   * @author gaede
   * @version $Id: SIOParticleHandler.h,v 1.8 2005-04-15 08:37:42 gaede Exp $
   */
  class SIOParticleHandler : public SIOObjectHandler {
  public:
    /// Constructor
    SIOParticleHandler() ;

    /// Reads lcio objects from an SIO stream.
    void read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) override ;

    /// Writes lcio objects to an SIO stream.
    void write( sio::write_device& device, const EVENT::LCObject* obj ) override ;

    /// Factory method to create an object of the type of the collection
    EVENT::LCObject *create() const override ;

    /// Restore the MCParticle parent <-> daughter relations
    static void restoreParentDaughterRelations( EVENT::LCEvent* evt ) ;
  }; // class

} // namespace
#endif /* ifndef SIO_SIOPARTICLEHANDLER_H */
