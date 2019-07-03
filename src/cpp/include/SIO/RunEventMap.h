#ifndef SIO_RunEventMap_H
#define SIO_RunEventMap_H 1

#include <ostream>
#include <map>

namespace SIO {

  class RunEventMap ;

  /** Helper struct that stores run and event positions in the file. The operator<() defines a lexicographical ordering
   *  in RunNum and EvtNum where all run records (EvtNum=-1) are orderd first.
   */
  struct RunEvent {
    typedef long long long64 ;

    RunEvent() = default ;
    ~RunEvent() = default ;
    RunEvent(int run, int evt): RunNum( run ), EvtNum( evt ) {}
    RunEvent(long64 runEvt): RunNum( (runEvt >> 32 ) & 0xffffffff  ), EvtNum( runEvt &  0xffffffff ) {}
    operator long64() const {  return  ( ( long64(RunNum) << 32)  |  ( long64(EvtNum) & 0xffffffff )  ) ; }

    int RunNum {0} ;
    int EvtNum {0} ;
  };

  std::ostream & operator<<(std::ostream& os, const RunEvent& re ) ;
  bool operator < ( const RunEvent& r0, const RunEvent& other)  ;
  std::ostream & operator<<(std::ostream& os, const RunEventMap& rm ) ;


/** Map that holds positions of Run and Event records.
 *
 * @author gaede
 * @version $Id: RunEventMap.h,v 1.3 2011-03-03 16:00:12 gaede Exp $
 */
  class RunEventMap {
    typedef long long long64 ;
    friend std::ostream & operator<<(std::ostream& os, const RunEventMap& rm ) ;

  public:
    // traits
    typedef std::map< RunEvent, long64> map_type ;
    typedef map_type::iterator iterator ;
    typedef map_type::const_iterator const_iterator ;
    static constexpr const int npos = -1 ;

  public:
    RunEventMap() = default ;
    ~RunEventMap() = default ;

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
    RunEvent minRunEvent() const ;

    /* Return the maximum run event object - (-1,-1) if empty.
    */
    RunEvent maxRunEvent() const ;


    int getNumberOfEventRecords() const { return _nEvt ; }

    int getNumberOfRunRecords() const  { return _nRun ; }

    // map funcitons
    size_t size() { return _map.size()  ; }

    iterator begin()  { return _map.begin() ; }
    iterator end()  { return _map.end() ; }
    const_iterator begin() const  { return _map.begin() ; }
    const_iterator end()  const { return _map.end() ; }
    bool empty() const { return _map.empty() ; }

    template <class InputIterator>
    void insert(InputIterator b, InputIterator e) { _map.insert( b , e  ) ; }

    void clear() {  _map.clear() ; _nRun=0 ; _nEvt =0 ; }

  protected:
    map_type _map{} ;
    int _nRun{0} ;
    int _nEvt{0} ;
  }; // class
 } // namespace

#endif /* ifndef SIO_RunEventMap_H */
