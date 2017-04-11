#ifndef SIO_RunEventMap_H
#define SIO_RunEventMap_H 1

#include <map>

#include "LCIORandomAccess.h"

namespace SIO {

  class RunEventMap ; 

  std::ostream & operator<<(std::ostream& os, const RunEventMap& rm ) ;


/** Map that holds positions of Run and Event records.
 * 
 * @author gaede
 * @version $Id: RunEventMap.h,v 1.3 2011-03-03 16:00:12 gaede Exp $
 */
  class RunEventMap {


    friend std::ostream & operator<<(std::ostream& os, const RunEventMap& rm ) ;
   
  public:

    typedef std::map< RunEvent, long64> MapType ;
    typedef MapType::iterator Map_IT ;
    typedef MapType::const_iterator Map_cIT ;

    static const int NPos = -1 ;

    RunEventMap() ; 
    ~RunEventMap() ;
    
    /** Add a new entry to the event map - if the RunEvent already exists the new position will be stored.
     */
    void add(const RunEvent& re, long64 pos ) ;
    
    /** Return the position of the specified Event record or Run record respectively (if EventNum == -1 ).
     *  Returns RunEventMap::NPos if no record found.
     */
    long64 getPosition(const RunEvent& re ) {
      return getPosition( long64( re ) ) ;
    }

    /** Same as getPosition(const RunEvent& re ). 
     */
    long64 getPosition( long64 re ) ; 
   

    /* Return the minimal run event object - (-1,-1) if empty.
    */
    RunEvent minRunEvent() ;
    
    /* Return the maximum run event object - (-1,-1) if empty.
    */
    RunEvent maxRunEvent() ;


    int getNumberOfEventRecords() const { return _nEvt ; }

    int getNumberOfRunRecords() const  { return _nRun ; }

    // map funcitons
    size_t size() { return _map.size()  ; } 

    Map_IT begin()  { return _map.begin() ; }
    Map_IT end()  { return _map.end() ; }
    Map_cIT begin() const  { return _map.begin() ; }
    Map_cIT end()  const { return _map.end() ; }
    bool empty() const { return _map.empty() ; }

    template <class InputIterator>
    void insert(InputIterator b, InputIterator e) { _map.insert( b , e  ) ; } 

    void clear() {  _map.clear() ; _nRun=0 ; _nEvt =0 ; }

  protected: 

    MapType _map{} ;

    int _nRun{0} ;
    int _nEvt{0} ;

  }; // class
 } // namespace

#endif /* ifndef SIO_RunEventMap_H */
