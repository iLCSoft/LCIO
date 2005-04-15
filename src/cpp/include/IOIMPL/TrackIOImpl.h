#ifndef SIO_TRACKIOIMPL_H
#define SIO_TRACKIOIMPL_H 1

#include "IMPL/TrackImpl.h"

// forward declaration
namespace SIO{ 
  class SIOTrackHandler ;
}

namespace IOIMPL {

  class SIOTrackHandler ;
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Mar 15, 2004
 */
  class TrackIOImpl : public IMPL::TrackImpl {
    
    friend class SIO::SIOTrackHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_TRACKIOIMPL_H */
