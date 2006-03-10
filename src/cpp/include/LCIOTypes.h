// -*- C++ -*-

/**Typedefs used in LCIO.
 */


#ifndef LCIO_TYPES
#define LCIO_TYPES 1

namespace EVENT
{

  /** 64 bit signed integer,e.g.to be used for timestamps **/
  typedef long long long64 ;

  /** 64 bit unsigned integer,e.g.to be used for masks **/
  typedef unsigned long long ulong64 ;
  
  //fg: this might have to be modified for other architectures, e.g. 64bit systems, or windows ?
  
  
}

#include "LCIOSTLTypes.h"

#endif
