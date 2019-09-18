#ifndef MT_LCREADER_H
#define MT_LCREADER_H 1

// -- std headers
#include <string>
#include <vector>
#include <fstream>
#include <memory>

// -- lcio headers
#include "MT/Types.h"
#include "MT/LCReaderListener.h"
#include "LCIOSTLTypes.h"
#include "LCIOTypes.h"
#include "EVENT/LCIO.h"

// need to make forward declaration to 
// avoid lcio dependencies to include directly sio headers

namespace sio {
  class buffer ;
}

namespace SIO {
  class LCIORandomAccessMgr ;
  class SIOHandlerMgr ;
}

namespace MT {

/** Implementation of a LCReader for parallel processing use
 *
 * @see EXAMPLE/lcio_parallel_processing.cc
 * @author rete
 * @version Feb 14, 2019
 */
class LCReader {
private:
  // No copy
  LCReader(const LCReader&) = delete;
  LCReader& operator=(const LCReader&) = delete;

public:
  /// Bit for direct access
  static constexpr int directAccess =  0x00000001 << 0  ;
  /// Bit for lazy unpacking of event records
  static constexpr int lazyUnpack =  0x00000002 << 0  ;

public:
  ~LCReader() = default;

  /**
   *  @brief  Constructor
   *
   *  @param  lcReaderFlag flag for reader options
   */
  LCReader( int lcReaderFlag ) ;

  /** Opens a file for reading (read-only).
   */
  void open( const std::string & filename );

  /** Opens a list of files for reading (read-only). All subsequent
   * read operations will operate on the list, i.e. if an EOF is encountered
   * the next file in the list will be opened and read transparently to the
   * user.
   */
  void open( const std::vector<std::string> & filenames ) ;

  /** Reads the next run header from the file.
   *  Returns NULL if 'EOF' read.
   *  Note that this method is not thread safe !
   */
  std::unique_ptr<EVENT::LCRunHeader> readNextRunHeader( int accessMode = EVENT::LCIO::READ_ONLY ) ;

  /** Reads the next event from the file.
   *  Returns NULL if 'EOF' read.
   *  Note that this method is not thread safe !
   */
  std::unique_ptr<EVENT::LCEvent> readNextEvent( int accessMode = EVENT::LCIO::READ_ONLY ) ;

  /** Return the number of events in the file - the file has to be open. In
   *  case several input files are specified in the open() method -
   *  the number of events in the file that is currently open is returned.
   */
  int getNumberOfEvents() ;

  /** Return the number of runs (run headers) in the file - the file has to be open. In
   *  case several input files are specified in the open() method -
   *  the number of runs (run headers) in the file that is currently open is returned.
   */
  int getNumberOfRuns() ;

  /** Return the run numbers of the runs (run headers) in the file - the file has to be open. In
   *  case several input files are specified in the open() method -
   *  the run numbers of the runs (run headers) in the file that is currently open is returned.
   */
  void getRuns( EVENT::IntVec & runs ) ;

  /** Return the run and event numbers of the events in the file - the file has to be open. In
   *  case several input files are specified in the open() method -
   *  the  run and event numbers of the events in the file that is currently open are returned.
   *  The size of the vector events will be twice the number of events, where i-th run number
   *  will be in events[2*i] and the i-th event number in  events[2*i+].
   */
  void getEvents( EVENT::IntVec & events ) ;

  /** Limit the collection names that are going to be read to the subset given in the vector -
   *  all other collection will be ignored. This might improve the reading performance
   *  considerably in cases where only a small subset of the collections in the event is needed.
   */
  void setReadCollectionNames( const std::vector<std::string> & colnames ) ;

  /** Skips the next n events from the current position.
   */
  void skipNEvents( int n ) ;

  /** Reads the specified runHeader from file. Returns NULL if
   *  the specified runHeader hasn't been found in the file.
   */
  std::unique_ptr<EVENT::LCRunHeader> readRunHeader( int runNumber , int accessMode = EVENT::LCIO::READ_ONLY ) ;

  /** Reads the specified event from file. Returns NULL if
   *  the specified event hasn't been found in the file.
   */
  std::unique_ptr<EVENT::LCEvent> readEvent( int runNumber, int evtNumber , int accessMode = EVENT::LCIO::READ_ONLY ) ;

  /** Closes the output file/stream etc.
   */
  void close() ;

  /** Reads the input stream and notifies
   * listeners according to the object type
   * found in the stream.
   */
  void readStream( const LCReaderListenerList & listeners ) ;

  /** Reads maxRecord from the input stream and notifies
   * listeners according to the object type found in the stream.
   * An exception is thrown if less than maxRecord where read from the file.
   */
  void readStream( const LCReaderListenerList & listeners , int maxRecord ) ;

  /** Reads the input stream and notifies
   * listeners according to the object type
   * found in the stream.
   */
  void readNextRecord( const LCReaderListenerList & listeners ) ;

  /** Reads the input stream and notifies the
   * listener according to the object type
   * found in the stream.
   */
  void readStream( LCReaderListener *listener ) ;

  /** Reads maxRecord from the input stream and notifies
   * listener according to the object type found in the stream.
   * An exception is thrown if less than maxRecord where read from the file.
   */
  void readStream( LCReaderListener *listener , int maxRecord ) ;

  /** Reads the input stream and notifies
   * listener according to the object type
   * found in the stream.
   */
  void readNextRecord( LCReaderListener *listener ) ;

private:
  // void readRecord( const sio::record_map &records , sio::record_read_result &readResult ) ;
  void postProcessEvent( EVENT::LCEvent *evt ) ;
  void getEventMap() ;

private:
  /// The input file stream
  std::ifstream                               _stream {} ;
  /// The raw buffer for extracting bytes from the stream
  std::shared_ptr<sio::buffer>                _rawBuffer {nullptr} ; //{1*sio::mbyte} ;
  /// The raw buffer for uncompression
  std::shared_ptr<sio::buffer>                _compBuffer {nullptr} ; //{2*sio::mbyte} ;
  /// The collection block handler manager for events
  std::shared_ptr<SIO::SIOHandlerMgr>         _eventHandlerMgr {nullptr} ;
  /// Whether to read the event map using the random access manager
  bool                                        _readEventMap {false} ;
  /// Whether to perform the lazy unpacking of event records
  bool                                        _lazyUnpack {false} ;
  /// The list of files to open and read
  std::vector<std::string>                    _myFilenames {} ;
  /// A restricted list of collections to read only
  std::vector<std::string>                    _readCollectionNames {} ;
  /// The current file list index when opening multiple files
  unsigned int                                _currentFileIndex {0} ;
  /// The random access manager for event/run random access in the file
  std::shared_ptr<SIO::LCIORandomAccessMgr>   _raMgr {nullptr} ;
  ///
  std::size_t                                 _bufferMaxSize {0} ;
}; // class

} // namespace MT

#endif /* ifndef MT_LCREADER_H */
