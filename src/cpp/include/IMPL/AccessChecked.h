#ifndef IMPL_ACCESSCHECKED_H
#define IMPL_ACCESSCHECKED_H 1


#include "Exceptions.h"

namespace IMPL {
  
  class  LCEventImpl ;
  class  LCCollectionVec ;

  /** Controls access to objects.
   * 
   * @author gaede
   * @version Sep 4, 2003
   */
  class AccessChecked {
    
    friend class LCEventImpl ;  // the event needs to set the access mode
    friend class LCCollectionVec ;  // the collection needs to set the access mode
    
  public:
    AccessChecked() ;

  protected:
    virtual void setReadOnly( bool readOnly ) ;
    
    void checkAccess() throw ( EVENT::ReadOnlyException ) ;
    void checkAccess(const char* what) throw ( EVENT::ReadOnlyException ) ;

  protected: 
    bool readOnly ;
    
  };
}; // namespace IMPL

#endif /* ifndef IMPL_ACCESSCHECKED_H */
