// -*- C++ -*-
/** Implementation of SIOObjectHandler to handle IO of TrackerHits.
 *
 * @author gaede
 * @version Mar 12, 2003
 */
#ifndef SIO_SIOTRACKHITHANDLER_H
#define SIO_SIOTRACKHITHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
  class SIOTrackHitHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads lcio tracker hit objects from an SIO stream.
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

#endif /* ifndef hep_lcio_event_SIOCALHITHANDLER_H */
