#ifndef LCIO_EXCEPTION_H
#define LCIO_EXCEPTION_H 1

#include <string>
#include <exception> 

// define some exceptions similar to the ones used in hep.lcd.io
// the exceptions should be part of the corresponding namespace
// even though they are not defined in the package (subdirectory)
// as these hold interfaces (abstract base classes) only ....
// hmmmm - does this make sence ?

namespace EVENT {

  /**Base exception class for LCIO - all other exceptions extend this.
   * @author gaede
   * @version Apr 30, 2003
   */
  //FIXME get compatible with gcc3.2 !!!
//gcc3.2  class Exception : public std::exception {
  class Exception : public exception {
    
  protected:
    std::string message ;
    
    Exception(){  /*no_op*/ ; } 
//gcc3.2    virtual ~Exception() throw() ; 
    
  public: 
    Exception( std::string text ){
      message = "lcio::Exception: " + text ;
    }
//gcc3.2    virtual const char* what() const  throw() { return  message.c_str() ; } 
    virtual const char* what() const { return  message.c_str() ; } 
  };

  /**EventException used for errors accessing the event data.
   * @author gaede
   * @version Apr 30, 2003
   */
  class EventException : public Exception{
  public: 
    EventException( std::string text ){
      message = "lcio::EventException: " + text ;
    }
  }; 
} // namespace
  
namespace IO {

  /**IOException used for reading/writing errors.
   * @author gaede
   * @version Apr 30, 2003
   */
  class IOException : public EVENT::Exception{
  public: 
    IOException( std::string text ){
      message = "lcio::IOException: " + text ;
    }
  }; 

  /**EndOfDataException for signaling the end of a data stream.
   * @author gaede
   * @version Apr 30, 2003
   */
  class EndOfDataException : public EVENT::Exception{
  public: 
    EndOfDataException( std::string text ){
      message = "lcio::EndOfDataException: " + text ;
    }
  }; 
  
} // namespace
#endif /* ifndef LCIO_EXCEPTION_H */
