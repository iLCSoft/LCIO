#ifndef SIO_SIOTPCPulseHandler_H
#define SIO_SIOTPCPulseHandler_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of TPCPulses.
 *
 * @author gaede
 * @version $Id: SIOTPCPulseHandler.h,v 1.1 2005-04-26 09:49:47 gaede Exp $
 */

  class SIOTPCPulseHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads lcio TPCPulse objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP)  ;
	
    /** Writes TPCPulse hit objects to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj) ;
	
  }; // class
} // namespace

#endif /* ifndef SIO_SIOTPCHITHANDLER_H */
