#ifndef SIO_LCSIO_H
#define SIO_LCSIO_H 1

// -- sio headers
#include <sio/definitions.h>

namespace SIO {

/**
 *  @brief  Collection of constants and helper functions
 *
 *  @author gaede
 *  @author rete
 *  @version June, 2019
 */
  class LCSIO {
    typedef long long long64 ;
  public:
    /// The lcio file extension name
    static constexpr const char * FileExtension     = ".slcio" ;

    // record and block names used in LCIO-SIO
    static constexpr const char * RunRecordName     = "LCRunHeader" ;
    static constexpr const char * RunBlockName      = "RunHeader" ;
    static constexpr const char * EventRecordName   = "LCEvent" ;
    static constexpr const char * EventBlockName    = "Event" ;
    static constexpr const char * HeaderRecordName  = "LCEventHeader" ;
    static constexpr const char * HeaderBlockName   = "EventHeader" ;
    static constexpr const char * AccessRecordName  = "LCIORandomAccess" ;
    static constexpr const char * AccessBlockName   = "LCIORandomAccess" ;
    static constexpr int          RandomAccessSize  = 136 ;
    static constexpr const char * IndexRecordName   = "LCIOIndex" ;
    static constexpr const char * IndexBlockName    = "LCIOIndex" ;

    /**
     *  @brief  Check for old version of LCIO (> v01-08 ar no longer supported)
     *          Throws an exception if not supported
     *
     *  @param  version the version to check
     */
    static void checkVersion( sio::version_type versionID ) ;

    /**
     *  @brief  Get the LCIO encoded version number for writing in sio blocks
     */
    static sio::version_type blockVersion() ;
  };

} // namespace
#endif // ifndef SIO_LCSIO_H
