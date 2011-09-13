#ifndef SIO_TRACKSTATEIOIMPL_H
#define SIO_TRACKSTATEIOIMPL_H 1

#include "IMPL/TrackStateImpl.h"

// forward declaration
namespace SIO{ 
  class SIOTrackStateHandler ;
  class SIOTrackHandler ;
}

namespace IOIMPL {

  class SIOTrackStateHandler ;
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede, engels
 * @version Mar 15, 2004
 */
  class TrackStateIOImpl : public IMPL::TrackStateImpl {
    
    // TrackStates are handled by the SIOTrackHandler !!
    // -> no collections of TrackStates in the event

    friend class SIO::SIOTrackHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_TRACKSTATEIOIMPL_H */
