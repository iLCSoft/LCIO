#pragma once

// -- std headers
#include <exception>
#include <string>

// exception helper macros
#define SIO_THROW( code, message ) throw sio::exception( code, __LINE__, __FUNCTION__, __FILE__, message )
#define SIO_RETHROW( orig, code, message ) throw sio::exception( orig, code, __LINE__, __FUNCTION__, __FILE__, message )

namespace sio {

  /**
   *  @brief  error_code enumerator
   */
  enum class error_code : unsigned int {
    invalid_argument,
    not_found,
    already_open,
    open_fail,
    not_open,
    eof,
    io_failure,
    no_marker,
    compress_error,
    bad_state,
    bad_alloc,
    out_of_range
  };

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  /**
   *  @brief  error_code_helper class.
   *
   *  Helper class for error_code manipulation
   */
  class error_code_helper {
  public:
    // static API only
    error_code_helper() = delete ;

  public:
    /**
     *  @brief  Convert error_code to string
     *
     *  @param  code the code to convert
     */
    static std::string to_string( error_code code ) noexcept ;
  };

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  /**
   *  @brief  exception class
   *
   *  Main sio exception class.
   */
  class exception : public std::exception {
  public:
    exception() = delete ;
    exception( const exception & ) = default ;
    ~exception() = default ;

    /**
     *  @brief  Constructor
     *
     *  @param  code the error code
     *  @param  line the exception line number
     *  @param  func the function name in which the exception has been thrown
     *  @param  fname the file in which the exception has been thrown
     *  @param  msg the exception message
     */
    exception( error_code code, unsigned int line, const std::string &func, const std::string &fname, const std::string &msg ) ;

    /**
     *  @brief  Constructor
     *
     *  @param  rhs any possible class having a method what() returning a string
     *  @param  code the error code
     *  @param  line the exception line number
     *  @param  func the function name in which the exception has been thrown
     *  @param  fname the file in which the exception has been thrown
     *  @param  msg the exception message
     */
    template <typename T>
    exception( const T &rhs, error_code code, unsigned int line, const std::string &func, const std::string &fname, const std::string &msg ) ;

    /**
     *  @brief  Get the full exception message
     */
    const char* what() const noexcept override ;
    
    /**
     *  @brief  Get the associated error code
     */
    sio::error_code code() const ;

  protected:
    /**
     *  @brief  Helper function creating the full exception message
     *
     *  @param  code the error code
     *  @param  line the exception line number
     *  @param  func the function name in which the exception has been thrown
     *  @param  fname the file in which the exception has been thrown
     *  @param  msg the exception message
     */
    std::string message( error_code code, unsigned int line, const std::string &func, const std::string &fname, const std::string &msg ) const ;

    /**
     *  @brief  Helper function creating the full exception message
     *
     *  @param  a previous message to prepend in the message (plus a line break)
     *  @param  code the error code
     *  @param  line the exception line number
     *  @param  func the function name in which the exception has been thrown
     *  @param  fname the file in which the exception has been thrown
     *  @param  msg the exception message
     */
    std::string message( const std::string &previous, error_code code, unsigned int line, const std::string &func, const std::string &fname, const std::string &msg ) const ;

  protected:
    ///< The associated error code
    const error_code        _code ;
    ///< The full exception message
    const std::string       _message ;
  };
  
  //--------------------------------------------------------------------------

  template <typename T>
  inline exception::exception(
    const T &rhs,
    error_code code,
    unsigned int line,
    const std::string &func,
    const std::string &fname,
    const std::string &msg ) :
    _code( code ) ,
    _message( message(rhs.what(), code, line, func, fname, msg) ) {
    /* nop */
  }

}
