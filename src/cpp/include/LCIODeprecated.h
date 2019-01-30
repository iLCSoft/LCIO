// -*- C++ -*-

/**Helper pre-processor macros to mark deprecated functions 
 */


#ifndef LCIO_DEPRECATED_H
#define LCIO_DEPRECATED_H 1

#define LCIO_DEFAULT_DEPRECATED(msg, func) func

// Helper to deprecate functions and methods
// See https://blog.samat.io/2017/02/27/Deprecating-functions-and-methods-in-Cplusplus/
// For C++14
#if __cplusplus >= 201402L
  #if defined(__has_cpp_attribute) && __has_cpp_attribute(deprecated)    
    #define LCIO_DEPRECATED(msg, func) [[deprecated(msg)]] func
  #else
    #define LCIO_DEPRECATED(msg, func) LCIO_DEFAULT_DEPRECATED(msg, func)
  #endif
// For everyone else
#else
  #ifdef __GNUC__
    #define LCIO_DEPRECATED(msg, func) func __attribute__ ((deprecated(msg)))
  #elif defined(_MSC_VER)
    #define LCIO_DEPRECATED(msg, func) __declspec(deprecated(msg)) func
  #else
    #define LCIO_DEPRECATED(msg, func) LCIO_DEFAULT_DEPRECATED(msg, func)
  #endif
#endif

#endif  //  LCIO_DEPRECATED_H
