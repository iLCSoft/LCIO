#include "IMPL/CalorimeterHitImpl.h"

namespace IMPL{
  
  CalorimeterHitImpl::CalorimeterHitImpl() :
    _cellID0(0),
    _cellID1(0),
    _energy(0){
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
  
  const float* CalorimeterHitImpl::getPosition() const {
    return _position ;
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
  
  void CalorimeterHitImpl::setPosition(const float pos[3]){
    checkAccess("CalorimeterHitImpl::setPosition") ;
    _position[0] = pos[0] ;
    _position[1] = pos[1] ;
    _position[2] = pos[2] ;
  }
  
 
}; // namespace IMPL
