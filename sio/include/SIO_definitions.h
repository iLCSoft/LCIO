// ----------------------------------------------------------------------------
// CVS $Id: SIO_definitions.h,v 1.2 2010-06-22 13:49:54 gaede Exp $
// ----------------------------------------------------------------------------
// => Assorted definitions for the SIO package.
// ----------------------------------------------------------------------------
//
// General Description:
//
// SIO_definitions.h provides all the static definitions for the SIO package.
//
// ----------------------------------------------------------------------------

#ifndef SIO_DEFINITIONS_H
#define SIO_DEFINITIONS_H 1

// -- std headers
#include <memory>
#include <map>

//
// Error format
//
#define SIO_M_SEVERITY              0x00000007
#define SIO_M_MESSAGE               0x0000FFF8
#define SIO_M_FACILITY              0x0FFF0000

#define SIO_V_SEVERITY              0
#define SIO_V_MESSAGE               3
#define SIO_V_FACILITY              16

//
// Error codes ... stream
//
#define SIO_STREAM_FACILITY         0x08000000
#define SIO_STREAM_SUCCESS          0x08000001
#define SIO_STREAM_NOTFOUND         0x08000014
#define SIO_STREAM_ALREADYOPEN      0x08000024
#define SIO_STREAM_OPENFAIL         0x08000034
#define SIO_STREAM_BADMODE          0x08000044
#define SIO_STREAM_NOTOPEN          0x08000054
#define SIO_STREAM_GOTEOF           0x08000064
#define SIO_STREAM_READONLY         0x08000074
#define SIO_STREAM_WRITEONLY        0x08000084
#define SIO_STREAM_NOSUCHRECORD     0x08000094
#define SIO_STREAM_BADWRITE         0x080000a4
#define SIO_STREAM_BADSTATE         0x080000b4
#define SIO_STREAM_OFFEND           0x080000c4
#define SIO_STREAM_NOALLOC          0x080000d4
#define SIO_STREAM_EOF              0x080000e4
#define SIO_STREAM_NORECMARKER      0x080000f4
#define SIO_STREAM_BADCOMPRESS      0x08000104

//
// Error codes ... record
//
#define SIO_RECORD_FACILITY         0x08010000
#define SIO_RECORD_SUCCESS          0x08010001
#define SIO_RECORD_NOTFOUND         0x08010014
#define SIO_RECORD_BADARGUMENT      0x08010024
#define SIO_RECORD_DUPCONNECT       0x08010034
#define SIO_RECORD_NOTCONNECTED     0x08010044
#define SIO_RECORD_NOBLKMARKER      0x08010054

//
// Error codes ... block
//
#define SIO_BLOCK_FACILITY          0x08020000
#define SIO_BLOCK_SUCCESS           0x08020001
#define SIO_BLOCK_NOTFOUND          0x08020014
#define SIO_BLOCK_SKIP              0x08020024

//
// Handy dandy unit specifiers.
//
#define SIO_KBYTE     0x00000400
#define SIO_MBYTE     0x00100000

//
// To make version handling easier.
//
#define SIO_VERSION_ENCODE( major, minor )  (((major) << 16) + (minor))
#define SIO_VERSION_MINOR( version )         ((version) & 0x0000ffff)
#define SIO_VERSION_MAJOR( version )        (((version) & 0xffff0000) >> 16)

//
// Enumerations for stream mode, status and verbosity.
//
typedef enum {
    SIO_MODE_READ,
    SIO_MODE_WRITE_NEW,
    SIO_MODE_WRITE_APPEND,
    SIO_MODE_READ_WRITE,
    SIO_MODE_UNDEFINED
} SIO_stream_mode; 

typedef enum {
    SIO_OP_READ,
    SIO_OP_WRITE,
    SIO_OP_CLEAR
} SIO_operation; 

typedef enum {
    SIO_STATE_OPEN,
    SIO_STATE_CLOSED,
    SIO_STATE_ERROR
} SIO_stream_state; 

typedef enum {
    SIO_SILENT,
    SIO_ERRORS,
    SIO_ALL
} SIO_verbosity;

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
// Old check was problematic because both macros could evaluate to true, and it would
// also be possible that SIO_POINTER_DECL was not defined at all.  Change as needed.
// --JM 
#if defined(__alpha__) || defined(_M_ALPHA) || defined(_LP64)
 #define SIO_POINTER_DECL   size_t
#else
 #define SIO_POINTER_DECL   unsigned int
#endif

//
// This turns up far too often to be ignored.
//
#define UCHR_CAST(pntr)    (reinterpret_cast<unsigned char*>((pntr)))

//------- if built with dcap support we need different file functions -------
#ifdef SIO_USE_DCAP

#include <dcap.h>

#define FOPEN  dc_fopen
#define FTELL  dc_ftell
#define FSEEK  dc_fseek
#define FCLOSE dc_fclose
#define FREAD  dc_fread
#define FWRITE dc_fwrite
#define FFLUSH dc_fflush
#define FSTAT  dc_stat

#else

#include <sys/stat.h>

#define FOPEN  fopen
#define FTELL  ftell
#define FSEEK  fseek
#define FCLOSE fclose
#define FREAD  fread
#define FWRITE fwrite
#define FFLUSH fflush
#define FSTAT  stat

#endif

namespace sio {
  
  class stream;
  class record;
  class block;
  
  using stream_ptr  = std::shared_ptr<stream>;
  using record_ptr  = std::shared_ptr<record>;
  using block_ptr   = std::shared_ptr<block>;
  using record_map  = std::map<std::string, record_ptr>;
  using block_map   = std::map<std::string, block_ptr>;
  using pointed_at_map = std::map< void*, void* >;
  using pointer_to_map = std::multimap< void*, void* >;
  
  /**
   *  @brief  record_write_result struct
   *          Result of a write operation for records
   */
  struct record_write_result {
    /// After-write operation status 
    unsigned int        _status{0};
    /// After-write record start position
    SIO_64BITINT        _record_begin{-1};
    /// After-write record end position
    SIO_64BITINT        _record_end{-1};
  };
  
  /**
   *  @brief  record_read_result struct
   *          Result of a read operation for records
   */
  struct record_read_result {
    /// After-read operation status 
    unsigned int        _status{0};
    /// After-read record pointer
    record_ptr          _record{nullptr};
    /// After-read record start position
    SIO_64BITINT        _record_begin{-1};
    /// After-read record end position
    SIO_64BITINT        _record_end{-1};
  };
  
}

// for interface compatibility
using SIO_stream = sio::stream;
using SIO_record = sio::record;
using SIO_block = sio::block;

#endif






