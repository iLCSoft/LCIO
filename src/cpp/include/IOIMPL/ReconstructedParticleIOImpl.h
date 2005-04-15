#ifndef SIO_RECONSTRUCTEDPARTICLEIOIMPL_H
#define SIO_RECONSTRUCTEDPARTICLEIOIMPL_H 1

#include "IMPL/ReconstructedParticleImpl.h"

// forward declaration
namespace SIO{ 
  class SIOReconstructedParticleHandler ;
}

namespace IOIMPL {

  class SIOReconstructedParticleHandler ;
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Mar 31, 2004
 */
  class ReconstructedParticleIOImpl : public IMPL::ReconstructedParticleImpl {
    
    friend class SIO::SIOReconstructedParticleHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_RECONSTRUCTEDPARTICLEIOIMPL_H */
