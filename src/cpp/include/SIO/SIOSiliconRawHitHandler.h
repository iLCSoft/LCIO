#ifndef SIO_SIOSiliconRawHitHandler_H
#define SIO_SIOSiliconRawHitHandler_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of SiliconRawHits.
 *
 * @author gaede
 * @version $Id: SIOSiliconRawHitHandler.h,v 1.1 2005-05-04 10:25:36 gaede Exp $
 */

  class SIOSiliconRawHitHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads lcio SiliconRawHit objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP)  ;
	
    /** Writes SiliconRawHit hit objects to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj) ;
	
  }; // class
} // namespace

#endif /* ifndef SIO_SIOTPCHITHANDLER_H */
