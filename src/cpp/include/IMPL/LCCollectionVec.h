#ifndef EVENT_LCCOLLECTIONVEC_H
#define EVENT_LCCOLLECTIONVEC_H 1

#include <string>
#include <vector>

#include "EVENT/LCCollection.h"
#include "EVENT/LCObject.h"
#include "AccessChecked.h"
#include "LCParametersImpl.h"

//#include "LCIO.h"

namespace IMPL {
  

  /** Implementation of the LCCollection using (inheriting from) an STL vector
   *  of LCObjects.
   * 
   * @author gaede 
   * @version $Id: LCCollectionVec.h,v 1.12 2010-01-21 16:27:36 gaede Exp $
   * @see LCObject
   * @see LCCollection
   */
  class LCCollectionVec : public EVENT::LCCollection , public EVENT::LCObjectVec 
    , public AccessChecked {
    
    
   public: 
    /**  Default Constructor - don't use - only public for auto-generated streamers 
     *   in the ROOT dictionary !! Should be protected really. 
     *   Every LCCollection needs to know the type of its elements.
     */
    LCCollectionVec() :  _typeName("UNKNOWN" ), _flag(0) { }
    
  public:
    
    /** The public default constructur that takes the name of the type of the elements.     */
    LCCollectionVec( const std::string& type ) ;
    
    /* Coppy constructor creating a deep copy of an LCCollection.
     */
    //    LCCollectionVec( const EVENT::LCCollection& col ) ;
    
    
    /** Destructor.
     */    
    virtual ~LCCollectionVec() ;
    
    /**Returns the number of entries in the collection.
     */
    virtual int getNumberOfElements() const ;
    
    /** Returns the type name of the collection - valid names are defined in LCIO.
     */
    virtual const std::string & getTypeName() const ;
    
    /** Returns pointer to element at index - no range check !.
     */
    virtual EVENT::LCObject * getElementAt(int index) const ;
    
    /** Returns flag word for collection. Bits 16-31 are reserved for LCIO
     *  Depending on the object type stored they have a special meaning, e.g. 
     *  for SimCalorimeterHits: <br>
     *  CHBIT_LONG = 31   -  store position <br>
     *  CHBIT_BARREL = 30 -  endcap or barrel <br>
     *  CHBIT_ID1 = 29 -   cellid1 is sored <br>
     *  CHBIT_PDG = 28 - store pdg of secondaries <br>
     *  &nbsp;<br>
     *  Bit 16 is used to flag collection as transient <br>
     *  Bit 17 is used to flag collection as default <br>
     *  Bit 18 is used to flag collection as subset <br>
     *  Bits 0-15 are subdetector/user specific.
     * @see isTransient()
     */
    virtual int getFlag() const ;


    /** True if collection is transient, i.e. will not be written to any LCIO file.
     *  Convenient method that checks bit 16 of the flag word.
     */
    virtual bool isTransient() const  ;
    

    /** Sets the transient flag for this collection. Transient collections are not written
     * to LCIO files.
     */
    void setTransient(bool val=true) ;

    /** True if collection is the default collection for the given type.
     *  This implies that the collection is complete and unambigous.
     *  Convenient method that checks bit BITDefault of the flag word.
     */
    virtual bool isDefault() const ;

    /** Sets the default flag for this collection. User need to make sure this 
     * flag is unique for a type.
     */
    void setDefault(bool val=true) ;


    /** True if the collection holds a subset of objects from other collections. 
     *  If the collection is not transient only the pointers/references to the original
     *  objects will be stored.
     *  Convenient method that checks bit BITSubset of the flag word.
     */
    virtual bool isSubset() const ;

    /** Sets the subset flag for this collection. Collections of subsets are only 
     *  written as pointers to LCIO files, i.e. the objects have to exist in 
     *  another non-transient collections.<br>
     *  NB: Do not add any 'real' objects to a collection flagged as subset, as 
     *  this will lead to memory leaks !
     */
    void setSubset(bool val=true) ;


    /** Sets the flag word for this collection.
     */
    void setFlag(int flag) ;
    

    /** Adds the given element to (end of) the collection. Throws an exception 
     * if the collection (event) is 'read only'.
     *
     * @throws ReadOnlyException
     */
    virtual void addElement(EVENT::LCObject * obj)  ;

    /** Removes the i-th element from the collection. Throws an exception 
     * if the collection (event) is 'read only'.
     *
     * @throws ReadOnlyException
     */
    virtual void removeElementAt(int i)  ;
    
    /** Parameters defined for this run.
     */
    virtual const EVENT::LCParameters & getParameters() const { return _params ; } 

    /** Parameters defined for this run.
     */
    virtual EVENT::LCParameters & parameters() { return _params ; } 
    

  protected:
    void setReadOnly(bool readOnly) ;

    std::string _typeName ;
    int _flag ;
    LCParametersImpl _params{} ;

}; // class
} // namespace IMPL
#endif /* ifndef EVENT_LCCOLLECTIONVEC_H */
