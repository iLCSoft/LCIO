// ----------------------------------------------------------------------------
// CVS $Id: SIO_functions.cc,v 1.9 2008-05-28 14:02:09 engels Exp $
// ----------------------------------------------------------------------------
// => Function package for SIO                            
// ----------------------------------------------------------------------------
//
// General Description:
//
// SIO_functions implements the basic I/O functions of SIO along with a number
// of general purpose utilities.
//
// ----------------------------------------------------------------------------

#ifdef _MSC_VER
#   pragma warning(disable:4786)        // >255 characters in debug information
#endif

#include <iostream>
#include <cstdlib>
#include <cstring>

#include "SIO_functions.h"

// ----------------------------------------------------------------------------
// Deal with 'endian-ness'.  Try to base this on the processor type (because
// the operating system doesn't guarantee endian-ness ... Linux runs happily
// on both x86 CPUs (little endian) and PPC CPUs (big endian)).
//
// Branch on flag provided by compiler:
//
// OS            CPU           Macro         Provided by   Endian-ness
// ------------  ------------  ------------  ------------  -----------
// AIX           PPC(?)        _AIX          GNU compiler  Big
// OSF1          Alpha         __alpha__     GNU compiler  Little
// Linux         x86           __i386__      GNU compiler  Little
// Linux         Opteron       _LP64         GNU compiler  Little
// Linux         itanium       _LP64         GNU compiler  Little
// SunOS         Sparc         __sparc__     GNU compiler  Big
// Windows/NT    Alpha         _M_ALPHA      VC  compiler  Little
// Windows/NT    x86           _M_IX86       VC  compiler  Little
// Windows/NT    MIPS          _M_MRX000     VC  compiler  ?
// Windows/NT    PPC           _M_PPC        VC  compiler  Big
// ----------------------------------------------------------------------------
#if defined(__alpha__) ||   defined(__i386__)  ||   defined(_M_ALPHA)  ||   defined(_M_IX86) || defined(_LP64) || defined(__LITTLE_ENDIAN__)
#define SIO_LITTLE_ENDIAN
#endif

#if defined(_AIX)      ||   defined(__sparc__) ||   defined(_M_PPC) || ( defined(__APPLE_CC__) && !defined(__LITTLE_ENDIAN__)  ) 
#define SIO_BIG_ENDIAN
#endif

namespace sio {
  
  // ****************************************************************************
  // Primitive copying functions.
  //
  // This is where the endian-ness problem is resolved.  Note that there are two
  // implementations of SIO_functions::copy.  Only (at most) one version gets
  // passed to the compiler by the preprocesor.  In the case of a new target CPU
  // whose endian-ness cannot determined by the preprocessor directives above,
  // -no- version is passed through to the compiler.  The result is a compilation
  // failure (i.e. you can't compile the SIO package on a particular CPU if the
  // CPU's endian-ness cannot be determined by the SIO preprocessor directives).
  //
  // In SIO all datasets have the same endian-ness.  The choice between a little-
  // or a big-endian dataset representation should be driven by the type of CPU
  // which will do most of the work (swapping the endian-ness on read or
  // write is somewhat inefficient).  In a ditributed multi-platform environment
  // that's a tough call, so the chice is really somewhat arbitrary.  SIO uses
  // big-endian xdr-like data representation.
  // ****************************************************************************

#ifdef SIO_BIG_ENDIAN
  // ----------------------------------------------------------------------------
  // => Keep the byte ordering during the copy.
  // ----------------------------------------------------------------------------
  void functions::copy(unsigned char *from, unsigned char *dest, const int size, const int count) {
    memcpy( dest, from, size * count );
  }
#endif

#ifdef SIO_LITTLE_ENDIAN
  // ----------------------------------------------------------------------------
  // => Reverse the byte ordering during the copy.
  // ----------------------------------------------------------------------------
  void functions::copy(unsigned char *from, unsigned char *dest, const int size, const int count) {
    dest += size;
    int jump  = size << 1;
    for( int icnt = 0; icnt < count; icnt++ ) {
      for( int ibyt = 0; ibyt < size; ibyt++  ) {
        *--dest = *from++;
      }
      dest += jump;
    }
  }
#endif

  // ****************************************************************************
  // Miscellaneous functions
  // ****************************************************************************

  // ----------------------------------------------------------------------------
  // => Test for a legal name
  // ----------------------------------------------------------------------------
  //
  // General Description:
  //
  // validateName tests the validity (according to SIO) of a name.  SIO only
  // accepts names starting with (regular expression) [A-Za-z_] and continuing
  // with [A-Za-z0-9_] (which most people will recognize as the definition of
  // a C/C++ variable name).
  //
  // ----------------------------------------------------------------------------
  bool functions::validateName( const char* name ) {
    if( *name < 0 ) {
      return false;
    }
    if( !isalpha( (int)*name ) && *name != '_' ) {
      return false;
    }
    for( name += 1; *name != '\0'; name++ ) {
      if( *name < 0 ) {
        return false;
      }
      if( !isalnum( (int)*name ) && *name != '_' ) {
        return false;
      }
    }
    return true;
  }
  
}

