// -*- C++ -*-
#ifndef EVENT_LCPARAMETERSIMPL_H
#define EVENT_LCPARAMETERSIMPL_H 1

#include "EVENT/LCParameters.h"
#include "IMPL/AccessChecked.h"

#include <map>
#include <vector>

namespace IMPL {


  class LCRunHeaderImpl ;
  class LCEventImpl ;
  class LCCollectionVec ;

  typedef std::map< std::string, EVENT::IntVec >    IntMap ;
  typedef std::map< std::string, EVENT::FloatVec >  FloatMap ;
  typedef std::map< std::string, EVENT::StringVec > StringMap ;
  

  /** Implementation of Simple interface to store generic named parameters of type
   *  int, float and string. 
   *  It can be used  to store (user) meta data that is 
   *  run, event or collection dependent. 
   * 
   * @author gaede 
   * @version Jun 23, 2004
   * @see LCRunHeader.parameters()
   * @see LCEvent.parameters()
   * @see LCCollection.parameters()
   */
  
  class LCParametersImpl : public EVENT::LCParameters , public AccessChecked{

    friend class LCRunHeaderImpl ;
    friend class LCEventImpl ;
    friend class LCCollectionVec ;
    
  public: 
    
    LCParametersImpl() ; 
    
    /// Destructor.
    virtual ~LCParametersImpl() { /* nop */; }
    
    /** Returns the first integer value for the given key.
     */
    virtual int getIntVal(const std::string & key) const  ;
    
    /** Returns the first float value for the given key.
     */
    virtual float getFloatVal(const std::string & key) const ;
    
    /** Returns the first string value for the given key.
     */
    virtual const std::string & getStringVal(const std::string & key) const ;
    
    /** Adds all integer values for the given key to values.
     *  Returns a reference to values for convenience.
     */
    virtual EVENT::IntVec & getIntVals(const std::string & key, EVENT::IntVec & values) const ;
    
    /** Adds all float values for the given key to values.
     *  Returns a reference to values for convenience.
     */
    virtual EVENT::FloatVec & getFloatVals(const std::string & key, EVENT::FloatVec & values) const ;
    
    /** Adds all float values for the given key to values.
     *  Returns a reference to values for convenience.
     */
    virtual  EVENT::StringVec & getStringVals(const std::string & key, EVENT::StringVec & values) const ;
    
    /** Returns a list of all keys of integer parameters.
     */
    virtual const EVENT::StringVec & getIntKeys( EVENT::StringVec & keys) const  ;

    /** Returns a list of all keys of float parameters.
     */
    virtual const EVENT::StringVec & getFloatKeys(EVENT::StringVec & keys)  const ;

    /** Returns a list of all keys of string parameters.
     */
    virtual const EVENT::StringVec & getStringKeys(EVENT::StringVec & keys)  const ;
    
    /** The number of integer values stored for this key.
     */ 
    virtual int getNInt(const std::string & key) const ;
    
    /** The number of float values stored for this key.
     */ 
    virtual int getNFloat(const std::string & key) const ;
    
    /** The number of string values stored for this key.
     */ 
    virtual int getNString(const std::string & key) const ;
    
    /** Set integer value for the given key.
     */
    virtual void setValue(const std::string & key, int value) ;

    /** Set float value for the given key.
     */
    virtual void setValue(const std::string & key, float value) ;

    /** Set string value for the given key.
     */
    virtual void setValue(const std::string & key, const std::string & value) ;

    /** Set integer values for the given key.
     */
    virtual void setValues(const std::string & key, const EVENT::IntVec & values);

    /** Set float values for the given key.
     */
    virtual void setValues(const std::string & key, const EVENT::FloatVec & values);

    /** Set string values for the given key.
     */
    virtual void setValues(const std::string & key, const EVENT::StringVec & values);


  protected:

    mutable IntMap _intMap ;
    mutable FloatMap _floatMap ;
    mutable StringMap _stringMap ;
    
  }; // class
} // namespace IMPL
#endif /* ifndef EVENT_LCPARAMETERSIMPL_H */
