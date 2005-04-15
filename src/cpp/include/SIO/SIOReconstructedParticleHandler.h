#ifndef SIO_SIORECONSTRUCTEDPARTICLEHANDLER_H
#define SIO_SIORECONSTRUCTEDPARTICLEHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of ReconstructedParticles.
 *
 * @author gaede
 * @version $Id: SIOReconstructedParticleHandler.h,v 1.4 2005-04-15 08:37:43 gaede Exp $
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
} // namespace

#endif /* ifndef SIO_SIORECONSTRUCTEDPARTICLEHANDLER_H */
