#ifndef SIO_SIOSIMTRACKHITHANDLER_H
#define SIO_SIOSIMTRACKHITHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of SimTrackerHits.
 *
 * @author gaede
 * @version $Id: SIOSimTrackHitHandler.h,v 1.5 2005-04-15 08:37:43 gaede Exp $
 */

  class SIOSimTrackHitHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads lcio tracker hit objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP)  ;
	
    /** Writes lcio objects to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj) ;
	
  }; // class
} // namespace

#endif /* ifndef SIO_SIOSIMTRACKHITHANDLER_H */
