#ifndef LCRELATIONIOIMPL_H
#define LCRELATIONIOIMPL_H 1

#include "IMPL/LCRelationImpl.h"

namespace SIO{
  class SIORelationHandler;
};

namespace IOIMPL {
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Apr 28, 2004
 */

  /** Small helper class to store relations when reading until pointer reallocation.
   */
  class Rel{
  public:
    Rel() : from(0) , to(0)  , wgt (1.) { } 
    EVENT::LCObject* from ;
    EVENT::LCObject* to ;
    float wgt ;
  } ;
  typedef std::vector<Rel> RelVec ;

  class LCRelationIOImpl  : public IMPL::LCRelationImpl {
    
    friend class SIO::SIORelationHandler ;

  public:
    LCRelationIOImpl() :  _relVec(0)  { }
    LCRelationIOImpl(const std::string& fromType, const std::string& toType) : 
      LCRelationImpl( fromType, toType ) , 
      _relVec(0)  { }
    //    LCRelationIOImpl(const std::string& type) : IMPL::LCRelationImpl(type) { } 
    
    
  protected:

    void fillMap() {
      if( _relVec == 0 ) return ;
      
      for(unsigned int i=0 ; i<_relVec->size() ; i++) {
	addRelation(  (*_relVec)[i].from , (*_relVec)[i].to , (*_relVec)[i].wgt  ) ;
      }

      delete _relVec ;
    }

    RelVec* _relVec ;

  }; // class
}; // namespace 
#endif /* ifndef LCRELATIONIMPL_H */
