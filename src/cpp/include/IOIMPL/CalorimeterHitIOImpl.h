#ifndef SIO_CALORIMETERHITIOIMPL_H
#define SIO_CALORIMETERHITIOIMPL_H 1

#include "IMPL/CalorimeterHitImpl.h"

namespace SIO{

  class SIOCalHitHandler ;

}

namespace IOIMPL {

  class SIOCalHitHandler ;
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Aug 8, 2003
 */
  class CalorimeterHitIOImpl : public IMPL::CalorimeterHitImpl {
    
    friend class SIO::SIOCalHitHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_CALORIMETERIOHITIMPL_H */
