#include "IMPL/AccessChecked.h"
#include <iostream>


namespace IMPL {
  
  AccessChecked::AccessChecked() : _readOnly(false) {
    // provide a simple unique id for LCObjects
    static int lCObjectId(0) ;
    _id = lCObjectId++ ;
  }
  

  void AccessChecked::setReadOnly( bool readOnly ) { _readOnly = readOnly ; }
  
  void AccessChecked::checkAccess() throw ( EVENT::ReadOnlyException ){

    if( _readOnly ) throw EVENT::ReadOnlyException("") ;
  }
  void AccessChecked::checkAccess(const char* what) throw ( EVENT::ReadOnlyException ){

    if( _readOnly ){
      //      std::cout << " throwing ReadOnlyException " << what << std::endl ;
      throw EVENT::ReadOnlyException(what) ;
    }
  }
  
  //    bool readOnly ;
  
}
