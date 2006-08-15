#ifndef SIO_VERTEXIOIMPL_H
#define SIO_VERTEXIOIMPL_H 1

#include "IMPL/VertexImpl.h"

// forward declaration
namespace SIO{ 
  class SIOVertexHandler ;
}

namespace IOIMPL {

  class SIOVertexHandler ;
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede, engels
 * @version Aug 15, 2006
 */
  class VertexIOImpl : public IMPL::VertexImpl {
    
    friend class SIO::SIOVertexHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_VERTEXIOIMPL_H */
