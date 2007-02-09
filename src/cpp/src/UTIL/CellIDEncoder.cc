#include "UTIL/CellIDEncoder.h"

namespace UTIL{


  /** Helper function that returns the bit for cellid1 through template specialization
   *  or -1 if no cellid1 exists. 
   */

  /** specialization that returns the proper bit for the second cellid */
  template<> int CellIDEncoder_cellID1Bit<EVENT::SimCalorimeterHit>() {  return LCIO::CHBIT_ID1 ; }

  /** specialization that returns the proper bit for the second cellid */
  template<> int CellIDEncoder_cellID1Bit<EVENT::RawCalorimeterHit>() {  return LCIO::RCHBIT_ID1 ; }

  /** specialization that returns the proper bit for the second cellid */
  template<> int CellIDEncoder_cellID1Bit<EVENT::CalorimeterHit>() {  return LCIO::RCHBIT_ID1 ; }

  /** specialization that returns the proper bit for the second cellid */
  template<> int CellIDEncoder_cellID1Bit<EVENT::TrackerData>() {  return LCIO::TRAWBIT_ID1 ; }

  /** specialization that returns the proper bit for the second cellid */
  template<> int CellIDEncoder_cellID1Bit<EVENT::TrackerPulse>() {  return LCIO::TRAWBIT_ID1 ; }

  /** specialization that returns the proper bit for the second cellid */
  template<> int CellIDEncoder_cellID1Bit<EVENT::TrackerRawData>() {  return LCIO::TRAWBIT_ID1 ; }
  
  /** Specialization for SimTrackerHits that have only one cellID */
  //  template<> void CellIDEncoder<IMPL::SimTrackerHitImpl>::setCellID( IMPL::SimTrackerHitImpl* hit) {
  template<> 
  void CellIDEncoder_setCellID<IMPL::SimTrackerHitImpl>( IMPL::SimTrackerHitImpl* hit, 
							 int low, int high){
    hit->setCellID( low ) ;
  }


}
