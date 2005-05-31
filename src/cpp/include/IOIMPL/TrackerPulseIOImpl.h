#ifndef SIO_TrackerPulseIOImpl_H
#define SIO_TrackerPulseIOImpl_H 1

#include "IMPL/TrackerPulseImpl.h"

namespace SIO{

  class SIOTrackerPulseHandler ;
}

namespace IOIMPL {

  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Sep 11, 2003
 */
  class TrackerPulseIOImpl : public IMPL::TrackerPulseImpl {
    
    friend class SIO::SIOTrackerPulseHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_TrackerPulseIOImpl_H */
