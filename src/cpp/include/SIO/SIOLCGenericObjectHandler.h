#ifndef SIO_SIOLCGENERICOBJECTHANDLER_H
#define SIO_SIOLCGENERICOBJECTHANDLER_H 1

#include "SIO/SIOObjectHandler.h"

#include "EVENT/LCEvent.h"

namespace SIO {
    
    

  /** Implementation of SIOObjectHandler to handle IO of LCGenericObjects.
   *
   * @author gaede
   * @version $Id: SIOLCGenericObjectHandler.h,v 1.1 2004-08-16 09:42:07 gaede Exp $
   */
  class SIOLCGenericObjectHandler : public SIOObjectHandler {

  public:
	
    /** Reads lcio LCGenericObject objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP,
			      unsigned int flag, 
			      unsigned int vers)  ;
	
    /** Writes lcio LCGenericObject objects to an SIO stream.
     */
	
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj,
			       unsigned int flag) ;


  }; // class
    
}; //namespace
#endif /* ifndef SIO_SIOLCGENERICOBJECTHANDLER_H */
