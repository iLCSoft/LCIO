#ifndef SIO_SIOCLUSTERHANDLER_H
#define SIO_SIOCLUSTERHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of Clusters.
 *
 * @author gaede
 * @version $Id: SIOClusterHandler.h,v 1.4 2005-04-15 08:37:42 gaede Exp $
 */

  class SIOClusterHandler : public SIOObjectHandler {
  public:
    /// Constructor
    SIOClusterHandler() ;
    
    /// Reads lcio objects from an SIO stream.
    void read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) ;
    
    /// Writes lcio objects to an SIO stream.
    void write( sio::write_device& device, const EVENT::LCObject* obj ) ;
    
    /// Factory method to create an object of the type of the collection
    EVENT::LCObject *create() const ;
  }; // class
} // namespace

#endif /* ifndef SIO_SIOCLUSTERHANDLER_H */
