#ifndef SIO_SIOINTVECHANDLER_H
#define SIO_SIOINTVECHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of IntVecs.
 *
 * @author gaede
 * @version $Id: SIOIntVecHandler.h,v 1.3 2004-08-20 16:45:24 gaede Exp $
 */
  class SIOIntVecHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads IntVec objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP)  ;
	
    /** Writes lcio IntVec objects to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj) ;
	
  }; // class
}; // namespace

#endif /* ifndef SIO_SIOINTVECHANDLER_H */
