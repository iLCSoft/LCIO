
#include "IMPL/LCGenericObjectImpl.h"

using namespace EVENT ;

namespace IMPL {
  

  std::string  LCGenericObjectImpl::_typeName = "LCGenericObject" ;
  std::string  LCGenericObjectImpl::_dataDescription = "" ;



  LCGenericObjectImpl::LCGenericObjectImpl():
  _isFixedSize( false ) {
    
  }


  LCGenericObjectImpl::LCGenericObjectImpl(int nInt, int nFloat, int nDouble):
    _isFixedSize( true ) {  // FIXME - fixed size is a collection property !!!
    _intVec.resize( nInt ) ;
    _floatVec.resize( nFloat ) ;
    _doubleVec.resize( nDouble ) ;
  }

  int LCGenericObjectImpl::getNInt() const{
    return _intVec.size() ;
  }
  int LCGenericObjectImpl::getNFloat() const{
    return _floatVec.size() ;
  }
  
  int LCGenericObjectImpl::getNDouble() const{
    return _doubleVec.size() ;
  }
  
  int LCGenericObjectImpl::getIntVal(int index) const{
    return _intVec[ index ] ;
  }

  float LCGenericObjectImpl::getFloatVal(int index) const{
    return _floatVec[ index ] ;
  }

  double LCGenericObjectImpl::getDoubleVal(int index) const{
    return _doubleVec[ index ] ;
  }
  
  bool LCGenericObjectImpl::isFixedSize() const{
    return _isFixedSize ;
  }
  
  void LCGenericObjectImpl::setIntVal(unsigned index, int value) {
    if( _intVec.size() <= index && !_isFixedSize ) 
      _intVec.resize( index+1 ) ;
    _intVec[ index ] = value ;
  }
  
  void LCGenericObjectImpl::setFloatVal(unsigned index, float value){
    if( _floatVec.size() <= index && !_isFixedSize ) 
      _floatVec.resize( index+1 ) ;
    _floatVec[ index ] = value ;
  }
  
  void LCGenericObjectImpl::setDoubleVal(unsigned index, double value){
    if( _doubleVec.size() <= index && !_isFixedSize ) 
      _doubleVec.resize( index+1 ) ;
    _doubleVec[ index ] = value ;
  }

  const std::string  LCGenericObjectImpl::getTypeName() const {
    return _typeName ;
  }

  const std::string  LCGenericObjectImpl::getDataDescription() const {
    return _dataDescription ;
  }

}
