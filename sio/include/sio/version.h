#pragma once

// -- sio headers
#include <sio/definitions.h>

namespace sio {

  /**
   *  @brief  version class.
   *
   *  Helper class to encode or decode version in a single variable
   */
  class version {
  public:
    using version_type   = sio::version_type ;
    using major_type     = uint16_t ;
    using minor_type     = uint16_t ;

  public:
    // static API only
    version() = delete ;

    /**
     *  @brief  Encode a version from a major and minor version number
     *
     *  @param  major the major version number
     *  @param  minor the minor version number
     */
    static version_type encode_version( major_type major, minor_type minor ) noexcept ;

    /**
     *  @brief  Decode a minor version number from the version number
     *
     *  @param  version the full version number
     */
    static minor_type minor_version( version_type version ) noexcept ;

    /**
     *  @brief  Decode a major version number from the version number
     *
     *  @param  version the full version number
     */
    static major_type major_version( version_type version ) noexcept ;
  };
  
}

