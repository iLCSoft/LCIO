#ifndef SIO_PARTICLEIDIOIMPL_H
#define SIO_PARTICLEIDIOIMPL_H 1

#include "IMPL/ParticleIDImpl.h"

// forward declaration
namespace SIO{ 
  class SIOParticleIDHandler ;
  class SIOReconstructedParticleHandler ;
  class SIOClusterHandler ;
}

namespace IOIMPL {

  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Mar 31, 2004
 */
  class ParticleIDIOImpl : public IMPL::ParticleIDImpl {
    
    // ParticleIDs are handled by the SIOReconstructedParticleHandler !!
    // -> no collections of ParticleIDs in the event
    friend class SIO::SIOReconstructedParticleHandler ;
    friend class SIO::SIOClusterHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_PARTICLEIDIOIMPL_H */
