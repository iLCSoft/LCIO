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

  class Exception : public std::exception {

    
  protected:
    std::string message ;
    
    Exception(){  /*no_op*/ ; } 
    
  public: 
    virtual ~Exception() throw() { /*no_op*/; } 
    
    Exception( const std::string& text ){
      message = "lcio::Exception: " + text ;
    }

    virtual const char* what() const  throw() { return  message.c_str() ; } 

  };

  /**EventException used for errors accessing the event data.
   * @author gaede
   * @version Apr 30, 2003
   */
  class EventException : public Exception{
    
  protected:
    EventException() {  /*no_op*/ ; } 
  public: 
    virtual ~EventException() throw() { /*no_op*/; } 

    EventException( std::string text ){
      message = "lcio::EventException: " + text ;
    }
  }; 

  /**EventException used for data not available.
   * @author gaede
   * @version Jun 5, 2003
   */
  class DataNotAvailableException : public EventException{

  public: 
    virtual ~DataNotAvailableException() throw() { /*no_op*/; } 

    DataNotAvailableException( std::string text ) {
      message = "lcio::DataNotAvailableException: " + text ;
    }
  }; 

  /**EventException used for signaling a 'read only exception'.
   * @author gaede
   * @version Jun 5, 2003
   */
  class ReadOnlyException : public EventException{

  public: 
    virtual ~ReadOnlyException() throw() { /*no_op*/; } 

    ReadOnlyException( std::string text ){
      message = "lcio::ReadOnlyException: " + text ;
    }
  }; 

} // namespace
  
namespace IO {

  /**IOException used for reading/writing errors.
   * @author gaede
   * @version Apr 30, 2003
   */
  class IOException : public EVENT::Exception{
    
  protected:
    IOException() { /* no_op */ } ;

  public: 
    IOException( std::string text ){
      message = "lcio::IOException: " + text ;
    }
  }; 

  // --- used only internally - switched back to null pointer at EOF ------
  /**EndOfDataException for signaling the end of a data stream.
   * @author gaede
   * @version Jun 5, 2003
   */
  class EndOfDataException : public IOException{
  public: 
    virtual ~EndOfDataException() throw() { /*no_op*/; } 

    EndOfDataException( std::string text ){
      message = "lcio::EndOfDataException: " + text ;
    }
  }; 
  
} // namespace
#endif /* ifndef LCIO_EXCEPTION_H */
