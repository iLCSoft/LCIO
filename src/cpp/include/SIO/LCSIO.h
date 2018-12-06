#ifndef SIO_LCSIO_H
#define SIO_LCSIO_H 1

// -- lcio headers
#include "LCIOTypes.h"

// -- sio headers
#include "SIO_record.h" 
#include "SIO_definitions.h"

// -- std headers
#include <string>

// record and block names used in LCIO-SIO
#define LCSIO_RUNRECORDNAME    "LCRunHeader"
#define LCSIO_RUNBLOCKNAME     "RunHeader"
#define LCSIO_EVENTRECORDNAME  "LCEvent"
#define LCSIO_EVENTBLOCKNAME   "Event"
#define LCSIO_HEADERRECORDNAME "LCEventHeader"
#define LCSIO_HEADERBLOCKNAME  "EventHeader"

#define LCSIO_ACCESSRECORDNAME "LCIORandomAccess"
#define LCSIO_ACCESSBLOCKNAME  "LCIORandomAccess"
#define LCSIO_RANDOMACCESS_SIZE 136 
#define LCSIO_INDEXRECORDNAME "LCIOIndex"
#define LCSIO_INDEXBLOCKNAME  "LCIOIndex"

namespace SIO {

#define LCSIO_READ( rec, pnt ) status = LCSIO::read( (rec), (pnt)  ); if( !(status & 1) ) return status;
// #define LCSIO_READ_LEN( rec, pnt , len ) status = LCSIO::read( (rec), (pnt) , (len)  ); if( !(status & 1) ) return status;
#define LCSIO_WRITE( rec, pnt ) status = LCSIO::write( (rec), (pnt)  ); if( !(status & 1) ) return status;

/** 
 *  @brief  Collection of static helper  functions for reading and writing
 *          data with SIO. Could go to the SIO_functions class.
 *          Also defines some constant names for lcio-SIO.
 *
 *  @author gaede
 *  @version Mar 6, 2003
 */
  class LCSIO{
  public:
    /// The lcio file extension name
    static const char* FILE_EXTENSION;

    /**
     *  @brief  Check for old version of LCIO (> v01-08 ar no longer supported)
     *          Throws an exception if not supported
     * 
     *  @param  version the version to check
     */
    static void checkVersion(int versionID );

    /** 
     *  @brief  Read a string from the stream
     */
    static unsigned int read( SIO_stream* stream , std::string &c);
  
    /** 
     *  @brief  Write an int to the stream.
     */
    static unsigned int write( SIO_stream* stream , int i);

    /** 
     *  @brief  Write an unsigned int to the stream.
     */
    static unsigned int write( SIO_stream* stream , unsigned int i);
  
#if defined(_LP64)  || defined(__APPLE_CC__)
    /**
     *  @brief  Write a vector size to the stream (needed for AMD64 architectures)
     */
    static unsigned int write( SIO_stream* stream , size_t i);
#endif

    /** 
     *  @brief  Write a 64bit integer to the stream 
     */
    static unsigned int write( SIO_stream* stream , EVENT::long64 i);
    
    /** 
     *  @brief  Write a float  to the stream.
     */
    static unsigned int write( SIO_stream* stream , float f);
    
    /** 
     *  @brief  Write a string to the stream. Writes 
     *          the length of the string first and adds '\0' to the end.
     */
    static unsigned int write(SIO_stream* stream , const std::string& s);
  };

} // namespace 
#endif // ifndef SIO_LCSIO_H
