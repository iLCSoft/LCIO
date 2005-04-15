#ifndef LCWGTRELATIONIOIMPL_H
#define LCWGTRELATIONIOIMPL_H 1

#include "IMPL/LCRelationImpl.h"

namespace SIO{
  class SIOLCRelationHandler;
}


namespace IOIMPL {
  
  /** Adding stuff needed for io (friend declarations, etc.)
   * 
   * @author gaede
   * @version $Id: LCRelationIOImpl.h,v 1.4 2005-04-15 08:37:40 gaede Exp $
   */
  
  class LCRelationIOImpl  : public IMPL::LCRelationImpl {
    
    friend class SIO::SIOLCRelationHandler ;
    
  public:
    LCRelationIOImpl() { }
    LCRelationIOImpl( EVENT::LCObject* from, EVENT::LCObject* to , float weight=1.0f ) 
      : LCRelationImpl(from, to, weight) {}
    //    LCRelationIOImpl(const std::string& type) : IMPL::LCRelationImpl(type) { } 

  }; // class
} // namespace 
#endif /* ifndef LCRELATIONIMPL_H */
