#ifndef SIO_SIOLCWGTRELATIONHANDLER_H
#define SIO_SIOLCWGTRELATIONHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
  /** Implementation of SIOObjectHandler to handle IO of CalorimeterHits.
   *
   * @author gaede
   * @version $Id: SIOLCRelationHandler.h,v 1.2 2005-04-15 08:37:42 gaede Exp $
   */
  class SIOLCRelationHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads lcio relation objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP)  ;
	
    /** Writes lcio objects to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj) ;
	
  }; // class
} // namespace

#endif /* ifndef hep_lcio_event_SIOLCWGTRELATIONHANDLER_H */
