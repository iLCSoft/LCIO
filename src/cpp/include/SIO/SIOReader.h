#ifndef SIO_SIOREADER_H
#define SIO_SIOREADER_H 1

#include <string>
#include <set>
#include <map>
#include "IO/LCReader.h"
#include "IO/LCEventListener.h"
#include "IO/LCRunListener.h"

#include "IOIMPL/LCEventIOImpl.h"
#include "IOIMPL/LCRunHeaderIOImpl.h"

#include "LCIORandomAccessMgr.h"
#include "LCIOTypes.h"
#include "EVENT/LCIO.h"


class SIO_record ;
class SIO_stream ;    


namespace SIO {

class SIORunHeaderHandler ;
class SIOEventHandler ;
  
/** Concrete implementation of LCWriter using SIO.
 * 
 * @author gaede
 * @version $Id: SIOReader.h,v 1.29 2011-03-03 16:00:12 gaede Exp $
 */
  class SIOReader : public IO::LCReader {
    
    //    typedef std::map< EVENT::long64 , EVENT::long64 > EventMap ;
    //    typedef RunEventMap EventMap ;
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
    virtual void open(const std::vector<std::string>& filenames) 
       ;


    /** Opens a file for reading (read-only).
     * @throws IOException
     */
    virtual void open(const std::string & filename)  ;
    
    /** Reads the next run header from the file. 
     *
     * @throws IOException
     */
    virtual EVENT::LCRunHeader * readNextRunHeader()  ;

    /** Same as readNextRunHeader() but allows to set the access mode 
     *  LCIO::READ_ONLY (default) or LCIO::Update. 
     *
     * @throws IOException
     */
    virtual EVENT::LCRunHeader * readNextRunHeader(int accessMode)  ;


    /** Reads the next event from the file. 
     *
     * @throws IOException
     */
    virtual EVENT::LCEvent* readNextEvent()  ;
    

    /** Same as readNextRunHeader() but allows to set the access mode 
     *  LCIO::READ_ONLY (default) or LCIO::Update
     *
     * @throws IOException
     */
    virtual EVENT::LCEvent* readNextEvent( int accessMode)  ;
    

    /** Return the number of events in the file - the file has to be open. In
     *  case several input files are specified in the open() method - 
     *  the number of events in the file that is currently open is returned. 
     */
   virtual int getNumberOfEvents()  ;


    /** Return the number of runs (run headers) in the file - the file has to be open. In
     *  case several input files are specified in the open() method - 
     *  the number of runs (run headers) in the file that is currently open is returned. 
     */
    virtual int getNumberOfRuns()  ;


    /** Return the run numbers of the runs (run headers) in the file - the file has to be open. In
     *  case several input files are specified in the open() method - 
     *  the run numbers of the runs (run headers) in the file that is currently open is returned. 
     */
    virtual void getRuns(EVENT::IntVec & runs) ;

    /** Return the run and event numbers of the events in the file - the file has to be open. In
     *  case several input files are specified in the open() method - 
     *  the  run and event numbers of the events in the file that is currently open are returned.
     *  The size of the vector events will be twice the number of events, where i-th run number
     *  will be in events[2*i] and the i-th event number in  events[2*i+].
     */
    virtual void getEvents(EVENT::IntVec & events) ;


    /** Skips the next n events from the current position. In fact simply reads the next n
      *  event headers so that the next event read is the (n+1)-th event.
      */
    virtual void skipNEvents(int n)     ;



    /** Limit the collection names that are going to be read to the subset given in the vector -
     *  all other collection will be ignored. This might improve the reading performance
     *  considerably in cases where only a small subset of the collections in the event is needed. 
     */
    virtual void setReadCollectionNames(const std::vector<std::string>& colnames) ;


    /** Reads the specified runHeader from file. Returns NULL if
     *  the specified runHeader hasn't been found in the file.
     *
     * @throws IOException
     */
    virtual EVENT::LCRunHeader * readRunHeader(int runNumber ) 
       ;

    /** Same as LCEvent* readRunHeader(int runNumber) 
     *  allowing to set the access mode LCIO::READ_ONLY (default) or LCIO::Update.
     *
     * @throws IOException
     */
    virtual EVENT::LCRunHeader * readRunHeader(int runNumber, int accessMode ) 
       ;

    /** Reads the specified event from file. Returns NULL if
     *  the specified event hasn't been found in the file.
     *
     * @throws IOException
     */
    virtual EVENT::LCEvent * readEvent(int runNumber, int evtNumber) 
       ;


    /** Same as LCEvent* readEvent(int runNumber, int evtNumber 
     *  allowing to set the access mode LCIO::READ_ONLY (default) or LCIO::Update.
     *
     * @throws IOException
     */
    virtual EVENT::LCEvent * readEvent(int runNumber, int evtNumber, int accessMode) 
       ;
    
    /** Closes the output file/stream etc.
     *
     * @throws IOException
     */
    virtual void close()  ;
    
    // interface for listeners
 
    /** Registers a listener for reading LCEvents from a stream.
     */ 
    virtual void registerLCEventListener(IO::LCEventListener * ls) ;

    /** Remove a listener for reading LCEvents from a stream.
     */ 
    virtual void removeLCEventListener(IO::LCEventListener * ls) ;

    /** Registers a listener for reading LCEventsLCRunHeaders from a stream.
     */ 
    virtual void registerLCRunListener(IO::LCRunListener * ls) ;

    /** Remove a listener for reading LCRunHeaders from a stream.
     */ 
    virtual void removeLCRunListener(IO::LCRunListener * ls) ;

    /** Reads the input stream and notifies registered 
     * listeners according to the object type 
     * found in the stream. 
     *
     * @throws IOException
     * @throws EndOfException
     */
    virtual void readStream()  ;

    /** Reads maxRecord from the input stream and notifies registered 
     * listeners according to the object type found in the stream. 
     * Throws EndOfException if less than maxRecord records are found in the stream. 
     *
     * @throws IOException
     * @throws EndOfException
     */
    virtual void readStream(int maxRecord)  ;




  protected:

    void setUpHandlers() ;
    void readRecord()  ;


    void postProcessEvent() ;

    void getEventMap() ;

    void recreateEventMap() ;

  protected:
    
    SIO_record *_dummyRecord {NULL};  // used for reading arbitrary records
    SIO_stream *_stream{NULL} ;

    SIORunHeaderHandler* _runHandler{NULL} ;
    SIOEventHandler* _evtHandler{NULL} ;

  private:
    
    IOIMPL::LCEventIOImpl *_defaultEvt{NULL} ; // used to add collections when reading 
    IOIMPL::LCEventIOImpl *_evt{NULL} ;
    IOIMPL::LCRunHeaderIOImpl *_run{NULL} ;

    std::set<IO::LCRunListener*> _runListeners{} ;
    std::set<IO::LCEventListener*> _evtListeners{} ;
    
    std::vector<std::string> _myFilenames{} ;
    unsigned int _currentFileIndex{0} ;

    bool _readEventMap{false} ;
    
    LCIORandomAccessMgr _raMgr{} ;

  }; // class
} // namespace

#endif /* ifndef SIO_SIOREADER_H */
