#ifndef EVENT_LCCOLLECTIONVEC_H
#define EVENT_LCCOLLECTIONVEC_H 1

#include <string>
#include <vector>

#include "EVENT/LCCollection.h"
#include "EVENT/LCObject.h"

//#include "LCIO.h"

namespace IMPL {
  
  typedef std::vector<EVENT::LCObject*> LCObjectVec ;

  /** Implementation of the LCCollection using (inheriting from) an STL vector
   *  of LCObjects.
   * 
   * @author gaede 
   * @version Mar 4, 2003
   * @see LCObject
   * @see LCCollection
   */
  class LCCollectionVec : public EVENT::LCCollection , public LCObjectVec  {
    
  protected:
    /**  Default Constructor is protected  - every LCCollection needs to know the type
     *   of its elements.
     */
    LCCollectionVec() {  /* no default c'tor */ }
    
  public:
    
    /** The public default constructur that takes the name of the type of the elements.
     */
    LCCollectionVec( const std::string& type ) ;
    
    /** Coppy constructor creating a deep copy of an LCCollection.
     */
    LCCollectionVec( const EVENT::LCCollection& col ) ;
    
    
    /// Destructor.
    virtual ~LCCollectionVec() ;
    
    /**Returns the number of entries in the collection.
     */
    virtual int getNumberOfElements() const ;
    
    /** Returns the type name of the collection - valid names are defined in LCIO.
     */
    virtual const std::string & getTypeName() const ;
    
    /** Returns pointer to element at index - no range check !.
     */
    virtual const EVENT::LCObject * getElementAt(int index) const ;
    
    /** Returns flag word for collection. 
     * @see EVENT::LCCollection::getFlag() 
     */
    virtual int getFlag() const ;
    

    /** Sets the flag word for this collection.
     */
    void setFlag(int flag) ;
    
    
  protected:
    
    std::string _typeName ;
    int _flag ;

}; // class
}; // namespace IMPL
#endif /* ifndef EVENT_LCCOLLECTIONVEC_H */
