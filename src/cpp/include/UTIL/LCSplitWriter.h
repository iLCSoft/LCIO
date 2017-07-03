#ifndef UTIL_LCSplitWriter_H
#define UTIL_LCSplitWriter_H 1

#include "IO/LCWriter.h"
#include "LCIOTypes.h"
#include <string>

namespace UTIL{
  
  /** LCWriter wrapper that automatically splits files if a given
   *  number of bytes is exceeded. File names are extended by
   *  a file number of the form .000 - .999 ... <br>
   * @author gaede
   * @version $Id: LCSplitWriter.h,v 1.2 2007-11-09 21:00:55 gaede Exp $
   */
  class LCSplitWriter : public IO::LCWriter {
    
  public:

    /** The constructor. Takes a pointer to a valid LCWriter instance and 
     * the file size in bytes that causes a new file to be opened if exceeded.<b>
     * example:<br>
     * // replace   LCWriter* lcWrt = LCFactory::getInstance()->createLCWriter() ;  // with
     *  LCSplitWriter* lcWrt = new LCSplitWriter( LCFactory::getInstance()->createLCWriter() , 2040109465  ) ;<br>
     *  to automatically split files after 1.9 GByte file size has been exceeded.
     */            

    LCSplitWriter( IO::LCWriter* wrt,  EVENT::long64 maxBytes ) :
      _wrt( wrt ), 
      _maxBytes(maxBytes),
      _baseFilename(""),
      _filename(""),
      _extension(""),
      _count(0) ,    
      _lastCount(4294967295UL) {    
    }    
   
    /// no copy constructor
    LCSplitWriter(const LCSplitWriter&) = delete ;
    /// no assignment operator
    LCSplitWriter& operator=(const LCSplitWriter&) = delete ;

    /** Destructor.
     */
    virtual ~LCSplitWriter() {;}
    
    
    //--------------------  the LCWriter interface: -------------------------------------


    /** Opens a file for writing where the filename has to include the extension but not the number, e.g.
     *  myfile.slcio. Note that this is  different from the LCWriter specification.
     *
     *@throws IO::IOException
     */
    virtual void open(const std::string & filename)  ;

    /** Not implemented - will throw an Exception if called.
     * Overwriting of or appending to split files is not straight forward. Pleas use the default write mode 
     * and remove exisiting files.
     * 
     * @throws IO::IOException
     */
    virtual void open(const std::string & filename, int writeMode)  ;

    /** Writes the given run header to file. Opens a new file if the given file size is already exceeded
     *  before the execution of the write access.
     *
     * @throws IO::IOException
     */
    virtual void writeRunHeader(const EVENT::LCRunHeader * hdr)  ;

    /** Set CompressionLevel.
     * @see LCWriter::setCompressionLevel()
     */
    virtual void setCompressionLevel(int level) { 
      _wrt->setCompressionLevel(level) ; 
    }  


    /** Writes the given file to file. Opens a new file if the given file size is already exceeded
     *  before the execution of the write access.
     *
     *@throws IO::IOException
     */
    virtual void writeEvent(const EVENT::LCEvent * evt)   ;

    /** Closes the output file/stream.
     *
     *@throws IO::IOException
     */
    virtual void close()   ;


    /** Flushes the output file/stream.
     *
     *@throws IO::IOException
     */
    virtual void flush()   ;

    
    //---------------------------------------------------------------------------------------------

    /** Return the current file size in bytes.
     */
    EVENT::long64 fileSize() ;



  protected:
    LCSplitWriter( );

    /** Helper function that returns the file size in bytes - o if the file does  not exist
     */
    EVENT::long64 file_size( const char *fname) ;

    /**  Get the complete filename w/o extensiomn,e.g. MyFilename.007
     */
    const std::string& getFilename() ;
    
    /** Helper function to determine base file name, i.e. w/o extension.
     */
    void setBaseFilename( const std::string& filename )  ;
    
    /** Returns the string representation of the file number, e.g. "007".
     */
    std::string getCountingString(unsigned count) ;

    IO::LCWriter*  _wrt ;
    EVENT::long64 _maxBytes ;
    std::string _baseFilename ;
    std::string _filename ;
    std::string _extension ;
    unsigned _count ;
    unsigned _lastCount ;
    
  }; // class
  
} // namespace UTIL

#endif /* ifndef UTIL_LCSplitWriter_H */
