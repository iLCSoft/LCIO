// -*- C++ -*-
#ifndef EVENT_LCRELATIONNAVIGATORBASEIMPL_H
#define EVENT_LCRELATIONNAVIGATORBASEIMPL_H 1

#include "EVENT/LCRelationNavigator.h"

#include <vector>
#include <map>
#include "EVENT/LCObject.h"

namespace IMPL {

/** Base implementation of the a relationship navigator object.
 *
 * 
 * @author gaede 
 * @version $Id: LCRelationNavigatorBaseImpl.h,v 1.1 2004-07-13 11:54:27 gaede Exp $
 */
  class LCRelationNavigatorBaseImpl : public EVENT::LCRelationNavigator {
    
    typedef std::map< EVENT::LCObject* , std::pair< EVENT::LCObjectVec , EVENT::FloatVec > > RelMap ;
    
    
  public: 
    LCRelationNavigatorBaseImpl() { /* nop */; }
    
    /// Destructor.
    virtual ~LCRelationNavigatorBaseImpl() { /* nop */; }
    
    /**The type of the 'from' objects in this relation.
     */
    virtual const std::string & getFromType() const ;
    
    /**The type of the 'to' objects in this relation.
     */
    virtual const std::string & getToType() const ;
    
    /** The objects related to this object.
     */
    virtual const EVENT::LCObjectVec & getRelatedObjects(EVENT::LCObject * from) const ;
    
    /** The weight of the given relation. 
     */
    virtual const  EVENT::FloatVec & getWeights(EVENT::LCObject * from) const ;
    
    /** Adds a relation.
     */
    virtual void addRelation(EVENT::LCObject * from, EVENT::LCObject * to, float weight = 1.0) ;
    
    /** Remove a given relation.
     */
    virtual void removeRelation(EVENT::LCObject * from, EVENT::LCObject * to) ;

    /** Creates an LCCollection of LCWgtRelation objects.
     */
    virtual EVENT::LCCollection * createLCCollection() ;

  protected:

    /** Make this class abstract
     */
    virtual void initialize( const EVENT::LCCollection* col ) = 0 ;

    mutable RelMap _map ;
    std::string _from ;
    std::string _to ;
    

}; // class
} // namespace EVENT
#endif /* ifndef EVENT_LCRELATIONNAVIGATOR_H */
