#include "SIO/RunEventMap.h"

#include <iostream>

namespace SIO {

  bool operator < ( const RunEvent& r0, const RunEvent& other) {

    //std::cout << r0 << " < " << other << " : ["
    //<<  ( r0.RunNum == other.RunNum ?  r0.EvtNum < other.EvtNum  :  r0.RunNum < other.RunNum )  << "]" << std::endl;

    if( r0.EvtNum < 0 ) { // sort run records (evtNu == -1 ) first

      return ( other.EvtNum < 0 ?  r0.RunNum < other.RunNum : true ) ;
    }
    else if( other.EvtNum < 0 ) return false ;

    return ( r0.RunNum == other.RunNum ?  r0.EvtNum < other.EvtNum  :  r0.RunNum < other.RunNum ) ;
  }

  //----------------------------------------------------------------------------

  std::ostream & operator<<(std::ostream& os, const RunEvent& re ) {

    os << " run: " << re.RunNum << " - evt: " << re.EvtNum ;

    return os ;
  }

  //----------------------------------------------------------------------------

  void RunEventMap::add(const RunEvent& re, long64 pos ) {
    auto p = _map.insert( std::make_pair( (long64) re , pos ) ) ;
    // if event/run exists don't count as new entry
    if( p.second ) {
      if( re.EvtNum > -1 ) {
        ++_nEvt ;
      }
      else {
        ++ _nRun ;
      }

    }
    // overwrite with new entry
    else {
      p.first->second = pos ;
    }
  }

  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------

  RunEventMap::long64 RunEventMap::getPosition( long64 re ) {
    auto it = _map.find( re ) ;
    return ( it != _map.end() ? it->second : npos ) ;
  }

  //----------------------------------------------------------------------------

  RunEvent RunEventMap::minRunEvent() const {
    if( _map.empty() ) {
      return RunEvent( -1 , -1 ) ;
    }
    return _map.begin()->first ;
  }

  //----------------------------------------------------------------------------

  RunEvent RunEventMap::maxRunEvent() const {
    if( _map.empty() ) {
      return RunEvent( -1 , -1 ) ;
    }
    auto it =  _map.rbegin() ;
    return it->first ;
  }

  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------

  std::ostream & operator<<(std::ostream& os, const RunEventMap& rm ) {
    os << " ------- RunEventMap : " << std::endl ;
    for( auto it = rm.begin() ; it != rm.end() ; ++it ) {
      os << "  " << RunEvent( it->first ).RunNum << ", " << RunEvent( it->first ).EvtNum << "  :  " << it->second << std::endl;
    }
    return os ;
  }

}
