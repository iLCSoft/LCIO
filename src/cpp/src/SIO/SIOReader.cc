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
// #include <cstring>
// #include <assert.h>

namespace SIO {


  SIOReader::SIOReader( int lcReaderFlag ) :
    _reader( lcReaderFlag ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------

  SIOReader::~SIOReader() {
    /* nop */
  }

  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------

  void SIOReader::setReadCollectionNames(const std::vector<std::string>& colnames){
    _reader.setReadCollectionNames( colnames ) ;
  }

  //----------------------------------------------------------------------------

  void SIOReader::open(const std::vector<std::string>& filenames) {
    _reader.open( filenames ) ;
  }

  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------

  void SIOReader::open(const std::string& filename)  {
    _reader.open( filename ) ;
  }

  //----------------------------------------------------------------------------

  int SIOReader::getNumberOfEvents()  {
    return _reader.getNumberOfEvents() ;
  }

  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------

  int SIOReader::getNumberOfRuns()  {
    return _reader.getNumberOfRuns() ;
  }

  //----------------------------------------------------------------------------

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
    return _reader.readNextRunHeader( accessMode ).release() ;
  }
  
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------

  EVENT::LCEvent* SIOReader::readNextEvent()  {
    return readNextEvent( EVENT::LCIO::READ_ONLY ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCEvent* SIOReader::readNextEvent(int accessMode)  {
    return _reader.readNextEvent( accessMode ).release() ;
  }

  //----------------------------------------------------------------------------

  void SIOReader::skipNEvents( int n ) {
    _reader.skipNEvents( n ) ;
  }

  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------

  EVENT::LCRunHeader * SIOReader::readRunHeader(int runNumber) {
    return readRunHeader( runNumber, EVENT::LCIO::READ_ONLY ) ;
  }

  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------

  EVENT::LCRunHeader * SIOReader::readRunHeader(int runNumber, int accessMode) {
    return _reader.readRunHeader( runNumber, accessMode ).release() ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCEvent * SIOReader::readEvent(int runNumber, int evtNumber) {
    return readEvent( runNumber, evtNumber , EVENT::LCIO::READ_ONLY ) ;
  }

  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------

  EVENT::LCEvent * SIOReader::readEvent(int runNumber, int evtNumber, int accessMode) {
    return _reader.readEvent( runNumber, evtNumber, accessMode ).release() ;
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
    const bool readUntilEOF = (maxRecord == std::numeric_limits<int>::max()) ;
    std::set<std::string> records = { LCSIO::RunRecordName, LCSIO::HeaderRecordName, LCSIO::EventRecordName } ;
    std::shared_ptr<IOIMPL::LCEventIOImpl> event = nullptr ;
    int recordsRead = 0 ;
    // loop over records in the stream. SIO does it nicely for us
    auto validator = [&] ( const sio::record_info &recinfo ) {
      return ( records.find( recinfo._name ) != records.end() ) ;
    } ;
    auto processor = [&] ( const sio::record_info &recinfo, const sio::buffer_span& recdata ) {
      const bool compressed = sio::api::is_compressed( recinfo._options ) ;
      if( compressed ) {
        _compBuffer.resize( recinfo._uncompressed_length ) ;
        sio::zlib_compression compressor ;
        compressor.uncompress( recdata, _compBuffer ) ;
      }
      auto data = compressed ? _compBuffer.span() : recdata ;
      // LCRunHeader case
      if( recinfo._name == LCSIO::RunRecordName ) {
        recordsRead++ ;
        auto runheader = std::make_shared<IOIMPL::LCRunHeaderIOImpl>() ;
        SIORunHeaderRecord::readBlocks( data, runheader.get() ) ;
        runheader->parameters().setValue( "LCIOFileName" ,  _myFilenames[ _currentFileIndex  ] ) ;
        std::set<IO::LCRunListener*>::iterator iter = _runListeners.begin() ;
        while( iter != _runListeners.end() ){
          runheader->setReadOnly( false ) ;
          (*iter)->modifyRunHeader( runheader.get() ) ;
          runheader->setReadOnly( true ) ;
          (*iter)->processRunHeader( runheader.get() ) ;
          iter++ ;
        }
      }
      // Event header case. Setup the event for the next record
      else if( recinfo._name == LCSIO::HeaderRecordName ) {
        event = std::make_shared<IOIMPL::LCEventIOImpl>() ;
        SIOEventHeaderRecord::readBlocks( data, event.get(), _readCollectionNames ) ;
      }
      else if( recinfo._name == LCSIO::EventRecordName ) {
        if( nullptr == event ) {
          throw IO::IOException( "SIOReader::readStream: Event record before an EventHeader record ..." ) ;
        }
        recordsRead++ ;
        SIOEventRecord::readBlocks( data, event.get(), _eventHandlerMgr ) ;
        std::set<IO::LCEventListener*>::iterator iter = _evtListeners.begin() ;
        while( iter != _evtListeners.end() ) {
          postProcessEvent( event.get() ) ;
          // fg20070813 changed order of update and process (needed for
          // Marlin modifying processors )
          event->setAccessMode( EVENT::LCIO::UPDATE ) ;
          (*iter)->modifyEvent( event.get() ) ;
          event->setAccessMode( EVENT::LCIO::READ_ONLY ) ; // set the proper acces mode
          (*iter)->processEvent( event.get() ) ;
          iter++ ;
        }
        // free ressources here
        event = nullptr ;
      }
      return ( recordsRead < maxRecord ) ;
    } ;
    while( 1 ) {
      try {
        sio::api::read_records( _stream, _rawBuffer, validator , processor ) ;
      }
      catch( sio::exception &e ) {
        if( e.code() != sio::error_code::eof ) {
          SIO_RETHROW( e, e.code(), "SIOReader::readStream: Couldn't read stream" ) ;
        }
        else {
          // we caught an eof exception here
          bool nextFileAvailable = (!_myFilenames.empty()  && _currentFileIndex+1 < _myFilenames.size()) ;
          if( nextFileAvailable ) {
            close() ;
            open( _myFilenames[ ++_currentFileIndex  ] ) ;
            if( readUntilEOF ) {
              // read all
              readStream() ;
            }
            else {
              // read the remaining number of records
              readStream( maxRecord - recordsRead ) ;
            }
            return ;
          }
          else {
            if( readUntilEOF ) {
              return ;
            }
            std::stringstream message ;
            message << "SIOReader::readStream(int maxRecord) : EOF before "
              << maxRecord << " records read from file" << std::ends ;
            throw IO::EndOfDataException( message.str() )  ;
          }
        }
        std::stringstream message ;
        message << "SIOReader::readStream(int maxRecord) : EOF before "
          << maxRecord << " records read from file" << std::ends ;
        throw IO::EndOfDataException( message.str() )  ;
      }
    }

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
