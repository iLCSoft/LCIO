#include "IMPL/LCRelationNavigator.h"

#include <algorithm>
#include <cassert>
#include "IMPL/LCCollectionVec.h"
#include "IMPL/LCFlagImpl.h"
#include "IMPL/LCWgtRelationImpl.h"
#include "EVENT/LCIO.h"

using namespace EVENT ;

namespace IMPL{


  LCRelationNavigator::LCRelationNavigator( const EVENT::LCCollection* col ) :
  
    _from( col->getParameters().getStringVal("FromType") ) ,
    _to( col->getParameters().getStringVal("ToType") ) { 
    
    initialize(col) ; 
  }

  void LCRelationNavigator::initialize( const LCCollection* col ) {


    if( col->getTypeName() != LCIO::LCWGTRELATION ) {
      return ;
    }
    
    int n = col->getNumberOfElements() ;
    
    for(int i=0; i < n; i++){
      
      LCWgtRelation* rel = dynamic_cast<LCWgtRelation*>( col->getElementAt(i) )  ;
      
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
    
    LCCollectionVec* col = new LCCollectionVec( LCIO::LCWGTRELATION ) ;
    
    
    col->parameters().setValue( "FromType" , getFromType() ) ;
    col->parameters().setValue( "ToType" , getToType() ) ;


    bool storeWeights = false ;
    for(RelMap::iterator iter = _map.begin() ;
	iter != _map.end() ; iter++ ) {
      
      LCObject* from = iter->first ;
      LCObjectVec& vTo = iter->second.first ;
      FloatVec & vWgt  = iter->second.second ;
      
      unsigned int n =  vTo.size() ;
      assert( n == vWgt.size() ) ;

      for( unsigned int i=0 ; i<n ; i++ ){

	col->addElement( new LCWgtRelationImpl( from , vTo[i] , vWgt[i] )    ) ;
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
}; 
