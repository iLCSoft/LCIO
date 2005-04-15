#ifndef SIO_SIOFLOATVECHANDLER_H
#define SIO_SIOFLOATVECHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of FloatVecs.
 *
 * @author gaede
 * @version $Id: SIOFloatVecHandler.h,v 1.6 2005-04-15 08:37:42 gaede Exp $
 */
  class SIOFloatVecHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads lcio FloatVecs objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP)  ;
	
    /** Writes lcio FloatVecs to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj) ;
	
  }; // class
} // namespace

#endif /* ifndef SIO_SIOFLOATVECHANDLER_H */
