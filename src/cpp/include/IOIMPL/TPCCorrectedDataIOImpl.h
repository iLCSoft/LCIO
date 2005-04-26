#ifndef SIO_TPCCorrectedDataIOImpl_H
#define SIO_TPCCorrectedDataIOImpl_H 1

#include "IMPL/TPCCorrectedDataImpl.h"

namespace SIO{

  class SIOTPCCorrectedDataHandler ;
}

namespace IOIMPL {

  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Sep 11, 2003
 */
  class TPCCorrectedDataIOImpl : public IMPL::TPCCorrectedDataImpl {
    
    friend class SIO::SIOTPCCorrectedDataHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_TPCCorrectedDataIOImpl_H */
