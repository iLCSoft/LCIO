#pragma once

// -- sio headers
#include <sio/definitions.h>

// -- std headers
#include <limits>

namespace sio {

  /**
   *  @brief  buffer_span class.
   *
   *  Implements a view on a byte array without owning it.
   *  A buffer_span can be obtained directly from a byte_array
   *  or from a buffer using the buffer::span() methods.
   *  The buffer_span class provides only const methods to
   *  work with the underlying byte array, except for the
   *  assignement operator which allow to change the underlying
   *  byte array span. Note that the implementation stores a
   *  pair of const_iterator on a byte_array. Thus the validity
   *  of the buffer_span object relies on the validity of these
   *  iterators.
   */
  class buffer_span {
  public:
    // traits
    using container = sio::byte_array ;
    using element_type = container::value_type ;
    using const_iterator = container::const_iterator ;
    using index_type = std::size_t ;
    using size_type = std::size_t ;
    using reference = container::reference ;
    using const_reference = container::const_reference ;
    using pointer = container::pointer ;
    using const_pointer = container::const_pointer ;

  public:
    /// Default copy constructor
    buffer_span( const buffer_span& ) = default ;
    /// Default move constructor
    buffer_span( buffer_span&& ) = default ;
    /// Default destructor
    ~buffer_span() = default ;
    /// Default assignement operator
    buffer_span& operator=( const buffer_span& ) = default ;
    /// Default move assignment operator
    buffer_span& operator=( buffer_span&& ) = default ;

    /**
     *  @brief  Default constructor
     */
    buffer_span() ;

    /**
     *  @brief  Constructor from a byte_array
     *
     *  @param  bytes the byte_array on which to construct the span
     */
    buffer_span( const container &bytes ) ;

    /**
     *  @brief  Constructor with two iterators
     *
     *  @param  first the start of the span
     *  @param  last the end of the span (not included)
     */
    buffer_span( const_iterator first, const_iterator last ) ;

    /**
     *  @brief  Constructor with iterator and bytes count
     *
     *  @param  first the start of the span
     *  @param  count the number of bytes to the end of the span
     */
    buffer_span( const_iterator first, size_type count ) ;

    /**
     *  @name Iterators
     */
    ///{@
    /**
     *  @brief  Get the iterator to the start of the span
     */
    const_iterator begin() const ;

    /**
     *  @brief  Get the iterator to the end of the span
     */
    const_iterator end() const ;
    ///@}

    /**
     *  @name Element access
     */
    ///{@
    /**
     *  @brief  Get the data buffer
     */
    const element_type *data() const ;

    /**
     *  @brief  Get the front element (if valid)
     */
    const_reference front() const ;

    /**
     *  @brief  Get the back element (if valid)
     */
    const_reference back() const ;

    /**
     *  @brief  Data access operator (no range check)
     *
     *  @param  index the index of a byte access
     */
    const_reference operator[]( index_type index ) const ;

    /**
     *  @brief  Data access operator (range check!)
     *
     *  @param  index the index of a byte access
     */
    const_reference at( index_type index ) const ;

    /**
     *  @brief  Get a pointer on the raw bytes at the given index
     *
     *  @param  index the index access
     */
    const_pointer ptr( index_type index ) const ;
    ///@}

    /**
     *  @name Capacity
     */
    ///{@
    /**
     *  @brief  Get the size of the span
     */
    size_type size() const ;

    /**
     *  @brief  Whether the span is empty
     */
    bool empty() const ;

    /**
     *  @brief  Whether the span is valid, meaning not default constructed
     */
    bool valid() const ;

    /**
     *  @brief  boolean operator. Returns true if the span is valid
     */
    operator bool() const noexcept ;
    ///@}

    /**
     *  @name  Operations
     */
    ///{@
    /**
     *  @brief  Get a sub span from a new start, untill the end
     *
     *  @param  start the subspan new start
     */
    buffer_span subspan( index_type start ) const ;

    /**
     *  @brief  Get a sub span from a new start and new end
     *
     *
     *  @param  start the subspan new start
     *  @param  count the size of the subspan
     */
    buffer_span subspan( index_type start, std::size_t count ) const ;
    
    /**
     *  @brief  Dump the buffer into standard output
     *  
     *  @param  base the numeric base of the output data (default octal)
     *  @param  line_split the number of bytes to print before a line break
     *  @param  max_bytes the maximum number of bytes to printout
     */
    void dump( int base = 8, unsigned int line_split = 20, size_type max_bytes = std::numeric_limits<size_type>::max() ) ;
    ///@}

  private:
    ///< An iterator to the begin of a byte_array
    const_iterator    _first{} ;
    ///< An iterator to the end of a byte_array
    const_iterator    _last{} ;
    ///< Whether the span is null (invalid)
    bool              _isnull {false} ;
  };

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  /**
   *  @brief  buffer class
   */
  class buffer {
  public:
    using container = sio::byte_array ;
    using element_type = container::value_type ;
    using iterator = container::iterator ;
    using const_iterator = container::const_iterator ;
    using reverse_iterator = container::reverse_iterator ;
    using const_reverse_iterator = container::const_reverse_iterator ;
    using index_type = std::size_t ;
    using size_type = std::size_t ;
    using reference = container::reference ;
    using const_reference = container::const_reference ;
    using pointer = container::pointer ;
    using const_pointer = container::const_pointer ;

  public:
    /// No default constructor
    buffer() = delete ;
    /// No copy constructor
    buffer( const buffer& ) = delete ;
    /// No assignment by copy
    buffer& operator=( const buffer& ) = delete ;

    /**
     *  @brief  Constructor with buffer size
     *
     *  @param  len the buffer size to allocate
     */
    buffer( size_type len ) ;

    /**
     *  @brief  Constructor with byte array (move!)
     *
     *  @param  bytes the byte array
     */
    buffer( container &&bytes ) ;

    /**
     *  @brief  Move constructor
     *
     *  @param  rhs the other buffer object to move
     */
    buffer( buffer&& rhs ) ;

    /**
     *  @brief  Move assignment operator
     *
     *  @param  rhs the other buffer object to move
     */
    buffer& operator=( buffer&& rhs ) ;

    /**
     *  @name Iterators
     */
    ///@{
    /**
     *  @brief  Get an iterator to the beginning of the byte array
     */
    const_iterator begin() const ;

    /**
     *  @brief  Get an iterator to the end of the byte array
     */
    const_iterator end() const ;

    /**
     *  @brief  Get an iterator to the beginning of the byte array
     */
    iterator begin() ;

    /**
     *  @brief  Get an iterator to the end of the byte array
     */
    iterator end() ;

    /**
     *  @brief  Get an iterator to the reverse beginning (end) of the byte array
     */
    const_reverse_iterator rbegin() const ;

    /**
     *  @brief  Get an iterator to the reverse end (begin) of the byte array
     */
    const_reverse_iterator rend() const ;

    /**
     *  @brief  Get an iterator to the reverse beginning (end) of the byte array
     */
    reverse_iterator rbegin() ;

    /**
     *  @brief  Get an iterator to the reverse end (begin) of the byte array
     */
    reverse_iterator rend() ;
    ///@}

    /**
     *  @name Element access
     */
    ///@{
    /**
     *  @brief  Get a byte at the specified position
     *
     *  @param  index the index access
     */
    const_reference operator[]( index_type index ) const ;

    /**
     *  @brief  Get a byte at the specified position
     *
     *  @param  index the index access
     */
    reference operator[]( index_type index ) ;

    /**
     *  @brief  Get a byte at the specified position
     *
     *  @param  index the index access
     */
    const_reference at( index_type index ) const ;

    /**
     *  @brief  Get a byte at the specified position
     *
     *  @param  index the index access
     */
    reference at( index_type index ) ;

    /**
     *  @brief  Get a byte at the front position
     */
    const_reference front() const ;

    /**
     *  @brief  Get a byte at the front position
     */
    reference front() ;

    /**
     *  @brief  Get a byte at the back position
     */
    const_reference back() const ;

    /**
     *  @brief  Get a byte at the back position
     */
    reference back() ;

    /**
     *  @brief  Get the raw buffer bytes as an array
     */
    const_pointer data() const ;

    /**
     *  @brief  Get the raw buffer bytes as an array
     */
    pointer data() ;

    /**
     *  @brief  Get a pointer on the raw bytes at the given index
     *
     *  @param  index the index access
     */
    const_pointer ptr( index_type index ) const ;

    /**
     *  @brief  Get a pointer on the raw bytes at the given index
     *
     *  @param  index the index access
     */
    pointer ptr( index_type index ) ;
    ///@}

    /**
     *  @name Capacity
     */
    ///@{
    /**
     *  @brief  Get the buffer size
     */
    size_type size() const ;

    /**
     *  @brief  Whether the buffer is empty
     */
    bool empty() const ;

    /**
     *  @brief  Get the underlying current buffer capacity 
     */
    size_type capacity() const ;
    ///@}

    /**
     *  @name Modifiers
     */
    ///@{
    /**
     *  @brief  Resize the buffer to the specified size
     *
     *  @param  newsize the new buffer size
     */
    void resize( size_type newsize ) ;

    /**
     *  @brief  Expand the buffer by adding new bytes
     *
     *  @param  nbytes the number of bytes to add
     *  @return the new buffer size
     */
    size_type expand( size_type nbytes = 32*sio::kbyte*sio::kbyte ) ;
    
    /**
     *  @brief  Shrink the internal buffer to its size.
     *          This doesn't modify the buffer size.
     */
    void shrink() ;

    /**
     *  @brief  Clear the buffer.
     *          Note that the memory is not released until the buffer
     *          gets deleted. To release the buffer memory, use shrink = true
     *
     * @param  shrink whether to shrink the buffer after the clear operation
     */
    void clear( bool shrink = false ) ;
    ///@}

    /**
     *  @name Operations
     */
    ///@{
    /**
     *  @brief  Re-use the buffer bytes. Creates a new buffer object
     *          and move the byte container in the new buffer object.
     *          The current buffer is invalidated
     */
    buffer reuse() ;

    /**
     *  @brief  Whether the buffer is valid
     *
     *  The buffer may be invalidated after a move operation
     *  occuring when:
     *  - std::move is called
     *  - buffer::reuse() is called
     */
    bool valid() const ;

    /**
     *  @brief  Get a the entire buffer as buffer_span
     */
    buffer_span span() const ;

    /**
     *  @brief  Get a sub span of the buffer
     *
     *  @param  start where the span starts
     */
    buffer_span span( index_type start ) const ;

    /**
     *  @brief  Get a sub span of the buffer
     *
     *  @param  start where the sub span starts
     *  @param  count the length of the sub span
     */
    buffer_span span( index_type start, size_type count ) const ;
    ///@}

  private:
    ///< The byte array container
    container           _bytes {} ;
    ///< Whether the buffer is in a valid state
    bool                _valid {true} ;
  };

}
