#ifndef SIO_SIOHANDLERMGR_H
#define SIO_SIOHANDLERMGR_H 1

#include<map>
#include<string>
#include "SIO/SIOObjectHandler.h"


namespace SIO {

  typedef  std::map< std::string, SIOObjectHandler* > SIOHandlerMap ;
  
/** Singleton that holds instances of all types of SIOObjectHandlers.
 * New types need to have their handler classes registered here. 
 *
 * @author gaede
 * @version $Id: SIOHandlerMgr.h,v 1.5 2005-04-15 08:37:42 gaede Exp $
 */
  class SIOHandlerMgr {
    
  private:
    
    SIOHandlerMgr() ; // no public c'tor for a singleton
    
  public:
    
    ~SIOHandlerMgr() ;
    
    /** Returns the instance of this class.
     */
    static SIOHandlerMgr* instance() ;
    
    /**Returns the instance of the handler for the given type - null if it doesn't exist.
     */
    SIOObjectHandler* getHandler( const std::string& type ) ;
    
    /** Registers an instance of a handler for the given type. 
     *  Fails if type name allready exists.
     */
    bool registerHandler( const std::string& type, SIOObjectHandler* handler ) ;
    
    
  private:
    
    SIOHandlerMap _map{} ;
    
    static SIOHandlerMgr* _me ;
    
    
  }; // class
  
} // namespace
#endif /* ifndef SIO_SIOHANDLERMGR_H */
