#include "IMPL/CalorimeterHitImpl.h"

namespace IMPL{
  
  CalorimeterHitImpl::CalorimeterHitImpl() :
    _cellID0(0),
    _cellID1(0),
    _energy(0.),
    _energyError(0.),
    _time(0.),
    _type(0),
    _rawHit(0) {
    _position[0] = 0. ;
    _position[1] = 0. ;
    _position[2] = 0. ; 
  }
  
  
  CalorimeterHitImpl::~CalorimeterHitImpl(){
  }
  
  int CalorimeterHitImpl::getCellID0() const {
    return _cellID0 ;
  }
  
  int CalorimeterHitImpl::getCellID1() const {
    return _cellID1 ;
  }
  
  float CalorimeterHitImpl::getEnergy() const {
    return _energy ;
  }

  float CalorimeterHitImpl::getEnergyError() const {
    return _energyError ;
  }
  
  float CalorimeterHitImpl::getTime() const {
    return _time ;
  }
  
  const float* CalorimeterHitImpl::getPosition() const {
    return _position ;
  }
  
  int CalorimeterHitImpl::getType() const {
    return _type ;
  }
  EVENT::LCObject * CalorimeterHitImpl::getRawHit() const {
    return _rawHit ;
  }
  
  void CalorimeterHitImpl::setCellID0(int id0){
    checkAccess("CalorimeterHitImpl::setCellID0") ;
    _cellID0 = id0 ;
  }
  
  void CalorimeterHitImpl::setCellID1(int id1){
    checkAccess("CalorimeterHitImpl::setCellID1") ;
    _cellID1 = id1 ;
  }
  
  void CalorimeterHitImpl::setEnergy(float en){
    checkAccess("CalorimeterHitImpl::setEnergy") ;
    _energy = en ;
  }
  
  void CalorimeterHitImpl::setEnergyError(float enerr){
    checkAccess("CalorimeterHitImpl::setEnergyError") ;
    _energyError = enerr ;
  }
 
  void CalorimeterHitImpl::setTime(float t){
    checkAccess("CalorimeterHitImpl::setTime") ;
    _time = t ;
  }
  
  void CalorimeterHitImpl::setPosition(const float pos[3]){
    checkAccess("CalorimeterHitImpl::setPosition") ;
    _position[0] = pos[0] ;
    _position[1] = pos[1] ;
    _position[2] = pos[2] ;
  }
  
 
  void CalorimeterHitImpl::setType(int type) {
    checkAccess("CalorimeterHitImpl::setType") ;
    _type = type ;
  }
  
  void CalorimeterHitImpl::setRawHit(EVENT::LCObject* rawHit ){
    checkAccess("CalorimeterHitImpl::setRawHit") ;
    _rawHit = rawHit ;
  }
  
} // namespace IMPL
