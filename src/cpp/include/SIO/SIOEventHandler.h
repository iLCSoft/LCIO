#ifndef SIO_SIOEVENTHANDLER_H
#define SIO_SIOEVENTHANDLER_H 1

#include <string>

#include "DATA/LCEventData.h"
#include "IOIMPL/LCEventIOImpl.h"

#include "SIO_block.h"

namespace SIO {
  
  
/** Handler for LCEvent/LCEventIOImpl objects.
 * 
 * @author gaede
 * @version Mar 6, 2003
 * fg 20030609 using data interface for writing
 */
  class SIOEventHandler : public SIO_block{
    
  protected:
    SIOEventHandler() : SIO_block("UNKNOWN") { /* no default c'tor*/  ;} 

  public:
    
    SIOEventHandler(const std::string& name) ;
    SIOEventHandler(const std::string& name, IOIMPL::LCEventIOImpl** evtP) ;
    virtual ~SIOEventHandler() ;
    
    // interface from SIO_block
    virtual unsigned int   xfer( SIO_stream*, SIO_operation, unsigned int ) ;
    virtual unsigned int   version() ;
    
    void setEvent(const DATA::LCEventData* evt ) ; 
    
  private: 
    // event implemenatation for reading 
    IOIMPL::LCEventIOImpl **_evtP ;  
    // event data interface for writing
    const DATA::LCEventData *_evt ;  
    
  }; // class
  
}; // namespace

#endif /* ifndef SIO_SIOEVENTHANDLER_H */
