#ifndef MT_LCREADERLISTENER_H
#define MT_LCREADERLISTENER_H 1

// -- lcio headers
#include "MT/Types.h"

namespace MT {

/**
 *  @brief  LCReaderListener class
 *          Interface for MT::LCReader::readStream() callbacks
 */
class LCReaderListener {
public:
  /**
   *  @brief  Destructor
   */
  virtual ~LCReaderListener() {}

  /**
   *  @brief  process an event 
   * 
   *  @param  event the event to process
   */
  virtual void processEvent( LCEventPtr event ) = 0 ;
  
  /**
   *  @brief  process a run header
   * 
   *  @param  hdr the run header to process
   */
  virtual void processRunHeader( LCRunHeaderPtr hdr ) = 0 ;
};

} // namespace MT

#endif /* ifndef MT_LCREADERLISTENER_H */
