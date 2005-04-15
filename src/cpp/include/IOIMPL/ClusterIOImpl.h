#ifndef SIO_CLUSTERIOIMPL_H
#define SIO_CLUSTERIOIMPL_H 1

#include "IMPL/ClusterImpl.h"

// forward declaration
namespace SIO{ 
  class SIOClusterHandler ;
}

namespace IOIMPL {

  class SIOClusterHandler ;
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Mar 15, 2004
 */
  class ClusterIOImpl : public IMPL::ClusterImpl {
    
    friend class SIO::SIOClusterHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_CLUSTERIOIMPL_H */
