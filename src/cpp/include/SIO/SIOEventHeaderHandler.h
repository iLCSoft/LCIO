// -*- C++ -*-
/** Handler for LCEvent/LCEventIOImpl objects.
 * 
 * @author gaede
 * @version Mar 6, 2003
 */
#ifndef SIO_SIOEVENTHEADERHANDLER_H
#define SIO_SIOEVENTHEADERHANDLER_H 1

#include <string>
#include "SIO_block.h"
#include "EVENT/LCEvent.h"
#include "IOIMPL/LCEventIOImpl.h"

namespace SIO {


  class SIOEventHeaderHandler : public SIO_block{
    
  public:
    static char* const EVENTHEADER="EventHeader" ;

  public:
    
    SIOEventHeaderHandler() ;
    SIOEventHeaderHandler(IOIMPL::LCEventIOImpl** evtP) ;
    ~SIOEventHeaderHandler() ;
    
    // interface from SIO_block
    virtual unsigned int   xfer( SIO_stream*, SIO_operation, unsigned int ) ;
    virtual unsigned int   version() ;
    
    void setEvent(const EVENT::LCEvent* evt ) ; 
    
  private: 
    IOIMPL::LCEventIOImpl ** evtP ;     // event for reading 
    const EVENT::LCEvent *evt ;  // event for writing
    
  }; // class
  
}; // namespace
#endif /* ifndef SIO_SIOEVENTHEADERHANDLER_H */
