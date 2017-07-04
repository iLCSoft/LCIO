#ifndef UTIL_LCFIXEDOBJECT_H
#define UTIL_LCFIXEDOBJECT_H 1

#include "lcio.h"
#include "IMPL/LCGenericObjectImpl.h"

#include <iostream>

namespace UTIL{

  /** Template class for fixed size LCGenericObject subclasses.
   *  By inheriting from this class users can very easily implement their
   *  own objects.<br>
   *  It uses an instance of LCGenericObjectImpl that holds the data, thus 
   *  there is no overhead when the data is read from a database or file
   *  for copying it  to some local structure (Decorator pattern).<br>
   *  This is still an abstract class: subclasses have to implement LCGenericObject::getTypeName()
   *  and LCGenericObject::getDataDescription().
   * 
   * @author gaede
   * @version $Id: LCFixedObject.h,v 1.3 2011-03-13 12:41:49 gaede Exp $
   */

  template <int NINT, int NFLOAT, int NDOUBLE>
  class LCFixedObject : public EVENT::LCGenericObject {
    
  public: 
    
    /// default copy constructor - use with care
    LCFixedObject(const LCFixedObject&) = default ;

    /// default assignment operator - use with care
    LCFixedObject& operator=(const LCFixedObject&) = default ;

    /** Default c'tor.
     */
    LCFixedObject():
      _createdObject( true )  {
      _obj = new IMPL::LCGenericObjectImpl( NINT, NFLOAT , NDOUBLE ) ;
    }
    
    /** C'tor to be used for elements of LCGenericObjects read from
     * an LCIO file or the database. Subclasses should 'override' this, e.g.:<br>
     * Myclass(LCObject* obj) : LCFixedObject(obj) {} <br>
     * 
     * 
     */
    LCFixedObject(LCObject* object) : 
      _createdObject(false)  {
      
      _obj = dynamic_cast<IMPL::LCGenericObjectImpl*>( object )  ;
      
      if( _obj==0 ){

	// could be an instance of LCFixedObject !?
	LCFixedObject<NINT,NFLOAT,NDOUBLE>* f = 
	  dynamic_cast< LCFixedObject<NINT,NFLOAT,NDOUBLE>* >( object ) ;

	if( f != 0 )
	  _obj = f->obj() ;

      }

      // do some sanity checks ...
      if( _obj==0 ){
        throw EVENT::Exception("Cannot create LCFixedObject from sth."
			" that is not LCGenericObjectImpl" ) ;
      } 
      
      if(  ( _obj->getNInt()    != NINT )    || 
	   ( _obj->getNFloat()  != NFLOAT )  || 
	   ( _obj->getNDouble() != NDOUBLE )   ) {
	
        throw EVENT::Exception("LCFixedObject(LCObject* obj): Wrong number of elements in object" ) ;
      }
    }
    
    /** The LCGenericObjectImpl . Sublcasses use this to access their data.
     */
    IMPL::LCGenericObjectImpl* obj() { return _obj ; }


    /** Clean up if we created a new LCGenericObjectImpl 
     */
    virtual ~LCFixedObject() { 
      if( _createdObject )  delete _obj ; 
    }
    

    /** Return the id of the underlying LCGenericObjectImpl */
    virtual int id() const { return _obj->id() ; }
     

    // ---- need to implement LCGenericObject interface:
    
    int getNInt() const    { return NINT ; } 
    int getNFloat() const  { return NFLOAT ; }  
    int getNDouble() const { return NDOUBLE ; } 
    
    int getIntVal(int index) const {
      return _obj->getIntVal( index ) ;
    }
    float getFloatVal(int index) const { 
      return _obj->getFloatVal( index ) ;
    }
    double getDoubleVal(int index) const {
      return  _obj->getDoubleVal( index ) ;
    }
    bool isFixedSize() const { return true ; }
    
    // ---- end of LCGenericObject interface
    
  protected:
    
    IMPL::LCGenericObjectImpl* _obj{NULL} ;
    bool _createdObject{false} ;
    
  }; 
  
} // namespace
#endif /* ifndef LCIO_LCFIXEDOBJECT_H */
