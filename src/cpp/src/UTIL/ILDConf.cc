#include "UTIL/ILDConf.h"

namespace UTIL {
  
  // this string should never be changed really
  const std::string ILDCellID0::encoder_string = "subdet:5,side:-2,layer:9,module:8,sensor:8" ;
  
  // Note: the definition of static const ints might not be strictly needed as in-class declarqtion and initialization
  //       is typically enough, however there are cases where this would result in undefined symbol linker errors
  //       ( e.g. when the constant is used in a method that takes a const reference as an argument, such as MYTESTS() 
  //         - see http://stackoverflow.com/questions/3025997/c-defining-static-const-integer-members-in-class-definition )
  //
  //       So in any case we define _and_ initialize the constants here, which also hides the actual int values from the user.
  //
  
  const unsigned ILDCellID0::subdet = 0 ;
  const unsigned ILDCellID0::side   = 1 ;
  const unsigned ILDCellID0::layer  = 2 ;
  const unsigned ILDCellID0::module = 3 ;
  const unsigned ILDCellID0::sensor = 4 ;
  
  const int ILDDetID::NOTUSED   =   0 ;
  const int ILDDetID::VXD       =   1 ;
  const int ILDDetID::SIT       =   2 ;
  const int ILDDetID::FTD       =   3 ; 
  const int ILDDetID::TPC       =   4 ;
  const int ILDDetID::SET       =   5 ;
  const int ILDDetID::ETD       =   6 ;
  const int ILDDetID::ECAL      =  20 ;
  const int ILDDetID::ECAL_PLUG =  21 ;
  const int ILDDetID::HCAL      =  22 ;
  const int ILDDetID::HCAL_RING =  23 ;
  const int ILDDetID::LCAL      =  24 ;
  const int ILDDetID::BCAL      =  25 ;
  const int ILDDetID::LHCAL     =  26 ;
  const int ILDDetID::YOKE      =  27 ;
  const int ILDDetID::bwd       = - 1 ;
  const int ILDDetID::barrel    =   0 ;
  const int ILDDetID::fwd       =   1 ;
  
  
  // type specifyers for LCIO TrackerHits used in the context of ILD reconstruction code
  
  const int ILDTrkHitType::NOTUSED               = 0 ;
  const int ILDTrkHitType::COMPOSITE_SPACEPOINT  = 1073741824 ; // bit 30
  const int ILDTrkHitType::ONE_DIMENSIONAL       =  536870912 ; // bit 29
  
}
