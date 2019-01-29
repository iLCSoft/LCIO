// ----------------------------------------------------------------------------
// CVS $Id: SIO_stream.cc,v 1.9 2010-06-22 13:49:54 gaede Exp $
// ----------------------------------------------------------------------------
// => Controller for a single SIO stream.                          
// ----------------------------------------------------------------------------
//
// General Description:
//
// SIO_stream manages operations of a single SIO stream.
//
// ----------------------------------------------------------------------------

#ifdef _MSC_VER
#   pragma warning(disable:4786)        // >255 characters in debug information
#endif

// -- std headers
#include <iostream>
#include <cstdlib>
#include <cstring>

// -- zlib headers
#include "zlib.h"

// -- sio headers
#include "SIO_block.h"
#include "SIO_definitions.h"
#include "SIO_functions.h"
#include "SIO_record.h"
#include "SIO_stream.h"

// IO markers definitions
static unsigned int
    SIO_align       = 0x00000003,
    SIO_mark_record = 0xabadcafe,
    SIO_mark_block  = 0xdeadbeef;

namespace sio {
  
  unsigned int stream::open(const std::string &fname, SIO_stream_mode m) {
    const char filemode[4][3] = { "rb", "wb", "ab","r+" };
    if(SIO_STATE_OPEN == _state || SIO_STATE_ERROR == _state) {
      return SIO_STREAM_ALREADYOPEN;
    }
    if(SIO_MODE_UNDEFINED == m) {
      return SIO_STREAM_BADMODE;
    }
    // open the file
    if( (_handle = FOPEN( fname.c_str(), filemode[m] )) == NULL ) {
      return SIO_STREAM_OPENFAIL;
    }
    _filename = fname;
    _mode = m;
    _state = SIO_STATE_OPEN;
    // allocate raw buffer
    _buffer_begin = static_cast<unsigned char*>(malloc( _reserve ));
    if(nullptr == _buffer_begin) {
      _state = SIO_STATE_ERROR;
      return SIO_STREAM_NOALLOC;
    }
    _buffer_current = _buffer_begin;
    _buffer_end = _buffer_begin + _reserve;
    // allocate compression buffer
    _cmp_begin = nullptr;
    _cmp_end = nullptr;
    _cmp_begin = static_cast<unsigned char*>(malloc( _reserve >> 2 ));
    if(nullptr == _cmp_begin) {
      _state = SIO_STATE_ERROR;
      return SIO_STREAM_BADCOMPRESS;
    }
    _cmp_end = _cmp_begin + (_reserve >> 2);
    // zstream initialization
    _z_stream = static_cast<z_stream*>( malloc( sizeof( z_stream ) ));
    _z_stream->zalloc = Z_NULL;
    _z_stream->zfree  = Z_NULL;
    _z_stream->opaque = 0;
    auto z_stat = (_mode == SIO_MODE_READ) ? inflateInit( _z_stream ) : deflateInit( _z_stream, _compression_level );
    if( z_stat != Z_OK ) {
        _state = SIO_STATE_ERROR;
        return SIO_STREAM_BADCOMPRESS;
    }
    return SIO_STREAM_SUCCESS;
  }
  
  //----------------------------------------------------------------------------
  
  unsigned int stream::close() {
    if(_state == SIO_STATE_CLOSED) {
        return SIO_STREAM_NOTOPEN;
    }
    unsigned int status = SIO_STREAM_SUCCESS;
    // clear pointer maps
    _pointed_at.clear();
    _pointer_to.clear();
    // clear zlib memory
    auto zstatus = clear_zstream();
    if(zstatus != SIO_STREAM_SUCCESS) {
      status = zstatus;
    }
    // clear local buffers
    if(nullptr != _cmp_begin) {
      free(_cmp_begin);
      _cmp_begin = nullptr;
      _cmp_end = nullptr;
    }
    if(nullptr != _buffer_begin)
    {
        free(_buffer_begin);
        _buffer_begin = NULL;
        _buffer_current = NULL;
        _buffer_end = NULL;
        _record_end = NULL;
        _block_end = NULL;
    }
    if( FCLOSE( _handle ) == EOF ) {
      status = SIO_STREAM_GOTEOF;
    }
    _filename.clear();
    _handle = nullptr;
    _mode = SIO_MODE_UNDEFINED;
    _state = SIO_STATE_CLOSED;
    return status;
  }
  
  //----------------------------------------------------------------------------
  
  unsigned int stream::flush() {
    if(_state == SIO_STATE_CLOSED) {
      return SIO_STREAM_NOTOPEN;
    }
    int rc = FFLUSH( _handle );
    return (rc == 0) ? SIO_STREAM_SUCCESS : SIO_STREAM_BADWRITE;
  }
  
  //----------------------------------------------------------------------------
  
  unsigned int stream::seek(SIO_64BITINT pos, int whence) {
    unsigned int status = FSEEK(_handle, pos, whence);    
    if( status != 0 ) {
      _state = SIO_STATE_ERROR;
      return SIO_STREAM_EOF;
    }
    return SIO_STREAM_SUCCESS;
  }
  
  //----------------------------------------------------------------------------
  
  unsigned int stream::reset( SIO_64BITINT pos ) {
    unsigned int status = this->seek( pos ) ;
    if( SIO_STREAM_SUCCESS == status ) {
      // if we can seek the file  the stream should be fine ...
      _state = SIO_STATE_OPEN;
    }
    return status;
  }
  
  //----------------------------------------------------------------------------
  
  void stream::set_compression_level(int level) {
    if(level < 0) {
      _compression_level = Z_DEFAULT_COMPRESSION;
    }
    else if(level > 9) {
      _compression_level = 9;
    }
    else {
      _compression_level = level;
    }
  }
  
  //----------------------------------------------------------------------------
  
  void stream::pointer_relocation_read() {
    // Pointer relocation on read.
    // Some of these variables are a little terse!  Expanded meanings:
    // ptol:  Iterator pointing to lower bound in the 'pointer to' multimap
    // ptoh:  Iterator pointing to upper bound in the 'pointer to' multimap
    // ptoi:  Iterator for the 'pointer to' multimap (runs [ptol, ptoh) )
    // pati:  Iterator in the 'pointed at' map (search map for ptol->first)
    auto ptol  = _pointer_to.begin();
    while( ptol != _pointer_to.end() ) {
      auto ptoh = _pointer_to.upper_bound( ptol->first );
      auto pati = _pointed_at.find( ptol->first );

      bool pat_found( pati != _pointed_at.end() ) ;
      
      // if the pointed at object is not found we set the pointer to null
      for( auto ptoi = ptol; ptoi != ptoh; ptoi++ ) {
        auto pointer = static_cast<SIO_POINTER_DECL *>(ptoi->second);
        *pointer = ( pat_found ? reinterpret_cast<SIO_POINTER_DECL>(pati->second) : 0 ) ;
      }
      ptol = ptoh;
    }
    _pointer_to.clear();
    _pointed_at.clear();
  }
  
  //----------------------------------------------------------------------------
  
  void stream::pointer_relocation_write() {
    // Pointer relocation on write.
    // Some of these variables are a little terse!  Expanded meanings:
    // ptol:  Iterator pointing to lower bound in the 'pointer to' multimap
    // ptoh:  Iterator pointing to upper bound in the 'pointer to' multimap
    // ptoi:  Iterator for the 'pointer to' multimap (runs [ptol, ptoh) )
    // pati:  Iterator in the 'pointed at' map (search map for ptol->first)
    unsigned int match = 0x00000001;
    auto ptol = _pointer_to.begin();
    while( ptol != _pointer_to.end() ) {
      auto ptoh = _pointer_to.upper_bound( ptol->first );
      auto pati = _pointed_at.find( ptol->first );
      if( pati != _pointed_at.end() ) {
        auto pointer = _buffer_begin + reinterpret_cast<SIO_POINTER_DECL>(pati->second);
        functions::copy( UCHR_CAST( &match ), pointer, SIO_LEN_QB, 1 );
        for( auto ptoi = ptol; ptoi != ptoh; ptoi++ ) {
          pointer = _buffer_begin + reinterpret_cast<SIO_POINTER_DECL>(ptoi->second);
          functions::copy( UCHR_CAST( &match ), pointer, SIO_LEN_QB, 1 );
	      }
      }
      match++;
      ptol = ptoh;
    }
    _pointer_to.clear();
    _pointed_at.clear();
  }
  
  //----------------------------------------------------------------------------
  
  unsigned int stream::clear_zstream() {
    unsigned int status = SIO_STREAM_SUCCESS;
    if( nullptr != _z_stream ) {
      auto z_stat = (_mode == SIO_MODE_READ) ? inflateEnd( _z_stream ) : deflateEnd( _z_stream );
      if( z_stat != Z_OK ) {
        status = SIO_STREAM_BADCOMPRESS;
      }
      free(_z_stream);
      _z_stream = nullptr;
    }
    return status;
  }
  
  //----------------------------------------------------------------------------
  
  unsigned int stream::read_pointed_at(SIO_POINTER_DECL *ptr) {
    // Read.  Keep a record of the "match" quantity read from the buffer and
    // the location in memory which will need relocating.
    const unsigned int SIO_ptag = 0xffffffff;
    unsigned int match = 0;
    unsigned int status = read_raw( SIO_LEN_QB, 1, UCHR_CAST( &match ) );
    if( !( status & 1 ) ) {
      return status;
    }
    // Ignore match = SIO_ptag. This is basically a pointer target which was
    // never relocated when the record was written. i.e. nothing points to it!
    // Don't clutter the maps with information that can never be used. 
    if( match != SIO_ptag ) {
      pointed_at_map::value_type entry = {reinterpret_cast<void *>(match), ptr };
      _pointed_at.insert( entry );
    }
    return SIO_STREAM_SUCCESS;
  }
  
  //----------------------------------------------------------------------------
  
  unsigned int stream::write_pointed_at(SIO_POINTER_DECL *ptr) {
    // Write.  Save the memory location of this object along with the offset
    // in the output buffer where the generated match quantity must go.  Put
    // a placeholder in the output buffer (it will be overwritten at the "output
    // relocation" stage).
    unsigned int SIO_ptag = 0xffffffff;
    pointed_at_map::value_type entry = {ptr, reinterpret_cast<void *>( _buffer_current - _buffer_begin ) };
    _pointed_at.insert( entry );
    return write_raw( SIO_LEN_QB, 1, UCHR_CAST(&SIO_ptag));
  }
  
  //----------------------------------------------------------------------------
  
  unsigned int stream::read_pointer_to(SIO_POINTER_DECL *ptr) {
    // Read.  Keep a record of the "match" quantity read from the buffer and
    // the location in memory which will need relocating.
    //
    // Placeholder value for 'pointer to'
    unsigned int SIO_pntr = 0x00000000;
    unsigned int match = 0;
    unsigned int status = read_raw( SIO_LEN_QB, 1, UCHR_CAST( &match ) );
    if( !( status & 1 ) ) {
      return status;
    }
    // Ignore match = SIO_pntr.  This is basically a null pointer which can
    // never be relocated, so don't fill the multimap with a lot of useless
    // information.
    if( match != SIO_pntr ) {
        pointer_to_map::value_type entry = {reinterpret_cast<void *>(match), ptr};
        _pointer_to.insert( entry );
    }
    // Hand -something- back to the caller.  The number passed back is -not-
    // a pointer, and pointer relocation will not occur until the whole record
    // has been read.  The only circumstance where the next line is important
    // is the case of a NULL pointer which the caller may be relying on to
    // find the end of (for instance) a singly linked list.
    *ptr = static_cast<SIO_POINTER_DECL>(match);
    return SIO_STREAM_SUCCESS;
  }
  
  //----------------------------------------------------------------------------
  
  unsigned int stream::write_pointer_to(SIO_POINTER_DECL *ptr) {
    // Write.  Keep a record of the "match" quantity (i.e. the value of the
    // pointer (which may be different lengths on different machines!)) and
    // the current offset in the output buffer.  Put a placeholder in the
    // output buffer (it will be overwritten at the "output relocation" stage).
    // 
    // Placeholder value for 'pointer to'
    unsigned int SIO_pntr = 0x00000000;
    // ptr is really a pointer-to-a-pointer.  This routine is most interested
    // in the value of *xfer when treated as a pointer.  C++ tends to object
    // to this as being 'not type safe'.  To keep the compiler happy (and purists
    // miserable), do one 'reinterpret_cast' immediately to make later code
    // easier to read.
    // Indirect ptr (actually **ptr)
    void *ifer = reinterpret_cast<void *>(*ptr);
    // Ignore NULL pointers.  These are always recorded in the buffer with a
    // zero match word (and are treated specially when read back).  There's no
    // point in putting useless information in the maps.
    //
    if( nullptr != ifer ) {
        pointer_to_map::value_type entry = {ifer, reinterpret_cast<void *>(_buffer_current - _buffer_begin) };
        _pointer_to.insert( entry );
    }
    return write_raw( SIO_LEN_QB, 1, UCHR_CAST(&SIO_pntr));
  }
  
  //----------------------------------------------------------------------------

  unsigned int stream::write_raw(const int size, const int count, unsigned char *to) {
    if( SIO_STATE_ERROR == _state ) {
      return SIO_STREAM_BADSTATE;
    }
    const int bytlen = count * size;
    const int padlen = (bytlen + 3) & 0xfffffffc;
    // resize the buffer ?
    if( (_buffer_current + padlen) > _buffer_end ) {
      const int minNeeded = (_buffer_current + padlen) - _buffer_end;
      const int bufSize = _buffer_end - _buffer_begin;
      int resizeFactor = 2;
      while( bufSize * (resizeFactor-1)  < minNeeded ) {
        ++resizeFactor ;
      }
      // create new buffer
      const int newlen = bufSize * resizeFactor;
      unsigned char *newbuf = (unsigned char *)malloc( newlen );
      if( nullptr == newbuf) {
        _state = SIO_STATE_ERROR;
        return SIO_STREAM_NOALLOC;
      }
      const int oldlen = _buffer_current - _buffer_current;
 
      memcpy(newbuf, _buffer_begin, oldlen);
      free(_buffer_begin);
      _buffer_end = newbuf + (_buffer_end - _buffer_begin);
      _buffer_end = newbuf + newlen;
      _buffer_current = newbuf + oldlen;
      _buffer_begin = newbuf;
    }
    // Copy and write padding as necessary (xdr format)
    functions::copy( to, _buffer_current, size, count );
    _buffer_current += bytlen;
    for( int bytcnt = bytlen; bytcnt < padlen; bytcnt++ ) {
      *_buffer_current++ = 0;
    }
    return SIO_STREAM_SUCCESS;
  }
  
  //----------------------------------------------------------------------------
  
  unsigned int stream::read_raw(const int size, const int count, unsigned char *from) {
    // check stream state
    if( SIO_STATE_ERROR == _state ) {
      return SIO_STREAM_BADSTATE;
    }
    const int bytlen = count * size;
    const int padlen = (bytlen + 3) & 0xfffffffc;
    // check buffer size
    if( (_buffer_current + padlen) > _buffer_end ) {
      _state = SIO_STATE_ERROR;
      return SIO_STREAM_OFFEND;
    }
    // Copy and skip over padding as necessary (xdr format)
    functions::copy( _buffer_current, from, size, count );
    _buffer_current += padlen;
    return SIO_STREAM_SUCCESS;
  }
  
  //----------------------------------------------------------------------------

  record_read_result stream::read_next_record( const record_map &records ) {
    record_read_result result;
    result._status = SIO_STREAM_SUCCESS;
    // check stream state first
    if( SIO_STATE_OPEN != _state ) {
      result._status = SIO_STREAM_NOTOPEN;
      SIO_DEBUG( "stream::read_next_record: not opened" );
      return result;
    }
    if( SIO_MODE_READ != _mode ) {
      result._status = SIO_STREAM_WRITEONLY;
      SIO_DEBUG( "stream::read_next_record: stream is read only" );
      return result;
    }
    // Loop over records until a requested one turns up.
    bool requested = false;
    while( not requested ) {
      // save record begin in file
      result._record_begin = FTELL( _handle );
      // Initialize the buffer and read the first eight bytes.  A read failure
      // at this point is treated as an end-of-file (even if there are a few
      // bytes dangling in the file).
      _buffer_current = _buffer_begin;
      result._status = FREAD( _buffer_current, SIO_LEN_SB, 8, _handle );
      if( result._status < 8 ) {
        result._status = SIO_STREAM_EOF;
        return result;
      }
      // Interpret: 1) The length of the record header.
      //            2) The record marker.
      _block_end = _buffer_begin + 8;
      unsigned int head_length(0), buftyp(0);
      SIO_STREAM_DATA( this, &head_length,  1 );
      SIO_STREAM_DATA( this, &buftyp,       1 );
      if( buftyp != SIO_mark_record ) {
        _state = SIO_STATE_ERROR;
        result._status = SIO_STREAM_NORECMARKER;
        SIO_DEBUG( "stream::read_next_record: No record marker. Read " << buftyp << ", expected " << SIO_mark_record );
        return result;
      }
      // Interpret: 3) The options word.
      //            4) The length of the record data (compressed).
      //            5) The length of the record name (uncompressed).
      //            6) The length of the record name.
      //            7) The record name.
      result._status = FREAD( _buffer_current, SIO_LEN_SB, (head_length - 8), _handle );
      if( result._status < (head_length - 8) ) {
        result._status = SIO_STREAM_EOF;
        return result;
      }
      _block_end = _buffer_begin + head_length;
      unsigned int options(0), data_length(0), ucmp_length(0), name_length(0);
      SIO_STREAM_DATA( this, &options,      1 );
      SIO_STREAM_DATA( this, &data_length,  1 );
      SIO_STREAM_DATA( this, &ucmp_length,  1 );
      SIO_STREAM_DATA( this, &name_length,  1 );
      std::string record_name(name_length, '\0');
      SIO_STREAM_DATA( this, &(record_name[0]), name_length );
      
      SIO_DEBUG( "=== Read record info ====" );
      SIO_DEBUG( "  options:     " << options );
      SIO_DEBUG( "  data_length: " << data_length );
      SIO_DEBUG( "  ucmp_length: " << ucmp_length );
      SIO_DEBUG( "  name_length: " << name_length );
      SIO_DEBUG( "  record_name: " << record_name );
      
      // find the record to read
      auto record_iter = records.find(record_name);
      record_ptr record = (records.end() != record_iter) ? record_iter->second : nullptr;
      // requested = ((records.end() != record_iter) and (record_iter->second->unpack()));
      requested = (records.end() != record_iter);
      // If the record's not interesting, move on.  Remember to skip over any
      // padding bytes inserted to make the next record header start on a
      // four byte boundary in the file.
      if( not requested ) {
        data_length += (4 - (data_length & SIO_align)) & SIO_align;
        result._status = FSEEK( _handle, data_length, 1 );
        if( result._status != 0 ) {
          _state = SIO_STATE_ERROR;
          result._status = SIO_STREAM_EOF;
          return result;
        }
        continue;
      }
      // Ensure sufficient buffering for the uncompressed record.
      unsigned int total_length = head_length + ucmp_length;
      if( total_length >= (_buffer_end - _buffer_current) ) {
        unsigned char *new_buffer = static_cast<unsigned char*>(malloc( total_length ));
        if( nullptr == new_buffer ) {
          result._status = SIO_STREAM_NOALLOC;
          SIO_DEBUG( "stream::read_next_record: malloc error" );
          return result;
        }
        memcpy( new_buffer, _buffer_begin, head_length );
        free( _buffer_begin );
        _buffer_begin = new_buffer;
        _buffer_current = new_buffer + head_length;
        _buffer_end = new_buffer + total_length;
      }
      // Deal with compression
      bool compressed_buffer = (options & SIO_OPT_COMPRESS);
      // Deal with uncompressed record
      if( not compressed_buffer ) {
        // Read the rest of the record data.  Note that uncompressed data is
        // -always- aligned to a four byte boundary in the file, so no pad
        // skipping is necessary.
        result._status = FREAD( _buffer_current, SIO_LEN_SB, data_length, _handle );
        if( result._status < data_length ) {
          _state = SIO_STATE_ERROR;
          result._status = SIO_STREAM_EOF;
          return result;
        }
      }
      else {
        // Ensure sufficient buffering for the compressed record.
        if( data_length >= (_cmp_end - _cmp_begin) ) {
          unsigned char *new_buffer = static_cast<unsigned char*>(malloc( data_length )); 
          if( nullptr == new_buffer ) {
            result._status = SIO_STREAM_NOALLOC;
            SIO_DEBUG( "stream::read_next_record: malloc error" );
            return result;
          }
          free( _cmp_begin );
          _cmp_begin = new_buffer;
          _cmp_end = new_buffer + data_length;
        }
        // Read the compressed record data.
        result._status = FREAD( _cmp_begin, SIO_LEN_SB, data_length, _handle );
        if( result._status < data_length ) {
          _state = SIO_STATE_ERROR;
          SIO_DEBUG( "stream::read_next_record: Failed to read compressed record: " << record_name );
          SIO_DEBUG( "stream::read_next_record: Expected to read " << data_length << " bytes but read " << result._status << " bytes" );
          result._status = SIO_STREAM_EOF;
          return result;
        }
        // Skip the read pointer over any padding bytes that may have been
        // inserted to make the next record header start on a four byte
        // boundary in the file.
        unsigned int pad_length = (4 - (data_length & SIO_align)) & SIO_align;
        if( pad_length > 0 ) {
          result._status = FSEEK( _handle, pad_length, 1 );
          if( result._status != 0 ) {
            _state = SIO_STATE_ERROR;
            result._status = SIO_STREAM_EOF;
            SIO_DEBUG( "stream::read_next_record: weird padding ..." );
            return result;
          }
        }
        // Set up for the decompression.
        _z_stream->next_in   = _cmp_begin;
        _z_stream->avail_in  = data_length;
        _z_stream->total_in  = 0;
        _z_stream->next_out  = _buffer_current;
        _z_stream->avail_out = _buffer_end - _buffer_current;
        _z_stream->total_out = 0;
        // Uncompress
        auto res = inflate( _z_stream, Z_FINISH );
        if( res != Z_STREAM_END ) {
          _state = SIO_STATE_ERROR;
          result._status = SIO_STREAM_BADCOMPRESS;
          SIO_DEBUG( "stream::read_next_record: bad uncompress" );
          return result;
        }
        res = inflateReset( _z_stream );
        if( res != Z_OK ) {
          _state = SIO_STATE_ERROR;
          result._status = SIO_STREAM_BADCOMPRESS;
          SIO_DEBUG( "stream::read_next_record: bad uncompress" );
          return result;
        }
      }
      _record_end = _buffer_begin + head_length + ucmp_length;
      result._record = record;
      record->set_options( options );
      result._status = do_read_record( record );
      result._record_end = FTELL( _handle );
    }
    return result;
  }
  
  //----------------------------------------------------------------------------
  
  unsigned int stream::do_read_record( record_ptr &record ) {
    // Walk along the record buffer unpacking blocks.
    unsigned int status;
    while( _buffer_current < _record_end ) {
      // Set the block maximum marker out of the way while interpreting the
      // block length and type (but even so, don't let reads escape off the
      // total length of the buffer!
      _block_end = _record_end;
      // Interpret: 1) The length of the block.
      //            2) The block marker.
      unsigned int buflen, buftyp, version, blknlen;
      SIO_DATA( this, &buflen, 1 );
      SIO_DATA( this, &buftyp, 1 );
      if( buftyp != SIO_mark_block ) {
        SIO_DEBUG( "stream::do_read_record: no block marker found" );
        return SIO_RECORD_NOBLKMARKER;
      }
      _block_end = _buffer_current + buflen - 8;
      // Read the block version.
      SIO_DATA( this, &version, 1 );
      // Read and interpret the block name.
      SIO_DATA( this, &blknlen, 1 );
      std::string block_name(blknlen, '\0');
      SIO_DATA( this, &block_name[0], blknlen );
      // Find the block in the record object
      auto block = record->get_block(block_name);
      // Try to unpack the block.
      if( nullptr != block ) {
        status = block->xfer( this, SIO_OP_READ, version );
        if( !(status & 1) ) {
          SIO_DEBUG( "stream::do_read_record: block " << block_name << " xfer READ error" );
          return status;
        }  
      }
      else {
        _buffer_current = _block_end;
      }
    }
    // Pointer relocation on read !
    pointer_relocation_read();
    return SIO_RECORD_SUCCESS;
  }
  
  //----------------------------------------------------------------------------
  
  record_write_result stream::write_record( record_ptr &record ) {
    record_write_result result;
    result._status = SIO_STREAM_SUCCESS;
    const unsigned char pad[4] = { 0, 0, 0, 0 };
    if( _state != SIO_STATE_OPEN ) {
      result._status = SIO_STREAM_NOTOPEN;
      SIO_DEBUG( "stream::write_record: not opened" );
      return result;
    }
    if( _mode == SIO_MODE_READ ) {
      result._status = SIO_STREAM_READONLY;
      SIO_DEBUG( "stream::write_record: read only, baby" );
      return result;
    }
    // Initialize the buffer.
    _buffer_current = _buffer_begin;
    // Save begin of record (current file end) 
    result._record_begin = FTELL( _handle );
    const bool compress = record->compression_option();
    unsigned int options  = record->options();
    const char *record_name = record->name().c_str();
    SIO_DEBUG( "==== Record write settings ====" );
    SIO_DEBUG( "   compress:            " << compress );
    SIO_DEBUG( "   options:             " << options );
    SIO_DEBUG( "   record_name:         " << record_name );
    SIO_DEBUG( "   _compression_level:  " << _compression_level );
    
    // Output: 1) A placeholder for the record header length.
    //         2) A 'framing' marker (to help in debugging).
    //         3) An options word.
    //         4) A placeholder for the record data length (compressed).
    //         5) A placeholder for the record data length (uncompressed).
    //         6) The length of the record name.
    //         7) The record name.
    unsigned int head_length_off = (_buffer_current - _buffer_begin);
    SIO_STREAM_DATA( this, &SIO_mark_record, 1 );
    SIO_STREAM_DATA( this, &SIO_mark_record, 1 );
    SIO_STREAM_DATA( this, &options,         1 );
    unsigned int data_length_off = _buffer_current - _buffer_begin;
    SIO_STREAM_DATA( this, &SIO_mark_record, 1 );
    unsigned int ucmp_length_off = _buffer_current - _buffer_begin;
    SIO_STREAM_DATA( this, &SIO_mark_record, 1 );
    unsigned int name_length = strlen( record_name );
    SIO_STREAM_DATA( this, &name_length,     1 );
    SIO_STREAM_DATA( this,  const_cast<char *>(record_name), name_length );
    // Back fill the length of the record header.
    unsigned int head_length = _buffer_current - _buffer_begin;
    functions::copy( UCHR_CAST(&head_length), (_buffer_begin + head_length_off), SIO_LEN_QB, 1 );
    // Ask the record object to fill the buffer with blocks. If an error is
    // reported, just print a complaint and skip writing this buffer.
    result._status = do_write_record( record );
    if( not (result._status & 1) ) {
      SIO_DEBUG( "stream::write_record: error on do_write_record" );
      return result;
    }
    // Fill in the uncompressed record length.
    unsigned int ucmp_length = (_buffer_current - _buffer_begin) - head_length;
    functions::copy( UCHR_CAST(&ucmp_length), (_buffer_begin + ucmp_length_off), SIO_LEN_QB, 1);
    // Write out the complete record.
    if( not compress ) {
      // Fill in the written length of the record data (identical with the
      // uncompressed length when no compression is being performed).
      unsigned int data_length = ucmp_length;
      functions::copy( UCHR_CAST(&data_length), (_buffer_begin + data_length_off), SIO_LEN_QB, 1 );
      // When not compressing, both the record header and the record data
      // can be written in one swell foop.  Note that uncompressed data always
      // satisfies the requirement that it ends on a four byte boundary, so
      // no padding is required.
      data_length += head_length;
      auto bufout = FWRITE( _buffer_begin, sizeof(char), data_length, _handle );
      // fg 20030514 - flush the record 
      if( bufout != data_length && ! FFLUSH( _handle ) ) {
        _state = SIO_STATE_ERROR;
        result._status = SIO_STREAM_BADWRITE;
        SIO_DEBUG( "stream::write_record: error while flushing to file" );
        return result;
      }
    }
    else {
      SIO_DEBUG( "stream::write_record: Writing buffer with compression ON, record: " << record_name );
      // Set up for the compression.
      _z_stream->next_in   = _buffer_begin + head_length;
      _z_stream->avail_in  = ucmp_length;
      _z_stream->total_in  = 0;  
      _z_stream->next_out  = _cmp_begin;
      _z_stream->avail_out = _cmp_end - _cmp_begin;
      _z_stream->total_out = 0;
      SIO_DEBUG( "=== Writing buffer with compression setup: ===" );
      SIO_DEBUG( "   _z_stream->next_in:     " << _z_stream->next_in );
      SIO_DEBUG( "   _z_stream->avail_in:    " << _z_stream->avail_in );
      SIO_DEBUG( "   _z_stream->total_in:    " << _z_stream->total_in );
      SIO_DEBUG( "   _z_stream->next_out:    " << _z_stream->next_out );
      SIO_DEBUG( "   _z_stream->avail_out:   " << _z_stream->avail_out );
      SIO_DEBUG( "   _z_stream->total_out:   " << _z_stream->total_out );
      SIO_DEBUG( "   _buffer_begin:          " << (void*)_buffer_begin );
      SIO_DEBUG( "   head_length:            " << head_length );
      SIO_DEBUG( "   ucmp_length:            " << ucmp_length );
      SIO_DEBUG( "   _cmp_begin:             " << (void*)_cmp_begin );
      SIO_DEBUG( "   _cmp_end:               " << (void*)_cmp_end );

      for(;;) {
        deflate( _z_stream, Z_FINISH );
        if( _z_stream->avail_out > 0 ) {
          SIO_DEBUG( "stream::write_record: while compressing available came out with: " << _z_stream->avail_out );
          break;
        }
        unsigned int newlen = (_cmp_end - _cmp_begin) << 1;
        SIO_DEBUG( "stream::write_record: Writing buffer with compression ON, size: " << newlen );
        unsigned char *newbuf = static_cast<unsigned char*>(malloc( newlen ));
        if( nullptr == newbuf ) {
          _state = SIO_STATE_ERROR;
          result._status = SIO_STREAM_NOALLOC;
          SIO_DEBUG( "stream::write_record: malloc failed while compressing" );
          return result;
        }
        unsigned int oldlen = (_z_stream->next_out - _cmp_begin);
        memcpy( newbuf, _cmp_begin, oldlen );
        free( _cmp_begin );
        _cmp_begin = newbuf;
        _cmp_end = newbuf + newlen;
        _z_stream->next_out  = _cmp_begin + oldlen;
        _z_stream->avail_out = _cmp_end - _z_stream->next_out;
      }
      auto z_stat = deflateReset( _z_stream );
      if( z_stat != Z_OK ) {
        _state = SIO_STATE_ERROR;
        result._status = SIO_STREAM_BADCOMPRESS;
        SIO_DEBUG( "stream::write_record: deflate reset failed" );
        return result;
      }
      // Fill in the length of the compressed buffer.
      unsigned int data_length = (_z_stream->next_out - _cmp_begin);
      SIO_DEBUG( "stream::write_record: Compressed buffer size: " << data_length );
      functions::copy( UCHR_CAST(&data_length), (_buffer_begin + data_length_off), SIO_LEN_QB, 1 );
      // Write the record header.
      auto bufout = FWRITE( _buffer_begin, sizeof(char), head_length, _handle );
      if( bufout != head_length ) {
        _state = SIO_STATE_ERROR;
        result._status = SIO_STREAM_BADWRITE;
        SIO_DEBUG( "stream::write_record: record write record to file" );
        return result;
      }
      // Write the compressed record data.
      bufout = FWRITE( _cmp_begin, sizeof(char), data_length, _handle );
      // fg 20030514 - flush the record
      if( bufout != data_length && ! FFLUSH(_handle) ) {
        _state = SIO_STATE_ERROR;
        result._status = SIO_STREAM_BADWRITE;
        SIO_DEBUG( "stream::write_record: compressed record write record to file" );
        return result;
      }
      // Insert any necessary padding to make the next record header start
      // on a four byte boundary in the file (to make it directly accessible
      // for xdr read).
      unsigned int newlen = (4 - (data_length & SIO_align)) & SIO_align;
      if( newlen > 0 ) {
        bufout = FWRITE( pad, sizeof(char), newlen, _handle );
        // fg 20030514 - flush the record
        if( bufout != newlen && ! FFLUSH(_handle)) {
          _state = SIO_STATE_ERROR;
          result._status = SIO_STREAM_BADWRITE;
          SIO_DEBUG( "stream::write_record: padding write error" );
          return result;
        }
      }
    }
    result._record_end = FTELL(_handle);
    return result;
  }
  
  //----------------------------------------------------------------------------
  
  unsigned int stream::do_write_record( record_ptr &record ) {
    unsigned int status;
    // Loop over blocks, getting their input.
    for(auto iter = record->begin(), end_iter = record->end() ; iter != end_iter ; ++iter) {
      auto block = iter->second;
      // Save the beginning of block pointer.  Reuse the stream->blkmax variable
      // (which has no use during buffer writing) to save it so that if the copy
      // routine in functions is forced to reallocate the output buffer,
      // the pointer to the beginning of block can be updated as well.
      _block_end = _buffer_current;
      // Output: 1) A placeholder where the block length will go.
      //         2) A 'framing' marker (to help in debugging).
      //         3) The version of the block.
      //         4) The length of the block name.
      //         5) The block name.
      unsigned int blkver = block->version();
      unsigned int namlen = block->name().size();
      const char *nampnt = block->name().c_str();
      SIO_DATA( this, &SIO_mark_block,             1      );
      SIO_DATA( this, &SIO_mark_block,             1      );
      SIO_DATA( this, &blkver,                     1      );
      SIO_DATA( this, &namlen,                     1      );
      SIO_DATA( this, const_cast<char *>(nampnt), namlen );
      
      // Write the block content.
      status = block->xfer( this, SIO_OP_WRITE, blkver );
      if( status != SIO_BLOCK_SUCCESS ) {
        SIO_DEBUG( "stream::do_write_record: block write failed " << block->name() );
        return status;
      }
      // Back fill the length of the block.
      unsigned int buflen = _buffer_current - _block_end;
      functions::copy( UCHR_CAST(&buflen), _block_end, SIO_LEN_QB, 1 );
    }
    pointer_relocation_write();
    return SIO_RECORD_SUCCESS;
  }

}



