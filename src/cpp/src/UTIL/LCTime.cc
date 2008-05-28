#include "UTIL/LCTime.h"
#include "Exceptions.h"
#include <time.h>

#include <sstream>
#include <iomanip>
#include <cstdlib>

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
    
    // do some sanity checks on date:
    if(  year  < YEAR0               || 
	 month < 0     || month > 12 ||
	 day   < 0     || day   > daysInMonth( month , year ) ){

      throw Exception( "LCTime::LCTime() invalid date:"+ getDateString() ) ;
    } 


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

    while( (daysLeft = nDays - daysInYear( _d.year ) )  >= 0 ){
      nDays = daysLeft ;
      _d.year ++ ;
    }
    
//     std::cout << " ---- daysLeft : " << daysLeft << "  year: " <<   _d.year << std::endl ;
    
    while( (daysLeft = nDays - daysInMonth( _d.month ,  _d.year ) ) >= 0 ){
      nDays = daysLeft ;
      _d.month ++ ;
    }
    
//     std::cout << " ---- daysLeft : " << daysLeft << "  month: " <<   _d.month << std::endl ;
    
    _d.day +=  nDays ;
    
//     std::cout << " ---- daysLeft : " <<  _d.day << std::endl ;
    

//     if( _d.day > daysInMonth( _d.month ,  _d.year ) ) { // happens on first day of a month
//       if( _d.month == 12 ) {
// 	_d.year += 1 ;
// 	_d.month = 1 ;
//       } else {
// 	_d.month += 1 ; 
//       }
//       _d.day = 1 ;
//     }



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


  bool LCTime::test( int nDates ) {
    
    std::cout << "LCTime::test: test LCTime with " << nDates << " random dates " << std::endl ;

    LCTime lcTime ;

    for(int i=0; i<nDates;i++){

      CalendarTime date ;
      date.year  = int( YEAR0  +  75  * ( 1.*std::rand()/RAND_MAX )  ) ;
      date.month = int( 1     +  12  * ( 1.*std::rand()/RAND_MAX )  ) ;

      date.day   = int( 1  +  lcTime.daysInMonth( date.month, date.year) * ( 1.*std::rand()/RAND_MAX )  ) ;
      date.hour  = int( 0  +  24 * ( 1.*std::rand()/RAND_MAX )  ) ;
      date.min   = int( 0  +  60 * ( 1.*std::rand()/RAND_MAX )  ) ;
      date.sec   = int( 0  +  60 * ( 1.*std::rand()/RAND_MAX )  ) ;
      date.ns    =int(  0   ) ;



      LCTime testTime0( (int) date.year, (int) date.month, (int) date.day, 
			(int) date.hour, (int) date.min, (int) date.sec ) ;

      LCTime testTime1( testTime0.timeStamp() ) ;


//       if( date.day == lcTime.daysInMonth( date.month, date.year) ) {
// 	std::cout << " last day in month : " << testTime0.getDateString() << std::endl ;
//       }

//       std::cout << " random date: " 
// 		<< setfill ('0') 
// 		<< std::setw(2) << (int) date.day 
// 		<< std::setw(1) << "." 
// 		<< std::setw(2) << (int) date.month 
// 		<< std::setw(1) << "." 
// 		<< std::setw(4) << date.year
// 		<< std::setw(1) << "  " 
// 		<< std::setw(2) << (int) date.hour 
// 		<< std::setw(1) << ":"   
// 		<< std::setw(2) << (int) date.min 
// 		<< std::setw(1) << ":"   
// 		<< std::setw(2) << (int) date.sec 
// 		<< std::setw(1) << "."   
// 		<< std::setw(9) << (int) date.ns  
// 		<< std::endl ;
 
//       std::cout << " time0 : " << testTime0.getDateString() << std::endl ;
//       std::cout << " time1 : " << testTime1.getDateString() << std::endl ;
//       std::cout << "--------------------------------------------------------" << std::endl ;


      if( testTime0.getDateString() !=  testTime1.getDateString() ){
	
	std::cout <<  " Erorr: incompatible date strings found: " << std::endl ;
	
	std::cout << " time0 : " << testTime0.getDateString() << std::endl ;
	std::cout << " time1 : " << testTime1.getDateString() << std::endl ;
	
      }
      if( date.year  !=  testTime0.year()  ||  
	  date.month !=  testTime0.month() || 
	  date.day   !=  testTime0.day()   ||    
	  date.hour  !=  testTime0.hour()  ||   
	  date.min   !=  testTime0.min()   ||    
	  date.sec   !=  testTime0.sec()   ||    
	  date.ns    !=  testTime0.ns () ) {

	std::cout <<  " Erorr: wrong date in LCTime: " << std::endl ;
	std::cout << " random date: " 
		  << setfill ('0') 
		  << std::setw(2) << (int) date.day 
		  << std::setw(1) << "." 
		  << std::setw(2) << (int) date.month 
		  << std::setw(1) << "." 
		  << std::setw(4) << date.year
		  << std::setw(1) << "  " 
		  << std::setw(2) << (int) date.hour 
		  << std::setw(1) << ":"   
		  << std::setw(2) << (int) date.min 
		  << std::setw(1) << ":"   
		  << std::setw(2) << (int) date.sec 
		  << std::setw(1) << "."   
		  << std::setw(9) << (int) date.ns  
		  << std::endl ;

	std::cout << " time0 :      " << testTime0.getDateString() << std::endl ;
	
      }

    }

    return true ;
  }

}
