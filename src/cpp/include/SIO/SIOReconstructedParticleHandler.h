#ifndef SIO_SIORECONSTRUCTEDPARTICLEHANDLER_H
#define SIO_SIORECONSTRUCTEDPARTICLEHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of ReconstructedParticles.
 *
 * @author gaede
 * @version $Id: SIOReconstructedParticleHandler.h,v 1.3 2004-08-20 16:45:24 gaede Exp $
 */

  class SIOReconstructedParticleHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads lcio ReconstructedParticle objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP)  ;
	
    /** Writes lcio ReconstructedParticle objects to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj) ;
	
  }; // class
}; // namespace

#endif /* ifndef SIO_SIORECONSTRUCTEDPARTICLEHANDLER_H */
