#ifndef SIO_SiliconRawHitIOImpl_H
#define SIO_SiliconRawHitIOImpl_H 1

#include "IMPL/SiliconRawHitImpl.h"

namespace SIO{

  class SIOSiliconRawHitHandler ;
}

namespace IOIMPL {

  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Sep 11, 2003
 */
  class SiliconRawHitIOImpl : public IMPL::SiliconRawHitImpl {
    
    friend class SIO::SIOSiliconRawHitHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_SiliconRawHitIOImpl_H */
