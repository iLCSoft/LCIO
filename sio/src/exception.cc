// -- std headers
#include <sio/exception.h>

namespace sio {

  inline std::string error_code_helper::to_string( error_code code ) noexcept {
    switch( code ) {
      case error_code::invalid_argument: return "invalid_argument" ;
      case error_code::not_found: return "not_found" ;
      case error_code::already_open: return "already_open" ;
      case error_code::open_fail: return "open_fail" ;
      case error_code::not_open: return "not_open" ;
      case error_code::eof: return "eof" ;
      case error_code::io_failure: return "io_failure" ;
      case error_code::no_marker: return "no_marker" ;
      case error_code::compress_error: return "compress_error" ;
      case error_code::bad_state: return "bad_state" ;
      case error_code::bad_alloc: return "bad_alloc" ;
      case error_code::out_of_range: return "out_of_range" ;
      default:  return "unknown" ;
    }
  }

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  exception::exception(
    error_code code,
    unsigned int line,
    const std::string &func,
    const std::string &fname,
    const std::string &msg ) :
    _code( code ),
    _message( message(code, line, func, fname, msg) ) {
    /* nop */
  }

  //--------------------------------------------------------------------------

  const char* exception::what() const noexcept {
    return _message.c_str() ;
  }
  
  //--------------------------------------------------------------------------
  
  sio::error_code exception::code() const {
    return _code ;
  }

  //--------------------------------------------------------------------------

  std::string exception::message(
    error_code code,
    unsigned int line,
    const std::string &func,
    const std::string &fname,
    const std::string &msg ) const {
    return fname +
      " (l." + std::to_string(line) + ") in " +
      func + ": " + msg +
      " [" + error_code_helper::to_string( code ) + "]" ;
  }

  //--------------------------------------------------------------------------

  std::string exception::message(
    const std::string &previous,
    error_code code,
    unsigned int line,
    const std::string &func,
    const std::string &fname,
    const std::string &msg ) const {
    if( previous.empty() ) {
      return message(code, line, func, fname, msg) ;
    }
    else {
      return previous + "\n" + message(code, line, func, fname, msg) ;
    }
  }

}
