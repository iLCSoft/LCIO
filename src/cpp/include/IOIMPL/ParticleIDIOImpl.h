#ifndef SIO_PARTICLEIDIOIMPL_H
#define SIO_PARTICLEIDIOIMPL_H 1

#include "IMPL/ParticleIDImpl.h"

// forward declaration
namespace SIO{ 
  class SIOParticleIDHandler ;
};

namespace IOIMPL {

  class SIOReconstructedParticleHandler ;
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Mar 31, 2004
 */
  class ParticleIDIOImpl : public IMPL::ParticleIDImpl {
    
    // ParticleIDs are handled by the SIOReconstructedParticleHandler !!
    // -> no collections of ParticleIDs in the event
    friend class SIO::SIOReconstructedParticleHandler ;
    
  }; // class

} ; // namespace

#endif /* ifndef SIO_PARTICLEIDIOIMPL_H */
