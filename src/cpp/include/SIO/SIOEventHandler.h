#ifndef SIO_SIOEVENTHANDLER_H
#define SIO_SIOEVENTHANDLER_H 1

#include <string>

#include "EVENT/LCEvent.h"
#include "IOIMPL/LCEventIOImpl.h"

#include "SIO_block.h"

namespace SIO {
  
  
/** Handler for LCEvent/LCEventIOImpl objects.
 * 
 * @author gaede
 * @version $Id: SIOEventHandler.h,v 1.8 2005-04-15 08:37:42 gaede Exp $
 */
  class SIOEventHandler : public SIO_block{
    
  protected:
    SIOEventHandler() : SIO_block("UNKNOWN") { /* no default c'tor*/  ;} 

  public:
    
    SIOEventHandler(const SIOEventHandler&) = delete;
    SIOEventHandler& operator=(const SIOEventHandler&) = delete ;

    SIOEventHandler(const std::string& name) ;
    SIOEventHandler(const std::string& name, IOIMPL::LCEventIOImpl** evtP) ;
    virtual ~SIOEventHandler() ;
    
    // interface from SIO_block
    virtual unsigned int   xfer( SIO_stream*, SIO_operation, unsigned int ) ;
    virtual unsigned int   version() ;
    
    void setEvent(const EVENT::LCEvent* evt ) ; 
    void setEventPtr( IOIMPL::LCEventIOImpl** evtP ) ; 
    
    void setReadCollectionNames(const std::vector<std::string>& colnames) ;

  private: 
    // event implementation for reading 
    IOIMPL::LCEventIOImpl **_evtP{NULL} ;  
    // event data interface for writing
    const EVENT::LCEvent *_evt{NULL} ;  
    
    std::set< std::string > _colSubSet{} ;

  }; // class
  
} // namespace

#endif /* ifndef SIO_SIOEVENTHANDLER_H */
