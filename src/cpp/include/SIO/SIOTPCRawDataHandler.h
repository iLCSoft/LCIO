#ifndef SIO_SIOTPCRawDataHandler_H
#define SIO_SIOTPCRawDataHandler_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of TPCRawDatas.
 *
 * @author gaede
 * @version $Id: SIOTPCRawDataHandler.h,v 1.1 2005-04-26 08:14:29 gaede Exp $
 */

  class SIOTPCRawDataHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads lcio TPCRawData objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP)  ;
	
    /** Writes TPCRawData hit objects to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj) ;
	
  }; // class
} // namespace

#endif /* ifndef SIO_SIOTPCHITHANDLER_H */
