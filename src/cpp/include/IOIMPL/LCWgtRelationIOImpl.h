#ifndef LCWGTRELATIONIOIMPL_H
#define LCWGTRELATIONIOIMPL_H 1

#include "IMPL/LCWgtRelationImpl.h"

namespace SIO{
  class SIOLCWgtRelationHandler;
};


namespace IOIMPL {
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Apr 28, 2004
 */

  class LCWgtRelationIOImpl  : public IMPL::LCWgtRelationImpl {
    
    friend class SIO::SIOLCWgtRelationHandler ;

  public:
    LCWgtRelationIOImpl() { }
    LCWgtRelationIOImpl( EVENT::LCObject* from, EVENT::LCObject* to , float weight=1.0f ) 
      : LCWgtRelationImpl(from, to, weight) {}
    //    LCWgtRelationIOImpl(const std::string& type) : IMPL::LCWgtRelationImpl(type) { } 

  }; // class
}; // namespace 
#endif /* ifndef LCRELATIONIMPL_H */
