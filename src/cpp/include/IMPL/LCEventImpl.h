#ifndef EVENT_LCEVENTIMPL_H
#define EVENT_LCEVENTIMPL_H 1

#include <string>
#include <map>
#include "EVENT/LCEvent.h"
#include "EVENT/LCCollection.h"


namespace IMPL{
  
  typedef std::map<std::string,EVENT::LCCollection*> LCCollectionMap ; 
  
  /**Implementation of the main event class. This is used by  
   * LCIO for reading events from file.
   * 
   * @author gaede
   * @version Mar 5, 2003
   * @see LCEvent
   * @see LCCollection
   */
class LCEventImpl : public EVENT::LCEvent {
    
  public: 
    LCEventImpl() ;
    /** Copy contructor, creates a deep copy of the event.
     * Not yet - needs pointer chasing ...
     */
    //    LCEventImpl(const EVENT::LCEvent& evt) ;  // copy c'tor
    
    /// Destructor.
    virtual ~LCEventImpl() ; 
    
    /** Return the run number off this event.
     */
    virtual int getRunNumber() const ;
    
    /** Returns this event's number .
     */
    virtual int getEventNumber() const ;
    
    /** Returns the name of the detector setup used in the simulation.
     */
    virtual const std::string & getDetectorName() const ;
    
    /** Returns the time stamp of the event.
     */
    virtual long getTimeStamp() const ;
    
    /** Returns the names of the collections in the  event.
     */
    virtual std::vector<std::string>* getCollectionNames() const ;
    
    /** Returns the collection for the given name - null if it doesn't exist.
     */ 
    virtual EVENT::LCCollection * getCollection(const std::string & name) const ;
    
    /** Adds a collection with the given name. This method should fail if the event 
     * is read only and return LCIO::ERROR.
     */ 
    virtual int addCollection(EVENT::LCCollection * col, const std::string & name) ;
    
    /** Removes (and deletes) the collection with the given name. This method should fail if the event 
     * is read only and return LCIO::ERROR.
     */ 
    virtual int removeCollection(const std::string & name) ;
    
    //---- set methods -----
    /** Sets the run number.
     */
    void setRunNumber( int rn ) ;
    
    /** Sets the event number.
     */
    void setEventNumber( int en ) ;
    
    /** Sets the detector name.
     */
    void setDetectorName(const char* dn ) ;
    
    /** Sets the event time stamp.
     */
    void setTimeStamp(long ts) ;
       
     
  protected:
    void setAccessMode( int accessMode ) ;
    
    
    // data members - declared protected to be accessible 
    // for friends of sub classes ...
    
  protected:  
    int _runNumber ;
    int _eventNumber ;
    long long _timeStamp ;
    std::string _detectorName ;
    
    // map has to be defined mutable in order to use _map[]  for const methods ...
    mutable LCCollectionMap _map ;
    
  private:
    int _access ;   // flag for access mode 
    
    
  }; // class

}; // namespace IMPL

#endif /* ifndef EVENT_LCEVENTIMPL_H */
