#ifndef SIO_SIOLCWGTRELATIONHANDLER_H
#define SIO_SIOLCWGTRELATIONHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
  /** Implementation of SIOObjectHandler to handle IO of CalorimeterHits.
   *
   * @author gaede
   * @version Mar 6, 2003
   */
  class SIOLCWgtRelationHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads lcio calorimeter hit objects from an SIO stream.
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

#endif /* ifndef hep_lcio_event_SIOLCWGTRELATIONHANDLER_H */
