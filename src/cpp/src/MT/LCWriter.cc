#include <MT/LCWriter.h>

// -- lcio headers
#include "EVENT/LCEvent.h"
#include "EVENT/LCRunHeader.h"
#include "EVENT/LCIO.h"
#include "EVENT/LCCollection.h"
#include "SIO/LCSIO.h"
#include "SIO/LCIORandomAccessMgr.h"

// -- sio headers
#include <sio/exception.h>
#include <sio/api.h>
#include <sio/compression/zlib.h>

// -- std headers
#include <sys/stat.h>

namespace MT {

  void LCWriter::open( const std::string &filename )  {
    auto sioFilename = getSIOFileName( filename ) ;
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

  void LCWriter::open( const std::string &filename, int writeMode ) {
    // make sure filename has the proper extension (.slcio)
    auto sioFilename = getSIOFileName( filename ) ;
    std::lock_guard<std::mutex> lock( _mutex ) ;
    _raMgr = std::make_shared<SIO::LCIORandomAccessMgr>() ;
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
      	bool hasRandomAccess = _raMgr->initAppend( istr ) ;
       	istr.close() ;
        SIO_DEBUG( "SIOWriter::open: Opening in write append mode 2" );
      	if( hasRandomAccess ) {
      	  _stream.open( sioFilename.c_str() , std::ios::binary | std::ios::out | std::ios::in ) ;
      	  // position at the beginnning of the file record which will then be overwritten with the next record ...
          _stream.seekp( 0 , std::ios_base::end ) ;
          auto endg = _stream.tellp() ;
          if( endg < SIO::LCSIO::RandomAccessSize ) {
            std::stringstream s ;  s << "[SIOWriter::open()] Can't seek stream to " << SIO::LCSIO::RandomAccessSize ;
            throw IO::IOException( s.str() ) ;
          }
          std::streampos tpos = SIO::LCSIO::RandomAccessSize ;
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

  void LCWriter::setCompressionLevel(int level) {
    _compressionLevel = level ;
  }

  //----------------------------------------------------------------------------

  void LCWriter::writeRunHeader( EVENT::LCRunHeader *hdr ) {
    // NOTE: the lock is acquired only on stream writing.     
    // create a buffer with a sufficient length
    auto buflen = _maxBufferSize.load() ;
    sio::buffer rawBuffer( buflen ) ;
    // write the record in the buffer
    sio::record_info recinfo {} ;
    SIO::SIORunHeaderRecord::writeRecord( rawBuffer, hdr, recinfo, 0 ) ;
    // deal with zlib compression here
    if( _compressionLevel.load() != 0 ) {
      sio::zlib_compression compressor {} ;
      compressor.set_level( _compressionLevel.load() ) ;
      // the compressed buffer will have a len
      // smaller than the uncompressed one
      sio::buffer compBuffer( rawBuffer.size() ) ;
      // compress !
      sio::api::compress_record( recinfo, rawBuffer, compBuffer, compressor ) ;
      // locked scope
      {
        std::lock_guard<std::mutex> lock( _mutex ) ;
        if( not _stream.is_open() ) {
          throw IO::IOException( "[MT::LCWriter::writeRunHeader] stream not opened") ;
        }
        sio::api::write_record( _stream, rawBuffer.span(0, recinfo._header_length), compBuffer.span(), recinfo ) ;
        // random access treatment
        _raMgr->add( SIO::RunEvent(  hdr->getRunNumber(), -1 ) , recinfo._file_start ) ;
        // update the maximum size of buffer
        if( _maxBufferSize.load() < rawBuffer.size() ) {
          _maxBufferSize = rawBuffer.size() ;
        }
      }
    }
    else {
      // locked scope
      std::lock_guard<std::mutex> lock( _mutex ) ;
      if( not _stream.is_open() ) {
        throw IO::IOException( "[MT::LCWriter::writeRunHeader] stream not opened") ;
      }
      sio::api::write_record( _stream, rawBuffer.span(), recinfo ) ;
      // random access treatment
      _raMgr->add( SIO::RunEvent(  hdr->getRunNumber(), -1 ) , recinfo._file_start ) ;
      // update the maximum size of buffer
      if( _maxBufferSize.load() < rawBuffer.size() ) {
        _maxBufferSize = rawBuffer.size() ;
      }
    }
  }

  //----------------------------------------------------------------------------

  void LCWriter::writeEvent( EVENT::LCEvent *evt ) {
    writeEvent( evt, {} ) ;
  }
  
  //----------------------------------------------------------------------------

  void LCWriter::writeEvent( EVENT::LCEvent *evt, const std::set<std::string> &colsOnly ) {
    // create buffers with a sufficient length
    auto buflen = _maxBufferSize.load() ;
    sio::buffer hdrRawBuffer( buflen ) ;
    sio::buffer evtRawBuffer( buflen ) ;
    // 1) write the event header record
    sio::record_info rechdrinfo {} ;
    SIO::SIOEventHeaderRecord::writeRecord( hdrRawBuffer, evt, rechdrinfo, colsOnly, 0 ) ;
    // 2) write the event record
    sio::record_info recinfo {} ;
    SIO::SIOHandlerMgr eventHandlerMgr {} ;
    SIO::SIOEventRecord::writeRecord( evtRawBuffer, evt, eventHandlerMgr, recinfo, colsOnly, 0 ) ;
    // deal with zlib compression here
    // and write in locked scope
    if( _compressionLevel.load() != 0 ) {
      sio::zlib_compression compressor {} ;
      compressor.set_level( _compressionLevel.load() ) ;
      sio::buffer hdrCompBuffer( hdrRawBuffer.size() ) ;
      sio::buffer evtCompBuffer( evtRawBuffer.size() ) ;
      // compress records
      sio::api::compress_record( rechdrinfo, hdrRawBuffer, hdrCompBuffer, compressor ) ;
      sio::api::compress_record( recinfo, evtRawBuffer, evtCompBuffer, compressor ) ;
      {
        std::lock_guard<std::mutex> lock( _mutex ) ;
        if( not _stream.is_open() ) {
          throw IO::IOException( "[SIOWriter::writeEvent] stream not opened") ;
        }
        // write to disk
        sio::api::write_record( _stream, hdrRawBuffer.span(0, rechdrinfo._header_length), hdrCompBuffer.span(), rechdrinfo ) ;
        sio::api::write_record( _stream, evtRawBuffer.span(0, recinfo._header_length), evtCompBuffer.span(), recinfo ) ;
        // random access treatment
        _raMgr->add( SIO::RunEvent(  evt->getRunNumber(), evt->getEventNumber()  ) , rechdrinfo._file_start ) ;
        // update the maximum size of buffer
        if( _maxBufferSize.load() < evtRawBuffer.size() ) {
          _maxBufferSize = evtRawBuffer.size() ;
        }
      }
    }
    else {
      std::lock_guard<std::mutex> lock( _mutex ) ;
      if( not _stream.is_open() ) {
        throw IO::IOException( "[SIOWriter::writeEvent] stream not opened") ;
      }
      sio::api::write_record( _stream, hdrRawBuffer.span(), rechdrinfo ) ;
      sio::api::write_record( _stream, evtRawBuffer.span(), recinfo ) ;
      // random access treatment
      _raMgr->add( SIO::RunEvent(  evt->getRunNumber(), evt->getEventNumber()  ) , rechdrinfo._file_start ) ;
      // update the maximum size of buffer
      if( _maxBufferSize.load() < evtRawBuffer.size() ) {
        _maxBufferSize = evtRawBuffer.size() ;
      }
    }
  }

  //----------------------------------------------------------------------------

  void LCWriter::close() {
    std::lock_guard<std::mutex> lock( _mutex ) ;
    _raMgr->writeRandomAccessRecords( _stream ) ;
    _raMgr->clear() ;
    _raMgr = nullptr ;
    _stream.close();
  }

  //----------------------------------------------------------------------------

  std::string LCWriter::getSIOFileName( const std::string& filename ) {
    if( filename.rfind( SIO::LCSIO::FileExtension ) == std::string::npos ||  // .slcio not found at all
     !( filename.rfind( SIO::LCSIO::FileExtension ) + strlen( SIO::LCSIO::FileExtension ) == filename.length() ) ) {  // found, but not at end
      return filename + SIO::LCSIO::FileExtension ;
    }
    else {
      return filename ;
    }
  }

}
