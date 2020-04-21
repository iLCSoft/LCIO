#include "SIO/SIOWriter.h"

// -- lcio headers
#include "EVENT/LCEvent.h"
#include "EVENT/LCRunHeader.h"
#include "EVENT/LCIO.h"
#include "EVENT/LCCollection.h"
#include "SIO/LCSIO.h"

// -- sio headers
#include <sio/exception.h>
#include <sio/api.h>
#include <sio/compression/zlib.h>

// -- std headers
#include <sys/stat.h>

namespace SIO {

  void SIOWriter::open(const std::string & filename)  {
    std::string sioFilename ;
    getSIOFileName( filename, sioFilename ) ;
    struct stat fileinfo ;
    // if the file exists we throw an exception
    if ( ::stat( sioFilename.c_str(), &fileinfo ) == 0 ) {
      throw IO::IOException( std::string( "[SIOWriter::open()] File already exists: "
    				      + sioFilename
				      + " \n              open it in append or new mode !\n"
				      )) ;
    }
    // open new file for writing
    open( filename, EVENT::LCIO::WRITE_NEW ) ;
  }

  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------

  void SIOWriter::getSIOFileName(const std::string& filename, std::string& sioFilename ) {
    if( filename.rfind( LCSIO::FileExtension ) == std::string::npos ||  // .slcio not found at all
	   !( filename.rfind(LCSIO::FileExtension) + strlen( LCSIO::FileExtension ) == filename.length() ) ) {  // found, but not at end
      sioFilename = filename + LCSIO::FileExtension ;
    }
    else {
      sioFilename = filename ;
    }
  }

  //----------------------------------------------------------------------------

  void SIOWriter::open(const std::string& filename, int writeMode)  {
    // make sure filename has the proper extension (.slcio)
    std::string sioFilename ;
    getSIOFileName( filename, sioFilename ) ;
    switch( writeMode ) {
      case EVENT::LCIO::WRITE_NEW :
      _stream.open( sioFilename , std::ios::binary ) ;
      	break ;
      case EVENT::LCIO::WRITE_APPEND :
      	// try to read the last LCIORandomAccess record at the end --------------------
        SIO_DEBUG( "SIOWriter::open: Opening in write append mode" );
        sio::ifstream istr ;
        istr.open( sioFilename, std::ios::binary ) ;
        // status  = _stream->open( sioFilename.c_str() , SIO_MODE_READ ) ;
      	if( not istr.is_open() ) {
      	  throw IO::IOException( std::string( "[SIOWriter::open()] Can't open stream for reading TOC: " + sioFilename ) ) ;
        }
        SIO_DEBUG( "SIOWriter::open: Opening in write append mode 1" );
      	bool hasRandomAccess = _raMgr.initAppend(istr ) ;
       	istr.close() ;
        SIO_DEBUG( "SIOWriter::open: Opening in write append mode 2" );
      	if( hasRandomAccess ) {
      	  _stream.open( sioFilename.c_str() , std::ios::binary | std::ios::out | std::ios::in ) ;
      	  // position at the beginnning of the file record which will then be overwritten with the next record ...
          _stream.seekp( 0 , std::ios_base::end ) ;
          auto endg = _stream.tellp() ;
          if( endg < LCSIO::RandomAccessSize ) {
            std::stringstream s ;  s << "[SIOWriter::open()] Can't seek stream to " << LCSIO::RandomAccessSize ;
            throw IO::IOException( s.str() ) ;
          }
          std::streampos tpos = LCSIO::RandomAccessSize ;
          _stream.seekp( endg - tpos , std::ios_base::beg ) ;
      	}
        else {
          _stream.open( sioFilename.c_str() , std::ios::binary | std::ios::out | std::ios::ate ) ;
      	}
        SIO_DEBUG( "SIOWriter::open: Opening in write append mode ... OK" );
      	break ;
    }
    if( not _stream.good() or not _stream.is_open() ) {
      SIO_THROW( sio::error_code::not_open, "[SIOWriter::open()] Couldn't open file: '" + sioFilename + "'" ) ;
    }
  }

  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------

  void SIOWriter::setCompressionLevel(int level) {
    _compressor.set_level( level ) ;
  }

  //----------------------------------------------------------------------------

  void SIOWriter::writeRunHeader(const EVENT::LCRunHeader * hdr)   {
    if( not _stream.is_open() ) {
      throw IO::IOException( "[SIOWriter::writeRunHeader] stream not opened") ;
    }
    sio::record_info recinfo {} ;
    SIORunHeaderRecord::writeRecord( _rawBuffer, const_cast<EVENT::LCRunHeader*>(hdr), recinfo, 0 ) ;
    // deal with zlib compression here
    if( _compressor.level() != 0 ) {
      sio::api::compress_record( recinfo, _rawBuffer, _compBuffer, _compressor ) ;
      sio::api::write_record( _stream, _rawBuffer.span(0, recinfo._header_length), _compBuffer.span(), recinfo ) ;
    }
    else {
      sio::api::write_record( _stream, _rawBuffer.span(), recinfo ) ;
    }
    // random access treatment
    _raMgr.add( RunEvent(  hdr->getRunNumber(), -1 ) , recinfo._file_start ) ;
  }

  //----------------------------------------------------------------------------

  void SIOWriter::writeEvent(const EVENT::LCEvent* evt) {

    if( not _stream.is_open() ) {
      throw IO::IOException( "[SIOWriter::writeEvent] stream not opened") ;
    }
    // 1) write the event header record
    sio::record_info rechdrinfo {} ;
    SIOEventHeaderRecord::writeRecord( _rawBuffer, const_cast<EVENT::LCEvent*>(evt), rechdrinfo, 0 ) ;
    // deal with zlib compression here
    if( _compressor.level() != 0 ) {
      sio::api::compress_record( rechdrinfo, _rawBuffer, _compBuffer, _compressor ) ;
      sio::api::write_record( _stream, _rawBuffer.span(0, rechdrinfo._header_length), _compBuffer.span(), rechdrinfo ) ;
    }
    else {
      sio::api::write_record( _stream, _rawBuffer.span(), rechdrinfo ) ;
    }
    // random access treatment
    _raMgr.add( RunEvent(  evt->getRunNumber(), evt->getEventNumber()  ) , rechdrinfo._file_start ) ;

    // 2) write the event record
    sio::record_info recinfo {} ;
    SIOEventRecord::writeRecord( _rawBuffer, const_cast<EVENT::LCEvent*>(evt), _eventHandlerMgr, recinfo, 0 ) ;
    // deal with zlib compression here
    if( _compressor.level() != 0 ) {
      sio::api::compress_record( recinfo, _rawBuffer, _compBuffer, _compressor ) ;
      sio::api::write_record( _stream, _rawBuffer.span(0, recinfo._header_length), _compBuffer.span(), recinfo ) ;
    }
    else {
      sio::api::write_record( _stream, _rawBuffer.span(), recinfo ) ;
    }
  }

  //----------------------------------------------------------------------------

  void SIOWriter::close() {
    _raMgr.writeRandomAccessRecords( _stream ) ;
    _raMgr.clear() ;
    _stream.close();
  }

  //----------------------------------------------------------------------------

  void SIOWriter::flush()  {
    _stream.flush() ;
  }

} // namespace
