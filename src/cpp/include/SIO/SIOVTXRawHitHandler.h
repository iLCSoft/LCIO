#ifndef SIO_SIOVTXRawHitHandler_H
#define SIO_SIOVTXRawHitHandler_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of VTXRawHits.
 *
 * @author gaede
 * @version $Id: SIOVTXRawHitHandler.h,v 1.1 2005-04-26 10:50:16 gaede Exp $
 */

  class SIOVTXRawHitHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads lcio VTXRawHit objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP)  ;
	
    /** Writes VTXRawHit hit objects to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj) ;
	
  }; // class
} // namespace

#endif /* ifndef SIO_SIOTPCHITHANDLER_H */
