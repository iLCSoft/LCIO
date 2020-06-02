// -- sio headers
#include <sio/api.h>
#include <sio/exception.h>
#include <sio/buffer.h>
#include <sio/io_device.h>
#include <sio/memcpy.h>
#include <sio/compression/zlib.h>
#include <sio/block.h>
#include <sio/version.h>

// -- std headers
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <algorithm>

namespace sio {

  void api::read_relocation( pointed_at_map& pointed_at, pointer_to_map& pointer_to ) {
    // Pointer relocation on read.
    // Some of these variables are a little terse!  Expanded meanings:
    // ptol:  Iterator pointing to lower bound in the 'pointer to' multimap
    // ptoh:  Iterator pointing to upper bound in the 'pointer to' multimap
    // ptoi:  Iterator for the 'pointer to' multimap (runs [ptol, ptoh) )
    // pati:  Iterator in the 'pointed at' map (search map for ptol->first)
    auto ptol  = pointer_to.begin() ;
    while( ptol != pointer_to.end() ) {
      auto ptoh = pointer_to.upper_bound( ptol->first ) ;
      auto pati = pointed_at.find( ptol->first ) ;
      bool pat_found( pati != pointed_at.end() ) ;
      // if the pointed at object is not found we set the pointer to null
      for( auto ptoi = ptol; ptoi != ptoh; ptoi++ ) {
        auto pointer = static_cast<sio::ptr_type *>( ptoi->second ) ;
        *pointer = ( pat_found ? reinterpret_cast<sio::ptr_type>( pati->second ) : 0 ) ;
      }
      ptol = ptoh ;
    }
  }

  //--------------------------------------------------------------------------

  void api::write_relocation( buffer::const_pointer rec_start, pointed_at_map& pointed_at, pointer_to_map& pointer_to ) {
    // Pointer relocation on write.
    // Some of these variables are a little terse!  Expanded meanings:
    // ptol:  Iterator pointing to lower bound in the 'pointer to' multimap
    // ptoh:  Iterator pointing to upper bound in the 'pointer to' multimap
    // ptoi:  Iterator for the 'pointer to' multimap (runs [ptol, ptoh) )
    // pati:  Iterator in the 'pointed at' map (search map for ptol->first)
    unsigned int match = 0x00000001 ;
    auto ptol = pointer_to.begin() ;
    while( ptol != pointer_to.end() ) {
      auto ptoh = pointer_to.upper_bound( ptol->first ) ;
      auto pati = pointed_at.find( ptol->first ) ;
      if( pati != pointed_at.end() ) {
        auto pointer = rec_start + reinterpret_cast<sio::ptr_type>( pati->second ) ;
        sio::memcpy::write( &match, (sio::byte*)pointer, 1 ) ;
        for( auto ptoi = ptol; ptoi != ptoh; ptoi++ ) {
          pointer = rec_start + reinterpret_cast<sio::ptr_type>( ptoi->second ) ;
          sio::memcpy::write( &match, (sio::byte*)pointer, 1 ) ;
        }
      }
      match++ ;
      ptol = ptoh ;
    }
  }

  //--------------------------------------------------------------------------

  void api::read_record_info( sio::ifstream &stream, record_info &rec_info, buffer &outbuf ) {
    if( not stream.is_open() ) {
      SIO_THROW( sio::error_code::not_open, "ifstream is not open!" ) ;
    }
    if( not stream.good() ) {
      SIO_THROW( sio::error_code::bad_state, "ifstream is in a bad state!" ) ;
    }
    rec_info._file_start = stream.tellg() ;
    outbuf.resize( sio::max_record_info_len ) ;
    SIO_DEBUG( "Reading first record bytes of input stream at position: " << stream.tellg() ) ;
    stream.read( outbuf.data(), 8 ) ;
    if( stream.eof() ) {
      SIO_THROW( sio::error_code::eof, "Reached end of file !" ) ;
    }
    if( not stream.good() ) {
      SIO_THROW( sio::error_code::bad_state, "ifstream is in a bad state after reading first record bytes!" ) ;
    }
    unsigned int marker(0) ;
    read_device device( outbuf.span() ) ;
    // Interpret: 1) The length of the record header.
    //            2) The record marker.
    device.data( rec_info._header_length ) ;
    device.data( marker ) ;
    if( marker != sio::record_marker ) {
      stream.setstate( sio::ifstream::failbit ) ;
      SIO_THROW( sio::error_code::no_marker, "Record marker not found!" ) ;
    }
    // Interpret: 3) The options word.
    //            4) The length of the record data (compressed).
    //            5) The length of the record name (uncompressed).
    //            6) The length of the record name.
    //            7) The record name.
    stream.read( outbuf.ptr(8), rec_info._header_length-8 ) ;
    device.seek( 8 ) ;
    device.data( rec_info._options ) ;
    device.data( rec_info._data_length ) ;
    device.data( rec_info._uncompressed_length ) ;
    unsigned int name_length(0) ;
    device.data( name_length ) ;
    if( name_length > sio::max_record_name_len ) {
      SIO_THROW( sio::error_code::no_marker, "Invalid record name size (limited)" ) ;
    }
    rec_info._name.assign( name_length, '\0' ) ;
    device.data( &(rec_info._name[0]), name_length ) ;
    const auto compressed = sio::api::is_compressed( rec_info._options ) ;
    // if the record is compressed skip the read pointer over
    // any padding bytes that may have been inserted to make
    // the next record header start on a four byte boundary in the file.
    auto tot_len = rec_info._data_length + rec_info._header_length ;
    SIO_DEBUG( "Total len before: " << tot_len ) ;
    if( compressed ) {
      tot_len += ((4 - (rec_info._data_length & sio::bit_align)) & sio::bit_align) ;
    }
    SIO_DEBUG( "Total len after padding: " << tot_len ) ;
    rec_info._file_end = rec_info._file_start ;
    rec_info._file_end += tot_len ;
    // a bit of debugging ...
    SIO_DEBUG( "=== Read record info ====" ) ;
    SIO_DEBUG( rec_info ) ;
    SIO_DEBUG( "read_record_info: Resizing buffer to " << rec_info._header_length ) ;
    outbuf.resize( rec_info._header_length ) ;
  }

  //--------------------------------------------------------------------------

  void api::read_record_data( sio::ifstream &stream, const record_info &rec_info, buffer &outbuf, std::size_t buffer_shift ) {
    if( not stream.is_open() ) {
      SIO_THROW( sio::error_code::not_open, "ifstream is not open!" ) ;
    }
    if( not stream.good() ) {
      SIO_THROW( sio::error_code::bad_state, "ifstream is in a bad state!" ) ;
    }
    // resize the buffer to the expected read size.
    // this may not re-allocate the buffer internally
    // if it was large enough
    outbuf.resize( buffer_shift + rec_info._data_length ) ;
    // go to record start
    auto seek_pos = rec_info._file_start ;
    seek_pos += rec_info._header_length ;
    stream.seekg( seek_pos ) ;
    if( not stream.good() ) {
      SIO_THROW( sio::error_code::bad_state, "ifstream is in a bad state after a seek operation!" ) ;
    }
    stream.read( outbuf.ptr( buffer_shift ), rec_info._data_length ) ;
    if( not stream.good() ) {
      SIO_THROW( sio::error_code::io_failure, "ifstream is in a bad state after a read operation!" ) ;
    }
    if( not stream.seekg( rec_info._file_end ).good() ) {
      SIO_THROW( sio::error_code::bad_state, "ifstream is in a bad state after a seek operation!" ) ;
    }
    SIO_DEBUG( "read_record_data: Resizing buffer to " << buffer_shift + rec_info._data_length ) ;
    outbuf.resize( buffer_shift + rec_info._data_length ) ;
  }

  //--------------------------------------------------------------------------

  void api::read_record( sio::ifstream &stream, record_info &rec_info, buffer &outbuf ) {
    // read out the record info
    api::read_record_info( stream, rec_info, outbuf ) ;
    // read out the record data in the buffer. Shift the buffer position by the
    // size of the record header to keep the record header bytes in the buffer too.
    api::read_record_data( stream, rec_info, outbuf, rec_info._header_length ) ;
  }

  //--------------------------------------------------------------------------

  std::pair<record_info, buffer> api::read_record( sio::ifstream &stream ) {
    record_info rec_info ;
    buffer outbuf( sio::mbyte ) ;
    api::read_record( stream, rec_info, outbuf ) ;
    return std::make_pair( rec_info, std::move( outbuf ) ) ;
  }

  //--------------------------------------------------------------------------

  void api::skip_n_records( sio::ifstream &stream, std::size_t nskip ) {
    std::size_t counter = 0 ;
    api::skip_records( stream, [&]( const record_info & ) {
      ++ counter ;
      return ( counter < nskip ) ;
    }) ;
  }

  //--------------------------------------------------------------------------

  void api::skip_records( sio::ifstream &stream, std::size_t nskip, const std::string &name ) {
    std::size_t counter = 0 ;
    api::skip_records( stream, [&]( const record_info &rec_info ) {
      if( name == rec_info._name ) {
        ++ counter ;
      }
      return ( counter < nskip ) ;
    }) ;
  }

  //--------------------------------------------------------------------------

  void api::go_to_record( sio::ifstream &stream, const std::string &name ) {
    record_info goto_info ;
    api::skip_records( stream, [&]( const record_info &rec_info ) {
      if( name == rec_info._name ) {
        goto_info = rec_info ;
        return false ;
      }
      return true ;
    }) ;
    stream.seekg( goto_info._file_start ) ;
    if( not stream.good() ) {
      SIO_THROW( sio::error_code::bad_state, "ifstream is in a bad state after a seek operation!" ) ;
    }
  }

  //--------------------------------------------------------------------------

  std::vector<block_info> api::read_block_infos( const buffer_span &buf ) {
    if( not buf.valid() ) {
      SIO_THROW( sio::error_code::bad_state, "Buffer is invalid." ) ;
    }
    std::vector<block_info> block_infos ;
    buffer_span::index_type current_pos (0) ;
    while( 1 ) {
      // end of block buffer ?
      if( current_pos >= buf.size() ) {
        break ;
      }
      auto block_data = sio::api::extract_block( buf, current_pos ) ;
      current_pos = block_data.first._record_end ;
      block_infos.push_back( block_data.first ) ;
    }
    return block_infos ;
  }

  //--------------------------------------------------------------------------

  std::pair<block_info, buffer_span> api::extract_block( const buffer_span &rec_buf, buffer_span::index_type index ) {
    if( index >= rec_buf.size() ) {
      SIO_THROW( sio::error_code::invalid_argument, "Start of block pointing after end of record!" ) ;
    }
    block_info info ;
    SIO_DEBUG( "Block buffer size is " << rec_buf.size() ) ;
    SIO_DEBUG( "Block index is " << index ) ;
    read_device device( rec_buf.subspan( index ) ) ;
    info._record_start = index ;
    unsigned int marker(0), block_len(0) ;
    device.data( block_len ) ;
    SIO_DEBUG( "Block len is " << block_len ) ;
    device.data( marker ) ;
    // check for a block marker
    if( sio::block_marker != marker ) {
      std::stringstream ss ;
      ss << "Block marker not found (block marker: " << sio::block_marker <<", record marker: " << sio::record_marker << ", got " << marker << ")" ;
      SIO_THROW( sio::error_code::no_marker, ss.str() ) ;
    }
    device.data( info._version ) ;
    unsigned int name_len(0) ;
    device.data( name_len ) ;
    info._name.assign( name_len, '\0' ) ;
    device.data( &(info._name[0]), name_len ) ;
    info._header_length = device.position() ;
    info._data_length = block_len - info._header_length ;
    device.seek( block_len ) ;
    info._record_end = index + block_len ;
    return std::make_pair( info, rec_buf.subspan( index, block_len ) ) ;
  }

  //--------------------------------------------------------------------------

  void api::read_blocks( const buffer_span &rec_buf, const std::vector<std::shared_ptr<block>>& blocks ) {
    if( not rec_buf.valid() ) {
      SIO_THROW( sio::error_code::bad_state, "Buffer is invalid." ) ;
    }
    buffer_span::index_type current_pos (0) ;
    read_device device ;
    while( 1 ) {
      // end of block buffer ?
      if( current_pos >= rec_buf.size() ) {
        break ;
      }
      auto block_data = sio::api::extract_block( rec_buf, current_pos ) ;
      current_pos = block_data.first._record_end ;
      // look for the block decoder
      auto block_iter = std::find_if( blocks.begin(), blocks.end(), [&]( std::shared_ptr<block> blk ) {
        return ( blk->name() == block_data.first._name ) ;
      }) ;
      // skip the block if no decoder
      if( blocks.end() == block_iter ) {
        continue ;
      }
      // prepare the read device
      device.set_buffer( block_data.second ) ;
      device.seek( block_data.first._header_length ) ;
      try {
        (*block_iter)->read( device, block_data.first._version ) ;
      }
      catch( sio::exception &e ) {
        SIO_RETHROW( e, sio::error_code::io_failure, "Failed to decode block buffer (" + block_data.first._name + ")" ) ;
      }
    }
    device.pointer_relocation() ;
  }

  //--------------------------------------------------------------------------

  void api::dump_records( sio::ifstream &stream, std::size_t skip, std::size_t count, bool detailed ) {
    try {
      // skip records first
      if( skip > 0 ) {
        sio::api::skip_n_records( stream, skip ) ;
      }
      sio::record_info rec_info ;
      sio::buffer info_buffer( sio::max_record_info_len ) ;
      sio::buffer rec_buffer( sio::mbyte ) ;
      sio::buffer uncomp_rec_buffer( sio::mbyte ) ;
      unsigned int record_counter (0) ;
      const unsigned int tab_len = 117 ;
      if( not detailed ) {
        std::cout << std::string( tab_len, '-' ) << std::endl ;
        std::cout <<
          std::setw(30) << std::left << "Record name " << " | " <<
          std::setw(15) << "Start" << " | " <<
          std::setw(15) << "End" << " | " <<
          std::setw(12) << "Options" << " | " <<
          std::setw(10) << "Header len" << " | " <<
          std::setw(15) << "Data len" <<
          std::endl ;
        std::cout << std::string( tab_len, '-' ) << std::endl ;
      }
      while(1) {
        if( record_counter >= count ) {
          break ;
        }
        SIO_DEBUG( "Start reading next record info from stream" ) ;
        sio::api::read_record_info( stream, rec_info, info_buffer ) ;
        if( detailed ) {
          SIO_DEBUG( "Detailed: Start reading next record data from stream" ) ;
          sio::api::read_record_data( stream, rec_info, rec_buffer ) ;
        }
        // seek after the record to read the next record info
        stream.seekg( rec_info._file_end ) ;
        ++ record_counter ;
        if( detailed ) {
          std::cout << std::string( tab_len, '-' ) << std::endl ;
          std::cout <<
            std::setw(30) << std::left << "Record name " << " | " <<
            std::setw(15) << "Start" << " | " <<
            std::setw(15) << "End" << " | " <<
            std::setw(12) << "Options" << " | " <<
            std::setw(10) << "Header len" << " | " <<
            std::setw(15) << "Data len" <<
            std::endl ;
        }
        std::stringstream size_str ;
        size_str << rec_info._data_length << " (" << rec_info._uncompressed_length << ")" ;
        std::cout <<
          std::setw(30) << std::left << rec_info._name << " | " <<
          std::setw(15) << rec_info._file_start << " | " <<
          std::setw(15) << rec_info._file_end << " | " <<
          std::setw(12) << rec_info._options << " | " <<
          std::setw(10) << rec_info._header_length << " | " <<
          std::setw(15) << size_str.str() <<
          std::endl ;
        if( detailed ) {
          std::cout << std::string( tab_len, '-' ) << std::endl ;
          std::cout <<
            std::setw(30) << std::left << "Block name " << " | " <<
            std::setw(15) << "Start" << " | " <<
            std::setw(15) << "End" << " | " <<
            std::setw(12) << "Version" << " | " <<
            std::setw(10) << "Header len" << " | " <<
            std::setw(15) << "Data len" <<
            std::endl ;
          std::cout << std::string( tab_len, '-' ) << std::endl ;
          const bool compressed = sio::api::is_compressed( rec_info._options ) ;
          if( compressed ) {
            // FIXME use zlib by default ??
            sio::zlib_compression compressor ;
            uncomp_rec_buffer.resize( rec_info._uncompressed_length ) ;
            compressor.uncompress( rec_buffer.span(), uncomp_rec_buffer ) ;
          }
          sio::buffer_span device_buffer = compressed ? uncomp_rec_buffer.span() : rec_buffer.span( 0, rec_info._data_length ) ;
          SIO_DEBUG( "Start extracting block infos" ) ;
          auto block_infos = sio::api::read_block_infos( device_buffer ) ;
          SIO_DEBUG( "Number of blocks found: " << block_infos.size() ) ;
          for( auto binfo : block_infos ) {
            std::stringstream version_str ;
            version_str << sio::version::major_version( binfo._version ) << "." << sio::version::minor_version( binfo._version ) ;
            std::cout <<
              std::setw(30) << std::left << binfo._name << " | " <<
              std::setw(15) << binfo._record_start << " | " <<
              std::setw(15) << binfo._record_end << " | " <<
              std::setw(12) << version_str.str() << " | " <<
              std::setw(10) << binfo._header_length << " | " <<
              std::setw(15) << binfo._data_length <<
              std::endl ;
          }
          std::cout << std::endl ;
        }
      }
    }
    catch( sio::exception &e ) {
      // we are finished !
      if( e.code() == sio::error_code::eof ) {
        return ;
      }
      throw e ;
    }
  }

  //--------------------------------------------------------------------------

  void api::write_blocks( write_device &device, const block_list &blocks ) {
    for( auto blk : blocks ) {
      auto blk_ptr = blk ;
      try {
        auto block_start = device.position() ;
        auto block_name = blk_ptr->name() ;
        unsigned int blkname_len = block_name.size() ;
        // write the block header. It will be updated after
        // the block has been written
        device.data( sio::block_marker ) ;
        device.data( sio::block_marker ) ;
        device.data( blk_ptr->version() ) ;
        device.data( blkname_len ) ;
        device.data( block_name.c_str(), blkname_len ) ;
        // write the block data
        blk_ptr->write( device ) ;
        // fill back the block length in block header
        auto blk_end = device.position() ;
        unsigned int blklen = blk_end - block_start ;
        SIO_DEBUG( "Block len :" << blklen ) ;
        device.seek( block_start ) ;
        device.data( blklen ) ;
        device.seek( blk_end ) ;
      }
      catch( sio::exception &e ) {
        SIO_RETHROW( e, sio::error_code::io_failure, "Couldn't write block to buffer (" + blk_ptr->name() + ")" ) ;
      }
    }
    device.pointer_relocation() ;
  }

  //--------------------------------------------------------------------------

  record_info api::write_record( const std::string &name, buffer &rec_buf, const block_list& blocks, sio::options_type opts ) {
    if( not sio::valid_record_name( name ) ) {
      SIO_THROW( sio::error_code::invalid_argument, "Record name '" + name + "' is invalid" ) ;
    }
    if( not rec_buf.valid() ) {
      SIO_THROW( sio::error_code::invalid_argument, "Buffer is invalid" ) ;
    }
    try {
      sio::api::set_compression( opts, false ) ;
      record_info info ;
      info._options = opts ;
      info._name = name ;
      write_device device( std::move(rec_buf) ) ;
      // Output: 1) A placeholder for the record header length.
      //         2) A 'framing' marker (to help in debugging).
      //         3) An options word.
      //         4) A placeholder for the record data length (compressed).
      //         5) A placeholder for the record data length (uncompressed).
      //         6) The length of the record name.
      //         7) The record name.
      device.data( sio::record_marker ) ;
      device.data( sio::record_marker ) ;
      device.data( opts ) ;
      auto datalen_pos = device.position() ;
      device.data( sio::record_marker ) ;
      device.data( sio::record_marker ) ;
      unsigned int name_len = name.size() ;
      device.data( name_len ) ;
      device.data( &(name[0]), name_len ) ;
      // write back the header len
      info._header_length = device.position() ;
      device.seek( 0 ) ;
      device.data( info._header_length ) ;
      device.seek( info._header_length ) ;
      // write the blocks
      sio::api::write_blocks( device, blocks ) ;
      // fill the data length and uncompressed record length
      auto end_pos = device.position() ;
      device.seek( datalen_pos ) ;
      info._data_length = end_pos - info._header_length ;
      info._uncompressed_length = end_pos - info._header_length ;
      device.data( info._data_length ) ;
      device.data( info._uncompressed_length ) ;
      // get back the buffer
      rec_buf = std::move( device.take_buffer() ) ;
      rec_buf.resize( end_pos ) ;
      return info ;
    }
    catch( sio::exception &e ) {
      SIO_RETHROW( e, sio::error_code::io_failure, "Couldn't write record into buffer" ) ;
    }
  }

  //--------------------------------------------------------------------------

  void api::write_record( sio::ofstream &stream, const buffer_span &rec_buf, record_info &rec_info ) {
    if( not stream.is_open() ) {
      SIO_THROW( sio::error_code::not_open, "ofstream is not open!" ) ;
    }
    if( not stream.good() ) {
      SIO_THROW( sio::error_code::bad_state, "ofstream is in a bad state!" ) ;
    }
    if( not rec_buf.valid() ) {
      SIO_THROW( sio::error_code::invalid_argument, "The record buffer is not valid" ) ;
    }
    rec_info._file_start = stream.tellp() ;
    // write data
    if( not stream.write( rec_buf.data(), rec_buf.size() ).good() ) {
      SIO_THROW( sio::error_code::io_failure, "Couldn't write record buffer to output stream" ) ;
    }
    // always add some padding bytes at the end
    auto padlen = (4 - (rec_buf.size() & sio::bit_align)) & sio::bit_align;
    if( padlen > 0 ) {
      SIO_DEBUG( "Writing " << padlen << " bytes for padding ..." ) ;
      if( not stream.write( sio::padding_bytes, padlen ).good() ) {
        SIO_THROW( sio::error_code::io_failure, "Couldn't write record buffer padding to output stream" ) ;
      }
    }
    // flush the stream
    if( not stream.flush().good() ) {
      SIO_THROW( sio::error_code::io_failure, "Couldn't flush output stream" ) ;
    }
    rec_info._file_end = stream.tellp() ;
    SIO_DEBUG( "Written record with info :\n" << rec_info ) ;
  }

  //--------------------------------------------------------------------------

  void api::write_record( sio::ofstream &stream, const buffer_span &hdr_span, const buffer_span &data_span, record_info &rec_info ) {
    if( not stream.is_open() ) {
      SIO_THROW( sio::error_code::not_open, "ofstream is not open!" ) ;
    }
    if( not stream.good() ) {
      SIO_THROW( sio::error_code::bad_state, "ofstream is in a bad state!" ) ;
    }
    if( not hdr_span.valid() ) {
      SIO_THROW( sio::error_code::invalid_argument, "The record header buffer is not valid" ) ;
    }
    if( not data_span.valid() ) {
      SIO_THROW( sio::error_code::invalid_argument, "The record data buffer is not valid" ) ;
    }
    rec_info._file_start = stream.tellp() ;
    // write record header
    if( not stream.write( hdr_span.data(), hdr_span.size() ).good() ) {
      SIO_THROW( sio::error_code::io_failure, "Couldn't write record header buffer to output stream" ) ;
    }
    // write record data
    if( not stream.write( data_span.data(), data_span.size() ).good() ) {
      SIO_THROW( sio::error_code::io_failure, "Couldn't write record data buffer to output stream" ) ;
    }
    // always add some padding bytes at the end
    auto padlen = (4 - (data_span.size() & sio::bit_align)) & sio::bit_align;
    if( padlen > 0 ) {
      SIO_DEBUG( "Writing " << padlen << " bytes for padding ..." ) ;
      if( not stream.write( sio::padding_bytes, padlen ).good() ) {
        SIO_THROW( sio::error_code::io_failure, "Couldn't write record buffer padding to output stream" ) ;
      }
    }
    // flush the stream
    if( not stream.flush().good() ) {
      SIO_THROW( sio::error_code::io_failure, "Couldn't flush output stream" ) ;
    }
    rec_info._file_end = stream.tellp() ;
    SIO_DEBUG( "Written record with info :\n" << rec_info ) ;
  }

  //--------------------------------------------------------------------------

  bool api::is_compressed( options_type opts ) {
    return static_cast<bool>( opts & sio::compression_bit ) ;
  }

  //--------------------------------------------------------------------------

  bool api::set_compression( options_type &opts, bool value ) {
    bool out = sio::api::is_compressed( opts ) ;
    opts &= ~sio::compression_bit ;
    if( value ) {
      opts |= sio::compression_bit ;
    }
    return out ;
  }

}
