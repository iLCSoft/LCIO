
#include "IMPL/LCCollectionVec.h"

namespace IMPL {

//     std::string _typeName ;
//     int _flag ;


LCCollectionVec::LCCollectionVec( const std::string& type ) :
  _typeName( type ),
  _flag(0) {

}
  
LCCollectionVec::LCCollectionVec( const EVENT::LCCollection& col ) :
  _typeName( col.getTypeName() ),
  _flag( col.getFlag() ) {

  // deep copy of all elements  - requires clone of original elements
  //
  int nElements = col.getNumberOfElements() ;

  for(int i=0; i< nElements ; i++){
    push_back(  col.getElementAt( i )->clone() ) ;
  }

}

LCCollectionVec::~LCCollectionVec() {

  // delete all elements
  LCObjectVec::const_iterator iter = begin() ;
  while( iter != end() ){
    delete *iter++ ;
  }

}


int LCCollectionVec::getNumberOfElements() const{
  return size() ;
}


const std::string & LCCollectionVec::getTypeName() const{
  return _typeName ;
}



const EVENT::LCObject * LCCollectionVec::getElementAt(int index) const{
  return this->operator[](index) ;
}


int LCCollectionVec::getFlag() const{
  return _flag ;
}


void LCCollectionVec::setFlag(int flag){
  _flag  = flag ;
}


}; // namespace IMPL

