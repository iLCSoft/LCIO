#include "IMPL/AccessChecked.h"
#include <iostream>


namespace IMPL {
  
  AccessChecked::AccessChecked() : readOnly(false) {
  }
  
  void AccessChecked::setReadOnly( bool readOnly ) { this->readOnly = readOnly ; }
  
  void AccessChecked::checkAccess() throw ( EVENT::ReadOnlyException ){

    if( readOnly ) throw EVENT::ReadOnlyException("") ;
  }
  void AccessChecked::checkAccess(const char* what) throw ( EVENT::ReadOnlyException ){

    if( readOnly ){
      //      std::cout << " throwing ReadOnlyException " << what << std::endl ;
      throw EVENT::ReadOnlyException(what) ;
    }
  }
  
  //    bool readOnly ;
  
};
