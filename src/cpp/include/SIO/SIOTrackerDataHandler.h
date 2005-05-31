#ifndef SIO_SIOTrackerDataHandler_H
#define SIO_SIOTrackerDataHandler_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of TrackerDatas.
 *
 * @author gaede
 * @version $Id: SIOTrackerDataHandler.h,v 1.1 2005-05-31 07:43:25 gaede Exp $
 */

  class SIOTrackerDataHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads lcio TrackerData objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP)  ;
	
    /** Writes TrackerData hit objects to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj) ;
	
  }; // class
} // namespace

#endif /* ifndef SIO_SIOTPCHITHANDLER_H */
