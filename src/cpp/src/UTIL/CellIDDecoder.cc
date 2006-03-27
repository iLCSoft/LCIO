#include "UTIL/CellIDDecoder.h"

namespace UTIL{

  export template <class T>
  std::string* CellIDDecoder<T>::_defaultEncoding 
  = new std::string("byte0:8,byte1:8,byte2:8,byte3:8,byte4:8,byte5:8,byte6:8,byte7:8") ;
  
  export template <>
  std::string* CellIDDecoder<SimTrackerHit>::_defaultEncoding 
  = new std::string("byte0:8,byte1:8,byte2:8,byte3:8") ;
  
  
}
