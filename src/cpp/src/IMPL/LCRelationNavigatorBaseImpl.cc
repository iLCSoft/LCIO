#include "IMPL/LCRelationNavigatorBaseImpl.h"

#include <algorithm>
#include "IMPL/LCCollectionVec.h"
#include "IMPL/LCFlagImpl.h"
#include "IMPL/LCWgtRelationImpl.h"
#include "EVENT/LCIO.h"

using namespace EVENT ;

namespace IMPL{

  const std::string & LCRelationNavigatorBaseImpl::getFromType() const { return _from ; }
  const std::string & LCRelationNavigatorBaseImpl::getToType() const { return _to ; }
  
  const EVENT::LCObjectVec& 
  LCRelationNavigatorBaseImpl::getRelatedObjects(EVENT::LCObject * from) const{

    return _map[ from ].first ;
  }

  const  EVENT::FloatVec & LCRelationNavigatorBaseImpl::getWeights(EVENT::LCObject * from) const {

    return _map[ from ].second ;
  }

  void LCRelationNavigatorBaseImpl::addRelation(EVENT::LCObject * from, 
						EVENT::LCObject * to, 
						float weight = 1.0) {

    LCObjectVec& vTo = _map[ from ].first ;
    FloatVec & vWgt = _map[ from ].second ;

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
  
  void LCRelationNavigatorBaseImpl::removeRelation(EVENT::LCObject * from, EVENT::LCObject * to) {

    RelMap::iterator iter =  _map.find( from ) ;
    if( iter != _map.end() ) {
      
      
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
	  _map.erase( iter ) ;
	}
      }
      
    }
  }

  EVENT::LCCollection * LCRelationNavigatorBaseImpl::createLCCollection() {
    
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
