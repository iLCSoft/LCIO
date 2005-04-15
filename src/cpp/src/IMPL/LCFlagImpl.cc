#include "IMPL/LCFlagImpl.h"


using namespace EVENT ;

namespace IMPL {
  
  LCFlagImpl::LCFlagImpl() : _flag(0) {}

  LCFlagImpl::LCFlagImpl(int flag) : _flag( flag ) {} 

  
  bool LCFlagImpl::bitSet(int bit) const { return (_flag & (1<< bit)   ) ? true : false ; } 

  int LCFlagImpl::getFlag() const { return _flag ; } 


  void LCFlagImpl::setBit(int bit) { 

    if( 0<=bit && bit<=31 &&  !bitSet( bit) ){
      
      _flag = _flag | ( 1 << bit ) ; 
      
    }   
  }
  

  void LCFlagImpl::unsetBit(int bit){

    if( 0<=bit && bit<=31 &&  bitSet( bit) ){
      
      _flag = _flag &  ~( 1 << bit ) ; 
      
    }   
  }

} // namespace
