#ifndef IOIMPL_LCRUNHEADERIOIMPL_H
#define IOIMPL_LCRUNHEADERIOIMPL_H 1


#include "IMPL/LCRunHeaderImpl.h"



// forward declarations of friend classes :
namespace SIO {
  class SIOReader ;
}


namespace IOIMPL {
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Sep 9, 2003
 */
  class LCRunHeaderIOImpl : public IMPL::LCRunHeaderImpl {
    
  // the reason for having this subclass
    friend class SIO::SIOReader ;
  
  }; // class

} // namespace
#endif /* ifndef IOIMPL_LCEVENTIOIMPL_H */


