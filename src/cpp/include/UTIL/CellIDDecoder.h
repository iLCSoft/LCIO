#ifndef CellIDDecoder_h
#define CellIDDecoder_h 1

//#include <vector>
//#include "EVENT/LCObject.h"
#include "EVENT/LCCollection.h"
//#include "EVENT/SimTrackerHit.h"
#include "UTIL/BitField64.h"
#include "lcio.h"
#include <string>

// fixes problem in gcc 4.0.3
#include "EVENT/LCParameters.h"
#include "LCIODeprecated.h"

//#include <sstream>
//#include <typeinfo>

namespace UTIL{


  /** Convenient class for decoding cellIDs from collection parameter LCIO::CellIDEncoding.
   *  See UTIL::BitField64 for a description of the encoding string. 
   * 
   *  @see BitField64
   *  @version $Id: CellIDDecoder.h,v 1.9.16.1 2011-03-04 14:09:07 engels Exp $
   */
  template <class T> 
  class CellIDDecoder {
    
  public:  

    CellIDDecoder() = default ;
    CellIDDecoder(const CellIDDecoder& ) = delete ;
    CellIDDecoder& operator=(const CellIDDecoder& ) = delete ;
    

    /** Constructor takes encoding string as argument.
     */
  CellIDDecoder( const std::string& encoder_str ) : _oldHit(0) {
      
      if( encoder_str.length() == 0 ){
      	throw( lcio::Exception( "CellIDDecoder : string of length zero provided as encoder string" ) ) ;
      }
      _b = new BitField64( encoder_str ) ; 
      
    }
    
    /** Constructor reads encoding string from collection parameter LCIO::CellIDEncoding.
     */
    CellIDDecoder( const EVENT::LCCollection* col ) : _oldHit(0) {
      
      std::string initString("") ; 

      if( col !=0 ) 
	initString = col->getParameters().getStringVal(  lcio::LCIO::CellIDEncoding ) ;
      
      if( initString.size() == 0 ) {
	
	initString = _defaultEncoding ;

	std::cout << "    ----------------------------------------- " << std::endl  
		  << "       WARNING: CellIDDecoder - no CellIDEncoding parameter in collection ! " 
		  << std::endl 
		  << "         -> using default : \"" << initString << "\"" 
		  << std::endl 
		  << "    ------------------------------------------ "  
		  << std::endl ;
      }
      
      _b = new BitField64(  initString ) ; 
    }
    
    ~CellIDDecoder(){ 
      
      delete _b ;
    } 
    
    
    /** Provides access to the bit fields, e.g. <br>
     *   int layer =  myCellIDEncoding( hit )[ "layer" ] ;
     * 
     */
    inline const BitField64 & operator()( const T* hit ){  
      
      if( hit != _oldHit && hit ) {
	

	lcio::long64 val = lcio::long64( hit->getCellID0() & 0xffffffff ) 
 	  |        ( lcio::long64( hit->getCellID1() ) << 32      ) ;
	
	_b->setValue( val ) ;

	_oldHit = hit ;
      }
      
      return  *_b ;
    }
    

    /** This can be used to set the default encoding that is used if no
     *  CellIDEncoding parameter is set in the collection, e.g. in older lcio files.
     */
    LCIO_DEPRECATED( "No longer supported for thread safety issues" , 
    static void setDefaultEncoding(const std::string& defaultEncoding ) ) ;
    
  protected:
    BitField64* _b{} ;
    const T* _oldHit{NULL} ;
    
    static const std::string _defaultEncoding ;
  } ; 
  
  template <class T>
  inline void CellIDDecoder<T>::setDefaultEncoding(const std::string& /*defaultEncoding*/ ) {
    std::cout << "=================== WARNING ===================" << std::endl;
    std::cout << "static method CellIDDecoder::setDefaultEncoding" << std::endl;
    std::cout << "does nothing anymore. Use the constructor to " << std::endl;
    std::cout << "specify your encoding string !!" << std::endl; 
    std::cout << "===============================================" << std::endl;
  }
  
  template <class T>
  const std::string CellIDDecoder<T>::_defaultEncoding
  = std::string("M:3,S-1:3,I:9,J:9,K-1:6") ;

  
} // namespace
#endif


