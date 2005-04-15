#ifndef SIO_SIMCALORIMETERHITIOIMPL_H
#define SIO_SIMCALORIMETERHITIOIMPL_H 1

#include "IMPL/SimCalorimeterHitImpl.h"

namespace SIO{

  class SIOSimCalHitHandler ;

}

namespace IOIMPL {

  class SIOSimCalHitHandler ;
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Mar 7, 2003
 */
  class SimCalorimeterHitIOImpl : public IMPL::SimCalorimeterHitImpl {
    
    friend class SIO::SIOSimCalHitHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_CALORIMETERIOHITIMPL_H */
