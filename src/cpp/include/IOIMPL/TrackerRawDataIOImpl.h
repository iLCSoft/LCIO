#ifndef SIO_TrackerRawDataIOImpl_H
#define SIO_TrackerRawDataIOImpl_H 1

#include "IMPL/TrackerRawDataImpl.h"

namespace SIO{

  class SIOTrackerRawDataHandler ;
}

namespace IOIMPL {

  class SIOCalHitHandler ;
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Sep 11, 2003
 */
  class TrackerRawDataIOImpl : public IMPL::TrackerRawDataImpl {
    
    friend class SIO::SIOTrackerRawDataHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_TrackerRawDataIOImpl_H */
