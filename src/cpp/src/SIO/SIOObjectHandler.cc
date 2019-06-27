#include "SIO/SIOObjectHandler.h"

namespace SIO {
  
  SIOObjectHandler::SIOObjectHandler( const std::string &colType ) :
    _collectionType( colType ) {
    /* nop */
  }
  
  //----------------------------------------------------------------------------
  
  void SIOObjectHandler::setFlag( unsigned int flag ) {
    _flag = flag ;
  }
  
}
