// -*- C++ -*-
#ifndef EVENT_LCRELATIONIMPL_H
#define EVENT_LCRELATIONIMPL_H 1

#include <vector>
#include <map>

#include "EVENT/LCRelation.h"
#include "EVENT/LCObject.h"
#include "AccessChecked.h"



namespace IMPL {
  
  typedef std::multimap< EVENT::LCObject*, std::pair< EVENT::LCObject*, float > > LCWgtMultiMap ;
  typedef LCWgtMultiMap::iterator LCRelationIter ;

  /** Implementation of the LCRelation.
   * 
   * @author gaede 
   * @version Apr 15, 2004
   */
  class LCRelationImpl : public EVENT::LCRelation , public LCWgtMultiMap
    , public AccessChecked {
    
    public:

    LCRelationImpl() { /* no default c'tor */ }

    /** Default constructor for users takes types of the related objects.
     */
    LCRelationImpl(const std::string& fromType, const std::string& toType ) ;
    
    /** Destructor.
     */    
    virtual ~LCRelationImpl() ;
    

    /**The type of the 'from' objects in this relation.
     */
    virtual const std::string & getFromType() const ;

    /**The type of the 'to' objects in this relation.
     */
    virtual const std::string & getToType() const ;

    /** The number of stored relations for the given object.
     */
    virtual int numberOfRelations(EVENT::LCObject * obj=0) ;

    /** The 'from object' of the given relation. Needed to analyze the relations without 
     * knowledge of the participating objects.
     */
    virtual EVENT::LCObject* getRelation( int index ) ;

    /** The relation with the given index for obj.
     */
    virtual EVENT::LCObject * getRelation(EVENT::LCObject * obj, int index=0) ;

    /** The weight of the relation with the given index.
     */
    virtual float getWeight(EVENT::LCObject * obj, int index=0) ;

    /** Add a new relation. 
     */
    virtual void addRelation(EVENT::LCObject * from, EVENT::LCObject * to, float weight=1.0) ;

    /**If caching is enabled the result of numberOfRelations() is stored
     * for subsequent calls to getRelation() and getWeight().
     */
    virtual void useCaching(bool val) ;


    virtual void setTypes( const std::string& fromType, const std::string& toType ) ;

  protected:
 
    bool _useCaching ;
    mutable std::pair<LCRelationIter,LCRelationIter> _cached ;
    std::string _fromType ; 
    std::string _toType ;



}; // class
}; // namespace IMPL
#endif /* ifndef EVENT_LCRELATIONIMPL_H */
