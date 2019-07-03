#ifndef SIO_SIOHANDLERMGR_H
#define SIO_SIOHANDLERMGR_H 1

// -- std headers
#include <map>
#include <string>
#include <memory>

// -- lcio headers
#include "SIO/SIOObjectHandler.h"

namespace SIO {

/** Holds instances of all types of SIOObjectHandlers.
 * New types need to have their handler classes registered here.
 *
 * @author gaede
 * @version $Id: SIOHandlerMgr.h,v 1.5 2005-04-15 08:37:42 gaede Exp $
 */
  class SIOHandlerMgr {
    typedef  std::map< std::string, std::shared_ptr<SIOObjectHandler> > SIOHandlerMap ;
  public:
    SIOHandlerMgr() ;
    ~SIOHandlerMgr() = default ;

    /** Returns the instance of the handler for the given type - null if it doesn't exist.
     */
    std::shared_ptr<SIOObjectHandler> getHandler( const std::string& type ) const ;

    /** Registers an instance of a handler for the given type.
     *  Fails if type name allready exists.
     */
    void registerHandler( const std::string& type, std::unique_ptr<SIOObjectHandler> handler ) ;

  private:
    /// The handler map
    SIOHandlerMap         _map {};
  }; // class

} // namespace
#endif /* ifndef SIO_SIOHANDLERMGR_H */
