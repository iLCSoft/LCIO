#ifndef IOIMPL_WriterDecorator_H
#define IOIMPL_WriterDecorator_H 1

#include <string>
#include <vector>
#include "IO/LCWriter.h"
#include "EVENT/LCEvent.h"
#include "EVENT/LCRunHeader.h"

namespace IOIMPL {
  
  /** Decorator for LCWriter that decides on io type in open.
   *
   * @see LCFactory
   * @author gaede
   * @version Sep, 2009
   */
  class WriterDecorator : public IO::LCWriter {
    
    IO::LCWriter* _wrt ;
    
  public:
    
    /**Default constructor.
     */
    WriterDecorator() : _wrt(0) , _compressionLevel(-1) {}  

    /**
     * Destructor
     */
    virtual ~WriterDecorator(){
      delete _wrt ;
    }
    
    /** Opens a file for writing. If file with given name exists, 
     * an exception is thrown. Use append or new mode instead.
     *
     *@throws IOException
     */
    virtual void open(const std::string & filename) throw (IO::IOException, std::exception) {

      initialize( filename ) ;
      _wrt->open( filename ) ;
    }


    /** Opens a file for writing.
     * Possible write modes are: LCIO::WRITE_NEW
     * (existing files are replaced) and LCIO::WRITE_APPEND. 
     *
     *@throws IOException
     */
    virtual void open(const std::string & filename, int writeMode) throw (IO::IOException, std::exception){

      initialize( filename ) ;
      _wrt->open( filename , writeMode) ;
    }    

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
    virtual void setCompressionLevel(int level) { 

      if( _wrt ) 
	_wrt->setCompressionLevel( level ) ; 
      else 
	_compressionLevel = level ;
    }


    /** Writes the given run header to file.
     *
     *@throws IOException
     */
    virtual void writeRunHeader(const EVENT::LCRunHeader * hdr) throw (IO::IOException, std::exception)  {
      
      _wrt->writeRunHeader( hdr ) ; 
    }
    
    /** Writes the given event to file.
     *
     *@throws IOException
     */
    virtual void writeEvent(const EVENT::LCEvent * evt) throw (IO::IOException, std::exception) {
      
      _wrt->writeEvent( evt ) ; 
    }


    /** Closes the output file/stream etc.
     *
     *@throws IOException
     */
    virtual void close() throw (IO::IOException, std::exception) {
      
      _wrt->close() ; 
    }

    
    /** Flushes the output file/stream etc.
     *
     *@throws IOException
     */
    virtual void flush() throw (IO::IOException, std::exception) {
      
      _wrt->flush() ; 
    }

  protected:

    void initialize(const std::string& filename) ;


    int _compressionLevel ;
    
  }; // class
} // namespace.

#endif 
