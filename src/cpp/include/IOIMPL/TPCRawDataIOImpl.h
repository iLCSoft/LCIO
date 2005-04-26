#ifndef SIO_TPCHITIOIMPL_H
#define SIO_TPCHITIOIMPL_H 1

#include "IMPL/TPCRawDataImpl.h"

namespace SIO{

  class SIOTPCRawDataHandler ;
}

namespace IOIMPL {

  class SIOCalHitHandler ;
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Sep 11, 2003
 */
  class TPCRawDataIOImpl : public IMPL::TPCRawDataImpl {
    
    friend class SIO::SIOTPCRawDataHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_TPCHITIOIMPL_H */
