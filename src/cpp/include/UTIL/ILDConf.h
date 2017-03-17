// -*- C++ -*-
#ifndef UTIL_ILDConf_H
#define UTIL_ILDConf_H 1

#include <string>
#include "CellIDEncoder.h"
#include "CellIDDecoder.h"
#include "LCTrackerConf.h"


#include "BitSet32.h"

// Some useful definitions that are used in the ILD software framework - in particular for
// encoding and decoding cellIDs.

namespace UTIL {
  
  


  /** Helper class to define constants for the canonical encoding of the CellID0 in tracking sub detectors
   *  in ILD (or ILD like detectors).
   * 
   *  F.Gaede, DESY, 2011
   *  @version $Id:$
   */
  struct ILDCellID0 {
    
    /** The canonical encoding string to be used for writing the CellID0 with the CellIDEncoder<T>: 
      *   <pre>"subdet:5,side:-2,layer:9,module:8:sensor:8"</pre> 
      *
      *  Use the CellIdDecoder to access the elements:
      *  <pre>
      *     CellIDDecoder<TrackerHit> idDec( trkHitCol ) ;
      *      // ...
      *      TrackerHit* hit = (TrackerHit*) trkHitCol.getElementAt( i ) ;
      *
      *      int layer  = idDec( hit )[ ILDCellID0::layer ] ;
      *  </pre>
      */
    static std::string encoder_string ; 
    
    /** Index of the field subdet in encoder_string - sub detector Id as defined in DetID, e.g. DetID::VXD or DetID::FTD. 
     */
    static const unsigned subdet ;
    
    /** Index of the field side in encoder_string - one of DetID::bwd, DetID::barrel, DetID::fwd. 
     */
    static const unsigned side ;
    
    /** Index of the field layer in encoder_string - local sub detector layer ID starting from 0 going away from the origin (IP). 
     */
    static const unsigned layer ;
    
    /** Index of the field module in encoder_string - module ID  as defined for the given subdetetor. */
    static const unsigned module ;
    
    /** Index of the field sensor in encoder_string -  sensor ID  as defined for the given subdetetor module. */
    static const unsigned sensor ;
    
  }; 
  
  

  /** Special wrapper to the CellIDEncoder<T> that enforces cellID0 to use the encoder string 
   *  defined in ILDCellID0::encoder_string.
   */
  template <class T> 
  class ILDCellIDEncoder : public CellIDEncoder<T> {
    
  public:  
    
    /** Constructor for using the canonical cellID0 as defined in ILDCellID0::encoder_string - cellID1 will not be used.
     */
    ILDCellIDEncoder( EVENT::LCCollection* col) : 
      CellIDEncoder<T>( ILDCellID0::encoder_string, col ) {}
    

    /** Constructor that prepends given ILDCellID0::encoder_string to the given encoding string.
     *  Throws an Exception if called with an empty string. 
     */
    ILDCellIDEncoder( const std::string& cellIDEncoding ,  EVENT::LCCollection* col) : 
      CellIDEncoder<T>( std::string( ILDCellID0::encoder_string + "," + cellIDEncoding )  , col ) { 
      
      if(  cellIDEncoding.size() == 0 ){
	throw EVENT::Exception(" ILDCellIDEncoder::ILDCellIDEncoder: cannot initilize with empty string ! " ) ;
      }
    }
  } ;
  




  /** Define integer constants for identifying ILD sub detectors. Typically used in CellIDs, for example together with
   *  ILD::CellID0::encoder_string.
   *  
   *  F.Gaede, DESY, 2011
   *  @version $Id:$
   */
  struct ILDDetID{
    
    
    static const int NOTUSED   ;
    static const int VXD       ;
    static const int SIT       ;
    static const int FTD       ; 
    static const int TPC       ;
    static const int SET       ;
    static const int ETD       ;
    
    static const int ECAL      ;
    static const int ECAL_PLUG ;
    static const int HCAL      ;
    static const int HCAL_RING ;
    static const int LCAL      ;
    static const int BCAL      ;
    static const int LHCAL     ;
    static const int YOKE      ;
    static const int COIL        ;
    static const int ECAL_ENDCAP ;
    static const int HCAL_ENDCAP ;
    static const int YOKE_ENDCAP ;
    
    static const int bwd       ;
    static const int barrel    ;
    static const int fwd       ;
  } ;
  
  /** Define integer constants for bits of the the lcio tracker hit type  used in the context of ILD reconstruction code
   *  
   *  S.J. Aplin, DESY, 2012
   *  @version $Id:$
   */
  
  struct ILDTrkHitTypeBit{
    static const int COMPOSITE_SPACEPOINT  ; // i.e. a 3D space-point composed of two independent strip measurements
    static const int ONE_DIMENSIONAL;        // e.g. a strip measurment
    
  } ;

  /** Define integer constants for bits of the the lcio tracker hit quality  used in the context of ILD reconstruction code
   *  
   *  F.Gaede, DESY, 2012
   *  @version $Id:$
   */
  struct ILDTrkHitQualityBit{
    static const int USED_IN_FIT ;
    static const int USED_IN_TRACK ;
    static const int DOUBLE_HIT_CANDIDATE ;
    static const int GOOD ;
  } ;

  

} // namespace UTIL
#endif // ifndef UTIL_ILDConf_H
