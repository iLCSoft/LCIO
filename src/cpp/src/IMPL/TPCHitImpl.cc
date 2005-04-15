#include "IMPL/TPCHitImpl.h"
#include "Exceptions.h"
#include <cstring>  // needed for memcpy

namespace IMPL{

  TPCHitImpl::TPCHitImpl() : 
    _cellID(0) ,
    _time(0) ,
    _charge(0) ,
    _quality(0) ,
    _rawSize(0) ,
    _rawArray(0) {
  }    

  /// Destructor.
  TPCHitImpl::~TPCHitImpl() {
    delete[] _rawArray ;
  }
  
  int TPCHitImpl::getCellID() const { return  _cellID ; }  

  float TPCHitImpl::getTime() const { return _time ; }

  float TPCHitImpl::getCharge() const { return _charge ; }

  int TPCHitImpl::getQuality() const { return _quality ; }

  int TPCHitImpl::getNRawDataWords() const { return _rawSize ; }

  int TPCHitImpl::getRawDataWord(int i) const { 

    if( i < 0 || i >= _rawSize )
      throw EVENT::DataNotAvailableException("TPCHitImpl::getRawDataWord: index out of range") ;

    return  _rawArray[i] ; 
  }
  

  void TPCHitImpl::setCellID(int cellID){
    checkAccess("TPCHitImpl::setCellID") ;
    _cellID = cellID ;
  }
  
  void TPCHitImpl::setTime(float time){
    checkAccess("TPCHitImpl::setTime") ;
    _time = time ;
  }

  void TPCHitImpl::setCharge(float charge){
    checkAccess("TPCHitImpl::setCharge") ;
    _charge = charge ;
  }

  void TPCHitImpl::setQuality(int quality){
   checkAccess("TPCHitImpl::setQuality") ;
   _quality = quality ;
  }

  void TPCHitImpl::setRawData(const int* raw, int size ){
   checkAccess("TPCHitImpl::setRawData") ;
   if( _rawArray  != 0 ) {
     delete[] _rawArray ;
     _rawArray = 0 ;
   }
   _rawArray = new int[size] ;
   _rawSize = size ;
   memcpy( _rawArray,  raw , size*sizeof(int) ) ;
  }

  void TPCHitImpl::initRawArray(int size){
   if( _rawArray  != 0 ) {
     delete[] _rawArray ;
     _rawArray = 0 ;
   }
    _rawArray = new int[size] ;
    _rawSize = size ;
  }


} // namespace IMPL
