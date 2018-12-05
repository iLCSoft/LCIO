// ----------------------------------------------------------------------------
// CVS $Id: SIO_functions.h,v 1.6 2010-07-14 18:13:08 jeremy Exp $
// ----------------------------------------------------------------------------
// => Primitive functions for reading/writing SIO streams         
// ----------------------------------------------------------------------------
//
// General Description:
//
// SIO_funtions provides the primitive functions for reading from/writing to
// SIO streams.
//
// ----------------------------------------------------------------------------

#ifndef SIO_FUNCTIONS_H
#define SIO_FUNCTIONS_H 1

namespace sio {
  
  /**
   *  @brief  functions class
   *          Helper class for serialization
   */
  class functions {
  public:
    /**
     *  @brief  Tests the validity (according to SIO) of a name
     *          SIO only accepts names starting with (regular expression) [A-Za-z_] and continuing
     *          with [A-Za-z0-9_] (which most people will recognize as the definition of a C/C++ variable name).
     * 
     *  @param  name the name to validate
     */
    static bool validateName( const char* name );
    
    /**
     *  @brief  Primitive copying functions.
     *  
     *  This is where the endian-ness problem is resolved.  Note that there are two
     *  implementations of SIO_functions::copy.  Only (at most) one version gets
     *  passed to the compiler by the preprocesor.  In the case of a new target CPU
     *  whose endian-ness cannot determined by the preprocessor directives above,
     *  -no- version is passed through to the compiler.  The result is a compilation
     *  failure (i.e. you can't compile the SIO package on a particular CPU if the
     *  CPU's endian-ness cannot be determined by the SIO preprocessor directives).
     *  In SIO all datasets have the same endian-ness.  The choice between a little-
     *  or a big-endian dataset representation should be driven by the type of CPU
     *  which will do most of the work (swapping the endian-ness on read or
     *  write is somewhat inefficient).  In a ditributed multi-platform environment
     *  that's a tough call, so the chice is really somewhat arbitrary.  SIO uses
     *  big-endian xdr-like data representation.
     *  
     *  @param  from buffer or data source to copy
     *  @param  to buffer or variable destination to copy to
     *  @param  size the size in memory of the variable to copy
     *  @param  count the number of variable to copy
     */
    static void copy( unsigned char *from, unsigned char *to, const int size, const int count);
  }; 
  
}

#endif
