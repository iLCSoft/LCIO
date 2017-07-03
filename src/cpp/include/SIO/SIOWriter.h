#ifndef SIO_SIOWRITER_H
#define SIO_SIOWRITER_H 1

#include <string>
#include <vector>
#include "IO/LCWriter.h"
#include "EVENT/LCEvent.h"
#include "EVENT/LCRunHeader.h"

#include "LCIORandomAccessMgr.h"

#include "SIO_block.h"

class SIO_record ;
class SIO_stream ;    


namespace SIO {

  class SIOEventHandler ;
  class SIORunHeaderHandler ;
  class SIOCollectionHandler ;

  class SIOReader ;
  class SIOUnpack ;

/** Concrete implementation of LCWriter using SIO.
 * Use LCFactory to instantiate.
 *
 * @see LCFactory
 * @author gaede
 * @version Mar 6, 2003
 */
  class SIOWriter : public IO::LCWriter {


    friend class SIOReader ; // SIO Reader uses SAME SIO records !!
    friend class SIOUnpack ;

  public:

    /**Default constructor.
     */
    SIOWriter() ;

    /// no copy constructor
    SIOWriter(const SIOWriter&) = delete ;
    /// no assignment operator
    SIOWriter& operator=(const SIOWriter&) = delete ;

    /**
     * Destructor
     */
    virtual ~SIOWriter() ;

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


  protected:

    /** Sets up the handlers for writing the current event.
     */
    void setUpHandlers(const EVENT::LCEvent * evt)  ;
    
    /** Creates a proper filename with extension 'slcio' 
     * in sioFilename.
     */
    void getSIOFileName(const std::string& filename, 
			std::string& sioFilename)  ; 
    
  protected:
    
    SIO_stream *_stream ;
    int _compressionLevel ;

  private:

    SIOEventHandler *_hdrHandler ;
    SIORunHeaderHandler *_runHandler ;
    std::vector<SIO_block*> _connectedBlocks{} ;

  protected:
    
    LCIORandomAccessMgr _raMgr{} ;

  }; // class

} // namespace.

#endif /* ifndef SIO_SIOWRITER_H */
