#ifndef LCTrackerConf_h
#define LCTrackerConf_h 1

#include <string>
#include "UTIL/BitField64.h"

namespace UTIL {


  /** Singleton helper class defining the cellID encoding used
   *  in the LC tracking packages. It is assumed that the cellID  
   *  contains the elements subdet, side, layer, module and sensor
   *  in that order. The actual encoding string can be set once,
   *  but only before it is accessed for the first time.
   *  The default encoding string is:
   *  "subdet:5,side:-2,layer:9,module:8,sensor:8"
   *   
   *  [should replace UTIL::ILDCellID0 and DDKalTest::CellIDEncoding ]
   *
   *  @author F.Gaede, DESY
   *  @date Mar, 2017
   */
  class LCTrackerCellID{
    
  public:

    static LCTrackerCellID& instance() { 
      static LCTrackerCellID _me ;
      return _me ;
    } 

    /// index of subdet in cellID
    static const unsigned subdet = 0 ;

    /// index of side in cellID
    static const unsigned side   = 1 ;

    /// index of layer in cellID
    static const unsigned layer  = 2 ;

    /// index of module in cellID
    static const unsigned module = 3 ;

    /// index of sensor in cellID
    static const unsigned sensor = 4 ;


    /// c'tor initialize the encoding string with the 'canonical' encoding 
    LCTrackerCellID() : _encoding("subdet:5,side:-2,layer:9,module:8,sensor:8" ),
		       _accessed(false) {}
    
    // get the current encoding string
    const std::string& encoding_string() {
      _accessed = true;
      return _encoding ;
    }
    
    /// return a string with the details of the given id:
    static std::string valueString( unsigned val ){
      instance().setAccessed();
      UTIL::BitField64 encoder( instance().encoding_string() ) ;
      encoder.setValue( val ) ;
      return encoder.valueString() ;
    }
    
    /// set the encoding string. Throws exception if it was already accessed to prevent inconsistencies
    void set_encoding_string( const std::string& encoding_string ) {
      if( _accessed ) throw std::logic_error( "The encoding string was already accessed! Changing it now will lead to inconsistencies! Fix your code!" );
      _encoding = encoding_string;
    }
    
    /// set accessed to true
    void setAccessed() {
      _accessed = true;
    }


  protected:
    std::string _encoding ;
    bool _accessed;
  } ;

}

#endif
