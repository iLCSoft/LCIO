#ifndef UTIL_LCOBJECTHANDLE_H
#define UTIL_LCOBJECTHANDLE_H 1

#include "EVENT/LCObject.h"
#include "Exceptions.h"
//#include "lcio.h"

namespace UTIL{

  /**Template handle class for LCObjects 
   * @author gaede
   * @version $Id: LCObjectHandle.h,v 1.4 2005-02-03 14:11:00 gaede Exp $
   */

  template<class T> class LCObjectHandle{
    
  protected: 
    T* _lcObj ;
    
  public: 
    virtual ~LCObjectHandle() { /*no_op*/; } 
    
    LCObjectHandle( T* obj) : _lcObj(obj) {
    }

    LCObjectHandle(EVENT::LCObject* obj){
      
      _lcObj = dynamic_cast< T* >( obj ) ;

      if( _lcObj == 0 )
	throw EVENT::Exception("dynamic cast failed for LCObjectHandle() !" ) ;
    }

    T* operator->() { return _lcObj ; } 

    T* lcObj() { return _lcObj ; } 

  }; 

} // namespace
#endif /* ifndef LCIO_OBJECTHANDLE_H */
