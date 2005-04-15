#ifndef SIO_MCPARTICLEIOIMPL_H
#define SIO_MCPARTICLEIOIMPL_H 1

#include "IMPL/MCParticleImpl.h"

namespace SIO{
 class SIOParticleHandler ;
}

namespace IOIMPL {

/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Mar 7, 2003
 */
class MCParticleIOImpl : public IMPL::MCParticleImpl{

  friend class SIO::SIOParticleHandler ;
};

} // namespace

#endif // SIO_MCPARTICLEIOIMPL_H
