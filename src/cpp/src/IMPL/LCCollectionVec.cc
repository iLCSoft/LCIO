
#include "IMPL/LCCollectionVec.h"
#include "EVENT/LCIO.h"
#include <iostream>

// #include "UTIL/LCTOOLS.h"

using namespace EVENT ;

namespace IMPL {

//     std::string _typeName ;
//     int _flag ;


LCCollectionVec::LCCollectionVec( const std::string& type ) :
  _typeName( type ),
  _flag(0){ 
  //,
  // _access(LCIO::UPDATE ) {

}
  
// LCCollectionVec::LCCollectionVec( const EVENT::LCCollection& col ) :
//   _typeName( col.getTypeName() ),
//   _flag( col.getFlag() ),
//   _access( col._access ) {

//   // deep copy of all elements  - requires clone of original elements
//   //
//   int nElements = col.getNumberOfElements() ;

//   for(int i=0; i< nElements ; i++){
//     push_back(  col.getElementAt( i )->clone() ) ;
//   }

// }

  // overwrite the default implementation
  // set flag in all elements
  void LCCollectionVec::setReadOnly(bool readOnly){

    AccessChecked::setReadOnly(readOnly ) ;

    LCObjectVec::iterator iter = begin() ;
    while( iter != end() ){
      AccessChecked* element = dynamic_cast<AccessChecked*>(*iter++) ;
      if(element){
	element->setReadOnly( readOnly ) ;
      }
    }
  }

LCCollectionVec::~LCCollectionVec() {

  if( ! isSubset() ){
    // delete all elements
    LCObjectVec::const_iterator iter = begin() ;
    //    std::cout << "deleting collection " 
    //  	    << std::endl ;
    //    UTIL::LCTOOLS::printParameters( parameters() )  ;
    
    while( iter != end() ){
      delete *iter++ ;
    }
  }
}


int LCCollectionVec::getNumberOfElements() const{
  return size() ;
}


const std::string & LCCollectionVec::getTypeName() const{
  return _typeName ;
}



EVENT::LCObject * LCCollectionVec::getElementAt(int index) const{
  return this->operator[](index) ;
}

bool LCCollectionVec::isTransient() const { 
  return (_flag & (1<<BITTransient) ) ; 
}

void LCCollectionVec::setTransient(bool val) {
  if(val) _flag |= (1<<BITTransient) ;
  else _flag &= ~(1<<BITTransient) ;
}


bool LCCollectionVec::isDefault() const { 
  return (_flag & (1<<BITDefault) ) ; 
}

void LCCollectionVec::setDefault(bool val) {
  if(val) _flag |= (1<<BITDefault) ;
  else _flag &= ~(1<<BITDefault) ;
}

bool LCCollectionVec::isSubset() const { 
  return (_flag & (1<<BITSubset) ) ; 
}

void LCCollectionVec::setSubset(bool val) {
  if(val) _flag |= (1<<BITSubset) ;
  else _flag &= ~(1<<BITSubset) ;
}


int LCCollectionVec::getFlag() const{
  return _flag ;
}


void LCCollectionVec::setFlag(int flag){
  _flag  = flag ;
}


  void LCCollectionVec::addElement(EVENT::LCObject * obj) throw (ReadOnlyException){
    
    //    if(_access != LCIO::UPDATE )
    //  throw ReadOnlyException("LCCollectionVec::addElement:  event is read only") ;
    checkAccess("LCCollectionVec::addElement") ;
    this->push_back( obj ) ; 
  }

  void LCCollectionVec::removeElementAt(int i) throw (EVENT::ReadOnlyException){
    
    //    if(_access != LCIO::UPDATE )
    //  throw ReadOnlyException("LCCollectionVec::addElement:  event is read only") ;
    checkAccess("LCCollectionVec::removeElementAt") ;
    this->erase( begin() + i ) ;

  }

} // namespace IMPL

