#ifndef SIO_TPCHITIOIMPL_H
#define SIO_TPCHITIOIMPL_H 1

#include "IMPL/TPCHitImpl.h"

namespace SIO{

  class SIOTPCHitHandler ;
}

namespace IOIMPL {

  class SIOCalHitHandler ;
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Sep 11, 2003
 */
  class TPCHitIOImpl : public IMPL::TPCHitImpl {
    
    friend class SIO::SIOTPCHitHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_TPCHITIOIMPL_H */
