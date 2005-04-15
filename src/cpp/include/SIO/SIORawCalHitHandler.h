#ifndef SIO_SIORAWCALHITHANDLER_H
#define SIO_SIORAWCALHITHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
  /** Implementation of SIOObjectHandler to handle IO of RawCalorimeterHits.
   *
   * @author gaede
   * @version $Id: SIORawCalHitHandler.h,v 1.2 2005-04-15 08:37:42 gaede Exp $
   */
  class SIORawCalHitHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads lcio raw calorimeter hit objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP)  ;
	
    /** Writes lcio raw calorimeter hit objects to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj) ;
	
  }; // class
} // namespace

#endif /* ifndef hep_lcio_event_SIORAWCALHITHANDLER_H */
