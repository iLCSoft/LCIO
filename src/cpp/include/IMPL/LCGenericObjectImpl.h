// -*- C++ -*-
#ifndef EVENT_LCGENERICOBJECTIMPL_H
#define EVENT_LCGENERICOBJECTIMPL_H 1

#include "EVENT/LCGenericObject.h"
#include "AccessChecked.h"
#include <vector>

namespace IMPL {

/** Default LCIO implementation of the interface to store generic 
 * user data.
 * 
 * @author gaede 
 * @version $Id: LCGenericObjectImpl.h,v 1.1 2004-08-16 09:42:06 gaede Exp $
 */

class LCGenericObjectImpl : public EVENT::LCGenericObject, public AccessChecked {

public: 

  /** Variable size c'tor.
   */
  LCGenericObjectImpl() ;

  /** Fixed size c'tor.
   */
  LCGenericObjectImpl(int nInt, int nFloat, int nDouble) ;

  /// Destructor.
  virtual ~LCGenericObjectImpl() { /* nop */; }
  
  virtual int id() { return simpleUID() ; }

  /** Number of integer values stored in this object.
   */
  virtual int getNInt() const;
  
  /** Number of float values stored in this object.
   */
  virtual int getNFloat() const;
  
  /** Number of double values stored in this object.
   */
  virtual int getNDouble() const;
  
  /** Returns the integer value for the given index.
   */
  virtual int getIntVal(int index) const;
  
  /** Returns the float value for the given index.
   */
  virtual float getFloatVal(int index) const;
  
  /** Returns the double value for the given index.
   */
  virtual float getDoubleVal(int index) const;
  


  /** Sets the integer value at the given index.
   */
  virtual void setIntVal(unsigned index, int value) ;

  /** Sets the float value at the given index.
   */
  virtual void setFloatVal(unsigned index, float value) ;

  /** Sets the double value at the given index.
   */
  virtual void setDoubleVal(unsigned index, double value) ;


  /** True if objects of the implementation class have a fixed size, i.e
   * getNInt, getNFloat and getNDouble will return values that are constant during 
   * the lifetime of the object.
   */
  virtual bool isFixedSize() const;

protected:
  
  std::vector<int> _intVec ;
  std::vector<float> _floatVec ;
  std::vector<double> _doubleVec ;
  bool _isFixedSize ;

}; // class
} // namespace EVENT
#endif /* ifndef EVENT_LCGENERICOBJECTIMPL_H */
