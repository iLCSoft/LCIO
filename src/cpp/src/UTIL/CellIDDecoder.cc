#include "UTIL/CellIDDecoder.h"

namespace UTIL{

  template <>
  std::string* CellIDDecoder<SimTrackerHit>::_defaultEncoding
  = new std::string("byte0:8,byte1:8,byte2:8,byte3:8") ;
  
}
