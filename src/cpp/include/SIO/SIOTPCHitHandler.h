#ifndef SIO_SIOTPCHITHANDLER_H
#define SIO_SIOTPCHITHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of TPCHits.
 *
 * @author gaede
 * @version Sep 11, 2003
 */

  class SIOTPCHitHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads lcio  TPC hit objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      DATA::LCObject** objP,  
			      unsigned int flag,
			      unsigned int vers)  ;
	
    /** Writes lcio TPC objects to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, 
			       const DATA::LCObject* obj,
			       unsigned int flag) ;
	
  }; // class
}; // namespace

#endif /* ifndef SIO_SIOTPCHITHANDLER_H */
