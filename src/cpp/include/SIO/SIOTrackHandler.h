#ifndef SIO_SIOTRACKHANDLER_H
#define SIO_SIOTRACKHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of Tracks.
 *
 * @author gaede
 * @version $Id: SIOTrackHandler.h,v 1.3 2004-08-20 16:45:25 gaede Exp $
 */

  class SIOTrackHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads lcio track objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP)  ;
	
    /** Writes lcio track objects to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj) ;
	
  }; // class
}; // namespace

#endif /* ifndef SIO_SIOTRACKHANDLER_H */
