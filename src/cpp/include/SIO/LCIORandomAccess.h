#ifndef SIO_LCIORandomAccess_H
#define SIO_LCIORandomAccess_H 1

//#include "EVENT/LCObject.h"
#include "LCIOTypes.h"

#include <iostream>


typedef EVENT::long64 long64 ;

namespace SIO{ // IO or IMPL ?

  /** Helper struct that stores run and event positions in the file. The operator<() defines a lexicographical ordering
   *  in RunNum and EvtNum where all run records (EvtNum=-1) are orderd first. 
   */
  struct RunEvent{
    RunEvent(): RunNum( 0 ), EvtNum( 0 ) {}
    RunEvent(int run, int evt): RunNum( run ), EvtNum( evt ) {}
    RunEvent(long64 runEvt): RunNum( (runEvt >> 32 ) & 0xffffffff  ), EvtNum( runEvt &  0xffffffff ) {}
    int RunNum ;
    int EvtNum ;
    
    operator long64() const {  return  ( long64( RunNum ) << 32  |  ( long64(EvtNum) & 0xffffffff )  ) ; } 
  };
  
  std::ostream & operator<<(std::ostream& os, const RunEvent& re ) ;
  
  bool operator < ( const RunEvent& r0, const RunEvent& other)  ;
  

  class SIORandomAccessHandler ;
  class LCIORandomAccess ;
  class LCIORandomAccessMgr ;
  
  std::ostream & operator<<(std::ostream& os, const LCIORandomAccess& ra ) ;

  bool operator < (const LCIORandomAccess ra0, const LCIORandomAccess& other)  ;


/**  Implementation class for LCIORandomAccess records.
 *
 * @author gaede
 * @version $Id: LCIORandomAccess.h,v 1.2 2010-06-22 13:49:54 gaede Exp $
 */
//  class LCIORandomAccess : public EVENT LCObject {
  class LCIORandomAccess {
    
    friend class SIORandomAccessHandler ;
    friend class LCIORandomAccessMgr ;
    friend std::ostream & operator<<(std::ostream& os, const LCIORandomAccess& ra ) ;
    friend bool operator < (const LCIORandomAccess ra0, const LCIORandomAccess& other)  ;

  public:
    
    virtual ~LCIORandomAccess(){ /* nop */; }
    
    long64 getIndexLocation() const  { return _indexLocation ; }
    long64 getPrevLocation()  const  { return _prevLocation ; }
    //   long64 getNextLocation()  const  { return _nextLocation ; }
    long64 getFirstRecordLocation() const  { return _firstRecordLocation ; }


    void setIndexLocation(long64 il)       { _indexLocation = il ; }
    void setPreviousLocation(long64 pl)    { _prevLocation = pl ; }
    //    void setNextLocation(long64 nl)        { _nextLocation = nl ; }
    void setFirstRecordLocation(long64 fl) { _firstRecordLocation = fl ; }

  protected:
    RunEvent _minRunEvt{} ;
    RunEvent _maxRunEvt{} ;
    int  _nRunHeaders{0} ;
    int  _nEvents{0} ;
    int  _recordsAreInOrder{0} ;  
    long64  _indexLocation{0} ;
    long64  _prevLocation{0} ;
    long64  _nextLocation{0} ;
    long64  _firstRecordLocation{0} ;
  }; // class



} // namespace 
#endif 
