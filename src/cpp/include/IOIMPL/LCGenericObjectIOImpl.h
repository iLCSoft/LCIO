#ifndef LCGENERICOBJECTIOIMPL_H
#define LCGENERICOBJECTIOIMPL_H 1

#include "IMPL/LCGenericObjectImpl.h"

namespace SIO{
  class SIOLCGenericObjectHandler;
}


namespace IOIMPL {
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version $Id: LCGenericObjectIOImpl.h,v 1.2 2005-04-15 08:37:40 gaede Exp $
 */

  class LCGenericObjectIOImpl  : public IMPL::LCGenericObjectImpl {
    
    friend class SIO::SIOLCGenericObjectHandler ;

  public:
    LCGenericObjectIOImpl() { }

    LCGenericObjectIOImpl(int nInt, int nFloat, int nDouble) 
      : LCGenericObjectImpl(nInt, nFloat, nDouble) {}


  }; // class
} // namespace 
#endif /* ifndef LCGENERICOBJECTIOIMPL_H */
