#ifndef SIO_SIOCLUSTERHANDLER_H
#define SIO_SIOCLUSTERHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of Clusters.
 *
 * @author gaede
 * @version Mar 29, 2004
 */

  class SIOClusterHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads lcio clusterer hit objects from an SIO stream.
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

#endif /* ifndef SIO_SIOCLUSTERHANDLER_H */
