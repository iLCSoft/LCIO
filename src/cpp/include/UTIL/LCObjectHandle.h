#ifndef LCIO_OBJECTHANDLE_H
#define LCIO_OBJECTHANDLE_H 1

#include "EVENT/LCObject.h"
#include "Exceptions.h"
//#include "lcio.h"

namespace UTIL{

  /**Template handle class for LCObjects 
   * @author gaede
   * @version Sep 24, 2003
   */

  template<class T> class LCObjectHandle{
    
  protected: 
    T* _lcObj ;
    
  public: 
    virtual ~LCObjectHandle() { /*no_op*/; } 
    
    LCObjectHandle( T* lcObj) : _lcObj(lcObj) {
    }

    LCObjectHandle(EVENT::LCObject* lcObj){
      
      _lcObj = dynamic_cast< T* >( lcObj ) ;

      if( _lcObj == 0 )
	throw EVENT::Exception("dynamic cast failed for LCObjectHandle() !" ) ;
    }

    T* operator->() { return _lcObj ; } 

    T* lcObj() { return _lcObj ; } 

  }; 

} // namespace
#endif /* ifndef LCIO_OBJECTHANDLE_H */
