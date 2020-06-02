#pragma once

// -- std headers
#include <iostream>
#include <cctype>
#include <vector>
#include <type_traits>
#include <fstream>
#include <map>
#include <string>
#include <memory>
#ifdef __APPLE__
#include <_types.h>
#include <_types/_uint16_t.h>
#include <_types/_uint32_t.h>
#include <_types/_uint64_t.h>
#include <_types/_uint8_t.h>
#include <sys/_types/_int16_t.h>
#include <sys/_types/_int64_t.h>
#else
#include <stdint.h>
#endif

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

#if !defined(SIO_LITTLE_ENDIAN) && !defined(SIO_BIG_ENDIAN)
#error "Couldn't determine endianess on this plateform!"
#endif

/**
 *  sio (simple io) namespace.
 *  Holds a persistency implementation using sio
 */
namespace sio {

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
  // also be possible that sio::ptr_type was not defined at all.  Change as needed.
  // --JM
#if defined(__alpha__) || defined(_M_ALPHA) || defined(_LP64)
  using ptr_type = std::size_t ;
#else
  using ptr_type = unsigned int ;
#endif

  class block ;
  class read_device ;
  class write_device ;
  class buffer ;
  class buffer_span ;

  // Bytes related types
  using byte = char ;
  using byte_array = std::vector<byte> ;
  using byte_traits = std::char_traits<byte> ;
  // Other types
  using index_type = std::size_t ;
  using options_type = unsigned int ;
  using version_type = uint32_t ;
  using pointed_at_map = std::map< void*, void* > ;
  using pointer_to_map = std::multimap< void*, void* > ;
  using ifstream = std::ifstream ;
  using ofstream = std::ofstream ;
  using fstream  = std::fstream ;
  using block_ptr = std::shared_ptr<block> ;
  using block_list = std::vector<block_ptr> ;

  /// The null byte definition
  static constexpr byte null_byte = '\0' ;
  
  static constexpr byte padding_bytes [4] = {null_byte} ;
  /// Kilo byte unit
  static constexpr std::size_t kbyte = 0x00000400 ;
  /// Mega byte unit
  static constexpr std::size_t mbyte = 0x00100000 ;
  /// The compression bit mask
  static constexpr unsigned int compression_bit = 0x00000001 ;
  /// The bit alignment mask
  static constexpr unsigned int bit_align = 0x00000003 ;
  /// The additional padding added in buffer IO
  static constexpr unsigned int padding = 3 ;
  /// The additional padding mask
  static constexpr unsigned int padding_mask = 0xfffffffc ;
  /// The record marker
  static constexpr unsigned int record_marker = 0xabadcafe ;
  /// The block marker
  static constexpr unsigned int block_marker  = 0xdeadbeef ;
  /// The maximum length of a record name
  static constexpr std::size_t max_record_name_len = 64 ;
  /// The maximum length of a record_info in memory
  static constexpr std::size_t max_record_info_len = 2*sizeof(sio::ifstream::pos_type) + 5*sizeof(unsigned int) + max_record_name_len ;

  // TODO: Do we still need all of this ??
  static constexpr std::size_t single_len = 1 ;
  static constexpr std::size_t double_len = 2 ;
  static constexpr std::size_t quad_len = 4 ;
  static constexpr std::size_t octo_len = 8 ;

  /**
   *  @brief  record_info struct.
   *
   *  Holds simple descriptive fields on record
   */
  struct record_info {
    ///< Position of the record start in the file
    sio::ifstream::pos_type       _file_start {0} ;
    ///< Position of the record end in the file
    sio::ifstream::pos_type       _file_end {0} ;
    ///< The size of the record header in memory
    unsigned int                  _header_length {0} ;
    ///< The record options
    unsigned int                  _options {0} ;
    ///< The size of the record data read out from the file
    unsigned int                  _data_length {0} ;
    ///< The size of the record data after uncompression (if compressed)
    unsigned int                  _uncompressed_length {0} ;
    ///< The record name
    std::string                   _name {} ;
  };

  /**
   *  @brief  block_info struct.
   *
   *  Holds simple descriptive fields on block
   */
  struct block_info {
    ///< The start position of the block in the record buffer
    unsigned int                  _record_start {0} ;
    ///< The end position of the block in the record buffer
    unsigned int                  _record_end {0} ;
    ///< The size of the block header in memory
    unsigned int                  _header_length {0} ;
    ///< The block version
    unsigned int                  _version {0} ;
    ///< The size of the block data
    unsigned int                  _data_length {0} ;
    ///< The block name
    std::string                   _name {} ;
  };

  /**
   *  @brief  Streaming operator for record_info
   */
  inline std::ostream &operator<<( std::ostream &stream, const record_info &info ) {
    stream << "- name:                  " << info._name << std::endl ;
    stream << "- file pos:              " << info._file_start << " - " << info._file_end << std::endl ;
    stream << "- header len:            " << info._header_length << std::endl ;
    stream << "- options:               " << info._options << std::endl ;
    stream << "- compressed len:        " << info._data_length << std::endl ;
    stream << "- uncompressed len:      " << info._uncompressed_length << std::endl ;
    return stream ;
  }

  /**
   *  @brief  Streaming operator for block_info
   */
  inline std::ostream &operator<<( std::ostream &stream, const block_info &info ) {
    stream << "- name:                  " << info._name << std::endl ;
    stream << "- record pos:            " << info._record_start << " - " << info._record_end << std::endl ;
    stream << "- header len:            " << info._header_length << std::endl ;
    stream << "- version:               " << info._version << std::endl ;
    stream << "- data len:              " << info._data_length << std::endl ;
    return stream ;
  }

  /**
   *  @brief  Validate a name.
   *
   *  SIO only accepts names starting with (regular expression) [A-Za-z_]
   *  and continuing with [A-Za-z0-9_] (which most people will recognize
   *  as the definition of a C/C++ variable name).
   *
   *  @param  name the string name to test
   */
  inline bool validate( const std::string &name ) {
    auto cname = name.c_str() ;
    if( *cname < 0 ) {
      return false;
    }
    if( !isalpha( (int)*cname ) && *cname != '_' ) {
      return false;
    }
    for( cname += 1; *cname != '\0'; cname++ ) {
      if( *cname < 0 ) {
        return false;
      }
      if( !isalnum( (int)*cname ) && *cname != '_' ) {
        return false;
      }
    }
    return true;
  }

  /**
   *  @brief  Validate a record name
   *
   *  @param  name the record name to validate
   */
  inline bool valid_record_name( const std::string &name ) {
    if( not sio::validate( name ) ) {
      return false ;
    }
    if( name.size() > sio::max_record_name_len ) {
      return false ;
    }
    return true ;
  }
}

// SIO_LOGLVL defines the log level. The verbosity is fixed
// at compile time to avoid performance issue due to logging
// Log levels:
// - silent: 0
// - debug: 1
// - info: 2
// - warning: 3
// - error: 4
#ifndef SIO_LOGLVL
#define SIO_LOGLVL 2
#endif

#if SIO_LOGLVL > 3
#define SIO_DEBUG( message ) std::cout << "[SIO DEBUG] " << __FUNCTION__ << " - " << message << std::endl
#else
#define SIO_DEBUG( message )
#endif

#if SIO_LOGLVL > 2
#define SIO_INFO( message ) std::cout << "[SIO INFO] - "  << __FUNCTION__ << " - " << message << std::endl
#else
#define SIO_INFO( message )
#endif

#if SIO_LOGLVL > 1
#define SIO_WARNING( message ) std::cout << "[SIO WARNING] - "  << __FUNCTION__ << " - " << message << std::endl
#else
#define SIO_WARNING( message )
#endif

#if SIO_LOGLVL > 0
#define SIO_ERROR( message ) std::cout << "[SIO ERROR] - "  << __FUNCTION__ << " - " << message << std::endl
#else
#define SIO_ERROR( message )
#endif

// address cast
#define SIO_BYTE_CAST(pntr)    (reinterpret_cast<sio::byte*>((pntr)))
#define SIO_CBYTE_CAST(pntr)    (reinterpret_cast<const sio::byte*>((pntr)))
#define SIO_UCHAR_CAST(pntr)    (reinterpret_cast<unsigned char*>((pntr)))
#define SIO_CUCHAR_CAST(pntr)    (reinterpret_cast<const unsigned char*>((pntr)))

// version decoding/encoding for backward compatibility
#define SIO_VERSION_MAJOR( v ) sio::version::major_version( v )
#define SIO_VERSION_MINOR( v ) sio::version::minor_version( v )
#define SIO_VERSION_ENCODE( maj, min ) sio::version::encode_version( maj, min )

#ifdef SIO_MACROS_WITH_EXCEPTION
#warning "SIO_MACROS_WITH_EXCEPTION activated!"
// Read or write data
#define SIO_DATA( dev, pnt, cnt ) \
  try { \
    SIO_DEBUG( "Reading/writing " << #pnt << " of size " << cnt ); \
    dev.data( pnt, cnt ) ; \
  } \
  catch( sio::exception &e ) { \
    SIO_RETHROW( e, sio::error_code::io_failure, "Failed to read or write data!" ) ; \
  }

// Specialized macro for simple data reading/writing
#define SIO_SDATA( dev, dat ) \
  try { \
    SIO_DEBUG( "Simple reading/writing " << #dat ); \
    dev.data( dat ) ; \
  } \
  catch( sio::exception &e ) { \
    SIO_RETHROW( e, sio::error_code::io_failure, "Failed to read or write data!" ) ; \
  }

// Read or write a pointer (pointer to)
#define SIO_PNTR( dev, pnt ) \
  try { \
    SIO_DEBUG( "Reading/writing pointer to" << #pnt ); \
    dev.pointer_to( (sio::ptr_type*)pnt ) ; \
  } \
  catch( sio::exception &e ) { \
    SIO_RETHROW( e, sio::error_code::io_failure, "Failed to read or write pointer to!" ) ; \
  }

// Read or write a pointer tag (pointed at)
#define SIO_PTAG( dev, pnt ) \
  try { \
    SIO_DEBUG( "Reading/writing pointed at" << #pnt ); \
    dev.pointed_at( (sio::ptr_type*)pnt ) ; \
  } \
  catch( sio::exception &e ) { \
    SIO_RETHROW( e, sio::error_code::io_failure, "Failed to read or write pointed at!" ) ; \
  }
#else
// Read or write data
#define SIO_DATA( dev, pnt, cnt ) dev.data( pnt, cnt )
// Specialized macro for simple data reading/writing
#define SIO_SDATA( dev, dat ) dev.data( dat )
// Read or write a pointer (pointer to)
#define SIO_PNTR( dev, pnt ) dev.pointer_to( (sio::ptr_type*)pnt )
// Read or write a pointer tag (pointed at)
#define SIO_PTAG( dev, pnt ) dev.pointed_at( (sio::ptr_type*)pnt )

#endif
