#ifndef SIO_SIOSTRVECHANDLER_H
#define SIO_SIOSTRVECHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of StrVecs.
 *
 * @author vogt
 * @version jun 18, 2004
 */
  class SIOStrVecHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP,  
			      unsigned int flag,
			      unsigned int vers)  ;
	
    /** Writes lcio objects to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj,
			       unsigned int flag) ;
	
  }; // class
}; // namespace

#endif /* ifndef SIO_SIOSTRVECHANDLER_H */
