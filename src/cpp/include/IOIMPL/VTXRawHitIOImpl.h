#ifndef SIO_VTXRawHitIOImpl_H
#define SIO_VTXRawHitIOImpl_H 1

#include "IMPL/VTXRawHitImpl.h"

namespace SIO{

  class SIOVTXRawHitHandler ;
}

namespace IOIMPL {

  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Sep 11, 2003
 */
  class VTXRawHitIOImpl : public IMPL::VTXRawHitImpl {
    
    friend class SIO::SIOVTXRawHitHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_VTXRawHitIOImpl_H */
