#include "IMPL/LCRelationImpl.h"


namespace IMPL{



  LCRelationImpl::LCRelationImpl() :  _useCaching(true) {
  }

  LCRelationImpl::~LCRelationImpl(){
  }

  
  int LCRelationImpl::numberOfRelations(EVENT::LCObject * obj){
    
    _last = equal_range( obj ) ;
    
    int n = 0 ;
    // is there a smarter way to get the number of relations ?
    for( LCRelationIter i = _last.first ; i != _last.second ; i++ ){
      n++ ;
    } 
    return  n ;
  }
  
  
  EVENT::LCObject * LCRelationImpl::getRelation(EVENT::LCObject * obj, int index) {
    
    LCRelationIter i ;
    
    if( ! _useCaching) {
      
      _last = equal_range( obj ) ;

    }

    i = _last.first ;

    while( index-- ) {
      i++ ;
      if( i == _last.second )
	return 0 ;
    }

    return i->second.first ;
  }

  
  float LCRelationImpl::getWeight(EVENT::LCObject * obj, int index){
    LCRelationIter i ;

    if( ! _useCaching) {

      _last = equal_range( obj ) ;

    }

    i = _last.first ;

    while( index-- ) {
      i++ ;
      if( i == _last.second )
	return 0 ;
    }

    return i->second.second ;
  }


  void LCRelationImpl::addRelation(EVENT::LCObject * from, EVENT::LCObject * to, float weight){

    insert( make_pair( from , make_pair( to, weight ) ) ) ;

  }

  
  void LCRelationImpl::useCaching(bool val){
    _useCaching = val  ;
  }

  
} ; // end namespace


