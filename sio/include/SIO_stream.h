// ----------------------------------------------------------------------------
// CVS $Id: SIO_stream.h,v 1.9 2010-06-22 13:49:54 gaede Exp $
// ----------------------------------------------------------------------------
// => Controller for a single SIO stream.                          
// ----------------------------------------------------------------------------
//
// General Description:
//
// SIO_stream manages operations of a single SIO stream.
//
// ----------------------------------------------------------------------------

#ifndef SIO_STREAM_H
#define SIO_STREAM_H 1

// -- std headers
#include <map>
#include <string>
#include <stdio.h>

// -- sio headers
#include "SIO_definitions.h"
#include "SIO_record.h"

//
// Take the drudgery out of error handling.
//
#define SIO_STREAM_DATA( rec, pnt, cnt ) { \
  result._status = (rec->mode() != SIO_MODE_READ) ? \
    rec->write_data(pnt, cnt) : \
    rec->read_data(pnt, cnt); \
  if( !(result._status & 1) ) \
    return result; \
}
#define SIO_DATA( rec, pnt, cnt ) { \
  status = (rec->mode() != SIO_MODE_READ) ? \
    rec->write_data(pnt, cnt) : \
    rec->read_data(pnt, cnt); \
  if( !(status & 1) ) \
    return status; \
}
#define SIO_PNTR( rec, pnt ) { \
  status = (rec->mode() != SIO_MODE_READ) ? \
    rec->write_pointer_to( (SIO_POINTER_DECL *)(pnt) ) : \
    rec->read_pointer_to( (SIO_POINTER_DECL *)(pnt) ); \
  if( !(status & 1) ) \
    return status; \
}
#define SIO_PTAG( rec, pnt ) { \
  status = (rec->mode() != SIO_MODE_READ) ? \
    rec->write_pointed_at( (SIO_POINTER_DECL *)(pnt) ) : \
    rec->read_pointed_at( (SIO_POINTER_DECL *)(pnt) ); \
  if( !(status & 1) ) \
    return status; \
}

struct z_stream_s;

namespace sio {
  
  /**
   *  @brief  stream class.
   *  
   *  File and buffer handler for reading / writing files with SIO
   */
  class stream {
  public:
    /**
     *  @brief  Open a new file
     *  
     *  @param  fname the file name
     *  @param  m the file opening mode
     */
    unsigned int open(const std::string &fname, SIO_stream_mode m);
    
    /**
     *  @brief  Close the current file
     */
    unsigned int close();
    
    /**
     *  @brief  Flush the current opened stream
     */
    unsigned int flush();
    
    /**
     *  @brief  Get current file pointer position
     */
    SIO_64BITINT file_tell();
    
    /**
     *  @brief  The name of currently opened file
     */
    const std::string &file_name() const;
    
    /**
     *  @brief  The current open mode
     */
    SIO_stream_mode mode() const;
    
    /**
     *  @brief  The stream state
     */
    SIO_stream_state state() const;
    
    /**
     *  @brief  Seek pointer in file
     *  
     *  @param  pos position in the file
     *  @param  whence from where
     */
    unsigned int seek(SIO_64BITINT pos, int whence = SEEK_SET);

    /**
     *  @brief  Reset the stream after a non-sucessful attempt to read a record (e.g. from an older file).
     *          Set the file position to pos
     *  
     *  @param  pos the position to set after reset 
     */
    unsigned int reset( SIO_64BITINT pos = 0 );

    /**
     *  @brief  Set the compression level:
     *          - -1  default 
     *          - 0   no compression 
     *          - 1-9 compression
     *  Note if level==0, user should set compression off for all records !
     * 
     *  @param  level the compression level
     */
    void set_compression_level(int level);
    
    /**
     *  @brief  Set the size to reserve for the raw buffer while reading/writing 
     * 
     *  @param  reserve the size to reserve
     */
    void set_reserve(unsigned int reserve);
    
    /**
     *  @brief  Write data to the raw buffer
     *  
     *  @param  data the data to write
     *  @param  count the number of variable to write
     */
    template <typename T>
    unsigned int write_data(T *data, const int count);
    
    /**
     *  @brief  Read data from the raw buffer
     *  
     *  @param  data the data to read
     *  @param  count the number of variable to read
     */
    template <typename T>
    unsigned int read_data(T *data, const int count);
    
    /**
     *  @brief  Read a 'pointed at' from ptr mapping
     * 
     *  @param  ptr a raw pointer
     */
    unsigned int read_pointed_at(SIO_POINTER_DECL *ptr);
    
    /**
     *  @brief  Write a 'pointed at' to ptr mapping
     * 
     *  @param  ptr a raw pointer
     */
    unsigned int write_pointed_at(SIO_POINTER_DECL *ptr);
    
    /**
     *  @brief  Read a 'pointer to' from ptr mapping
     * 
     *  @param  ptr a raw pointer
     */
    unsigned int read_pointer_to(SIO_POINTER_DECL *ptr);
    
    /**
     *  @brief  Write a 'pointer to' to ptr mapping
     * 
     *  @param  ptr a raw pointer
     */
    unsigned int write_pointer_to(SIO_POINTER_DECL *ptr);

    /**
     *  @brief  Read the next record from file
     *
     *  @param  records the possible records to be read
     *  
     *  @return a structure containing the read record and read status
     */
    record_read_result read_next_record( const record_map &records );

    /**
     *  @brief  Write a record to file
     *  
     *  @param  record the record to write
     *  @return a structure containing the record write result
     */
    record_write_result write_record( record_ptr &record );
    
  private:
    /**
     *  @brief  Perform an after-read pointer relocation
     */
    void pointer_relocation_read();
    
    /**
     *  @brief  Perform an after-write pointer relocation
     */
    void pointer_relocation_write();
    
  private:
    unsigned int clear_zstream();
    unsigned int write_raw(const int size, const int count, unsigned char *to);
    unsigned int read_raw(const int size, const int count, unsigned char *from);
    unsigned int do_read_record( record_ptr &record );
    unsigned int do_write_record( record_ptr &record );
    
  private:
    /// The buffer pointer (beginning)
    unsigned char *_buffer_begin{nullptr};
    /// The buffer pointer (current)
    unsigned char *_buffer_current{nullptr};
    /// The buffer pointer (end)
    unsigned char *_buffer_end{nullptr};
    /// The record pointer (end)
    unsigned char *_record_end{nullptr};
    /// The block  pointer (end)
    unsigned char *_block_end{nullptr};
    /// The compression buffer pointer (beg)
    unsigned char *_cmp_begin{nullptr};
    /// The compression buffer pointer (end)
    unsigned char *_cmp_end{nullptr};
    /// The compression buffer control
    struct z_stream_s *_z_stream{nullptr};
    /// Stream's associated file
    std::string _filename{};      
    /// File handle
    FILE *_handle{nullptr};
    /// Map of 'pointed at'
    pointed_at_map _pointed_at{};
    /// Multimap of 'pointer to'
    pointer_to_map _pointer_to{};
    /// Stream mode
    SIO_stream_mode _mode{SIO_MODE_UNDEFINED};
    /// Reserved size of buffer
    unsigned int _reserve{32*SIO_KBYTE*SIO_KBYTE};
    /// Stream state  
    SIO_stream_state _state{SIO_STATE_CLOSED};
    /// The compression level
    int _compression_level{0};   
  };
  
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  
  inline SIO_64BITINT stream::file_tell() {
    return FTELL(_handle);
  }
  
  //----------------------------------------------------------------------------
  
  inline const std::string &stream::file_name() const {
    return _filename;
  }
  
  //----------------------------------------------------------------------------
  
  inline SIO_stream_mode stream::mode() const {
    return _mode;
  }
  
  //----------------------------------------------------------------------------
  
  inline SIO_stream_state stream::state() const {
    return _state;
  }
  
  //----------------------------------------------------------------------------
  
  inline void stream::set_reserve(unsigned int reserve) {
    _reserve = reserve;
  }
  
  //----------------------------------------------------------------------------
  
  template <typename T>
  inline unsigned int stream::write_data(T * /*data*/, const int /*count*/) {
    std::stringstream ss; ss << "stream::write_data: Unknown type for writing: " << typeid(T).name();
    throw std::runtime_error( ss.str().c_str() );
    return 0;
  }
  
  //----------------------------------------------------------------------------
  
  template <typename T>
  inline unsigned int stream::read_data(T * /*data*/, const int /*count*/) {
    std::stringstream ss; ss << "stream::read_data: Unknown type for reading: " << typeid(T).name();
    throw std::runtime_error( ss.str().c_str() );
    return 0;
  }
  
  //---------------------------------------------------------------------------
  
#define WRITE_DATA_TIMPL(TYPE, SIZE) \
template <> \
inline unsigned int stream::write_data(TYPE *data, const int count) { \
  return write_raw(SIZE, count, UCHR_CAST(data)); \
}
#define READ_DATA_TIMPL(TYPE, SIZE) \
template <> \
inline unsigned int stream::read_data(TYPE *data, const int count) { \
  return read_raw(SIZE, count, UCHR_CAST(data)); \
}
  
  // write impl
  WRITE_DATA_TIMPL(char, SIO_LEN_SB)
  WRITE_DATA_TIMPL(unsigned char, SIO_LEN_SB)
  WRITE_DATA_TIMPL(short, SIO_LEN_DB)
  WRITE_DATA_TIMPL(unsigned short, SIO_LEN_DB)
  WRITE_DATA_TIMPL(int, SIO_LEN_QB)
  WRITE_DATA_TIMPL(unsigned int, SIO_LEN_QB)
  WRITE_DATA_TIMPL(std::size_t, SIO_LEN_QB)
  WRITE_DATA_TIMPL(SIO_64BITINT, SIO_LEN_OB)
  WRITE_DATA_TIMPL(unsigned SIO_64BITINT, SIO_LEN_OB)
  WRITE_DATA_TIMPL(float, SIO_LEN_QB)
  WRITE_DATA_TIMPL(double, SIO_LEN_OB)
  // read impl
  READ_DATA_TIMPL(char, SIO_LEN_SB)
  READ_DATA_TIMPL(unsigned char, SIO_LEN_SB)
  READ_DATA_TIMPL(short, SIO_LEN_DB)
  READ_DATA_TIMPL(unsigned short, SIO_LEN_DB)
  READ_DATA_TIMPL(int, SIO_LEN_QB)
  READ_DATA_TIMPL(unsigned int, SIO_LEN_QB)
  READ_DATA_TIMPL(std::size_t, SIO_LEN_QB)
  READ_DATA_TIMPL(SIO_64BITINT, SIO_LEN_OB)
  READ_DATA_TIMPL(unsigned SIO_64BITINT, SIO_LEN_OB)
  READ_DATA_TIMPL(float, SIO_LEN_QB)
  READ_DATA_TIMPL(double, SIO_LEN_OB)
  
#undef WRITE_DATA_TIMPL
#undef READ_DATA_TIMPL
  
}

#endif
