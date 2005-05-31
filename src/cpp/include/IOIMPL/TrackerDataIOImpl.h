#ifndef SIO_TrackerDataIOImpl_H
#define SIO_TrackerDataIOImpl_H 1

#include "IMPL/TrackerDataImpl.h"

namespace SIO{

  class SIOTrackerDataHandler ;
}

namespace IOIMPL {

  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Sep 11, 2003
 */
  class TrackerDataIOImpl : public IMPL::TrackerDataImpl {
    
    friend class SIO::SIOTrackerDataHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_TrackerDataIOImpl_H */
