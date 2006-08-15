#ifndef SIO_SIOVERTEXHANDLER_H
#define SIO_SIOVERTEXHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of Vertexes.
 *
 * @author gaede, engels
 * @version $Id: SIOVertexHandler.h,v 1.1.2.1 2006-08-15 13:01:14 engels Exp $
 */

  class SIOVertexHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads lcio Vertex objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP)  ;
	
    /** Writes lcio Vertex objects to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj) ;
	
  }; // class
} // namespace

#endif /* ifndef SIO_SIOVERTEXHANDLER_H */
