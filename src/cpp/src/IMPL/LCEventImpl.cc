
#include "IMPL/LCEventImpl.h"
#include <iostream>
#include "IMPL/AccessChecked.h"
#include "IMPL/LCCollectionVec.h"


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

  if( it == _colMap.end() ) 
    throw(DataNotAvailableException( std::string("LCEventImpl::getCollection: collection not in event:" 
						 + name) )) ; 
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

};
