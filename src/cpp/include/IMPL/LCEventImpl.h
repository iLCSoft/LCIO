#ifndef EVENT_LCEVENTIMPL_H
#define EVENT_LCEVENTIMPL_H 1

#include <string>
#include <map>
#include "EVENT/LCEvent.h"
#include "EVENT/LCCollection.h"
#include "EVENT/LCIO.h"
#include "AccessChecked.h"

namespace IMPL{

  //class  EVENT::LCCollection ;
 
  typedef std::map<std::string,EVENT::LCCollection*> LCCollectionMap ; 
  
  /**Implementation of the main event class. This is used by  
   * LCIO for reading events from file.
   * 
   * @author gaede
   * @version Mar 5, 2003
   * @see LCEvent
   * @see LCCollection
   */
class LCEventImpl : public EVENT::LCEvent, public AccessChecked {
    
  public: 
    LCEventImpl() ;
    /** Copy contructor, creates a deep copy of the event.
     * Not yet - needs pointer chasing ...
     */
    //    LCEventImpl(const EVENT::LCEvent& evt) ;  // copy c'tor
    
    /**Destructor.
     */
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
    virtual const std::vector<std::string>  * getCollectionNames() const;
    
    /** Returns the collection for the given name.
     * Same as getCollection() except that no cast to (LCCollection) and check for 
     * NULL pointer/reference is needed.  
     *
     * @throws NotAvailableException
     */
    EVENT::LCCollection * getCollection(const std::string & name) const 
      throw (EVENT::DataNotAvailableException, std::exception) ;

//     /** Returns the collection for the given name - null if it doesn't exist.
//      *  Returns the identical object as getCollection()  except for the type.
//      */ 
//     EVENT::LCCollection * getCollection(const std::string & name) const ;


    /** Adds a collection with the given name. Throws an exception if the name already
     * exists in the event. NB: Adding collections is allowed even when the event is 'read only'.
     * 
     *@throws EventException
     */ 
    virtual void addCollection(EVENT::LCCollection * col, const std::string & name)  
      throw (EVENT::EventException, std::exception) ;
    
    /** Removes (and deletes) the collection with name (if it exists in the event). 
     * Throws an exception if the event is 'read only' as defined by the read mode in LCReader.
     *
     *@throws ReadOnlyException
     */ 
    virtual void removeCollection(const std::string & name) throw (EVENT::ReadOnlyException, std::exception)  ;
    
    //---- set methods -----
    /** Sets the run number.
     */
    void setRunNumber( int rn ) ;
    
    /** Sets the event number.
     */
    void setEventNumber( int en ) ;
    
    /** Sets the detector name.
     */
    void setDetectorName(const std::string& dn ) ;
    
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
    mutable std::vector<std::string> _colNames ;
    
    
  }; // class

}; // namespace IMPL

#endif /* ifndef EVENT_LCEVENTIMPL_H */
