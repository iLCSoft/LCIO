#ifndef EVENT_LCEVENTLAZYIMPL_H
#define EVENT_LCEVENTLAZYIMPL_H 1

// -- lcio headers
#include "IOIMPL/LCEventIOImpl.h"
#include "SIO/SIOHandlerMgr.h"

// -- std headers
#include <memory>

// -- sio headers
#include <sio/definitions.h>
#include <sio/buffer.h>

namespace IOIMPL {
    
  /**  
   * Implementation of the event class with a lazy record unpacking. 
   * This is used by the MT::LCReader LCIO file reader implementation.
   * SIO internal buffer are kept  
   * 
   * @author gaede
   * @version Mar 5, 2003
   * @see LCEvent
   * @see LCCollection
   */
  class LCEventLazyImpl : public IOIMPL::LCEventIOImpl {
  public: 
    /** Constructor
     */
    LCEventLazyImpl() = default ;
    
    /**Destructor.
     */
    ~LCEventLazyImpl() = default ; 
    
    /** 
     * Returns the collection for the given name.
     * Triggers the event unpacking
     *
     * @throws DataNotAvailableException
     */
    EVENT::LCCollection * getCollection(const std::string & name) const ;

    /** Returns the collection for the given name and transfers the ownership of the collection
     *  to the caller. The caller is responsible for deleting the collection _after_ the Event is 
     *  deleted. The collection is still in the event but flagged as transient 
     *  after the call returns.<br>
     *  This is usefull when you want to keep the collection for the next events.<br>
     *  Use with care!
     *  Triggers the event unpacking
     * @throws DataNotAvailableException
     */
    EVENT::LCCollection * takeCollection(const std::string & name) const ;
    
    /** Removes (and deletes) the collection with name (if it exists in the event). 
     * Throws an exception if the event is 'read only' as defined by the read mode in LCReader.
     *
     *@throws ReadOnlyException
     */ 
    void removeCollection(const std::string & name) ;
    
    /** Set the event record buffer. 
     */
    void setBuffer( const sio::record_info &recinfo, sio::buffer &&recordBuffer ) ;
    
  private:
    /** Unpack the event record
     */
    void unpackEvent() ;
    
    /** Post process the event after unpacking
     */
    void postProcessEvent() ;
    
    /** Setup the collection blocks before unpacking
     */
    void setupBlocks() ;
    
  private:
    std::unique_ptr<sio::buffer>     _recordBuffer {nullptr} ;
    sio::record_info                 _recordInfo {} ;
    sio::block_list                  _blocks {} ;
    bool                             _unpacked {false} ;
    SIO::SIOHandlerMgr               _eventHandlerMgr {} ;
  }; // class

} // namespace IOIMPL

#endif /* ifndef EVENT_LCEVENTLAZYIMPL_H */
