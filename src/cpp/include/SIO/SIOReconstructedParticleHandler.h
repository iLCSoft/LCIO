#ifndef SIO_SIORECONSTRUCTEDPARTICLEHANDLER_H
#define SIO_SIORECONSTRUCTEDPARTICLEHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of ReconstructedParticles.
 *
 * @author gaede
 * @version Mar 31, 2004
 */

  class SIOReconstructedParticleHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads lcio reconstructedParticleer hit objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      DATA::LCObject** objP,  
			      unsigned int flag,
			      unsigned int vers)  ;
	
    /** Writes lcio objects to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, 
			       const DATA::LCObject* obj,
			       unsigned int flag) ;
	
  }; // class
}; // namespace

#endif /* ifndef SIO_SIORECONSTRUCTEDPARTICLEHANDLER_H */
