// -*- C++ -*-
/** Concrete implementation of LCWriter using SIO.
 * 
 * @author gaede
 * @version Mar 6, 2003
 */
#ifndef SIO_SIOWRITER_H
#define SIO_SIOWRITER_H 1

#include <string>
#include <vector>
#include "IO/LCWriter.h"
#include "EVENT/LCEvent.h"
#include "EVENT/LCRunHeader.h"

class SIO_record ;
class SIO_stream ;    


namespace SIO {

  class SIOEventHandler ;
  class SIOCollectionHandler ;

    
  class SIOWriter : public IO::LCWriter {


  public:

    SIOWriter() ;
    /**
     * Destructor
     */
    virtual ~SIOWriter() ;

    /** Opens a file for writing.
     */
    virtual int open(const std::string& filename) ;
    
    /** Writes the given run header to file and returns LCIO::SUCCESS if no error occured.
     */
    virtual int writeRunHeader(const EVENT::LCRunHeader * hdr) ;

    /** Writes the given event to file.
     */
    virtual int writeEvent(const EVENT::LCEvent * evt) ;

    /** Closes the output file/stream etc.
     */
    virtual int close() ;

  protected:

    void setUpHandlers(const EVENT::LCEvent * evt)  ;
    

  protected:

    SIO_record *_evtRecord ;
    SIO_record *_hdrRecord ;
    SIO_record *_runRecord ;
    SIO_stream *_stream ;
    

  private:

    SIOEventHandler *_evtHandler ;
    SIOEventHandler *_hdrHandler ;
    std::vector<SIOCollectionHandler*> _colVector ;

    bool isFirstEvent ;
    EVENT::LCEvent **evtP ;

  }; // class

}; // namespace

#endif /* ifndef SIO_SIOWRITER_H */
