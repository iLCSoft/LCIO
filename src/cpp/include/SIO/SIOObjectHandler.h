#ifndef SIO_SIOOBJECTHANDLER_H
#define SIO_SIOOBJECTHANDLER_H 1

#include "DATA/LCObject.h"

class SIO_stream ;


namespace SIO{


/**  Interface for all lcio object SIO-handlers, has to be implemented  
 * for all event entities (hits, tracks, clusters,...).
 * 
 * @author gaede
 * @version Mar 6, 2003
 */
class SIOObjectHandler {
  
public:
  
  /** Reads lcio objects from an SIO stream.
   */
  virtual unsigned int read(SIO_stream* stream, 
			    DATA::LCObject** objP,  
			    unsigned int flag,
			    unsigned int vers) =0 ;
  
  /** Writes lcio objects to an SIO stream.
   */
  virtual unsigned int write(SIO_stream* stream, 
			     const DATA::LCObject* obj,
			     unsigned int flag) =0 ;
  
}; // class

}; // namespace 
#endif /* ifndef hep_lcio_event_SIOOBJECTHANDLER_H */
