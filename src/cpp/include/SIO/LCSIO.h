// -*- C++ -*-
/** Collection of static helper  functions for reading and writing
 * data with SIO. Could go to the SIO_functions class.
 * Also defines some constant names for lcio-SIO.
 *
 * @author gaede
 * @version Mar 6, 2003
 */
#ifndef SIO_LCSIO_H
#define SIO_LCSIO_H 1


class SIO_stream ;

#include <string>

namespace SIO {


#define LCSIO_READ( rec, pnt ) status = LCSIO::read( (rec), (pnt)  ); if( !(status & 1) ) return status;

#define LCSIO_WRITE( rec, pnt ) status = LCSIO::write( (rec), (pnt)  ); if( !(status & 1) ) return status;

  class LCSIO{
  
  public :
  
    // define some names of records and blocks
    static const char* RUNRECORDNAME ; 
    static const char* RUNBLOCKNAME ; 
    static const char* EVENTRECORDNAME ; 
    static const char* EVENTBLOCKNAME;
    static const char* HEADERRECORDNAME ; 
    static const char* HEADERBLOCKNAME;


    /** Read a string from the stream into a dummy buffer. 
     * Warning the same buffer is used for each call.
     * So the return value needs to be copied to its final memory destination.
     */
    static unsigned int read( SIO_stream* stream ,char** c ) ;
  
    /** Write an int to the stream.
     */
    static unsigned int write( SIO_stream* stream , int i) ;

    /** Write an unsigned int to the stream.
     */
    static unsigned int write( SIO_stream* stream , unsigned int i) ;
  

    /** Write a long to the stream (in fact written as a 64-bit long long). 
     */
    static unsigned int write( SIO_stream* stream , long i) ;

    /** Write a float  to the stream.
     */
    static unsigned int write( SIO_stream* stream , float f) ;
  
    /** Write a string to the stream. Writes 
     * the length of the string first and adds '\0' to the end.
     */
    static unsigned int write(SIO_stream* stream , const std::string& s) ;
  
  
  private:

    static char* dummy  ;
    static int dummy_size  ;
    static const int dummy_initial_size = 1024  ;

  } ;

}; // namespace 
#endif // ifndef SIO_LCSIO_H
