#ifndef SIO_LCIORandomAccess_H
#define SIO_LCIORandomAccess_H 1

/// -- lcio headers
#include "SIO/RunEventMap.h"

// -- std headers
#include <iostream>

namespace SIO {

  class SIORandomAccessHandler ;
  class LCIORandomAccess ;
  class LCIORandomAccessMgr ;

  std::ostream & operator<<(std::ostream& os, const LCIORandomAccess& ra ) ;
  bool operator < (const LCIORandomAccess& ra0, const LCIORandomAccess& other)  ;


/**  Implementation class for LCIORandomAccess records.
 *
 * @author gaede
 * @version $Id: LCIORandomAccess.h,v 1.2 2010-06-22 13:49:54 gaede Exp $
 */
  class LCIORandomAccess {
    using long64 = RunEvent::long64 ;
    friend class SIORandomAccessHandler ;
    friend class LCIORandomAccessMgr ;
    friend std::ostream & operator<<(std::ostream& os, const LCIORandomAccess& ra ) ;
    friend bool operator < (const LCIORandomAccess& ra0, const LCIORandomAccess& other)  ;

  public:
    LCIORandomAccess() = default ;
    ~LCIORandomAccess() = default ;

    /// Get the random access index location
    long64 getIndexLocation() const ;

    /// Get the previous record location
    long64 getPrevLocation()  const ;

    /// Get the first record location
    long64 getFirstRecordLocation() const ;

    /// Set the random access index location
    void setIndexLocation(long64 il) ;

    /// Set the previous record location
    void setPreviousLocation(long64 pl) ;

    /// Set the first record location
    void setFirstRecordLocation(long64 fl) ;

  protected:
    RunEvent      _minRunEvt {} ;
    RunEvent      _maxRunEvt {} ;
    int           _nRunHeaders {0} ;
    int           _nEvents {0} ;
    int           _recordsAreInOrder {0} ;
    long64        _indexLocation {0} ;
    long64        _prevLocation {0} ;
    long64        _nextLocation {0} ;
    long64        _firstRecordLocation {0} ;
  };

} // namespace
#endif
