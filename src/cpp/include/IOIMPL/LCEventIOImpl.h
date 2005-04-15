#ifndef IOIMPL_LCEVENTIOIMPL_H
#define IOIMPL_LCEVENTIOIMPL_H 1


#include "IMPL/LCEventImpl.h"

// forward declarations of friend classes :
namespace SIO {
  class SIOEventHandler ;
  class SIOEventHeaderHandler ;
  class SIOReader ;
}


namespace IOIMPL {
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Mar 6, 2003
 */
  class LCEventIOImpl : public IMPL::LCEventImpl {
    
  // the reason for having this subclass
    friend class SIO::SIOReader ;
    friend class SIO::SIOEventHeaderHandler ;
    friend class SIO::SIOEventHandler ;
  
  }; // class

} // namespace
#endif /* ifndef IOIMPL_LCEVENTIOIMPL_H */


