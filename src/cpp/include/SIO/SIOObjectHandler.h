#ifndef SIO_SIOOBJECTHANDLER_H
#define SIO_SIOOBJECTHANDLER_H 1

#include "EVENT/LCObject.h"
#include "EVENT/LCCollection.h"
//#include "SIO_functions.h"
#include "SIO_block.h"

class SIO_stream ;


namespace SIO{


/**  Interface for all lcio object SIO-handlers, has to be implemented  
 * for all event entities (hits, tracks, clusters,...).
 * 
 * @author gaede
 * @version $Id: SIOObjectHandler.h,v 1.10 2008-11-27 16:53:35 engels Exp $
 */
class SIOObjectHandler {
  
public:

  virtual ~SIOObjectHandler(){ /* nop */; }
  
  /** Reads lcio objects from an SIO stream.
   */
  virtual unsigned int read(SIO_stream* stream, 
			    EVENT::LCObject** objP) =0 ;
  
  /** Writes lcio objects to an SIO stream.
   */
  virtual unsigned int write(SIO_stream* stream, 
			     const EVENT::LCObject* obj ) =0 ;
  

  /** Initialize the handler and/or the collection.
   * Read/write the flag and the parameters.
   * Overwrite for classes that need specific processing.
   */
  virtual unsigned int init( SIO_stream* stream,
			     SIO_operation op,
			     EVENT::LCCollection* col,
			     unsigned int vers ) ; 
  

  /** Calls read() if the collection is not a subset otherwise only reads the pointers.*/
  virtual unsigned int readBase(SIO_stream* stream, 
			    EVENT::LCObject** objP)  ;
  

  /** Calls write() if the collection is not a subset otherwise only writes the pointers.*/
  virtual unsigned int writeBase(SIO_stream* stream, 
			     const EVENT::LCObject* obj ) ;

 protected:


  unsigned int _flag{0} ; 
  unsigned int _vers{0} ;
  
}; // class

} // namespace 
#endif /* ifndef hep_lcio_event_SIOOBJECTHANDLER_H */
