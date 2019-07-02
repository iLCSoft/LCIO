#ifndef SIO_SIOLCGENERICOBJECTHANDLER_H
#define SIO_SIOLCGENERICOBJECTHANDLER_H 1

#include "SIO/SIOObjectHandler.h"

namespace SIO {

  /** Implementation of SIOObjectHandler to handle IO of LCGenericObjects.
   *
   * @author gaede
   * @version $Id: SIOLCGenericObjectHandler.h,v 1.3 2005-04-15 08:37:42 gaede Exp $
   */
  class SIOLCGenericObjectHandler : public SIOObjectHandler {
  public:
    /// Constructor
    SIOLCGenericObjectHandler() ;

    /// Init collection reading
    void initReading( sio::read_device &device, EVENT::LCCollection *collection, sio::version_type vers ) override ;

    /// Init collection reading
    void initWriting( sio::write_device &device, EVENT::LCCollection *collection ) override ;

    /// Reads lcio objects from an SIO stream.
    void read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) override ;

    /// Writes lcio objects to an SIO stream.
    void write( sio::write_device& device, const EVENT::LCObject* obj ) override ;

    /// Factory method to create an object of the type of the collection
    EVENT::LCObject *create() const override ;

  private:
    int         _nInt {0} ;
    int         _nFloat {0} ;
    int         _nDouble {0} ;
    bool        _isFixedSize {false} ;

  }; // class

} // namespace
#endif /* ifndef SIO_SIOLCGENERICOBJECTHANDLER_H */
