#ifndef RIO_RIOWriter_H
#define RIO_RIOWriter_H 1

#include "RIO/RIO.h"

#include <string>
#include <cstring>
#include <vector>
#include "IO/LCWriter.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/LCRunHeaderImpl.h"

class TFile ;
class TTree ;

namespace RIO {

  
  class RIOBranchHandler ;

  //  typedef std::vector< RIOBranchHandler * > BranchVector ;
  typedef std::map< std::string, RIOBranchHandler*> BranchHandlerMap ;


  //  class RIOReader ;


  /** Concrete implementation of LCWriter using ROOT I/O
   *  Use LCFactory to instantiate.
   *
   * @see LCFactory
   * @author gaede
   * @version Sep 9, 2009
   */

  class RIOWriter : public IO::LCWriter {
    
    //     friend class RIOReader ; 
    
  public:
    
    /**Default constructor.
     */
    RIOWriter() ;
    /**
     * Destructor
     */
    virtual ~RIOWriter() ;
    
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
    virtual void writeRunHeader(const EVENT::LCRunHeader * hdr)throw (IO::IOException, std::exception) ;

    /** Writes the given event to file.
     *
     *@throws IOException
     */
    virtual void writeEvent(const EVENT::LCEvent * evt) throw (IO::IOException, std::exception) ;

    /** Closes the output file/stream etc.
     *
     *@throws IOException
     */
    virtual void close() throw (IO::IOException, std::exception) ;
    
    /** Flushes the output file/stream etc.
     *
     *@throws IOException
     */
    virtual void flush() throw (IO::IOException, std::exception) ;

  protected:

    /** Sets up the handlers for writing the current event.
     */
    void setUpHandlers(const EVENT::LCEvent * evt)  ;
    
    /** Creates a proper filename with extension RIO_FILE_EXTENSION
     *  in rioFilename.
     */
    void getRIOFileName(const std::string& filename, 
			std::string& rioFilename)  ; 
    
  protected:
    
    int _compressionLevel ;
    
    TFile* _file ; 
    TTree* _tree ;
    
    IMPL::LCEventImpl* _evtImpl ;
    const IMPL::LCRunHeaderImpl* _runImpl ;
    
    BranchHandlerMap _branches ;
    bool _haveBranches ; 
    

  }; // class
} // namespace.

#endif /* ifndef RIO_RIOWriter_H */
