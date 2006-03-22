#ifndef CellIDEncoder_h
#define CellIDEncoder_h 1

#include "EVENT/LCCollection.h"
#include "UTIL/BitField64.h"
#include "lcio.h"
#include <string>

using namespace lcio ;

namespace EVENT{
  class SimCalorimeterHit ;
  class RawCalorimeterHit ;
  class TrackerData ;
  class TrackerPulse ;
  class TrackerRawData ;
}
namespace IMPL{
  class SimTrackerHitImpl ;
}

namespace UTIL{


  /** Convenient class for encoding cellIDs for various hit objects.
   *  It sets the proper collection parameter LCIO::CellIDEncoding and
   *  sets the proper flag bit for storing a second cellid if necessary.
   *  See UTIL::BitField64 for a description of the encoding string. 
   *  Example:<br>
   *  &nbsp;   CellIDEncoder<SimCalorimeterHitImpl> cd( "i:20,j:20,k:20" ,calVec )  ;<br>
   *  &nbsp;   for(int j=0;j<NHITS;j++){<br>
   *  &nbsp;&nbsp;  SimCalorimeterHitImpl* hit = new SimCalorimeterHitImpl ;<br>
   *  &nbsp;&nbsp;    cd["i"] = j ;<br>
   *  &nbsp;&nbsp;    cd["j"] = j + 100 ;<br>
   *  &nbsp;&nbsp;    cd["k"] = j + 200 ;<br>
   *  &nbsp;&nbsp;    cd.setCellID( hit ) ;<br>
   *  &nbsp;   } <br>
   * 
   *  @see BitField64
   *  @version $Id: CellIDEncoder.h,v 1.1 2006-03-22 17:38:22 gaede Exp $
   */
  template <class T> 
  class CellIDEncoder : public BitField64 {

  public:  
    
    /** Constructor reads encoding string from collection parameter LCIO::CellIDEncoding.
     */
    CellIDEncoder( const std::string& cellIDEncoding ,  LCCollection* col) :

      BitField64( cellIDEncoding  ),

      _col( col ) {
      
	_col->parameters().setValue( LCIO::CellIDEncoding , cellIDEncoding ) ;

	setCellIDFlag() ;
    }

    void setCellID( T* hit) {
      
      hit->setCellID0( lowWord()  ) ;
      
      hit->setCellID1( highWord() ) ;

    }

    /** Helper method that sets/unsets the proper bit for storing a second cellid word 
     */
    void setCellIDFlag() {
      
      int bit = cellID1Bit() ;
      
      if(  bit >= 0 ) {
	
	LCFlagImpl f( _col->getFlag() ) ;
	
	if( highestBit() > 31 ) {
	  
	  // 	  std::cout << " setting bit " <<  bit << std::endl ;
	  f.setBit( bit ) ;
	  
	} else {
	  // 	  std::cout << " unsetting bit " <<  bit << std::endl ;
	  f.unsetBit( bit ) ;
	}
	
	_col->setFlag( f.getFlag()  ) ;
      }
    }
    
  protected:
    
    int cellID1Bit() { return -1 ; }

 
    LCCollection* _col ;
    T* _oldHit ;
  } ; 
  
  /** Specialization for SimTrackerHits that have only one cellID */
  template<> void CellIDEncoder<IMPL::SimTrackerHitImpl>::setCellID( IMPL::SimTrackerHitImpl* hit) {
    hit->setCellID( lowWord()  ) ;
  }

  /** specialization that returns the proper bit for the second cellid */
  template<> int CellIDEncoder<EVENT::SimCalorimeterHit>::cellID1Bit() {  return LCIO::CHBIT_ID1 ; }

  /** specialization that returns the proper bit for the second cellid */
  template<> int CellIDEncoder<EVENT::RawCalorimeterHit>::cellID1Bit() {  return LCIO::RCHBIT_ID1 ; }

  /** specialization that returns the proper bit for the second cellid */
  template<> int CellIDEncoder<EVENT::TrackerData>::cellID1Bit() {  return LCIO::TRAWBIT_ID1 ; }

  /** specialization that returns the proper bit for the second cellid */
  template<> int CellIDEncoder<EVENT::TrackerPulse>::cellID1Bit() {  return LCIO::TRAWBIT_ID1 ; }

  /** specialization that returns the proper bit for the second cellid */
  template<> int CellIDEncoder<EVENT::TrackerRawData>::cellID1Bit() {  return LCIO::TRAWBIT_ID1 ; }



} // namespace
#endif


