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
  const std::vector<std::string> * LCRunHeaderImpl::getActiveSubdetectors() const { return &_activeSubdetectors ;}

  void LCRunHeaderImpl::setRunNumber( int rn) { 
    checkAccess("LCRunHeaderImpl::setRunNumber") ;
    _runNumber  = rn  ; 
  }
  void LCRunHeaderImpl::setDetectorName(const std::string& dn) { 
    checkAccess("LCRunHeaderImpl::setDetectorName") ;
    _detectorName = dn ; 
  }
  void LCRunHeaderImpl::setDescription(const std::string& dsc) { 
    checkAccess("LCRunHeaderImpl::setDescription") ;
    _description = dsc ;
  }
  void LCRunHeaderImpl::addActiveSubdetector(const std::string&  adn) { 
    checkAccess("LCRunHeaderImpl::setDescription") ;
    _activeSubdetectors.push_back( adn ) ;
  }


}; // namespace 
