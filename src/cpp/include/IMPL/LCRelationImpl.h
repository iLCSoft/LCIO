// -*- C++ -*-
#ifndef EVENT_LCRELATIONIMPL_H
#define EVENT_LCRELATIONIMPL_H 1

#include <vector>
#include <map>

#include "EVENT/LCRelation.h"
#include "EVENT/LCObject.h"
#include "AccessChecked.h"



namespace IMPL {
  
  typedef std::multimap<EVENT::LCObject*,pair<EVENT::LCObject*,float> > LCWgtMultiMap ;
  typedef LCWgtMultiMap::iterator LCRelationIter ;

  /** Implementation of the LCRelation.
   * 
   * @author gaede 
   * @version Apr 15, 2004
   */
  class LCRelationImpl : public EVENT::LCRelation , public LCWgtMultiMap
    , public AccessChecked {
    
    public:

    /** Default constructor
     */
    LCRelationImpl() ;
    
    /** Destructor.
     */    
    virtual ~LCRelationImpl() ;
    
    /** The number of stored relations for the given object.
     */
    virtual int numberOfRelations(EVENT::LCObject * obj) ;

    /** The relation with the given index.
     */
    virtual EVENT::LCObject * getRelation(EVENT::LCObject * obj, int index) ;

    /** The weight of the relation with the given index.
     */
    virtual float getWeight(EVENT::LCObject * obj, int index) ;

    /** Add a new relation. 
     */
    virtual void addRelation(EVENT::LCObject * from, EVENT::LCObject * to, float weight) ;

    /**If caching is enabled the result of numberOfRelations() is stored
     * for subsequent calls to getRelation() and getWeight().
     */
    virtual void useCaching(bool val) ;

  protected:

    bool _useCaching ;
    mutable pair<LCRelationIter,LCRelationIter> _last ;

}; // class
}; // namespace IMPL
#endif /* ifndef EVENT_LCRELATIONIMPL_H */
