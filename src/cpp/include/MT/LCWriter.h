#ifndef MT_LCWRITER_H
#define MT_LCWRITER_H 1

// -- std headers
#include <string>
#include <vector>
#include <fstream>
#include <memory>

// -- lcio headers
#include "MT/Types.h"
#include "EVENT/LCIO.h"

// need to make forward declaration to 
// avoid lcio dependencies to include directly sio headers

namespace sio {
  class buffer ;
}

namespace SIO {
  class LCIORandomAccessMgr ;
}

namespace MT {

  /**
   *  @brief  LCWriter class.
   *          Implement a writer for events and run headers using SIO.
   *          The implementation of the write methods are meant to be thread safe
   */
  class LCWriter {
  public:
    /// Default constructor
    LCWriter() ;
    /// no copy constructor
    LCWriter(const LCWriter&) = delete ;
    /// no assignment operator
    LCWriter& operator=(const LCWriter&) = delete ;
    /// Destructor
    ~LCWriter() = default ;

    /** 
     *  @brief  Opens a file for writing. If file with given name exists, 
     *          an exception is thrown. Use append or new mode instead.
     */
    void open( const std::string &filename ) ;

    /** 
     *  @brief  Opens a file for writing.
     *          Possible write modes are: LCIO::WRITE_NEW
     *          (existing files are replaced) and LCIO::WRITE_APPEND. 
     */
    void open( const std::string &filename, int writeMode ) ;

    /** 
     *  @brief  Set the compression level - needs to be called before open() otherwise
     *          call will have no effect. If not called the Writer will use default compression.<br>
     *          Valid compression levels are:
     *          - level <  0 : default compression </li>
     *          - level == 0 : no compression</li>
     *          - level >  0 : compression level (typically 1 (fastest) - 9 (best compression))
     *          No data race expected (atomic variable in use) but a race condition can occur if
     *          the compression level is set while write a record
     *          
     *  @param level compression level
     */
    void setCompressionLevel( int level ) ;

    /** 
     *  @brief  Writes the given run header to file.
     */
    void writeRunHeader( EVENT::LCRunHeader *hdr ) ;

    /** 
     *  @brief  Writes the given event to file.
     */
    void writeEvent( EVENT::LCEvent *evt ) ;

    /** 
     *  @brief  Closes the output file/stream.
     */
    void close() ;
    
  private:
    static std::string getSIOFileName( const std::string& filename ) ;
    
  private:
    /// The output file stream
    std::ofstream                        _stream {} ;
    /// The raw buffer for writing bytes to the stream
    std::shared_ptr<sio::buffer>         _rawBuffer {nullptr} ;
    /// The raw buffer for compression
    std::shared_ptr<sio::buffer>         _compBuffer {nullptr} ;
    /// The compression level
    std::atomic<int>                     _compressionLevel {0} ;
    /// The random access manager for event/run random access in the file
    std::shared_ptr<SIO::LCIORandomAccessMgr>   _raMgr {nullptr} ;
  }; // class

}

#endif