// -- sio headers
#include <sio/buffer.h>
#include <sio/exception.h>

// -- std headers
#include <sstream>
#include <iomanip>

namespace sio {

  buffer_span::buffer_span() :
    _isnull(true) {
    /* nop */
  }

  //--------------------------------------------------------------------------

  buffer_span::buffer_span( const container &bytes ) :
    _first( bytes.begin() ),
    _last( bytes.end() ) {
    /* nop */
  }

  //--------------------------------------------------------------------------

  buffer_span::buffer_span( const_iterator first, const_iterator last ) :
    _first(first),
    _last(last) {
    /* nop */
  }

  //--------------------------------------------------------------------------

  buffer_span::buffer_span( const_iterator first, std::size_t count ) :
    _first(first),
    _last(std::next(first, count)) {
    /* nop */
  }

  //--------------------------------------------------------------------------

  buffer_span::const_iterator buffer_span::begin() const {
    return _first ;
  }

  //--------------------------------------------------------------------------

  buffer_span::const_iterator buffer_span::end() const {
    return _last ;
  }

  //--------------------------------------------------------------------------

  const buffer_span::element_type *buffer_span::data() const {
    return _isnull ? nullptr : &(*_first) ;
  }

  //--------------------------------------------------------------------------

  buffer_span::const_reference buffer_span::front() const {
    return (*_first) ;
  }

  //--------------------------------------------------------------------------

  buffer_span::const_reference buffer_span::back() const {
    return *(_last-1) ;
  }

  //--------------------------------------------------------------------------

  buffer_span::const_reference buffer_span::operator[]( index_type index ) const {
    return data() [ index ] ;
  }

  //--------------------------------------------------------------------------

  buffer_span::const_reference buffer_span::at( index_type index ) const {
    if( index >= size() ) {
      std::stringstream ss ;
      ss << "index: " << index << ", size: " << size() ;
      SIO_THROW( error_code::out_of_range, ss.str() ) ;
    }
    return data() [ index ] ;
  }

  //--------------------------------------------------------------------------

  buffer_span::const_pointer buffer_span::ptr( index_type index ) const {
    return data() + index ;
  }

  //--------------------------------------------------------------------------

  std::size_t buffer_span::size() const {
    return _isnull ? 0 : std::distance( _first, _last ) ;
  }

  //--------------------------------------------------------------------------

  bool buffer_span::empty() const {
    return (size() == 0) ;
  }

  //--------------------------------------------------------------------------

  bool buffer_span::valid() const {
    return (not _isnull) ;
  }

  //--------------------------------------------------------------------------

  buffer_span::operator bool() const noexcept {
    return valid() ;
  }

  //--------------------------------------------------------------------------

  buffer_span buffer_span::subspan( index_type start ) const {
    if( start > size() ) {
      std::stringstream ss ;
      ss << "start: " << start << ", size: " << size() ;
      SIO_THROW( error_code::out_of_range, ss.str() ) ;
    }
    return buffer_span( std::next(_first, start) , _last ) ;
  }

  //--------------------------------------------------------------------------

  buffer_span buffer_span::subspan( index_type start, std::size_t count ) const {
    if( start+count > size() ) {
      std::stringstream ss ;
      ss << "start: " << start << ", count: " << count << ", size: " << size() ;
      SIO_THROW( error_code::out_of_range, ss.str() ) ;
    }
    return buffer_span( std::next(_first, start) , std::next(_first, start+count) ) ;
  }
  
  //--------------------------------------------------------------------------
  
  void buffer_span::dump( int base, unsigned int line_split, size_type max_bytes ) {
    std::ios_base::fmtflags f( std::cout.flags() );
    for( size_type i=0 ; i<size() ; i++ ) {
      std::cout << std::setbase( base ) << (int)at(i) << " " ;
      if( (i != 0) and (i%line_split == 0) ) {
        std::cout << std::endl ;
      }
      if( i > max_bytes ) {
        break ;
      }
    }
    std::cout << std::endl ;
    std::cout.flags( f );
  }

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  buffer::buffer( size_type len ) :
    _bytes( len, sio::null_byte ) {
    if( 0 == len ) {
      SIO_THROW( sio::error_code::invalid_argument, "Can't construct a buffer with length of 0!" ) ;
    }
  }

  //--------------------------------------------------------------------------

  buffer::buffer( container &&bytes ) :
    _bytes( std::move( bytes ) ) {
    /* nop */
  }

  //--------------------------------------------------------------------------

  buffer::buffer( buffer&& rhs ) {
    _bytes = std::move( rhs._bytes ) ;
    _valid = rhs._valid ;
    rhs._valid = false ;
  }

  //--------------------------------------------------------------------------

  buffer& buffer::operator=( buffer&& rhs ) {
    _bytes = std::move( rhs._bytes ) ;
    _valid = rhs._valid ;
    rhs._valid = false ;
    return *this ;
  }

  //--------------------------------------------------------------------------

  buffer::const_iterator buffer::begin() const {
    return _bytes.begin() ;
  }

  //--------------------------------------------------------------------------

  buffer::const_iterator buffer::end() const {
    return _bytes.end() ;
  }

  //--------------------------------------------------------------------------

  buffer::iterator buffer::begin() {
    return _bytes.begin() ;
  }

  //--------------------------------------------------------------------------

  buffer::iterator buffer::end() {
    return _bytes.end() ;
  }

  //--------------------------------------------------------------------------

  buffer::const_reverse_iterator buffer::rbegin() const {
    return _bytes.rbegin() ;
  }

  //--------------------------------------------------------------------------

  buffer::const_reverse_iterator buffer::rend() const {
    return _bytes.rend() ;
  }

  //--------------------------------------------------------------------------

  buffer::reverse_iterator buffer::rbegin() {
    return _bytes.rbegin() ;
  }

  //--------------------------------------------------------------------------

  buffer::reverse_iterator buffer::rend() {
    return _bytes.rend() ;
  }

  //--------------------------------------------------------------------------

  buffer::const_reference buffer::operator[]( index_type index ) const {
    return _bytes [ index ] ;
  }

  //--------------------------------------------------------------------------

  buffer::reference buffer::operator[]( index_type index ) {
    return _bytes [ index ] ;
  }

  //--------------------------------------------------------------------------

  buffer::const_reference buffer::at( index_type index ) const {
    return _bytes.at( index ) ;
  }

  //--------------------------------------------------------------------------

  buffer::reference buffer::at( index_type index ) {
    return _bytes.at( index ) ;
  }

  //--------------------------------------------------------------------------

  buffer::const_reference buffer::front() const {
    return _bytes.front() ;
  }

  //--------------------------------------------------------------------------

  buffer::reference buffer::front() {
    return _bytes.front() ;
  }

  //--------------------------------------------------------------------------

  buffer::const_reference buffer::back() const {
    return _bytes.back() ;
  }

  //--------------------------------------------------------------------------

  buffer::reference buffer::back() {
    return _bytes.back() ;
  }

  //--------------------------------------------------------------------------

  buffer::const_pointer buffer::data() const {
    return _bytes.data() ;
  }

  //--------------------------------------------------------------------------

  buffer::pointer buffer::data() {
    return _bytes.data() ;
  }

  //--------------------------------------------------------------------------

  buffer::const_pointer buffer::ptr( index_type index ) const {
    return &(_bytes[ index ]) ;
  }

  //--------------------------------------------------------------------------

  buffer::pointer buffer::ptr( index_type index ) {
    return &(_bytes[ index ]) ;
  }

  //--------------------------------------------------------------------------

  buffer::size_type buffer::size() const {
    return _bytes.size() ;
  }

  //--------------------------------------------------------------------------

  bool buffer::empty() const {
    return _bytes.empty() ;
  }

  //--------------------------------------------------------------------------

  buffer::size_type buffer::capacity() const {
    return _bytes.capacity() ;
  }

  //--------------------------------------------------------------------------

  void buffer::resize( size_type newsize ) {
    _bytes.resize( newsize ) ;
  }

  //--------------------------------------------------------------------------

  buffer::size_type buffer::expand( std::size_t nbytes ) {
    auto len = size() ;
    resize( len + nbytes ) ;
    return len ;
  }
  
  //--------------------------------------------------------------------------
  
  void buffer::shrink() {
    _bytes.shrink_to_fit() ;
  }

  //--------------------------------------------------------------------------

  void buffer::clear( bool shrink ) {
    _bytes.clear() ;
    if( shrink ) {
      _bytes.shrink_to_fit() ;
    }
  }

  //--------------------------------------------------------------------------

  buffer buffer::reuse() {
    buffer new_buffer( std::move( _bytes ) ) ;
    _valid = false ;
    return new_buffer ;
  }

  //--------------------------------------------------------------------------

  bool buffer::valid() const {
    return _valid ;
  }

  //--------------------------------------------------------------------------

  buffer_span buffer::span() const {
    return buffer_span( begin() , end() ) ;
  }

  //--------------------------------------------------------------------------

  buffer_span buffer::span( index_type start ) const {
    if( start > size() ) {
      std::stringstream ss ;
      ss << "start: " << start << ", size: " << size() ;
      SIO_THROW( error_code::out_of_range, ss.str() ) ;
    }
    return buffer_span( std::next(begin(), start) , end() ) ;
  }

  //--------------------------------------------------------------------------

  buffer_span buffer::span( index_type start, size_type count ) const {
    if( start+count > size() ) {
      std::stringstream ss ;
      ss << "start: " << start << ", count: " << count << ", size: " << size() ;
      SIO_THROW( error_code::out_of_range, ss.str() ) ;
    }
    return buffer_span( std::next(begin(), start) , std::next(begin(), start+count) ) ;
  }

}
