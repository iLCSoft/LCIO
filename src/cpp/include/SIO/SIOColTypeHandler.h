#ifndef hep_lcio_event_SIOCALHITHANDLER_H
#define hep_lcio_event_SIOCALHITHANDLER_H 1

#include "SIOObjectHandler.h"

/** Interface for IO of CalorimeterHit objects.
 */
class SIOColTypeHandler : public SIOObjectHandler {

public:

  /** Reads lcio collection types from an SIO stream.
   */
  virtual unsigned int read(SIO_stream* stream, 
			    LCIOObject* obj,  
			    unsigned int vers)  ;

  /** Writes lcio collection types to an SIO stream.
   */

  virtual unsigned int write(SIO_stream* stream, 
			    const LCIOObject* obj) ;

}; // class

#endif /* ifndef hep_lcio_event_SIOCALHITHANDLER_H */
