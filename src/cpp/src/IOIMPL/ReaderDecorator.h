#ifndef IOIMPL_ReaderDecorator_H
#define IOIMPL_ReaderDecorator_H 1

#include <string>
// #include <set>
// #include <map>
#include "IO/LCReader.h"
#include "IO/LCEventListener.h"
#include "IO/LCRunListener.h"



namespace IOIMPL {

  
  /** Decorator for LCReader that decides on io type in open.
   *
   * @see LCFactory
   * @author gaede
   * @version Sep, 2009
   */
  
  class ReaderDecorator : public IO::LCReader {
    
    IO::LCReader* _rdr ;
    int _lcReaderFlag ;

  public:
    
    /** Default constructor.
     */
    ReaderDecorator( int lcReaderFlag=0 ) : 
      _rdr(0) ,
      _lcReaderFlag( lcReaderFlag ) { 

    }     

    // Destructor
    virtual ~ReaderDecorator() {

      delete _rdr ;
    }

    /** Opens a list of files for reading (read-only). All subsequent
     * read operations will operate on the list, i.e. if an EOF is encountered
     * the next file in the list will be opened and read transparently to the
     * user.
     * @throws IOException
     */
    virtual void open(const std::vector<std::string>& filenames) 
      throw (IO::IOException, std::exception)  {
      
      if( filenames.size() > 0 ) 

	initialize( filenames[0] ) ;

      else{
	std::string empty("") ;
	initialize(  empty ) ;
      }
      _rdr->open( filenames ) ;
    }
    


    /** Opens a file for reading (read-only).
     * @throws IOException
     */
    virtual void open(const std::string & filename) throw (IO::IOException, std::exception){

      initialize( filename ) ;
      _rdr->open( filename ) ;
    }    
    
    /** Reads the next run header from the file. 
     *
     * @throws IOException
     */
    virtual EVENT::LCRunHeader * readNextRunHeader() throw (IO::IOException, std::exception) {

      return _rdr->readNextRunHeader() ;
    }

    /** Same as readNextRunHeader() but allows to set the access mode 
     *  LCIO::READ_ONLY (default) or LCIO::Update. 
     *
     * @throws IOException
     */
    virtual EVENT::LCRunHeader * readNextRunHeader(int accessMode) throw (IO::IOException, std::exception) {

      return _rdr->readNextRunHeader( accessMode ) ;
    }


    /** Reads the next event from the file. 
     *
     * @throws IOException
     */
    virtual EVENT::LCEvent* readNextEvent() throw (IO::IOException, std::exception) {

      return _rdr->readNextEvent() ;
    }
    
    
    /** Same as readNextRunHeader() but allows to set the access mode 
     *  LCIO::READ_ONLY (default) or LCIO::Update
     *
     * @throws IOException
     */
    virtual EVENT::LCEvent* readNextEvent( int accessMode) throw (IO::IOException, std::exception) {

      return _rdr->readNextEvent( accessMode ) ;
    }
    
 
    

    /** Skips the next n events from the current position. In fact simply reads the next n
     *  event headers so that the next event read is the (n+1)-th event.
     */
    virtual void skipNEvents(int n) {
      
      _rdr->skipNEvents( n ) ;
    }


    /** Reads the specified event from file. 
     *  To be used with care: events have to be read in sequential 
     *  order (as LCIO has no direct access yet).
     *
     * @throws IOException
     */
    virtual EVENT::LCEvent * readEvent(int runNumber, int evtNumber) 
      throw (IO::IOException, std::exception/*, EVENT::NotAvailableException */) {

      return _rdr->readEvent( runNumber, evtNumber  ) ;
    }

    /** Closes the output file/stream etc.
     *
     * @throws IOException
     */
    virtual void close() throw (IO::IOException, std::exception) {
      
      _rdr->close() ;
    }
    
    // interface for listeners
 
    /** Registers a listener for reading LCEvents from a stream.
     */ 
    virtual void registerLCEventListener(IO::LCEventListener * ls) {
      
      _rdr->registerLCEventListener( ls ) ;
    }
    
    /** Remove a listener for reading LCEvents from a stream.
     */ 
    virtual void removeLCEventListener(IO::LCEventListener * ls ) {
      
      _rdr->removeLCEventListener( ls ) ;
    }
    
    /** Registers a listener for reading LCEventsLCRunHeaders from a stream.
     */ 
    virtual void registerLCRunListener(IO::LCRunListener * ls) {
      
      _rdr->registerLCRunListener( ls ) ;
    }
    
    /** Remove a listener for reading LCRunHeaders from a stream.
     */ 
    virtual void removeLCRunListener(IO::LCRunListener * ls) {
      
      _rdr->removeLCRunListener( ls ) ;
    }
    
    
    /** Reads the input stream and notifies registered 
     * listeners according to the object type 
     * found in the stream. 
     *
     * @throws IOException
     * @throws EndOfException
     */
    virtual void readStream() throw (IO::IOException, std::exception) {
     
      _rdr->readStream() ;
    }

    /** Reads maxRecord from the input stream and notifies registered 
     * listeners according to the object type found in the stream. 
     * Throws EndOfException if less than maxRecord records are found in the stream. 
     *
     * @throws IOException
     * @throws EndOfException
     */
    virtual void readStream(int maxRecord) throw (IO::IOException, std::exception) {

      _rdr->readStream( maxRecord ) ;
    }




  protected:

    void initialize(const std::string& filename ) ;


  }; // class
} // namespace

#endif /* ifndef SIO_SIOREADER_H */
