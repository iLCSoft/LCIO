// -*- C++ -*-
/** Concrete implementation of LCWriter using SIO.
 * 
 * @author gaede
 * @version Mar 6, 2003
 */
#ifndef SIO_SIOREADER_H
#define SIO_SIOREADER_H 1

#include <string>
#include <set>
#include "IO/LCReader.h"
#include "IO/LCEventListener.h"
#include "IO/LCRunListener.h"

#include "IOIMPL/LCEventIOImpl.h"
#include "IMPL/LCRunHeaderImpl.h"

#include "EVENT/LCExceptions.h"


class SIO_record ;
class SIO_stream ;    

namespace SIO {
  
  //class SIOEventHeaderHandler ;
  //class SIOEventHandler ;
  
  class SIOReader : public IO::LCReader {
    
  public:
    
    /** Default constructor.
     */
    SIOReader() ;
    
    // Destructor
    virtual ~SIOReader() ;
    
    /** Opens a file for reading (read-only).
     * @throws IOException
     */
    virtual void open(const std::string & filename) throw (IO::IOException) ;
    
    /** Reads the next run header from the file. Returns null if no more 
     * run headers or error occured.
     */
    virtual EVENT::LCRunHeader * readNextRunHeader() ;  


    /** Reads the next event in read only mode from file - in case of error or EOF NULL is returned.
     */
    virtual EVENT::LCEvent* readNextEvent() ;
    

    /** Reads the next event from file with the given access mode - 
     * in case of error or EOF NULL is returned.
     */
    virtual EVENT::LCEvent* readNextEvent( int accessMode) ;
    

    /** Closes the output file and returns LCIO::SUCCESS.
     */
    virtual int close() ;
    
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
     * found in the stream. Returns LCIO::SUCCESS and LCIO::ERROR respectively.
     */
    virtual int readStream() ;

  protected:

    void setUpHandlers() ;
    int readRecord() ;

  protected:
    
    // we need an SIO record for every type
    SIO_record *_evtRecord ;
    SIO_record *_hdrRecord ;
    SIO_record *_runRecord ;
    SIO_record *_dummyRecord ;  // used for reading arbitrary records
    SIO_stream *_stream ;
    
  private:
    
    IOIMPL::LCEventIOImpl *_defaultEvt ; // used to add collections when reading 
    IOIMPL::LCEventIOImpl **_evtP ;
    IMPL::LCRunHeaderImpl **_runP ;

    std::set<IO::LCRunListener*> _runListeners ;
    std::set<IO::LCEventListener*> _evtListeners ;
    
  }; // class
}; // namespace

#endif /* ifndef SIO_SIOREADER_H */
