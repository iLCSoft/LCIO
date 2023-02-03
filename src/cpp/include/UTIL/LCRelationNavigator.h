// -*- C++ -*-
#ifndef EVENT_LCRELATIONNAVIGATORIMPL_H
#define EVENT_LCRELATIONNAVIGATORIMPL_H 1

#include "EVENT/LCObject.h"
#include "EVENT/LCCollection.h"
#include "LCIOSTLTypes.h"
#include <vector>
#include <map>
#include <algorithm>


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
    ~LCRelationNavigator() { /* nop */; }
    
    /**The type of the 'from' objects in this relation.
     */
    const std::string & getFromType() const ;
    
    /**The type of the 'to' objects in this relation.
     */
    const std::string & getToType() const ;
    
    /** All objects that the given from-object is related to.
     *  LCObjects are of type getToType().
     */
    const EVENT::LCObjectVec & getRelatedToObjects(EVENT::LCObject * from) const ;

    /** All from-objects related to the given object ( the  inverse relationship).
     *  LCObjects are of type getFromType().
     */
    const EVENT::LCObjectVec & getRelatedFromObjects(EVENT::LCObject * to) const ;

    /** The weights of the relations returned by  a call to getRelatedToObjects(from). 
     * @see getRelatedToObjects
     */
    const EVENT::FloatVec & getRelatedToWeights(EVENT::LCObject * from) const ;

    /** The weights of the relations returned by  a call to getRelatedFromObjects(to). 
     * @see getRelatedFromObjects
     */
    const EVENT::FloatVec & getRelatedFromWeights(EVENT::LCObject * to) const ;

    /** Object with a highest weight that the given from-object is related to.
     *  LCObject is of type getToType(). CompareF is a comparison function object
     *  with the signature bool(float a, float b) which returns ​true if a is less than b.
     */

    template<typename CompareF>
    size_t getMaxWeightIdx(const std::vector<float>& weights, CompareF&& compare) {
        const auto maxWeightIt = std::max_element(weights.begin(), weights.end(), compare);
        return std::distance(weights.begin(), maxWeightIt);
    }

    template <typename CompareF = std::less<float> >
    const EVENT::LCObject* getRelatedToMaxWeightObject(EVENT::LCObject* from, CompareF&& compare = CompareF() ) const{
        const auto& objects = getRelatedToObjects(from);
        if ( objects.empty() ) return nullptr;

        const auto& weights = getRelatedToWeights(from);
        size_t i = getMaxWeightIdx(weights, compare);
        return objects[i];
    }

    /** From-object related to the given object with a highest weight (the inverse relationship).
     *  LCObject is of type getFromType(). CompareF is a comparison function object
     *  with the signature bool(float a, float b) which returns ​true if a is less than b.
     */
    template <typename CompareF = std::less<float> >
    const EVENT::LCObject* getRelatedFromMaxWeightObject(EVENT::LCObject* to, CompareF&& compare = CompareF() ) const{
        const auto& objects = getRelatedFromObjects(to);
        if ( objects.empty() ) return nullptr;

        const auto& weights = getRelatedFromWeights(to);
        size_t i = getMaxWeightIdx(weights, compare);
        return objects[i];
    }

    /** The highest weight of the relations returned by a call to getRelatedToObjects(from).
     * @see getRelatedToObjects. CompareF is a comparison function object
     *  with the signature bool(float a, float b) which returns ​true if a is less than b.
     */
    template <typename CompareF = std::less<float> >
    float getRelatedToMaxWeight(EVENT::LCObject* from, CompareF&& compare = CompareF() ) const {
        const auto& objects = getRelatedToObjects(from);
        if ( objects.empty() ) return 0.;

        const auto& weights = getRelatedToWeights(from);
        return *std::max_element(weights.begin(), weights.end(), compare);
    }

    /** The highest weight of the relations returned by a call to getRelatedFromObjects(to). 
     * @see getRelatedFromObjects. CompareF is a comparison function object
     *  with the signature bool(float a, float b) which returns ​true if a is less than b.
     */
    template <typename CompareF = std::less<float> >
    float getRelatedFromMaxWeight(EVENT::LCObject* to, CompareF&& compare = CompareF() ) const {
        const auto& objects = getRelatedFromObjects(to);
        if ( objects.empty() ) return 0.;

        const auto& weights = getRelatedFromWeights(to);
        return *std::max_element(weights.begin(), weights.end(), compare);
    }

    /** Adds a relation. If there is already an existing relation between the two given objects
     * the weight (or default weight 1.0) is added to that relationship's weight.
     */
    void addRelation(EVENT::LCObject * from, EVENT::LCObject * to, float weight = 1.0) ;
    
    /** Remove a given relation.
     */
    void removeRelation(EVENT::LCObject * from, EVENT::LCObject * to) ;

    /** Remove a given relation. To reduce the weight of the relationship, call 
     *  addRelation( from, to, weight  ) with  weight<0.
     */
    EVENT::LCCollection * createLCCollection() ;

  protected:

    LCRelationNavigator() ;

    void initialize( const EVENT::LCCollection* col ) ;

    void removeRelation(EVENT::LCObject * from, EVENT::LCObject * to, RelMap& map ) ;
    void addRelation(EVENT::LCObject * from, EVENT::LCObject * to, float weight, RelMap& map) ;

    mutable RelMap _map{} ;
    mutable RelMap _rMap{} ;
    std::string _from ;
    std::string _to ;
    

}; // class
} // namespace EVENT
#endif /* ifndef EVENT_LCRELATIONNAVIGATOR_H */
