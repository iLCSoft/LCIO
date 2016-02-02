#ifndef SIO_SIOPARTICLEHANDLER_H
#define SIO_SIOPARTICLEHANDLER_H 1

#include "SIO/SIOObjectHandler.h"

#include "EVENT/LCEvent.h"

namespace SIO {
    
    

  /** Implementation of SIOObjectHandler to handle IO of MCParticles.
   *
   * @author gaede
   * @version $Id: SIOParticleHandler.h,v 1.8 2005-04-15 08:37:42 gaede Exp $
   */
  class SIOParticleHandler : public SIOObjectHandler {

  public:
	
    /** Reads lcio MCParticle objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP)  ;
	
    /** Writes lcio MCParticle objects to an SIO stream.
     */
	
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj) ;


    static void restoreParentDaughterRelations( EVENT::LCEvent* evt) ;


  }; // class
    
} // namespace
#endif /* ifndef SIO_SIOPARTICLEHANDLER_H */
