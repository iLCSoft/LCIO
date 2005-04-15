#ifndef SIO_SIMTRACKERHITIOIMPL_H
#define SIO_SIMTRACKERHITIOIMPL_H 1

#include "IMPL/SimTrackerHitImpl.h"

// forward declaration
namespace SIO{ 
  class SIOSimTrackHitHandler ;
}

namespace IOIMPL {

  class SIOSimTrackHitHandler ;
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Mar 12, 2003
 */
  class SimTrackerHitIOImpl : public IMPL::SimTrackerHitImpl {
    
    friend class SIO::SIOSimTrackHitHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_SIMTRACKERHITIOIMPL_H */
