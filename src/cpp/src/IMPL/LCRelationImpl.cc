#include "IMPL/LCRelationImpl.h"


using namespace EVENT ;

namespace IMPL{



  LCRelationImpl::LCRelationImpl(const std::string& fromType, const std::string& toType ) :  
    _useCaching( true ) , 
    _fromType( fromType ) ,
    _toType( toType ) {
  }

  LCRelationImpl::~LCRelationImpl(){
  }

  
  const std::string &  LCRelationImpl::getFromType() const  { return _fromType ; } 
  
  const std::string &  LCRelationImpl::getToType()  const { return _toType ; } 
  

  //  int numberOfRelations() const ;
  LCObject * LCRelationImpl::getFrom(int index) const {}
  LCObject * LCRelationImpl::getTo(int index) const {}
  float LCRelationImpl::getWeight(int index) const {}
  IntVec LCRelationImpl::getRelationsFrom(LCObject * from) const {}
  IntVec LCRelationImpl::getRelationsTo(LCObject * to) const {}
  void LCRelationImpl::addRelation(LCObject * from, LCObject * to, float weight) {}
  void LCRelationImpl::removeRelation(int index) {}
  
  
  int LCRelationImpl::numberOfRelations() const {
    return  distance( _map.begin() , _map.end() ) ;
  }
  
  
//   EVENT::LCObject * LCRelationImpl::getRelation(int index) {

//     LCRelationIter i = begin() ;    
//     while( index-- ) {
//       i++ ;
//       if( i == end()  )
// 	return 0 ;
//     }
//     return i->first  ;
//   }


//   EVENT::LCObject * LCRelationImpl::getRelation(EVENT::LCObject * obj, int index) {
    
//     LCRelationIter i ;
    
//     if( ! _useCaching) {
      
//       _cached = equal_range( obj ) ;

//     }

//     i = _cached.first ;

//     while( index-- ) {
//       i++ ;
//       if( i == _cached.second )
// 	return 0 ;
//     }

//     return i->second.first ;
//   }

  
//   float LCRelationImpl::getWeight(EVENT::LCObject * obj, int index){
//     LCRelationIter i ;

//     if( ! _useCaching) {

//       _cached = equal_range( obj ) ;

//     }

//     i = _cached.first ;

//     while( index-- ) {
//       i++ ;
//       if( i == _cached.second )
// 	return 0 ;
//     }

//     return i->second.second ;
//   }


//   void LCRelationImpl::addRelation(EVENT::LCObject * from, EVENT::LCObject * to, float weight){

//     insert( std::make_pair( from , std::make_pair( to, weight ) ) ) ;

//   }

  
//   void LCRelationImpl::useCaching(bool val){
//     _useCaching = val  ;
//   }

  void LCRelationImpl::setTypes( const std::string& fromType, const std::string& toType ){
    _fromType= fromType ;
    _toType = toType ;
  }
  
  
} ; // end namespace


