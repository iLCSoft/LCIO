#ifndef SIO_SIOPARTICLEHANDLER_H
#define SIO_SIOPARTICLEHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    

  /** Implementation of SIOObjectHandler to handle IO of MCParticles.
   *
   * @author gaede
   * @version Mar 6, 2003
   */
  class SIOParticleHandler : public SIOObjectHandler {

  public:
	
    /** Reads lcio MCParticle objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      DATA::LCObject** objP,
			      unsigned int flag, 
			      unsigned int vers)  ;
	
    /** Writes lcio MCParticle objects to an SIO stream.
     */
	
    virtual unsigned int write(SIO_stream* stream, 
			       const DATA::LCObject* obj,
			       unsigned int flag) ;
	
  }; // class
    
}; //namespace
#endif /* ifndef SIO_SIOPARTICLEHANDLER_H */
