// -*- C++ -*-
#ifndef EVENT_LCRELATIONSYMNAVIGATORIMPL_H
#define EVENT_LCRELATIONSYMNAVIGATORIMPL_H 1

#include "IMPL/LCRelationNavigatorImpl.h"


namespace IMPL {

/**  Implementation of the a relationship navigator object
 * that treats the relationships symmetrical wrt. the from and 
 * to-object.
 * 
 * @author gaede 
 * @version $Id: LCRelationSymNavigatorImpl.h,v 1.1 2004-07-22 13:56:36 gaede Exp $
 */
  class LCRelationSymNavigatorImpl : public LCRelationNavigatorBaseImpl {
    
  public: 

    /** Default constructor
     */
    LCRelationSymNavigatorImpl(const std::string &fromType, const std::string &toType) :
      LCRelationNavigatorBaseImpl( fromType, toType ) { /* nop */; }
    
    /**Create the navigator object from an existing collection of relations
     */
    LCRelationSymNavigatorImpl( const EVENT::LCCollection* col ) ;
    
    /// Destructor.
    virtual ~LCRelationSymNavigatorImpl() { /* nop */; }
    
    /**The type of the 'from' objects in this relation.
     */
    virtual const std::string & getFromType() const ;
    
    /**The type of the 'to' objects in this relation.
     */
    virtual const std::string & getToType() const ;
    
    /** The objects related to this object.
     */
    virtual const EVENT::LCObjectVec & getRelatedObjects(EVENT::LCObject * from) const ;

    /** All from-objects that have a relationship to this object.
     */
    virtual const EVENT::LCObjectVec & getRelatedFromObjects(EVENT::LCObject * to) const ;
    
    /** The weights of the relations returned by getRelatedObjects. 
     */
    virtual const  EVENT::FloatVec & getWeights(EVENT::LCObject * from) const ;

    /** The weight of the  relations returned by getRelatedFromObjects. 
     */
    virtual const  EVENT::FloatVec & getFromWeights(EVENT::LCObject * to) const ;
    
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

    LCRelationSymNavigatorImpl() ;

    virtual void initialize( const EVENT::LCCollection* col ) ;

    void removeRelation(EVENT::LCObject * from, EVENT::LCObject * to, RelMap& map ) ;
    void addRelation(EVENT::LCObject * from, EVENT::LCObject * to, float weight, RelMap& map) ;

    mutable RelMap _map ;
    mutable RelMap _rMap ;
    std::string _from ;
    std::string _to ;
    

}; // class
} // namespace EVENT
#endif /* ifndef EVENT_LCRELATIONNAVIGATOR_H */
