#ifndef SIO_SIOTPCCorrectedDataHandler_H
#define SIO_SIOTPCCorrectedDataHandler_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of TPCCorrectedDatas.
 *
 * @author gaede
 * @version $Id: SIOTPCCorrectedDataHandler.h,v 1.1 2005-04-26 08:34:49 gaede Exp $
 */

  class SIOTPCCorrectedDataHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads lcio TPCCorrectedData objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP)  ;
	
    /** Writes TPCCorrectedData hit objects to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj) ;
	
  }; // class
} // namespace

#endif /* ifndef SIO_SIOTPCHITHANDLER_H */
