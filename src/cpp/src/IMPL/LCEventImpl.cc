#include "IMPL/LCEventImpl.h"


using namespace EVENT ;

namespace IMPL {
  
LCEventImpl::LCEventImpl() :
  _runNumber(0),
  _eventNumber(0),
  _timeStamp(0),
  _detectorName("unknown"),
  _access( LCIO::UPDATE ) {
  
  //  std::cout << " LCEventImpl() : " << this << std::endl ;

}
  
// LCEventImpl::LCEventImpl(const LCEvent& evt) : 
//   _runNumber( evt.getRunNumber() ),
//   _eventNumber( evt.getEventNumber() ),
//   _timeStamp( evt.getTimeStamp() ),
//   _detectorName( evt.getDetectorName().c_str() ),
//   _access( LCIO::UPDATE ) { // copy will be updateable
  
//   StringVec* strVec = evt.getCollectionNames() ;
//   int nCol = strVec->size() ;
  
//   for( StringVec::iterator name = strVec->begin() ; name != strVec->end() ; name++){
    
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

    
const StringVec* LCEventImpl::getCollectionNames() const {

  // return pointer to updated vector _colNames 
  typedef LCCollectionMap::const_iterator LCI ;
  
  _colNames.clear() ;

  for ( LCI i=_map.begin() ; i != _map.end() ; i++ ){
    _colNames.push_back( i->first  ) ; 
  }
  return &_colNames ;
}

    
LCCollection * LCEventImpl::getCollection(const std::string & name) const {

  if( _map.find( name ) == _map.end() ) return 0 ;
  return  _map[ name ] ;
}

    

int LCEventImpl::addCollection(LCCollection * col, const std::string & name) {

  // add a new collection only, if access mode == update
  if( _access != LCIO::UPDATE )
    return LCIO::ERROR ;

  _map[ name ]  = col ;
  return LCIO::SUCCESS  ;  

}

    

int LCEventImpl::removeCollection(const std::string & name) {

  // remove collection only, if access mode == update
  if( _access != LCIO::UPDATE )
    return LCIO::ERROR ;
  
  // return error if name wasn't in the event - do we care ?
  if( _map.erase( name ) == 0 ) return LCIO::ERROR ;
  
  return LCIO::SUCCESS ;
}

    
void LCEventImpl::setRunNumber( int rn ) {
  _runNumber = rn ;
}


void LCEventImpl::setEventNumber( int en ) {
  _eventNumber = en ;
}

    
void LCEventImpl::setDetectorName(const std::string& dn ) {
  _detectorName = dn ;
}

    
void LCEventImpl::setTimeStamp(long ts) {
  _timeStamp =  ts ;
}

       
     
void LCEventImpl::setAccessMode( int accessMode ) {
  _access = accessMode ;
}

    
};
