#include "IMPL/RawCalorimeterHitImpl.h"

namespace IMPL{
  
  RawCalorimeterHitImpl::RawCalorimeterHitImpl() :
    _cellID0(0),
    _cellID1(0),
    _amplitude(0),
    _timeStamp(0){
  }
  
  
  RawCalorimeterHitImpl::~RawCalorimeterHitImpl(){
  }
  
  int RawCalorimeterHitImpl::getCellID0() const {
    return _cellID0 ;
  }
  
  int RawCalorimeterHitImpl::getCellID1() const {
    return _cellID1 ;
  }
  
  int RawCalorimeterHitImpl::getAmplitude() const {
    return _amplitude ;
  }

  int RawCalorimeterHitImpl::getTimeStamp() const {
    return _timeStamp ;
  }
  
  
  
  void RawCalorimeterHitImpl::setCellID0(int id0){
    checkAccess("RawCalorimeterHitImpl::setCellID0") ;
    _cellID0 = id0 ;
  }
  
  void RawCalorimeterHitImpl::setCellID1(int id1){
    checkAccess("RawCalorimeterHitImpl::setCellID1") ;
    _cellID1 = id1 ;
  }
  
  void RawCalorimeterHitImpl::setAmplitude(int amplitude){
    checkAccess("RawCalorimeterHitImpl::setAmplitude") ;
    _amplitude = amplitude ;
  }

  void RawCalorimeterHitImpl::setTimeStamp(int timeStamp){
    checkAccess("RawCalorimeterHitImpl::setTimeStamp") ;
    _timeStamp = timeStamp ;
  }
  

  
 
} // namespace IMPL
