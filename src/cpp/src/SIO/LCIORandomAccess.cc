#include "SIO/LCIORandomAccess.h"


namespace SIO{

  
  bool operator < ( const RunEvent& r0, const RunEvent& other) { 
    
    //std::cout << r0 << " < " << other << " : [" 
    //<<  ( r0.RunNum == other.RunNum ?  r0.EvtNum < other.EvtNum  :  r0.RunNum < other.RunNum )  << "]" << std::endl;
    
    if( r0.EvtNum < 0 ) { // sort run records (evtNu == -1 ) first 
      
      return ( other.EvtNum < 0 ?  r0.RunNum < other.RunNum : true ) ;
    }
    else if( other.EvtNum < 0 ) return false ;
    
    return ( r0.RunNum == other.RunNum ?  r0.EvtNum < other.EvtNum  :  r0.RunNum < other.RunNum ) ; 
  } 
  
  std::ostream & operator<<(std::ostream& os, const RunEvent& re ) {
    
    os << " run: " << re.RunNum << " - evt: " << re.EvtNum ;
    
    return os ;
  }
  
  bool operator<(const LCIORandomAccess ra0, const LCIORandomAccess& other) {  
    
    return ra0._maxRunEvt < other._minRunEvt ;
  } 

  std::ostream& operator<<(std::ostream& os, const LCIORandomAccess& ra ){

    os << " LCIORandomAccess:  ----------------------- " << std::endl  
       << " minRun " <<  ra._minRunEvt.RunNum << std::endl
       << " minEvt " <<  ra._minRunEvt.EvtNum  << std::endl
       << " maxRun " <<  ra._maxRunEvt.RunNum << std::endl  
       << " maxEvt " <<  ra._maxRunEvt.EvtNum << std::endl  
       << " nRunHeaders " <<  ra._nRunHeaders << std::endl 
       << " nEvents " <<  ra._nEvents  << std::endl
       << " recordsAreInOrder " <<  ra._recordsAreInOrder  << std::endl  
       << " indexLocation " <<  ra._indexLocation  << std::endl
       << " prevLocation " <<  ra._prevLocation  << std::endl
       << " nextLocation " <<  ra._nextLocation  << std::endl
       << " firstRecordLocation " <<  ra._firstRecordLocation  << std::endl
       << "---------------------------------------------"  
       << std::endl ;

    return os ;
  }

}



