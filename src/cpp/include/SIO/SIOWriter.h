#ifndef SIO_SIOWRITER_H
#define SIO_SIOWRITER_H 1

// -- std headers
#include <string>
#include <vector>

// -- lcio headers
#include "IO/LCWriter.h"
#include "EVENT/LCEvent.h"
#include "EVENT/LCRunHeader.h"
#include "SIO/LCIORecords.h"
#include "SIO/LCIORandomAccessMgr.h"

// -- sio headers
#include "sio/definitions.h"
#include "sio/buffer.h"
#include <sio/compression/zlib.h>

namespace SIO {

/** Concrete implementation of LCWriter using SIO.
 * Use LCFactory to instantiate.
 *
 * @see LCFactory
 * @author gaede
 * @version Mar 6, 2003
 */
  class SIOWriter : public IO::LCWriter {
  public:

    /// Default constructor
    SIOWriter() = default ;
    /// no copy constructor
    SIOWriter(const SIOWriter&) = delete ;
    /// no assignment operator
    SIOWriter& operator=(const SIOWriter&) = delete ;
    /// Destructor
    ~SIOWriter() = default ;

    /** Opens a file for writing. If file with given name exists,
     * an exception is thrown. Use append or new mode instead.
     *
     *@throws IOException
     */
    virtual void open(const std::string & filename)  ;

    /** Opens a file for writing.
     * Possible write modes are: LCIO::WRITE_NEW
     * (existing files are replaced) and LCIO::WRITE_APPEND.
     *
     *@throws IOException
     */
    virtual void open(const std::string & filename, int writeMode)  ;

    /** Set the compression level - needs to be called before open() otherwise
     *  call will have no effect. If not called the Writer will use default compression.<br>
     *  Valid compression levels are:
     *  <ul>
     *    <li> level <  0 : default compression </li>
     *    <li> level == 0 : no compression</li>
     *    <li> level >  0 : 1 (fastest) - 9 (best compression)
     *    </li>
     *  </ul>
     *  Experimental code - don't use for production.
     *
     *@param level compression level
     */
    virtual void setCompressionLevel(int level) ;


    /** Writes the given run header to file.
     *
     *@throws IOException
     */
    virtual void writeRunHeader(const EVENT::LCRunHeader * hdr)  ;

    /** Writes the given event to file.
     *
     *@throws IOException
     */
    virtual void writeEvent(const EVENT::LCEvent * evt)  ;

    /** Closes the output file/stream etc.
     *
     *@throws IOException
     */
    virtual void close()  ;

    /** Flushes the output file/stream etc.
     *
     *@throws IOException
     */
    virtual void flush()  ;

  private:
    /** Creates a proper filename with extension 'slcio'
     * in sioFilename.
     */
    void getSIOFileName(const std::string& filename, std::string& sioFilename)  ;

  private:
    /// The output file stream
    sio::ofstream                        _stream {} ;
    /// The raw buffer for writing bytes to the stream
    sio::buffer                          _rawBuffer {2*sio::mbyte} ;
    /// The raw buffer for compression
    sio::buffer                          _compBuffer {sio::mbyte} ;
    /// The compression in use
    sio::zlib_compression                _compressor {} ;
    /// The collection block handler manager for events
    SIOHandlerMgr                        _eventHandlerMgr {} ;
    /// The random access manager
    LCIORandomAccessMgr                  _raMgr {} ;
  }; // class

} // namespace.

#endif /* ifndef SIO_SIOWRITER_H */
