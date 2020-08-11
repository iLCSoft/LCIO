#ifndef SIO_SIOREADER_H
#define SIO_SIOREADER_H 1

// -- std headers
#include <set>

// -- lcio headers
#include "IO/LCReader.h"
#include "IO/LCEventListener.h"
#include "IO/LCRunListener.h"
#include "MT/LCReaderListener.h"
#include "MT/LCReader.h"
// #include "IOIMPL/LCEventIOImpl.h"
// #include "IOIMPL/LCRunHeaderIOImpl.h"
// #include "SIO/SIOHandlerMgr.h"
// #include "SIO/LCIORandomAccessMgr.h"
// #include "EVENT/LCIO.h"
// #include "LCIOTypes.h"

// -- sio headers
// #include "sio/definitions.h"
// #include "sio/buffer.h"

namespace SIO {

/** Concrete implementation of LCWriter using SIO.
 *
 * @author gaede
 * @version $Id: SIOReader.h,v 1.29 2011-03-03 16:00:12 gaede Exp $
 */
  class SIOReader : public IO::LCReader, private MT::LCReaderListener {  // TODO restart from here !
  public:
    /** Default constructor.
     */
    SIOReader( int lcReaderFlag=0 ) ;

    /// no copy constructor
    SIOReader(const SIOReader&) = delete ;
    /// no assignment operator
    SIOReader& operator=(const SIOReader&) = delete ;

    /// Destructor
    virtual ~SIOReader() ;

    /** Opens a list of files for reading (read-only). All subsequent
     * read operations will operate on the list, i.e. if an EOF is encountered
     * the next file in the list will be opened and read transparently to the
     * user.
     * @throws IOException
     */
    void open(const std::vector<std::string>& filenames) override ;

    /** Opens a file for reading (read-only).
     * @throws IOException
     */
    void open(const std::string & filename) override ;

    /** Reads the next run header from the file.
     *
     * @throws IOException
     */
    EVENT::LCRunHeader * readNextRunHeader() override ;

    /** Same as readNextRunHeader() but allows to set the access mode
     *  LCIO::READ_ONLY (default) or LCIO::Update.
     *
     * @throws IOException
     */
    EVENT::LCRunHeader * readNextRunHeader(int accessMode) override ;


    /** Reads the next event from the file.
     *
     * @throws IOException
     */
    EVENT::LCEvent* readNextEvent() override ;


    /** Same as readNextRunHeader() but allows to set the access mode
     *  LCIO::READ_ONLY (default) or LCIO::Update
     *
     * @throws IOException
     */
    EVENT::LCEvent* readNextEvent( int accessMode) override ;


    /** Return the number of events in the file - the file has to be open. In
     *  case several input files are specified in the open() method -
     *  the number of events in the file that is currently open is returned.
     */
    int getNumberOfEvents() override ;


    /** Return the number of runs (run headers) in the file - the file has to be open. In
     *  case several input files are specified in the open() method -
     *  the number of runs (run headers) in the file that is currently open is returned.
     */
    int getNumberOfRuns() override ;


    /** Return the run numbers of the runs (run headers) in the file - the file has to be open. In
     *  case several input files are specified in the open() method -
     *  the run numbers of the runs (run headers) in the file that is currently open is returned.
     */
    void getRuns(EVENT::IntVec & runs) override ;

    /** Return the run and event numbers of the events in the file - the file has to be open. In
     *  case several input files are specified in the open() method -
     *  the  run and event numbers of the events in the file that is currently open are returned.
     *  The size of the vector events will be twice the number of events, where i-th run number
     *  will be in events[2*i] and the i-th event number in  events[2*i+].
     */
    void getEvents(EVENT::IntVec & events) override ;


    /** Skips the next n events from the current position. In fact simply reads the next n
      *  event headers so that the next event read is the (n+1)-th event.
      */
    void skipNEvents(int n) override ;



    /** Limit the collection names that are going to be read to the subset given in the vector -
     *  all other collection will be ignored. This might improve the reading performance
     *  considerably in cases where only a small subset of the collections in the event is needed.
     */
    void setReadCollectionNames(const std::vector<std::string>& colnames) override ;


    /** Reads the specified runHeader from file. Returns NULL if
     *  the specified runHeader hasn't been found in the file.
     *
     * @throws IOException
     */
    EVENT::LCRunHeader * readRunHeader(int runNumber ) override ;

    /** Same as LCEvent* readRunHeader(int runNumber)
     *  allowing to set the access mode LCIO::READ_ONLY (default) or LCIO::Update.
     *
     * @throws IOException
     */
    EVENT::LCRunHeader * readRunHeader(int runNumber, int accessMode ) override ;

    /** Reads the specified event from file. Returns NULL if
     *  the specified event hasn't been found in the file.
     *
     * @throws IOException
     */
    EVENT::LCEvent * readEvent(int runNumber, int evtNumber) override ;


    /** Same as LCEvent* readEvent(int runNumber, int evtNumber
     *  allowing to set the access mode LCIO::READ_ONLY (default) or LCIO::Update.
     *
     * @throws IOException
     */
    EVENT::LCEvent * readEvent(int runNumber, int evtNumber, int accessMode) override ;

    /** Closes the output file/stream etc.
     *
     * @throws IOException
     */
    void close() override ;

    // interface for listeners

    /** Registers a listener for reading LCEvents from a stream.
     */
    void registerLCEventListener(IO::LCEventListener * ls) override ;

    /** Remove a listener for reading LCEvents from a stream.
     */
    void removeLCEventListener(IO::LCEventListener * ls) override ;

    /** Registers a listener for reading LCEventsLCRunHeaders from a stream.
     */
    void registerLCRunListener(IO::LCRunListener * ls) override ;

    /** Remove a listener for reading LCRunHeaders from a stream.
     */
    void removeLCRunListener(IO::LCRunListener * ls) override ;

    /** Reads the input stream and notifies registered
     * listeners according to the object type
     * found in the stream.
     *
     * @throws IOException
     * @throws EndOfException
     */
    void readStream() override ;

    /** Reads maxRecord from the input stream and notifies registered
     * listeners according to the object type found in the stream.
     * Throws EndOfException if less than maxRecord records are found in the stream.
     *
     * @throws IOException
     * @throws EndOfException
     */
    void readStream(int maxRecord) override ;
    
  private:
    void processEvent( std::shared_ptr<EVENT::LCEvent> event ) override ;
    void processRunHeader( std::shared_ptr<EVENT::LCRunHeader> hdr ) override ;

  private:
    /// The underlying implementation of the LCReader
    MT::LCReader                         _reader ;
    /// The run listeners
    std::set<IO::LCRunListener*>         _runListeners {} ;
    /// The event listeners
    std::set<IO::LCEventListener*>       _evtListeners {} ;
    /// pointer to current Event
    EVENT::LCEvent* _currentEvent = nullptr ;
    /// pointer to current RunHeader
    EVENT::LCRunHeader* _currentRun = nullptr ;

  }; // class
} // namespace

#endif /* ifndef SIO_SIOREADER_H */
