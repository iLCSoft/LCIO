#ifndef SIO_SIORUNHEADERHANDLER_H
#define SIO_SIORUNHEADERHANDLER_H 1

#include <string>

#include "DATA/LCRunHeaderData.h"
#include "IMPL/LCRunHeaderImpl.h"

#include "SIO_block.h"

namespace SIO {
  
  
/** Handler for LCRunHeader/LCRunHeaderImpl objects.
 * 
 * @author gaede
 * @version Mar 11, 2003
 * fg 20030609 using data interface for writing
 */
  class SIORunHeaderHandler : public SIO_block{
    
  protected:
    SIORunHeaderHandler() : SIO_block("UNKNOWN") { /* no default c'tor*/  ;} 

  public:
    
    /** C'tor for writing
     */
    SIORunHeaderHandler(const std::string& name) ;
    /** C'tor for reading.
     */
    SIORunHeaderHandler(const std::string& name, IMPL::LCRunHeaderImpl** rhP) ;
    virtual ~SIORunHeaderHandler() ;
    
    // interface from SIO_block
    virtual unsigned int   xfer( SIO_stream*, SIO_operation, unsigned int ) ;
    virtual unsigned int   version() ;
    
    void setRunHeader(const DATA::LCRunHeaderData* hdr ) ; 
    
  private: 
    IMPL::LCRunHeaderImpl** _rhP ;  // address for reading
    const DATA::LCRunHeaderData* _hdr ;  // runheader for writing
    
  }; // class
  
}; // namespace

#endif /* ifndef SIO_SIORUNHEADERHANDLER_H */
