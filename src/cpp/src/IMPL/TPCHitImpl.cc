#include "IMPL/TPCHitImpl.h"
#include "Exceptions.h"
#include <cstring>  // needed for memcpy

namespace IMPL{

  TPCHitImpl::TPCHitImpl() : 
    _cellID(0) ,
    _time(0) ,
    _charge(0) ,
    _quality(0) ,
    _rawDataSize(0) ,
    _rawDataArray(0) {
  }    

  /// Destructor.
  TPCHitImpl::~TPCHitImpl() {
    delete[] _rawDataArray ;
  }
  
  int TPCHitImpl::getCellID() const { return  _cellID ; }  

  float TPCHitImpl::getTime() const { return _time ; }

  float TPCHitImpl::getCharge() const { return _charge ; }

  int TPCHitImpl::getQuality() const { return _quality ; }

  int TPCHitImpl::getNRawDataWords() const { return _rawDataSize ; }

  int TPCHitImpl::getRawDataWord(int i) const { 

    if( i < 0 || i >= _rawDataSize )
      throw EVENT::DataNotAvailableException("TPCHitImpl::getRawDataWord: index out of range") ;

    return  _rawDataArray[i] ; 
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

  void TPCHitImpl::setRawData(const int* rawData, int size ){
   checkAccess("TPCHitImpl::setRawData") ;
   if( _rawDataArray  != 0 ) delete[] _rawDataArray ;
   _rawDataArray = new int[size] ;
   _rawDataSize = size ;
   memcpy( _rawDataArray,  rawData , size*sizeof(int) ) ;
  }


}; // namespace IMPL
