// -*- C++ -*-
/** Implementation of SIOObjectHandler to handle IO of FloatVecs.
 *
 * @author gaede
 * @version Mar 12, 2003
 */
#ifndef SIO_SIOFLOATVECHANDLER_H
#define SIO_SIOFLOATVECHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
  class SIOFloatVecHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads lcio tracker hit objects from an SIO stream.
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

#endif /* ifndef SIO_SIOFLOATVECHANDLER_H */
