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

class SIO_record ;
class SIO_stream ;    


namespace SIO {

  class SIOEventHandler ;
  //  class SIOEventHeaderHandler ;
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
    virtual int open(std::string filename) ;

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
