#include "IMPL/LCEventImpl.h"
#include <iostream>
#include "IMPL/AccessChecked.h"

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
  
  for ( LCI i=_map.begin() ; i != _map.end() ; i++ ){
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

    
long LCEventImpl::getTimeStamp() const {
  return _timeStamp ;
}

    
const std::vector<std::string>* LCEventImpl::getCollectionNames() const {

  // return pointer to updated vector _colNames 
  typedef LCCollectionMap::const_iterator LCI ;
  
  _colNames.clear() ;

  for ( LCI i=_map.begin() ; i != _map.end() ; i++ ){
    _colNames.push_back( i->first  ) ; 
  }
  return &_colNames ;
}

    

LCCollection * LCEventImpl::getCollection(const std::string & name) const throw (DataNotAvailableException) {

  if( _map.find( name ) == _map.end() ) 
    throw(DataNotAvailableException( std::string("LCEventImpl::getCollection: collection not in event:" 
						 + name) )) ; 
  return  _map[ name ] ;
}

DATA::LCCollectionData * LCEventImpl::getCollectionData(const std::string & name) const {
    if( _map.find( name ) == _map.end() ) return 0 ;
    return  _map[ name ] ;
}

    

void  LCEventImpl::addCollection(LCCollection * col, const std::string & name) throw (EventException)  {

  // check if name exists
  if( _map.find( name ) != _map.end() )
    
    // FIXME: what is this - no error handling ?
    std::cout << " addCollection: collection already exists: " << name << std::endl ;
    //    throw EventException( std::string("LCEventImpl::addCollection() name already exists in event: "+name) ) ; 

  _map[ name ]  = col ;
 
}

    

void LCEventImpl::removeCollection(const std::string & name) throw (ReadOnlyException) {

  // remove collection only, if access mode == update
  checkAccess("LCEventImpl::removeCollection") ;
  _map.erase( name ) ;  

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

    
void LCEventImpl::setTimeStamp(long ts) {
  checkAccess("LCEventImpl::setTimeStamp") ;
  _timeStamp =  ts ;
}

       
     
void LCEventImpl::setAccessMode( int accessMode ) {

  // loop over all collections and set the access mode
  bool readOnly = ( accessMode == LCIO::READ_ONLY ) ;

  setReadOnly( readOnly ) ;

  typedef LCCollectionMap::const_iterator LCI ;
  
  for ( LCI i=_map.begin() ; i != _map.end() ; i++ ){

    AccessChecked* col = dynamic_cast<AccessChecked*>( i->second ) ; 
    if( col ){
      col->setReadOnly( readOnly ) ;
    }
  }


}

};
