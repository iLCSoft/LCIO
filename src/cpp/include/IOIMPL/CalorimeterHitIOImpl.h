// -*- C++ -*-
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Mar 7, 2003
 */
#ifndef SIO_CALORIMETERHITIOIMPL_H
#define SIO_CALORIMETERHITIOIMPL_H 1

#include "IMPL/CalorimeterHitImpl.h"
//#include "SIOCalHitHandler.h"

namespace SIO{

  class SIOCalHitHandler ;

};

namespace IOIMPL {

  class SIOCalHitHandler ;
  
  class CalorimeterHitIOImpl : public IMPL::CalorimeterHitImpl {
    
    friend class SIO::SIOCalHitHandler ;
    
  }; // class

} ; // namespace

#endif /* ifndef SIO_CALORIMETERIOHITIMPL_H */
