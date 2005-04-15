#ifndef SIO_TRACKERHITIOIMPL_H
#define SIO_TRACKERHITIOIMPL_H 1

#include "IMPL/TrackerHitImpl.h"

// forward declaration
namespace SIO{ 
  class SIOTrackerHitHandler ;
}

namespace IOIMPL {

  class SIOTrackerHitHandler ;
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Mar 12, 2003
 */
  class TrackerHitIOImpl : public IMPL::TrackerHitImpl {
    
    friend class SIO::SIOTrackerHitHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_TRACKERHITIOIMPL_H */
