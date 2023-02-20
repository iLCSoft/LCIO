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

   inline constexpr float identity(float x) {
       return x;
   }

   /** Helper function to get maximum element from std::vector
    * based on specific function object of signature float(float) which indicates how valuable elemets are.
    */
   template<typename DecodeF>
   size_t getMaxWeightIdx(const std::vector<float>& weights, DecodeF&& decode = identity) {
       const auto maxWeightIt = std::max_element(weights.begin(), weights.end(),
           [&decode](auto a, auto b){ return decode(a) < decode(b); } );
       return std::distance(weights.begin(), maxWeightIt);
   }


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

    /** Return the object related to the given from-object with the highest (recalculated/decoded in case a decode function object argument is specified) weight.
     *  LCObject is of type getToType(). DecodeF is a function object (anything 
     *  callable with the right signature works) that returns recalculated weight
     *  based on the weight stored in relation collection. It is meant be used to decode
     *  physicaly meaningful weight in case it is stored in any way encoded.
     *  By default DecodeF simply returns identical number stored inside LCRelation collection without any modifications.
     *  The required signature is float(float weight) which returns recalculated weight.
     */
    template <typename DecodeF = decltype(identity) >
    const EVENT::LCObject* getRelatedToMaxWeightObject(EVENT::LCObject* from, DecodeF&& decode = identity ) const{
        const auto& objects = getRelatedToObjects(from);
        if ( objects.empty() ) return nullptr;

        const auto& weights = getRelatedToWeights(from);
        size_t i = getMaxWeightIdx(weights, decode);
        return objects[i];
    }


    /** Return the object related to the given to-object with the highest (recalculated/decoded in case a decode function object argument is specified) weight.
     *  LCObject is of type getFromType(). DecodeF is a function object (anything 
     *  callable with the right signature works) that returns recalculated weight
     *  based on the weight stored in LCRelation collection. It is meant be used to decode
     *  physicaly meaningful weight in case it is stored in any way encoded.
     *  By default DecodeF simply returns identical number stored inside LCRelation collection without any modifications.
     *  The required signature is float(float weight) which returns recalculated weight.
     */
    template <typename DecodeF = decltype(identity) >
    const EVENT::LCObject* getRelatedFromMaxWeightObject(EVENT::LCObject* to, DecodeF&& decode = identity ) const{
        const auto& objects = getRelatedFromObjects(to);
        if ( objects.empty() ) return nullptr;

        const auto& weights = getRelatedFromWeights(to);
        size_t i = getMaxWeightIdx(weights, decode);
        return objects[i];
    }


    /** Return the highest (recalculated/decoded in case a decode function object argument is specified) weight among all objects the given from-object is related to.
     *  DecodeF is a function object (anything 
     *  callable with the right signature works) that returns recalculated weight
     *  based on the weight stored in LCRelation collection. It is meant be used to decode
     *  physicaly meaningful weight in case it is stored in any way encoded.
     *  By default DecodeF simply returns identical number stored inside LCRelation collection without any modifications.
     *  The required signature is float(float weight) which returns recalculated weight.
     */
    template <typename DecodeF = decltype(identity) >
    float getRelatedToMaxWeight(EVENT::LCObject* from, DecodeF&& decode = identity ) const {
        const auto& objects = getRelatedToObjects(from);
        if ( objects.empty() ) return 0.;

        const auto& weights = getRelatedToWeights(from);
        size_t i = getMaxWeightIdx(weights, decode);
        return decode(weights[i]);
    }

    /** Return the highest (recalculated/decoded in case a decode function object argument is specified) weight among all objects the given to-object is related to.
     *  DecodeF is a function object (anything 
     *  callable with the right signature works) that returns recalculated weight
     *  based on the weight stored in LCRelation collection. It is meant be used to decode
     *  physicaly meaningful weight in case it is stored in any way encoded.
     *  By default DecodeF simply returns identical number stored inside LCRelation collection without any modifications.
     *  The required signature is float(float weight) which returns recalculated weight.
     */
    template <typename DecodeF = decltype(identity) >
    float getRelatedFromMaxWeight(EVENT::LCObject* to, DecodeF&& decode = identity ) const {
        const auto& objects = getRelatedFromObjects(to);
        if ( objects.empty() ) return 0.;

        const auto& weights = getRelatedFromWeights(to);
        size_t i = getMaxWeightIdx(weights, decode);
        return decode(weights[i]);
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
