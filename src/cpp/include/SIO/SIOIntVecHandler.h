#ifndef SIO_SIOINTVECHANDLER_H
#define SIO_SIOINTVECHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of IntVecs.
 *
 * @author gaede
 * @version Aug 8, 2003
 */
  class SIOIntVecHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      DATA::LCObject** objP,  
			      unsigned int flag,
			      unsigned int vers)  ;
	
    /** Writes lcio objects to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, 
			       const DATA::LCObject* obj,
			       unsigned int flag) ;
	
  }; // class
}; // namespace

#endif /* ifndef SIO_SIOINTVECHANDLER_H */
