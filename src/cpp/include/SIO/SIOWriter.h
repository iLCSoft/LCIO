#ifndef SIO_SIOWRITER_H
#define SIO_SIOWRITER_H 1

#include <string>
#include <vector>
#include "IO/LCWriter.h"
#include "DATA/LCEventData.h"
#include "DATA/LCRunHeaderData.h"

class SIO_record ;
class SIO_stream ;    


namespace SIO {

  class SIOEventHandler ;
  class SIORunHeaderHandler ;
  class SIOCollectionHandler ;

    
/** Concrete implementation of LCWriter using SIO.
 * Use LCFactory to instantiate.
 *
 * @see LCFactory
 * @author gaede
 * @version Mar 6, 2003
 */
  class SIOWriter : public IO::LCWriter {


  public:

    /**Default constructor.
     */
    SIOWriter() ;
    /**
     * Destructor
     */
    virtual ~SIOWriter() ;

    /** Opens a file for writing. If file with given name exists, 
     * an exception is thrown. Use append or new mode instead.
     *
     *@throws IOException
     */
    virtual void open(const std::string & filename) throw (IO::IOException, std::exception) ;

    /** Opens a file for writing.
     * Possible write modes are: LCIO::WRITE_NEW
     * (existing files are replaced) and LCIO::WRITE_APPEND. 
     *
     *@throws IOException
     */
    virtual void open(const std::string & filename, int writeMode)throw (IO::IOException, std::exception) ;
    
    /** Writes the given run header to file.
     *
     *@throws IOException
     */
    virtual void writeRunHeader(const DATA::LCRunHeaderData * hdr)throw (IO::IOException, std::exception) ;

    /** Writes the given event to file.
     *
     *@throws IOException
     */
    virtual void writeEvent(const DATA::LCEventData * evt) throw (IO::IOException, std::exception) ;

    /** Closes the output file/stream etc.
     *
     *@throws IOException
     */
    virtual void close() throw (IO::IOException, std::exception) ;

  protected:

    /** Sets up the handlers for writing the current event.
     */
    void setUpHandlers(const DATA::LCEventData * evt)  ;
    
    /** Creates a proper filename with extension 'slcio' 
     * in sioFilename.
     */
    void getSIOFileName(const std::string& filename, 
			std::string& sioFilename)  ; 
    
  protected:
    
    SIO_record *_evtRecord ;
    SIO_record *_hdrRecord ;
    SIO_record *_runRecord ;
    SIO_stream *_stream ;

  private:

    SIOEventHandler *_hdrHandler ;
    SIORunHeaderHandler *_runHandler ;
    std::vector<SIOCollectionHandler*> _colVector ;

  }; // class

}; // namespace.

#endif /* ifndef SIO_SIOWRITER_H */
