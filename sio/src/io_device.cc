// -- sio headers
#include <sio/io_device.h>
#include <sio/api.h>

namespace sio {

  read_device::read_device( buffer_span buf ) :
    _buffer(std::move(buf)) {
    /* nop */
  }

  //--------------------------------------------------------------------------

  void read_device::set_buffer( const buffer_span &buf ) {
    _buffer = buf ;
  }

  //--------------------------------------------------------------------------

  void read_device::set_buffer( buffer_span &&buf ) {
    _buffer = buf ;
  }

  //--------------------------------------------------------------------------

  read_device::cursor_type read_device::position() const {
    return _cursor ;
  }

  //--------------------------------------------------------------------------

  void read_device::seek( cursor_type pos ) {
    if( pos > _buffer.size() ) {
      SIO_THROW( sio::error_code::out_of_range, "Can't seek device cursor: out of range!" ) ;
    }
    _cursor = pos ;
  }

  //--------------------------------------------------------------------------

  void read_device::pointer_to( ptr_type *ptr ) {
    // Read.  Keep a record of the "match" quantity read from the buffer and
    // the location in memory which will need relocating.
    // Placeholder value for 'pointer to'
    unsigned int match = 0 ;
    data( match ) ;
    // Ignore match = 0x00000000.  This is basically a null pointer which can
    // never be relocated, so don't fill the multimap with a lot of useless
    // information.
    if( match != 0x00000000 ) {
      sio::pointer_to_map::value_type entry = { reinterpret_cast<void *>(match), ptr } ;
      _pointer_to.insert( entry ) ;
    }
    *ptr = static_cast<sio::ptr_type>( match ) ;
  }

  //--------------------------------------------------------------------------

  void read_device::pointed_at( ptr_type *ptr ) {
    // Read.  Keep a record of the "match" quantity read from the buffer and
    // the location in memory which will need relocating.
    unsigned int match = 0 ;
    data( match ) ;
    // Ignore match = SIO_ptag. This is basically a pointer target which was
    // never relocated when the record was written. i.e. nothing points to it!
    // Don't clutter the maps with information that can never be used.
    if( match != 0xffffffff ) {
      pointed_at_map::value_type entry = { reinterpret_cast<void *>( match ), ptr } ;
      _pointed_at.insert( entry ) ;
    }
  }

  //--------------------------------------------------------------------------

  void read_device::pointer_relocation() {
    sio::api::read_relocation( _pointed_at, _pointer_to ) ;
    _pointer_to.clear() ;
    _pointed_at.clear() ;
  }

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  write_device::write_device( buffer&& buf ) :
    _buffer( std::move( buf ) ) {
    /* nop */
  }

  //--------------------------------------------------------------------------

  void write_device::set_buffer( buffer&& buf ) {
    _buffer = std::move( buf ) ;
  }

  //--------------------------------------------------------------------------

  buffer write_device::take_buffer() {
    return std::move( _buffer ) ;
  }

  //--------------------------------------------------------------------------

  write_device::cursor_type write_device::position() const {
    return _cursor ;
  }

  //--------------------------------------------------------------------------

  void write_device::seek( cursor_type pos ) {
    if( pos > _buffer.size() ) {
      SIO_THROW( sio::error_code::out_of_range, "Can't seek device cursor: out of range!" ) ;
    }
    _cursor = pos ;
  }

  //--------------------------------------------------------------------------

  void write_device::pointer_to( ptr_type *ptr ) {
    // Write.  Keep a record of the "match" quantity (i.e. the value of the
    // pointer (which may be different lengths on different machines!)) and
    // the current offset in the output buffer.  Put a placeholder in the
    // output buffer (it will be overwritten at the "output relocation" stage).
    //
    // Placeholder value for 'pointer to'
    unsigned int SIO_pntr = 0x00000000 ;
    // ptr is really a pointer-to-a-pointer.  This routine is most interested
    // in the value of *xfer when treated as a pointer.  C++ tends to object
    // to this as being 'not type safe'.  To keep the compiler happy (and purists
    // miserable), do one 'reinterpret_cast' immediately to make later code
    // easier to read.
    // Indirect ptr (actually **ptr)
    void *ifer = reinterpret_cast<void *>(*ptr) ;
    // Ignore NULL pointers.  These are always recorded in the buffer with a
    // zero match word (and are treated specially when read back).  There's no
    // point in putting useless information in the maps.
    if( nullptr != ifer ) {
      sio::pointer_to_map::value_type entry = { ifer, reinterpret_cast<void *>( _buffer.ptr(_cursor) - _buffer.data() ) } ;
      _pointer_to.insert( entry ) ;
    }
    data( SIO_pntr ) ;
  }

  //--------------------------------------------------------------------------

  void write_device::pointed_at( ptr_type *ptr ) {
    // Write.  Save the memory location of this object along with the offset
    // in the output buffer where the generated match quantity must go.  Put
    // a placeholder in the output buffer (it will be overwritten at the "output
    // relocation" stage).
    unsigned int SIO_ptag = 0xffffffff ;
    sio::pointed_at_map::value_type entry = { ptr, reinterpret_cast<void *>( _buffer.ptr(_cursor) - _buffer.data() ) } ;
    _pointed_at.insert( entry ) ;
    data( SIO_ptag ) ;
  }

  //--------------------------------------------------------------------------

  void write_device::pointer_relocation() {
    sio::api::write_relocation( _buffer.data(), _pointed_at, _pointer_to ) ;
    _pointer_to.clear() ;
    _pointed_at.clear() ;
  }

}
