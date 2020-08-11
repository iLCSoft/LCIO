#include "SIO/SIOReader.h"

// // -- lcio headers
// #include "SIO/LCSIO.h"
#include "IOIMPL/LCEventIOImpl.h"
#include "IOIMPL/LCRunHeaderIOImpl.h"
// #include "Exceptions.h"
// 
// #include <sio/exception.h>
// #include <sio/api.h>
// #include <sio/compression/zlib.h>
// 
// #include <sys/stat.h>
// #include <sstream>
// #include <algorithm>
#include <limits>
// #include <assert.h>

namespace SIO {


  SIOReader::SIOReader( int lcReaderFlag ) :
    _reader( lcReaderFlag ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  SIOReader::~SIOReader() {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIOReader::setReadCollectionNames(const std::vector<std::string>& colnames){
    _reader.setReadCollectionNames( colnames ) ;
  }

  //----------------------------------------------------------------------------

  void SIOReader::open(const std::vector<std::string>& filenames) {
    _reader.open( filenames ) ;
  }

  //----------------------------------------------------------------------------

  void SIOReader::open(const std::string& filename)  {
    _reader.open( filename ) ;
  }

  //----------------------------------------------------------------------------

  int SIOReader::getNumberOfEvents()  {
    return _reader.getNumberOfEvents() ;
  }

  //----------------------------------------------------------------------------

  int SIOReader::getNumberOfRuns()  {
    return _reader.getNumberOfRuns() ;
  }

  //----------------------------------------------------------------------------

  void SIOReader::getRuns(EVENT::IntVec & runs) {
    _reader.getRuns( runs ) ;
  }

  //----------------------------------------------------------------------------

  void SIOReader::getEvents(EVENT::IntVec & events) {
    _reader.getEvents( events ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCRunHeader* SIOReader::readNextRunHeader()  {
    return readNextRunHeader( EVENT::LCIO::READ_ONLY ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCRunHeader* SIOReader::readNextRunHeader(int accessMode) {

    if( _currentRun != nullptr ){
      delete _currentRun ;
    }
    _currentRun = _reader.readNextRunHeader( accessMode ).release() ;

    return _currentRun ;
  }
  
  //----------------------------------------------------------------------------

  EVENT::LCEvent* SIOReader::readNextEvent()  {
    return readNextEvent( EVENT::LCIO::READ_ONLY ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCEvent* SIOReader::readNextEvent(int accessMode)  {

    if( _currentEvent != nullptr ){
      delete _currentEvent ;
    }
    _currentEvent = _reader.readNextEvent( accessMode ).release() ;

    return _currentEvent ;
  }

  //----------------------------------------------------------------------------

  void SIOReader::skipNEvents( int n ) {
    _reader.skipNEvents( n ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCRunHeader * SIOReader::readRunHeader(int runNumber) {
    return readRunHeader( runNumber, EVENT::LCIO::READ_ONLY ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCRunHeader * SIOReader::readRunHeader(int runNumber, int accessMode) {

    if( _currentRun != nullptr ){
      delete _currentRun ;
    }
    _currentRun = _reader.readRunHeader( runNumber, accessMode ).release() ;

    return _currentRun ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCEvent * SIOReader::readEvent(int runNumber, int evtNumber) {
    return readEvent( runNumber, evtNumber , EVENT::LCIO::READ_ONLY ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCEvent * SIOReader::readEvent(int runNumber, int evtNumber, int accessMode) {

    if( _currentEvent != nullptr ){
      delete _currentEvent ;
    }
    _currentEvent = _reader.readEvent( runNumber, evtNumber, accessMode ).release() ;

    return _currentEvent ;
  }

  //----------------------------------------------------------------------------

  void SIOReader::close() {
    _reader.close() ;
  }

  //----------------------------------------------------------------------------

  void SIOReader::registerLCEventListener(IO::LCEventListener * ls) {
    _evtListeners.insert( _evtListeners.end() , ls );
  }

  //----------------------------------------------------------------------------

  void SIOReader::removeLCEventListener(IO::LCEventListener * ls) {
    _evtListeners.erase( _evtListeners.find( ls )  );
  }

  //----------------------------------------------------------------------------

  void SIOReader::registerLCRunListener(IO::LCRunListener * ls) {
    _runListeners.insert( _runListeners.end() , ls );
  }

  //----------------------------------------------------------------------------

  void SIOReader::removeLCRunListener(IO::LCRunListener * ls) {
    _runListeners.erase( _runListeners.find( ls ) );
  }

  //----------------------------------------------------------------------------

  void SIOReader::readStream() {
    readStream( std::numeric_limits<int>::max() ) ;
  }

  //----------------------------------------------------------------------------

  void SIOReader::readStream(int maxRecord) {
    _reader.readStream( this, maxRecord ) ;
  }

  //----------------------------------------------------------------------------

  void SIOReader::processEvent( std::shared_ptr<EVENT::LCEvent> event ) {
    auto eventImpl = dynamic_cast<IOIMPL::LCEventIOImpl*>( event.get() ) ;
    std::set<IO::LCEventListener*>::iterator iter = _evtListeners.begin() ;
    while( iter != _evtListeners.end() ) {
      eventImpl->setAccessMode( EVENT::LCIO::UPDATE ) ;
      (*iter)->modifyEvent( eventImpl ) ;
      eventImpl->setAccessMode( EVENT::LCIO::READ_ONLY ) ; // set the proper acces mode
      (*iter)->processEvent( eventImpl ) ;
      iter++ ;
    }
  }
  
  //----------------------------------------------------------------------------
  
  void SIOReader::processRunHeader( std::shared_ptr<EVENT::LCRunHeader> hdr ) {
    auto hdrImpl = dynamic_cast<IOIMPL::LCRunHeaderIOImpl*>( hdr.get() ) ;
    std::set<IO::LCRunListener*>::iterator iter = _runListeners.begin() ;
    while( iter != _runListeners.end() ){
      hdrImpl->setReadOnly( false ) ;
      (*iter)->modifyRunHeader( hdrImpl ) ;
      hdrImpl->setReadOnly( true ) ;
      (*iter)->processRunHeader( hdrImpl ) ;
      iter++ ;
    }
  }

} // namespace
