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
    

    enum {
      subdetID = 0 ,
      sideID   = 1 ,
      layerID  = 2 ,
      moduleID = 3 ,
      sensorID = 4 ,
    } ;


  public:

    static LCTrackerCellID& instance() { 
      static LCTrackerCellID _me ;
      return _me ;
    } 

    /// index of subdet in cellID
    static unsigned subdet() { return subdetID ; }

    /// index of side in cellID
    static unsigned side()  { return sideID ; }

    /// index of layer in cellID
    static unsigned layer()  { return layerID ; }

    /// index of module in cellID
    static unsigned module()  { return moduleID ; }

    /// index of sensor in cellID
    static unsigned sensor() { return sensorID ; }


    /// c'tor initialize the encoding string with the 'canonical' encoding 
    LCTrackerCellID() : _encoding("subdet:5,side:-2,layer:9,module:8,sensor:8" ),
		       _accessed(false) {}
    
    // get the current encoding string
    static const std::string& encoding_string() {
      instance()._accessed = true;
      return instance()._encoding ;
    }
    
    /// return a string with the details of the given id:
    static std::string valueString( unsigned val ){
      instance().setAccessed();
      UTIL::BitField64 encoder( instance().encoding_string() ) ;
      encoder.setValue( val ) ;
      return encoder.valueString() ;
    }
    
    /// set the encoding string. Throws exception if it was already accessed to prevent inconsistencies
    void set_encoding_string( const std::string& encodingString ) {
      if( _accessed ) throw std::logic_error( "The encoding string was already accessed! Changing it now will lead to inconsistencies! Fix your code!" );

      bool isValid = true ;

      unsigned long long subdetPos = encodingString.find( "subdet" );
      unsigned long long systemPos = encodingString.find( "system" );
      unsigned long long sidePos   = encodingString.find( "side" ) ;
      unsigned long long layerPos  = encodingString.find( "layer" ) ;
      unsigned long long modulePos = encodingString.find( "module" ) ;
      unsigned long long sensorPos = encodingString.find( "sensor" ) ;

      isValid = ( (subdetPos  != std::string::npos || systemPos != std::string::npos) &&
		  sidePos    != std::string::npos &&
		  layerPos   != std::string::npos &&
		  modulePos  != std::string::npos &&
		  sensorPos  != std::string::npos &&
		  (
		   (( subdetPos < sidePos ) || (systemPos < sidePos ) ) &&
		   ( sidePos < layerPos ) &&
		   ( layerPos < modulePos ) &&
		   ( modulePos < sensorPos)
		  )
		);

      if( ! isValid ) throw std::runtime_error(" LCTrackerCellID::set_encoding_string(): string needs to contain"
					       " \"subdet:A,side:B,layer:C,module:D,sensor:E\" " ) ;

      _encoding = encodingString;
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
