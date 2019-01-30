#include "IMPL/AccessChecked.h"
#include <iostream>


namespace IMPL {
  
  std::atomic_int AccessChecked::_lCObjectId(0);
  
  AccessChecked::AccessChecked() : _readOnly(false) {
    // provide a simple unique id for LCObjects
    _id = _lCObjectId++ ;
  }
  
  void AccessChecked::setReadOnly( bool readOnly ) { 
    _readOnly = readOnly ; 
  }
  
  void AccessChecked::checkAccess() {
    if( _readOnly ) { 
      throw EVENT::ReadOnlyException("") ;
    }
  }
  
  void AccessChecked::checkAccess(const char* what) {
    if( _readOnly ) {
      throw EVENT::ReadOnlyException(what) ;
    }
  }
  
}
