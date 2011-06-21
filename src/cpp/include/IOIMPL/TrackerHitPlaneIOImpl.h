#ifndef SIO_TRACKERHITPLANEIOIMPL_H
#define SIO_TRACKERHITPLANEIOIMPL_H 1

#include "IMPL/TrackerHitPlaneImpl.h"

// forward declaration
namespace SIO{ 
  class SIOTrackerHitPlaneHandler ;
}

namespace IOIMPL {

  class SIOTrackerHitPlaneHandler ;
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author engels
 * @version 06-2011
 */
  class TrackerHitPlaneIOImpl : public IMPL::TrackerHitPlaneImpl {
    
    friend class SIO::SIOTrackerHitPlaneHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_TRACKERHITPLANEIOIMPL_H */
