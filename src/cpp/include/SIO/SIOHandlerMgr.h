#ifndef SIO_SIOHANDLERMGR_H
#define SIO_SIOHANDLERMGR_H 1

// -- std headers
#include <map>
#include <string>

// -- lcio headers
#include "SIO/SIOObjectHandler.h"

namespace SIO {

  typedef  std::map< std::string, SIOObjectHandler* > SIOHandlerMap ;
  
/** Holds instances of all types of SIOObjectHandlers.
 * New types need to have their handler classes registered here. 
 *
 * @author gaede
 * @version $Id: SIOHandlerMgr.h,v 1.5 2005-04-15 08:37:42 gaede Exp $
 */
  class SIOHandlerMgr {
  public:
    SIOHandlerMgr() ;
    ~SIOHandlerMgr() ;
    
    /** Returns the instance of the handler for the given type - null if it doesn't exist.
     */
    SIOObjectHandler* getHandler( const std::string& type ) const ;
    
    /** Registers an instance of a handler for the given type. 
     *  Fails if type name allready exists.
     */
    bool registerHandler( const std::string& type, SIOObjectHandler* handler ) ;
    
  private:
    
    SIOHandlerMap _map{};
  }; // class
  
} // namespace
#endif /* ifndef SIO_SIOHANDLERMGR_H */
