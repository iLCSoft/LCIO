// -*- C++ -*-
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Mar 12, 2003
 */
#ifndef SIO_TRACKERHITIOIMPL_H
#define SIO_TRACKERHITIOIMPL_H 1

#include "IMPL/TrackerHitImpl.h"

// forward declaration
namespace SIO{ 
  class SIOTrackHitHandler ;
};

namespace IOIMPL {

  class SIOTrackHitHandler ;
  
  class TrackerHitIOImpl : public IMPL::TrackerHitImpl {
    
    friend class SIO::SIOTrackHitHandler ;
    
  }; // class

} ; // namespace

#endif /* ifndef SIO_TRACKERHITIOIMPL_H */
