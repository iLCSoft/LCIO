#include "IMPL/LCRelationSymNavigatorImpl.h"

#include <algorithm>
#include <cassert>
#include "IMPL/LCCollectionVec.h"
#include "IMPL/LCFlagImpl.h"
#include "IMPL/LCWgtRelationImpl.h"
#include "EVENT/LCIO.h"

using namespace EVENT ;

namespace IMPL{


  LCRelationSymNavigatorImpl::LCRelationSymNavigatorImpl( const EVENT::LCCollection* col ) :
  
    LCRelationNavigatorBaseImpl( col->getParameters().getStringVal("FromType") , 
				 col->getParameters().getStringVal("ToType") ) { 

    initialize(col) ; 
  }

  void LCRelationSymNavigatorImpl::initialize( const LCCollection* col ) {


    if( col->getTypeName() != LCIO::LCWGTRELATION ) {
      return ;
    }
    
    int n = col->getNumberOfElements() ;
    
    for(int i=0; i < n; i++){
      
      LCWgtRelation* rel = dynamic_cast<LCWgtRelation*>( col->getElementAt(i) )  ;
      
      addRelation( rel->getFrom() , rel->getTo() , rel->getWeight()  ) ; 
      
    }
  }


  const std::string & LCRelationSymNavigatorImpl::getFromType() const { return _from ; }
  const std::string & LCRelationSymNavigatorImpl::getToType() const { return _to ; }
  
  const EVENT::LCObjectVec& 
  LCRelationSymNavigatorImpl::getRelatedObjects(EVENT::LCObject * from) const{

    return _map[ from ].first ;
  }

  const EVENT::LCObjectVec& 
  LCRelationSymNavigatorImpl::getRelatedFromObjects(EVENT::LCObject * to) const{

    return _rMap[ to ].first ;
  }

  const  EVENT::FloatVec & LCRelationSymNavigatorImpl::getWeights(EVENT::LCObject * from) const {

    return _map[ from ].second ;
  }

  const  EVENT::FloatVec & LCRelationSymNavigatorImpl::getFromWeights(EVENT::LCObject * to) const {

    return _rMap[ to ].second ;
  }

  void LCRelationSymNavigatorImpl::addRelation(EVENT::LCObject * from, 
					       EVENT::LCObject * to, 
					       float weight) {
    addRelation( from , to , weight ,  _map ) ;
    addRelation( to , from, weight ,  _rMap ) ;
  }
  
  void LCRelationSymNavigatorImpl::addRelation(EVENT::LCObject * from, 
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
  


  void LCRelationSymNavigatorImpl::removeRelation(EVENT::LCObject * from, EVENT::LCObject * to) {
    removeRelation( from, to, _map ) ;
    removeRelation( to, from, _rMap ) ;
  }
  
  
  
  void LCRelationSymNavigatorImpl::removeRelation(EVENT::LCObject * from, EVENT::LCObject * to, RelMap& map ){

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

  EVENT::LCCollection * LCRelationSymNavigatorImpl::createLCCollection() {
    
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
