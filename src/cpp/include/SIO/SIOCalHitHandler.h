#ifndef SIO_SIOCALHITHANDLER_H
#define SIO_SIOCALHITHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
  /** Implementation of SIOObjectHandler to handle IO of CalorimeterHits.
   *
   * @author gaede
   * @version $Id: SIOCalHitHandler.h,v 1.7 2005-04-15 08:37:42 gaede Exp $
   */
  class SIOCalHitHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads lcio calorimeter hit objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP)  ;
	
    /** Writes lcio calorimeter hit objects to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj) ;
	
  }; // class
} // namespace

#endif /* ifndef hep_lcio_event_SIOCALHITHANDLER_H */
