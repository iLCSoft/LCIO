#ifndef SIO_TPCPulseIOImpl_H
#define SIO_TPCPulseIOImpl_H 1

#include "IMPL/TPCPulseImpl.h"

namespace SIO{

  class SIOTPCPulseHandler ;
}

namespace IOIMPL {

  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Sep 11, 2003
 */
  class TPCPulseIOImpl : public IMPL::TPCPulseImpl {
    
    friend class SIO::SIOTPCPulseHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_TPCPulseIOImpl_H */
