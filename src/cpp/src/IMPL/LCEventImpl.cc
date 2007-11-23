
#include "IMPL/LCEventImpl.h"
#include "IMPL/AccessChecked.h"
#include "IMPL/LCCollectionVec.h"

#define EVT_WGT "_weight" 

#include <iostream>
#include <sstream>


using namespace EVENT ;
//using namespace DATA ;

namespace IMPL {
  
LCEventImpl::LCEventImpl() :
  _runNumber(0),
  _eventNumber(0),
  _timeStamp(0),
  _detectorName("unknown") {
}
  
// LCEventImpl::LCEventImpl(const LCEvent& evt) : 
//   _runNumber( evt.getRunNumber() ),
//   _eventNumber( evt.getEventNumber() ),
//   _timeStamp( evt.getTimeStamp() ),
//   _detectorName( evt.getDetectorName().c_str() ),
  
//   std::vector<std::string>* strVec = evt.getCollectionNames() ;
//   int nCol = strVec->size() ;
  
//   for( std::vector<std::string>::iterator name = strVec->begin() ; name != strVec->end() ; name++){
    
//     const LCCollection* col = evt.getCollection( *name ) ;
//     col->getTypeName() ;
    
//     // to be done - need to create new LCCollectionVec and add to the event ...
//   }

// }

LCEventImpl::~LCEventImpl() {
  //  std::cout << " ~LCEventImpl() : " << this << std::endl ;

  // here we need to delete all collections in the event
  typedef LCCollectionMap::const_iterator LCI ;
  
  for ( LCI i=_colMap.begin() ; i != _colMap.end() ; i++ ){
    
    // except collections whose ownership we gave away
    if( _notOwned.find( i->second ) ==  _notOwned.end() )
      delete i->second ;
  }

}
    
int LCEventImpl::getRunNumber() const {
  return _runNumber ;
}

    
int LCEventImpl::getEventNumber() const {
  return _eventNumber ;
}

    
const std::string & LCEventImpl::getDetectorName() const {
  return _detectorName ;
}

    
long64 LCEventImpl::getTimeStamp() const {
  return _timeStamp ;
}

double LCEventImpl::getWeight() const {
  double w = _params.getFloatVal( EVT_WGT ) ;
    
  return w == 0 ? 1. : w ;

}

   
const std::vector<std::string>* LCEventImpl::getCollectionNames() const {

  // return pointer to updated vector _colNames 
  typedef LCCollectionMap::const_iterator LCI ;
  
  _colNames.clear() ;

  for ( LCI i=_colMap.begin() ; i != _colMap.end() ; i++ ){
    _colNames.push_back( i->first  ) ; 
  }
  return &_colNames ;
}

    

LCCollection * LCEventImpl::getCollection(const std::string & name) const 
  throw (DataNotAvailableException, std::exception) {

  LCCollectionMap::iterator it = _colMap.find( name )  ;

  if( it == _colMap.end() ) {
    
    std::stringstream ss ;
    ss << "LCEventImpl::getCollection: collection not in event:" << name ;

    throw( DataNotAvailableException( ss.str() ) ) ; 
  }
  return  it->second ;

  //   if( _colMap.find( name ) == _colMap.end() ) 
  //     throw(DataNotAvailableException( std::string("LCEventImpl::getCollection: collection not in event:" // 						 + name) )) ; 
  //   return  _colMap[ name ] ;
  
}


LCCollection * LCEventImpl::takeCollection(const std::string & name) const 
  throw (DataNotAvailableException, std::exception) {

  LCCollectionVec* col = dynamic_cast<LCCollectionVec*> ( getCollection( name ) ) ;

  col->setTransient( true ) ;
  
  _notOwned.insert( col ) ;

  return  col ;
}


    

void  LCEventImpl::addCollection(LCCollection * col, const std::string & name) 
  throw (EventException, std::exception)  {

  
  if( ! validateCollectionName(name.c_str()) ){

    throw EventException( std::string("LCEventImpl::addCollection() invalid name (has to be C/C++ name): "
				      +name) ) ; 
  }
      
  // check if name exists
  if( _colMap.find( name ) != _colMap.end() )
    
    throw EventException( std::string("LCEventImpl::addCollection() name already exists: "
				      +name) ) ; 
  // check if col != 0
  if( col == 0  )

    throw EventException( std::string("LCEventImpl::addCollection()  cannot add NULL collection for : "
				      +name) ) ; 

  _colMap[ name ]  = col ;
 
}

    

void LCEventImpl::removeCollection(const std::string & name) throw (ReadOnlyException, std::exception) {

  // remove collection only, if access mode == update
  checkAccess("LCEventImpl::removeCollection") ;
  _colMap.erase( name ) ;  

}

    

void LCEventImpl::setRunNumber( int rn ) {
  checkAccess("LCEventImpl::setRunNumber") ;
  _runNumber = rn ;
}


void LCEventImpl::setEventNumber( int en ) {
  checkAccess("LCEventImpl::setEventNumber") ;
  _eventNumber = en ;
}

    
void LCEventImpl::setDetectorName(const std::string& dn ) {
  checkAccess("LCEventImpl::setDetectorName") ;
  _detectorName = dn ;
}

    
void LCEventImpl::setTimeStamp(long64 ts) {
  checkAccess("LCEventImpl::setTimeStamp") ;
  _timeStamp =  ts ;
}


void LCEventImpl::setWeight(double w) {
  checkAccess("LCEventImpl::setWeight") ;
  _params.setValue(  EVT_WGT , (float) w ) ;
}

     
void LCEventImpl::setAccessMode( int accessMode ) {

  // loop over all collections and set the access mode
  bool readOnly = ( accessMode == LCIO::READ_ONLY ) ;

  setReadOnly( readOnly ) ;

  typedef LCCollectionMap::iterator LCI ;
  
  for ( LCI i=_colMap.begin() ; i != _colMap.end() ; i++ ){

    AccessChecked* col = dynamic_cast<AccessChecked*>( i->second ) ; 
    if( col ){
      col->setReadOnly( readOnly ) ;
    }
  }


}
  
  /**Tests the validity of a collection name. SIO only accepts names starting with 
   * (regular expression) [A-Za-z_] and continuing with [A-Za-z0-9_] (C/C++ variable name).
   */
  bool LCEventImpl::validateCollectionName( const char* name ){ //copy of SIO_functions::validateName
    
    if( *name < 0 ) 
      return false ;
    
    if( !isalpha( (int)*name ) && *name != '_' ) 
      return false ;
    
    for( name += 1; *name != '\0'; name++ ){
      if( *name < 0 ) 
	return false ;
      if( !isalnum( (int)*name ) && *name != '_' ) 
	return false ;
    } 
    return true ;
  }

}
