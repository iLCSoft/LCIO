#ifndef hep_lcio_event_SIOPARTICLEHANDLER_H
#define hep_lcio_event_SIOPARTICLEHANDLER_H 1

#include "SIOObjectHandler.h"

/** Interface for IO of CalorimeterHit objects.
 */
class SIOParticleHandler : public SIOObjectHandler {

public:

  /** Reads lcio calorimeter hit objects from an SIO stream.
   */
  virtual unsigned int read(SIO_stream* stream, 
			    LCIOObject** objP,
			    unsigned int flag, 
			    unsigned int vers)  ;

  /** Writes lcio calorimeter hit objects to an SIO stream.
   */

  virtual unsigned int write(SIO_stream* stream, 
			     const LCIOObject* obj,
			     unsigned int flag) ;

}; // class

#endif /* ifndef hep_lcio_event_SIOPARTICLEHANDLER_H */
