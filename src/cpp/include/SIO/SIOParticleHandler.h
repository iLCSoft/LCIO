// -*- C++ -*-
/** Implementation of SIOObjectHandler to handle IO of MCParticles.
 *
 * @author gaede
 * @version Mar 6, 2003
 */
#ifndef SIO_SIOPARTICLEHANDLER_H
#define SIO_SIOPARTICLEHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
  /** Interface for IO of CalorimeterHit objects.
   */
  class SIOParticleHandler : public SIOObjectHandler {

  public:
	
    /** Reads lcio calorimeter hit objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP,
			      unsigned int flag, 
			      unsigned int vers)  ;
	
    /** Writes lcio calorimeter hit objects to an SIO stream.
     */
	
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj,
			       unsigned int flag) ;
	
  }; // class
    
}; //namespace
#endif /* ifndef SIO_SIOPARTICLEHANDLER_H */
