#include "IMPL/LCRunHeaderImpl.h"

using namespace EVENT ;

namespace IMPL{


  LCRunHeaderImpl::LCRunHeaderImpl() :
    _runNumber(0){ 
  }
  
  LCRunHeaderImpl::~LCRunHeaderImpl() { } 
  
  int LCRunHeaderImpl::getRunNumber() const { return _runNumber ; }
  const std::string & LCRunHeaderImpl::getDetectorName() const  { return _detectorName ; }
  const std::string & LCRunHeaderImpl::getDescription() const {  return _description ; }
  const StringVec * LCRunHeaderImpl::getActiveSubdetectors() const { return &_activeSubdetectors ;}

  void LCRunHeaderImpl::setRunNumber( int rn) {  _runNumber  = rn  ; }
  void LCRunHeaderImpl::setDetectorName(const string& dn) { _detectorName = dn ; }
  void LCRunHeaderImpl::setDescription(const string& dsc) { _description = dsc ;}
  void LCRunHeaderImpl::addActiveSubdetector(const string&  adn) { 
    _activeSubdetectors.push_back( adn ) ;
  }


}; // namespace 
