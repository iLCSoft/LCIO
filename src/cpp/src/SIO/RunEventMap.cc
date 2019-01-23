#include "SIO/RunEventMap.h"

//#include "EVENT/LCIO.h"

#include <iostream>


namespace SIO  {

  const int RunEventMap::npos ;

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

  long64 RunEventMap::getPosition( long64 re ) {
    auto it = _map.find( re ) ;
    return ( it != _map.end() ? it->second : npos ) ;
  } 

  //----------------------------------------------------------------------------

  RunEvent RunEventMap::minRunEvent() {
    if( _map.empty() ) {
      return RunEvent( -1 , -1 ) ;
    }
    return _map.begin()->first ;
  }
  
  //----------------------------------------------------------------------------
  
  RunEvent RunEventMap::maxRunEvent() {
    if( _map.empty() ) {
      return RunEvent( -1 , -1 ) ;
    }
    auto it =  _map.rbegin() ;
    return it->first ;
  }
  
  //----------------------------------------------------------------------------

  std::ostream & operator<<(std::ostream& os, const RunEventMap& rm ) {
    os << " ------- RunEventMap : " << std::endl ;
    for( auto it = rm.begin() ; it != rm.end() ; ++it ) {
      os << "  " << RunEvent( it->first ).RunNum << ", " << RunEvent( it->first ).EvtNum << "  :  " << it->second << std::endl;
    }
    return os ;
  }
  
}
