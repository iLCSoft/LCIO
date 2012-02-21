// -*- C++ -*-
#ifndef UTIL_BitSet32_H
#define UTIL_BitSet32_H 1


#include <bitset>
#include <stdarg.h>


namespace UTIL {

  
  /** Convenient helper class for setting single bits in a 32bit-field, such as 
   *  collection flags or hit types - extends  std::bitset<32>.
   *  Use convenient methods make_bitset32(...) 
   *  to create a bit set with some bits set. <br>
   *  Examples:
   *  <pre>
   *    // set collection flags :
   *    LCCollectionVec* calVec = new LCCollectionVec( LCIO::SIMCALORIMETERHIT )  ;
   *	calVec->setFlag( UTIL::make_bitset32(  LCIO::CHBIT_LONG, LCIO::CHBIT_STEP ) );
   *   
   *    // check TrackerHit type:
   *    BitSet32 type( hit->getType() ) ;
   *    bool isComposite = type.test( ILDTrkHitTypeBit::COMPOSITE_SPACEPOINT ) ; // test w/ range check
   *    // or shorter:
   *    if( BitSet32( hit->getType() )[ ILDTrkHitTypeBit::COMPOSITE_SPACEPOINT ] ) {...}
   *
   *  </pre>
   */
  class BitSet32 : public std::bitset<32>{
    
  public:
    /** Allow to construct this from an int */
    BitSet32(int val ) : std::bitset<32>( val ) {}
    
    /** Allow assignment from an int */
    BitSet32& operator=(int val){
      std::bitset<32>::operator=( val ) ;
      return *this ;
    }
    
    /**Direct conversion to int */
    operator int() { return (int) to_ulong() ; }
  } ;
  
  
  /** Convenient helper that creates a BitSet32 with bit0 set */
  inline BitSet32 make_bitset32( int bit0 ) { 
    return BitSet32( 1 << bit0 ) ;
  }
  /** Convenient helper that creates a BitSet32 with bit0 and bit1 set */
  inline BitSet32 make_bitset32( int bit0, int bit1) { 
    return BitSet32( 1 << bit0  |
		     1 << bit1 ) ;
  }
  /** Convenient helper that creates a BitSet32 with bit0 - bit2 set */
  inline BitSet32 make_bitset32( int bit0, int bit1, int bit2) { 
    return BitSet32( 1 << bit0 |
		     1 << bit1 |
		     1 << bit2 ) ;
  }
  /** Convenient helper that creates a BitSet32 with bit0 - bit3 set */
  inline BitSet32 make_bitset32( int bit0, int bit1, int bit2, int bit3) { 
    return BitSet32( 1 << bit0 |
		     1 << bit1 |
		     1 << bit2 |
		     1 << bit3 ) ;
  }
  /** Convenient helper that creates a BitSet32 with bit0 - bit4 set */
  inline BitSet32 make_bitset32( int bit0, int bit1, int bit2, int bit3, int bit4) { 
    return BitSet32( 1 << bit0 |
		     1 << bit1 |
		     1 << bit2 |
		     1 << bit3 | 
		     1 << bit4 ) ;
  }
  
  /** Convenient helper to set a bit in a given int  - for example:<br>
   *   trkHit->setType( set_bit( trkHit->getType() ,  UTIL::ILDTrkHitTypeBit::ONE_DIMENSIONAL ) ) ;
   */
  inline int set_bit( int flag , int bit ){
    BitSet32 bs( flag ) ;
    bs.set( bit ) ;
    return (int) bs ;
  }

  /** Convenient helper to unset a bit in a given int  - for example:<br>
   *   trkHit->setType( set_bit( trkHit->getType() ,  UTIL::ILDTrkHitTypeBit::ONE_DIMENSIONAL ) ) ;
   */
  inline int unset_bit( int flag , int bit ){
    BitSet32 bs( flag ) ;
    bs.set( bit, 0 ) ;
    return (int) bs ;
  }



}
#endif
