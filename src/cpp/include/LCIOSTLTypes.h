// -*- C++ -*-

/**Typedefs for std::vectors of basic types used in LCIO
 */

#ifndef LCIO_STD_TYPES
#define LCIO_STD_TYPES 1


#include <string>
#include <vector>

namespace EVENT
{
typedef std::vector< std::string > StringVec ;
typedef std::vector< float  > FloatVec ;
typedef std::vector< double  > DoubleVec ;
typedef std::vector< int > IntVec ;
typedef std::vector< short > ShortVec ;

}

#endif
