// -*- C++ -*-
#ifndef EVENT_LCRELATIONNAVIGATORIMPL_H
#define EVENT_LCRELATIONNAVIGATORIMPL_H 1

#include <vector>
#include <map>
#include "EVENT/LCObject.h"
#include "EVENT/LCCollection.h"
#include "LCIOSTLTypes.h"


namespace UTIL {

  /** The LCRelationNavigator makes repeated lookup of relations more conveneient and efficient.
   * The relations are treated symmetrical, i.e. lookup of relations is equally efficient and 
   * fast for either direction (from-to and to-from)
   *  at the price of a slower (by a factor of ~2) modification speed.
   * 
   * @author gaede 
   * @version $Id: LCRelationNavigator.h,v 1.2 2004-09-06 14:35:51 gaede Exp $
   */

  class LCRelationNavigator {
    
    typedef std::map< EVENT::LCObject* , std::pair< EVENT::LCObjectVec , EVENT::FloatVec > > RelMap ; 

  public: 

                                          
    /** Default constructor
     */
    LCRelationNavigator(const std::string &fromType, const std::string &toType) :
      _from( fromType ),
      _to( toType )  { /* nop */; }
    
    /**Create the navigator object from an existing collection of relations
     */
    LCRelationNavigator( const EVENT::LCCollection* col ) ;
    
    /// Destructor.
    virtual ~LCRelationNavigator() { /* nop */; }
    
    /**The type of the 'from' objects in this relation.
     */
    virtual const std::string & getFromType() const ;
    
    /**The type of the 'to' objects in this relation.
     */
    virtual const std::string & getToType() const ;
    
    /** All objects that the given from-object is related to.
     *  LCObjects are of type getToType().
     */
    virtual const EVENT::LCObjectVec & getRelatedToObjects(EVENT::LCObject * from) const ;

    /** All from-objects related to the given object ( the  inverse relationship).
     *  LCObjects are of type getFromType().
     */
    virtual const EVENT::LCObjectVec & getRelatedFromObjects(EVENT::LCObject * to) const ;

    /** The weights of the relations returned by  a call to getRelatedToObjects(from). 
     * @see getRelatedToObjects
     */
    virtual const EVENT::FloatVec & getRelatedToWeights(EVENT::LCObject * from) const ;

    /** The weights of the relations returned by  a call to getRelatedFromObjects(to). 
     * @see getRelatedFromObjects
     */
    virtual const EVENT::FloatVec & getRelatedFromWeights(EVENT::LCObject * to) const ;

    /** Adds a relation. If there is already an existing relation between the two given objects
     * the weight (or default weight 1.0) is added to that relationship's weight.
     */
    virtual void addRelation(EVENT::LCObject * from, EVENT::LCObject * to, float weight = 1.0) ;
    
    /** Remove a given relation.
     */
    virtual void removeRelation(EVENT::LCObject * from, EVENT::LCObject * to) ;

    /** Remove a given relation. To reduce the weight of the relationship, call 
     *  addRelation( from, to, weight  ) with  weight<0.
     */
    virtual EVENT::LCCollection * createLCCollection() ;

  protected:

    LCRelationNavigator() ;

    virtual void initialize( const EVENT::LCCollection* col ) ;

    void removeRelation(EVENT::LCObject * from, EVENT::LCObject * to, RelMap& map ) ;
    void addRelation(EVENT::LCObject * from, EVENT::LCObject * to, float weight, RelMap& map) ;

    mutable RelMap _map{} ;
    mutable RelMap _rMap{} ;
    std::string _from ;
    std::string _to ;
    

}; // class
} // namespace EVENT
#endif /* ifndef EVENT_LCRELATIONNAVIGATOR_H */
