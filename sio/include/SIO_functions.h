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

#define SIO_LEN_SB        1
#define SIO_LEN_DB        2
#define SIO_LEN_QB        4
#define SIO_LEN_OB        8

// ----------------------------------------------------------------------------
// Deal with declarations for 64 bit integers.  This really amounts to "who is
// ANSI compiant?"  ANSI defines "long long" as the declaration for 64 bit
// integers.  M$ hasn't caught up yet and still requires __int64.
//
// Branch on flag provided by compiler:
//
// OS            CPU           Macro         Provided by   Declaration
// ------------  ------------  ------------  ------------  -----------
// AIX           PPC(?)        _AIX          GNU compiler  long long
// OSF1          Alpha         __alpha__     GNU compiler  long long
// Linux         x86           __i386__      GNU compiler  long long
// Linux         Opteron       _LP64         GNU compiler  long long
// Linux         Itanium       _LP64         GNU compiler  long long
// SunOS         Sparc         __sparc__     GNU compiler  long long
// Windows/NT    Alpha         _M_ALPHA      VC  compiler  __int64
// Windows/NT    x86           _M_IX86       VC  compiler  __int64
// Windows/NT    MIPS          _M_MRX000     VC  compiler  __int64
// Windows/NT    PPC           _M_PPC        VC  compiler  __int64
// ----------------------------------------------------------------------------
#if defined(_AIX)      ||  defined(__alpha__) || defined(__i386__)  || defined(__sparc__) || defined(__APPLE_CC__) || defined(_LP64)

// fg: gcc complains about long long - what to do about it ?
// warning: ANSI C++ does not support `long long'
 #define SIO_64BITINT   long long
// #define SIO_64BITINT   long
#endif

#if defined(_M_ALPHA)  || defined(_M_IX86)   || defined(_M_PPC)
 #define SIO_64BITINT   __int64
#endif

// ----------------------------------------------------------------------------
// Deal with pointer length.  Currently, the only problem is alpha which uses
// 64 bit pointers.                                                          
//                                                              
// OS            CPU           Macro         Provided by   Pointer size
// ------------  ------------  ------------  ------------  -----------
// AIX           PPC(?)        _AIX          GNU compiler  4 bytes
// OSF1          Alpha         __alpha__     GNU compiler  8 bytes
// Linux         x86           __i386__      GNU compiler  4 bytes
// Linux         Opteron       _LP64    _    GNU compiler  8 bytes
// Linux         Itanium       _LP64         GNU compiler  8 bytes
// SunOS         Sparc         __sparc__     GNU compiler  4 bytes
// Windows/NT    Alpha         _M_ALPHA      VC  compiler  8 bytes
// Windows/NT    x86           _M_IX86       VC  compiler  4 bytes
// Windows/NT    MIPS          _M_MRX000     VC  compiler  ? bytes
// Windows/NT    PPC           _M_PPC        VC  compiler  4 bytes
// ----------------------------------------------------------------------------
//#if defined(__alpha__) || defined(_M_ALPHA) || defined(_LP64)
// #define SIO_POINTER_DECL   size_t
//#endif

//#if defined(_AIX)      || defined(__i386__)  || defined(__sparc__) || defined(_M_IX86) || defined(_M_PPC) || defined(__APPLE_CC__)
// #define SIO_POINTER_DECL   unsigned int
//#endif

// Old check was problematic because both macros could evaluate to true, and it would
// also be possible that SIO_POINTER_DECL was not defined at all.  Change as needed.
// --JM 
#if defined(__alpha__) || defined(_M_ALPHA) || defined(_LP64)
 #define SIO_POINTER_DECL   size_t
#else
 #define SIO_POINTER_DECL   unsigned int
#endif

//
// Take the drudgery out of error handling.
//
#define SIO_DATA( rec, pnt, cnt ) status = SIO_functions::data( (rec), (pnt), (cnt) ); if( !(status & 1) ) return status;

#define SIO_PNTR( rec, pnt )   status = SIO_functions::pointer_to( (rec), (SIO_POINTER_DECL *)(pnt) );   if( !(status & 1) )       return status;

#define SIO_PTAG( rec, pnt )   status = SIO_functions::pointed_at( (rec), (SIO_POINTER_DECL *)(pnt) );   if( !(status & 1) )       return status;

//
// This turns up far too often to be ignored.
//
#define UCHR_CAST(pntr)    (reinterpret_cast<unsigned char*>((pntr)))

class SIO_stream;

class SIO_functions
{
public:

    static unsigned int data( SIO_stream*,          char*,         const int );
    static unsigned int data( SIO_stream*, unsigned char*,         const int );
    static unsigned int data( SIO_stream*,          short*,        const int );
    static unsigned int data( SIO_stream*, unsigned short*,        const int );
    static unsigned int data( SIO_stream*,          int*,          const int );
    static unsigned int data( SIO_stream*, unsigned int*,          const int );
    static unsigned int data( SIO_stream*,          SIO_64BITINT*, const int );
    static unsigned int data( SIO_stream*, unsigned SIO_64BITINT*, const int );
    static unsigned int data( SIO_stream*,          float*,        const int );
    static unsigned int data( SIO_stream*,          double*,       const int );

    static unsigned int pointed_at( SIO_stream*, SIO_POINTER_DECL* );
    static unsigned int pointer_to( SIO_stream*, SIO_POINTER_DECL* );

    static bool validateName( const char* );

private:

    static void         copy( unsigned char*, unsigned char*, 
                              const int,      const int      );
    static unsigned int xfer( SIO_stream*,    const int, 
                              const int,      unsigned char* );

    friend class SIO_stream;              // Direct access to copy
    friend class SIO_record;              // Direct access to copy
}; 

#endif
