#ifndef SIO_SIORUNHEADERHANDLER_H
#define SIO_SIORUNHEADERHANDLER_H 1

#include <string>

#include "EVENT/LCRunHeader.h"
#include "IOIMPL/LCRunHeaderIOImpl.h"

#include "SIO_block.h"

namespace SIO {
  
  
/** Handler for LCRunHeader/LCRunHeaderImpl objects.
 * 
 * @author gaede
 * @version $Id: SIORunHeaderHandler.h,v 1.8 2005-04-15 08:37:43 gaede Exp $
 * fg 20030609 using data interface for writing
 */
  class SIORunHeaderHandler : public SIO_block{
    
  protected:
    SIORunHeaderHandler() : SIO_block("UNKNOWN") { /* no default c'tor*/  ;} 

  public:
    
    SIORunHeaderHandler(const SIORunHeaderHandler&) = delete;
    SIORunHeaderHandler& operator=(const SIORunHeaderHandler&) = delete ;

    /** C'tor for writing
     */
    SIORunHeaderHandler(const std::string& name) ;
    /** C'tor for reading.
     */
    SIORunHeaderHandler(const std::string& name, IOIMPL::LCRunHeaderIOImpl** rhP) ;
    virtual ~SIORunHeaderHandler() ;
    
    // interface from SIO_block
    virtual unsigned int   xfer( SIO_stream*, SIO_operation, unsigned int ) ;
    virtual unsigned int   version() ;
    
    void setRunHeader(const EVENT::LCRunHeader* hdr ) ; 
    void setRunHeaderPtr(IOIMPL::LCRunHeaderIOImpl** hdrP ) ; 
    
  private: 
    IOIMPL::LCRunHeaderIOImpl** _rhP{NULL} ;  // address for reading
    const EVENT::LCRunHeader* _hdr{NULL} ;  // runheader for writing
    
  }; // class
  
} // namespace

#endif /* ifndef SIO_SIORUNHEADERHANDLER_H */
