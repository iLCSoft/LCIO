#ifndef UTIL_BitField64_H
#define UTIL_BitField64_H 1

#include <iostream>

#include <string>
#include <vector>
#include <map>
#include <sstream>

#include "lcio.h"
#include "LCIOTypes.h"

namespace UTIL {
  
  /** Helper class  for string tokenization. Usage:<br>
   *    std::vector<std::string> tokens ; <br>
   *    LCTokenizer t( tokens ,',') ; <br>
   *    std::for_each( aString.begin(), aString.end(), t ) ;  <br>
   */
  class LCTokenizer{
    
    std::vector< std::string >& _tokens ;
    char _del ;
    char _last ;

  public:
    
    /** Only c'tor, give (empty) token vector and delimeter character */
    LCTokenizer( std::vector< std::string >& tokens, char del ) 
      : _tokens(tokens) 
	, _del(del), 
	_last(del) {
    }
    
    /** Operator for use with algorithms, e.g. for_each */
    void operator()(const char& c) { 
      
      if( c != _del  ) {
	
	if( _last == _del  ) {
	  _tokens.push_back("") ; 
	}
	_tokens.back() += c ;
      }
      _last = c ;
    } 
    
  };


  /** Helper class for BitField64 that corresponds to one field value. 
   */

  class BitFieldValue{
  
  public :
    virtual ~BitFieldValue() {}
  
    /** The default c'tor.
     * @param  bitfield      reference to the 64bit bitfield
     * @param  offset        offset of field
     * @param  signedWidth   width of field, negative if field is signed
     */
    BitFieldValue( lcio::long64& bitfield, const std::string& name, 
		   unsigned offset, int signedWidth ) ; 


    /** Returns the current field value 
     */
    lcio::long64 value() const ;
  
    /** Assignment operator for user convenience 
     */
    BitFieldValue& operator=(lcio::long64 in) ;

    /** Conversion operator for lcio::long64 - allows to write:<br>
     *  lcio::long64 index = myBitFieldValue ;
     */
    operator lcio::long64() const { return value() ; } 
    
    /** fg: removed because it causes ambiguities with operator lcio::long64().
     *  Conversion operator for int - allows to write:<br>
     *  int index = myBitFieldValue ;
     */
    //     operator int() const { return (int) value() ; } 
    
    /** The field's name */
    const std::string& name() const { return _name ; }

    /** The field's offset */
    unsigned offset() const { return _offset ; }

    /** The field's width */
    unsigned width() const { return _width ; }

    /** True if field is interpreted as signed */
    bool isSigned() const { return _isSigned ; }

    /** The field's mask */
    lcio::ulong64 mask() const { return _mask ; }


  protected:
  
    lcio::long64& _b ;
    lcio::ulong64 _mask ;
    std::string _name ;
    unsigned _offset ;
    unsigned _width ;
    int _minVal ;
    int _maxVal ;
    bool _isSigned ;

  };

  /** A bit field of 64bits that allows convenient declaration and 
   *  manipulation of sub fields of various widths.<br>
   *  Example:<br>
   *    BitField64 b("layer:7,system:-3,barrel:3,theta:32:11,phi:11" ) ; <br> 
   *    b[ "layer"  ]  = 123 ;         <br> 
   *    b[ "system" ]  = -4 ;          <br> 
   *    b[ "barrel" ]  = 7 ;           <br> 
   *    b[ "theta" ]   = 180 ;         <br> 
   *    b[ "phi" ]     = 270 ;         <br> 
   *    ...                            <br>
   *    int theta = b["theta"] ;                    <br>
   *    ...                                         <br>
   *    unsigned phiIndex = b.index("phi) ;         <br>
   *    int phi = b[  phiIndex ] ;                  <br>
   */  
  class BitField64{

    friend std::ostream& operator<<(std::ostream& os, const BitField64& b) ;

  public :

    typedef std::map<std::string, unsigned int> IndexMap ;


    ~BitField64() {  // clean up
      for(unsigned i=0;i<_fields.size();i++){
	delete _fields[i] ;
      }
    }
  
    /** The c'tor takes an initialization string of the form:<br>
     *  <fieldDesc>[,<fieldDesc>...]<br>
     *  fieldDesc = name:[start]:[-]length<br>
     *  where:<br>
     *  name: The name of the field<br>
     *  start: The start bit of the field. If omitted assumed to start 
     *  immediately following previous field, or at the least significant 
     *  bit if the first field.<br>
     *  length: The number of bits in the field. If preceeded by '-' 
     *  the field is signed, otherwise unsigned.<br>
     *  Bit numbering is from the least significant bit (bit 0) to the most 
     *  significant (bit 63). <br>
     *  Example: "layer:7,system:-3,barrel:3,theta:32:11,phi:11"
     */
    BitField64( const std::string& initString ) : _value(0), _joined(0){
    
      init( initString ) ;
    }

    /** Returns the current 64bit value 
     */
    lcio::long64 getValue() { return _value ; } 
    
    /** Set a new 64bit value 
     */
    void  setValue(lcio::long64 value ) { _value = value ; }
    

    /** Acces to field through index 
     */
    BitFieldValue& operator[](size_t index) { 
      return *_fields.at( index )  ; 
    }
    
    /** Const acces to field through index 
     */
    const BitFieldValue& operator[](size_t index) const { 
      return *_fields.at( index )  ; 
    }

    /** Highest bit used in fields [0-63]
     */
    unsigned highestBit() ;
    
    /** Number of values */
    size_t size() { return _fields.size() ; }

    /** Index for field named 'name' 
     */
    size_t index( const std::string& name) const ;

    /** Access to field through name .
     */
    BitFieldValue& operator[](const std::string& name) { 

      return *_fields[ index( name ) ] ;
    }
    /** Const Access to field through name .
     */
    const BitFieldValue& operator[](const std::string& name) const { 

      return *_fields[ index( name ) ] ;
    }


    /** The low  word, bits 0-31
     */
    unsigned lowWord() const { return unsigned( _value &  0xffffFFFF )  ; } 

    /** The high  word, bits 32-63
     */
    unsigned highWord() const { return unsigned( _value >> 32  ) ; } 


    /** Return a valid description string of all fields
     */
    std::string fieldDescription() const ;

    /** Return a string with a comma separated list of the current sub field values 
     */
    std::string valueString() const ;

  protected:

    /** Add an additional field to the list 
     */
    void addField( const std::string& name,  unsigned offset, int width ); 

    /** Decode the initialization string as described in the constructor.
     *  @see BitField64( const std::string& initString )
     */
    void init( const std::string& initString) ;

    /** No default c'tor */
    BitField64() : _value(0) , _joined(0) { }


    // -------------- data members:--------------

    std::vector<BitFieldValue*>  _fields ;
    lcio::long64 _value ;
    IndexMap _map ;
    lcio::long64 _joined ;


  };



  /** Operator for dumping BitField64 to streams 
   */
  std::ostream& operator<<(std::ostream& os, const BitField64& b) ;


} // end namespace

#endif




