// -*- C++ -*-
/** Concrete implementation of LCWriter using SIO.
 * 
 * @author gaede
 * @version Mar 6, 2003
 */
#ifndef SIO_SIOREADER_H
#define SIO_SIOREADER_H 1

#include <string>
#include "IO/LCReader.h"
#include "EVENT/LCEvent.h"
#include "IOIMPL/LCEventIOImpl.h"

class SIO_record ;
class SIO_stream ;    

namespace SIO {
  
  class SIOEventHeaderHandler ;
  class SIOEventHandler ;
  
  class SIOReader : public IO::LCReader {
    
  public:
    
    /** Default constructor.
     */
    SIOReader() ;
    
    // Destructor
    virtual ~SIOReader() ;
    
    /** Opens the file filename for writing and returns LCIO::SUCCESS if succeeded, 
     *  otherwise LCIO::ERROR is returned. 
     */
    virtual int open(std::string filename) ;
    
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
    
    
  protected:

    void setUpHandlers() ;
    
  protected:
    
    SIO_record *_evtRecord ;
    SIO_record *_hdrRecord ;
    SIO_stream *_stream ;
    
  private:
    
    SIOEventHandler *_evtHandler ;
    SIOEventHeaderHandler *_hdrHandler ;
    IOIMPL::LCEventIOImpl **_evtP ;
    
  }; // class
}; // namespace

#endif /* ifndef SIO_SIOREADER_H */
