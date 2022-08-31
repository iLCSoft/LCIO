#include "UTIL/LCRelationNavigator.h"

#include <algorithm>
#include <cassert>
#include "IMPL/LCCollectionVec.h"
#include "IMPL/LCFlagImpl.h"
#include "IMPL/LCRelationImpl.h"
#include "EVENT/LCIO.h"

#define RELATIONFROMTYPESTR "FromType"
#define RELATIONTOTYPESTR "ToType"

using namespace EVENT ;
using namespace IMPL ;

namespace UTIL{


  LCRelationNavigator::LCRelationNavigator( const EVENT::LCCollection* col ) :
  
    _from( col->getParameters().getStringVal( RELATIONFROMTYPESTR ) ) ,
    _to( col->getParameters().getStringVal( RELATIONTOTYPESTR ) ) { 
    
    initialize(col) ; 
  }

  void LCRelationNavigator::initialize( const LCCollection* col ) {


    if( col->getTypeName() != LCIO::LCRELATION ) {
      return ;
    }
    
    int n = col->getNumberOfElements() ;
    
    for(int i=0; i < n; i++){
      
      LCRelation* rel = dynamic_cast<LCRelation*>( col->getElementAt(i) )  ;
      
      addRelation( rel->getFrom() , rel->getTo() , rel->getWeight()  ) ; 
      
    }
  }


  const std::string & LCRelationNavigator::getFromType() const { return _from ; }
  const std::string & LCRelationNavigator::getToType() const { return _to ; }
  
  const EVENT::LCObjectVec& 
  LCRelationNavigator::getRelatedToObjects(EVENT::LCObject * from) const{

    return _map[ from ].first ;
  }

  const EVENT::LCObjectVec& 
  LCRelationNavigator::getRelatedFromObjects(EVENT::LCObject * to) const{

    return _rMap[ to ].first ;
  }

  const  EVENT::FloatVec & LCRelationNavigator::getRelatedToWeights(EVENT::LCObject * from) const {

    return _map[ from ].second ;
  }

  const  EVENT::FloatVec & LCRelationNavigator::getRelatedFromWeights(EVENT::LCObject * to) const {

    return _rMap[ to ].second ;
  }

  const EVENT::LCObject* LCRelationNavigator::getRelatedToMaxWeightObject(EVENT::LCObject* from, const std::string& weightType) const {
      const auto& objects = getRelatedToObjects(from);
      const auto& weights = getRelatedToWeights(from);
      if ( objects.empty() ) return nullptr;
      auto maxWeightIt = std::max_element(weights.begin(), weights.end(), [](float a, float b){return a < b;});
      if (weightType == "track") maxWeightIt = std::max_element(weights.begin(), weights.end(), [](float a, float b){return (int(a)%10000)/1000. < (int(b)%10000)/1000.;});
      else if (weightType == "cluster") maxWeightIt = std::max_element(weights.begin(), weights.end(), [](float a, float b){return (int(a)/10000)/1000. < (int(b)/10000)/1000. ;});

      int i = std::distance(weights.begin(), maxWeightIt);
      return objects[i];    
  }

  const EVENT::LCObject* LCRelationNavigator::getRelatedFromMaxWeightObject(EVENT::LCObject* to, const std::string& weightType) const {
      const auto& objects = getRelatedToObjects(to);
      const auto& weights = getRelatedToWeights(to);
      if ( objects.empty() ) return nullptr;

      auto maxWeightIt = std::max_element(weights.begin(), weights.end(), [](float a, float b){return a < b;});
      if (weightType == "track") maxWeightIt = std::max_element(weights.begin(), weights.end(), [](float a, float b){return (int(a)%10000)/1000. < (int(b)%10000)/1000.;});
      else if (weightType == "cluster") maxWeightIt = std::max_element(weights.begin(), weights.end(), [](float a, float b){return (int(a)/10000)/1000. < (int(b)/10000)/1000. ;});

      int i = std::distance(weights.begin(), maxWeightIt);
      return objects[i];    
  }

  float LCRelationNavigator::getRelatedToMaxWeight(EVENT::LCObject* from, const std::string& weightType) const {
      const auto& objects = getRelatedToObjects(from);
      const auto& weights = getRelatedToWeights(from);
      if ( objects.empty() ) return 0.;

      float maxWeight = 0.;
      if (weightType == "track") maxWeight = *std::max_element(weights.begin(), weights.end(), [](float a, float b){return (int(a)%10000)/1000. < (int(b)%10000)/1000.;});
      else if (weightType == "cluster") maxWeight = *std::max_element(weights.begin(), weights.end(), [](float a, float b){return (int(a)/10000)/1000. < (int(b)/10000)/1000. ;});
      else maxWeight = *std::max_element(weights.begin(), weights.end(), [](float a, float b){return a < b ;});
      return maxWeight;
  }

  float LCRelationNavigator::getRelatedFromMaxWeight(EVENT::LCObject* to, const std::string& weightType) const {
      const auto& objects = getRelatedToObjects(to);
      const auto& weights = getRelatedToWeights(to);
      if ( objects.empty() ) return 0.;

      float maxWeight = 0.;
      if (weightType == "track") maxWeight = *std::max_element(weights.begin(), weights.end(), [](float a, float b){return (int(a)%10000)/1000. < (int(b)%10000)/1000.;});
      else if (weightType == "cluster") maxWeight = *std::max_element(weights.begin(), weights.end(), [](float a, float b){return (int(a)/10000)/1000. < (int(b)/10000)/1000. ;});
      else maxWeight = *std::max_element(weights.begin(), weights.end(), [](float a, float b){return a < b ;});
      return maxWeight;
  }

  void LCRelationNavigator::addRelation(EVENT::LCObject * from, 
					       EVENT::LCObject * to, 
					       float weight) {
    addRelation( from , to , weight ,  _map ) ;
    addRelation( to , from, weight ,  _rMap ) ;
  }
  
  void LCRelationNavigator::addRelation(EVENT::LCObject * from, 
					       EVENT::LCObject * to, 
					       float weight,
					       RelMap& map) {
    
    LCObjectVec& vTo = map[ from ].first ;
    FloatVec & vWgt = map[ from ].second ;
    
    bool isNewObject = true ;
    int n = vTo.size() ;
    for(int i=0; i<n ; i++){
      if( to == vTo[i] ){
	vWgt[i] += weight ;
	isNewObject = false ;
	break ;
      }
    }
    if( isNewObject ){
      vTo.push_back( to ) ;
      vWgt.push_back( weight) ;      
    }
  }
  


  void LCRelationNavigator::removeRelation(EVENT::LCObject * from, EVENT::LCObject * to) {
    removeRelation( from, to, _map ) ;
    removeRelation( to, from, _rMap ) ;
  }
  
  
  
  void LCRelationNavigator::removeRelation(EVENT::LCObject * from, EVENT::LCObject * to, RelMap& map ){

    RelMap::iterator iter =  map.find( from ) ;
    if( iter != map.end() ) {
      
      
      LCObjectVec& vTo = iter->second.first ;
      FloatVec & vWgt  = iter->second.second ;
      
      // doesn't work as we need to remove/erase the corresponding weight as well ...
      // vTo.erase( remove(vTo.begin(),vTo.end(), to ), vTo.end()).

      LCObjectVec::iterator iTo = find( vTo.begin(), vTo.end() , to ) ;

      if( iTo != vTo.end() ){

	FloatVec::iterator iWgt =  vWgt.begin() ;

	advance( iWgt , distance( vTo.begin() , iTo )  ) ;

	vTo.erase( iTo ) ;
	vWgt.erase( iWgt ) ;

	if( vTo.empty() ) {   // remove empty relation vectors
	  assert( vWgt.empty() ) ;
	  map.erase( iter ) ;
	}
      }
      
    }
  }

  EVENT::LCCollection * LCRelationNavigator::createLCCollection() {
    
    LCCollectionVec* col = new LCCollectionVec( LCIO::LCRELATION ) ;
    
    
    col->parameters().setValue( RELATIONFROMTYPESTR , getFromType() ) ;
    col->parameters().setValue( RELATIONTOTYPESTR , getToType() ) ;


    bool storeWeights = false ;
    for(RelMap::iterator iter = _map.begin() ;
	iter != _map.end() ; iter++ ) {
      
      LCObject* from = iter->first ;
      LCObjectVec& vTo = iter->second.first ;
      FloatVec & vWgt  = iter->second.second ;
      
      unsigned int n =  vTo.size() ;
      assert( n == vWgt.size() ) ;

      for( unsigned int i=0 ; i<n ; i++ ){

	col->addElement( new LCRelationImpl( from , vTo[i] , vWgt[i] )    ) ;
	if( vWgt[i] != 1.0f ) storeWeights = true ;
      }
    }
    if( storeWeights ) {
      LCFlagImpl flag(0) ; 
      flag.setBit( LCIO::LCREL_WEIGHTED ) ;
      col->setFlag( flag.getFlag() ) ;
    }
      

    return col ;
  }
} 
