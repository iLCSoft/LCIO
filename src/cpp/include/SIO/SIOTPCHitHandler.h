#ifndef SIO_SIOTPCHITHANDLER_H
#define SIO_SIOTPCHITHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of TPCHits.
 *
 * @author gaede
 * @version $Id: SIOTPCHitHandler.h,v 1.4 2005-04-15 08:37:43 gaede Exp $
 */

  class SIOTPCHitHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads lcio  TPC hit objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP)  ;
	
    /** Writes lcio TPC hit objects to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj) ;
	
  }; // class
} // namespace

#endif /* ifndef SIO_SIOTPCHITHANDLER_H */
