#include "SIO/RunEventMap.h"

//#include "EVENT/LCIO.h"

#include <iostream>


namespace SIO  {

  const int RunEventMap::NPos ;

  RunEventMap::RunEventMap() : 
    _nRun( 0 ) ,  
    _nEvt( 0 ) {
  }

  RunEventMap::~RunEventMap(){ }


  void RunEventMap::add(const RunEvent& re, long64 pos ) {

    std::pair< Map_IT, bool> p = 
      _map.insert( std::make_pair( (long64) re   , pos ) ) ; 

//     std::cout << " *****************RunEventMap::add : " << re.RunNum <<", "<< re.EvtNum 
// 	      << " this: " << this
// 	      << *this << std::endl ;

    if( p.second ) {  // if event/run exists don't count as new entry

      if( re.EvtNum > -1 ) 
	++ _nEvt ;
      else {
	++ _nRun ;
	//	std::cout << " *****************RunEventMap::add : " << re << std::endl ;
      }
    } else {    // overwrite with new entry
      
      //      std::cout << " *****************RunEventMap::add - overwriting : " << re << " -> "  <<   (long64) re  <<
      //	std::hex <<  "0x" << (long64) re   << std::dec << std::endl ;

      p.first->second = pos ;

    }
  }

  long64  RunEventMap::getPosition( long64 re ) {
    
    Map_IT it = _map.find( re ) ;

    return ( it != _map.end() ? it->second  :  NPos ) ;
  } 


  RunEvent RunEventMap::minRunEvent() {
    
    if( _map.empty() )
      return RunEvent( -1 , -1 ) ;

    return _map.begin()->first ;

  }
  
  RunEvent RunEventMap::maxRunEvent() {

    if( _map.empty() )
      return RunEvent( -1 , -1 ) ;

    Map_IT it =  _map.end() ;

    --it ;

    return it->first ;

  }

  std::ostream & operator<<(std::ostream& os, const RunEventMap& rm ) {

    os << " ------- RunEventMap : " << std::endl ;

    for( RunEventMap::Map_cIT it = rm.begin() ; it != rm.end() ; ++it ){
      
      os << "  " << RunEvent( it->first).RunNum 
	 << ", " << RunEvent( it->first).EvtNum << "  :  " << it->second << std::endl;    }
    
    return os ;
  }


}
