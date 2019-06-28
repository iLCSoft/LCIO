#ifndef SIO_SIOLCPARAMETERS_H
#define SIO_SIOLCPARAMETERS_H 1

#include "IMPL/LCParametersImpl.h"

#include <sio/definitions.h>

namespace SIO {
    
    
/** IO of LCParameters.
 *
 * @author gaede
 * @version $Id: SIOLCParameters.h,v 1.4 2005-04-15 08:37:42 gaede Exp $
 */
  class SIOLCParameters {
  public:
    /** Reads objects from an SIO stream.
     */
    static void read( sio::read_device &device, EVENT::LCParameters& params, sio::version_type vers ) ;
    
    /** Writes lcio objects to an SIO stream.
     */
    static void write( sio::write_device &device, const EVENT::LCParameters& params ) ;
  }; // class
} // namespace

#endif /* ifndef SIO_SIOLCPARAMETERS_H */
