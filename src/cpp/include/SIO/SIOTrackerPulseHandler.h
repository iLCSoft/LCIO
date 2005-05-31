#ifndef SIO_SIOTrackerPulseHandler_H
#define SIO_SIOTrackerPulseHandler_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of TrackerPulses.
 *
 * @author gaede
 * @version $Id: SIOTrackerPulseHandler.h,v 1.1 2005-05-31 07:43:25 gaede Exp $
 */

  class SIOTrackerPulseHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads lcio TrackerPulse objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP)  ;
	
    /** Writes TrackerPulse hit objects to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj) ;
	
  }; // class
} // namespace

#endif /* ifndef SIO_SIOTPCHITHANDLER_H */
