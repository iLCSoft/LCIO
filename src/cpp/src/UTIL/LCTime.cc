#include "UTIL/LCTime.h"
#include "Exceptions.h"
#include <time.h>

#include <sstream>
#include <iomanip>

// the canonical year zero
#define YEAR0 1970 

// seconds per min, hour, day
#define SPM 60
#define SPH 3600   // 60*60
#define SPD 86400  // 60*60*24

#define DPY 365 // days per year - no leap year
#define NPS 1000000000LL // ns per s - need 64bit to force result of conversion to be 64bit

using namespace EVENT ;

#include <iostream>
using namespace std ;

namespace UTIL{

  int LCTime::dpm[13]   = { 0,31,28,31,30,31,30,31,31,30,31,30,31 } ;


  LCTime::LCTime(){

    time_t now ;
    time ( &now ) ;

    _t = now * NPS ;

    convertToCalTime() ;


//     // selftest :
//     cout << "--- date : " << getDateString() 
// 	 << endl 
// 	 << "--- time: " << _t << endl 
// 	 << endl ;

//     convertFromCalTime() ;
//     convertToCalTime() ;

//     cout << "after reconversion  " << endl ;
//     // selftest :
//     cout << "--- date : " << getDateString() 
// 	 << endl 
// 	 << "--- time: " << _t << endl 
// 	 << endl ;

  }

  LCTime::LCTime( long64 time) : _t( time )  {

    convertToCalTime() ;
  }


  LCTime::LCTime(int year, int month, int day, int hour, int min, int s ){
    

    _d.year = year  ;
    _d.month = month  ;
    _d.day = day  ;
    _d.hour = hour  ;
    _d.min = min  ;
    _d.sec = s  ;
    _d.ns = 0 ; 
    
    convertFromCalTime() ;
  }
  
  LCTime::LCTime( int unixTime ) {

    _t = unixTime * NPS ;

    convertToCalTime() ;
  }
  


  void LCTime::convertToCalTime(){

    // initialize date to 1.1.1970
    _d.year =  YEAR0  ;
    _d.month = 1 ; 
    _d.day = 1 ;
    _d.hour = 0 ;
    _d.min = 0 ;
    _d.sec = 0 ;
    _d.ns = 0 ;

    // don't support negative times 
    if( _t < 0LL ) {      
      _t = 0 ;
      return ;  // map to 1.1.1970 00:00:00
    }

    // start with unix time, i.e. seconds since 1.1.1970
    long64 s = _t / NPS ; 
    _d.ns = _t % NPS ;

    // total number of days 
    int nDays = s / SPD ;

    int daysLeft ;

    while( (daysLeft = nDays - daysInYear( _d.year ) )  > 0 ){
      nDays = daysLeft ;
      _d.year ++ ;
    }
    while( (daysLeft = nDays - daysInMonth( _d.month ,  _d.year ) ) > 0 ){
      nDays = daysLeft ;
      _d.month ++ ;
    }
    
    _d.day +=  nDays ;

    s = s % SPD ; // the seconds of the current day

    _d.hour = s / SPH ; // the hours of the current day

    s = s % SPH ; // the seconds of the current hour
    
    _d.min  = s / SPM ; // the min of the current hour

    _d.sec = s % SPM  ;  // secs

  }
  
  void LCTime::convertFromCalTime(){
    
    // don't support dates before 1.1.1970 00:00:00
    if( _d.year < YEAR0 ) {   // map to 1.1.1970 00:00:00
      _t = -1  ;  
      convertToCalTime() ;
    }
    
    // add up all days that have passed
    unsigned nDays = 0 ; 
    
    for( int y = YEAR0 ; y < _d.year ; y ++ ){  
      nDays += daysInYear( y ) ; 
    }

    for( int m = 1 ;  m < _d.month ; m ++ ){
      nDays += daysInMonth( m , _d.year ) ; 
    }

    nDays += _d.day - 1 ;  // current day hasn't passed yet

    // add up the seconds
    _t = nDays * SPD + _d.hour  * SPH + _d.min * SPM + _d.sec ;
 
    // now make ns
    _t *= NPS ;

  }

   
  int LCTime::daysInYear( int year ) {
    int d = DPY ;
    if( isLeapYear( year ) ) d++ ;
    return d ;
  }
  
  int LCTime::daysInMonth( int month , int year ) {
    
    if( month < 1 || month > 12 ) 
      return 0 ;
    int d = dpm[ month ] ;
    
    if( month == 2 &&  isLeapYear( year ) ) d++ ;
    return d ;
  }
  
  
  void LCTime::operator+=(EVENT::long64 t) {
    _t += t ;
    convertToCalTime() ;
  }



  std::string LCTime::getDateString() {

    std::stringstream out ;

    // dd.mm.yyyy  hh:mm:ss._ms_us_ns
    out << setfill ('0') 
	<< std::setw(2) << (int) _d.day 
	<< std::setw(1) << "." 
	<< std::setw(2) << (int) _d.month 
	<< std::setw(1) << "." 
	<< std::setw(4) << _d.year
	<< std::setw(1) << "  " 
 	<< std::setw(2) << (int) _d.hour 
	<< std::setw(1) << ":"   
	<< std::setw(2) << (int) _d.min 
	<< std::setw(1) << ":"   
	<< std::setw(2) << (int) _d.sec 
	<< std::setw(1) << "."   
	<< std::setw(9) << (int) _d.ns  ;  
    
    return out.str() ;
  }

} ;
