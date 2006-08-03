// -*- C++ -*-

/**Typedefs for std::vectors of basic types used in LCIO
 */

#ifndef LCIO_STD_TYPES
#define LCIO_STD_TYPES 1


#include <string>
#include <vector>

namespace EVENT
{
/**Vector of strings.*/
typedef std::vector< std::string > StringVec ;
/**Vector of floats.*/
typedef std::vector< float  > FloatVec ;
/**Vector of doubles.*/
typedef std::vector< double  > DoubleVec ;
/**Vector of ints.*/
typedef std::vector< int > IntVec ;
/**Vector of shorts.*/
typedef std::vector< short > ShortVec ;

}

#endif
