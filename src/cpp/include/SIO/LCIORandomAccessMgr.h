#ifndef SIO_LCIORandomAccessMgr_H
#define SIO_LCIORandomAccessMgr_H 1

// -- lcio headers
#include "SIO/LCIORandomAccess.h"
#include "SIO/LCIORecords.h"
#include "SIO/RunEventMap.h"

// -- sio headers
#include <sio/definitions.h>
#include <sio/buffer.h>

// -- std headers
#include <iostream>
#include <map>
#include <list>

namespace SIO {

  class LCIORandomAccessMgr ;
  class SIOIndexHandler ;
  class SIORandomAccessHandler ;

  std::ostream & operator<<(std::ostream& os, const LCIORandomAccessMgr& ra ) ;

/**  Manager class for LCIO direct access. Provides the functionality for reading and writing the LCIORandomAccess and
 *   LCIOIndex records as needed by SIOReader and SIOWriter.
 *   Direct Access is implemented through appending LCIOIndex and LCIORandomAccess and records to the end of the LCIO file.
 *   The LCIOIndex records hold the locations of the RunHeader and EventHeader in the file and are referenced by corresponding
 *   LCIORandomAccess records. The last record in the file will always be an LCIORandomAccess record - and previous records can
 *   be found through pointers to the previous record (simply linked list). LCIORandomAccess are always stored uncompressed.
 *   When reading a file with direct access mode a RunEvent map is created by reading all LCIORandomAccess and LCIOIndex records
 *   or - for old files - is recreated from all RunHeader and EventHeader records in the file. If an old file is opened in APPEND
 *   mode the corresponding records are created and written at the end on close() @see writeRandomAccessRecords().
 *   @TODO: currently the last LCIORandomAccess record is found by seeking a fixed size from the end - need to store the actual
 *          length as last word in the file for future changes of the records length
 *
 *
 * @author gaede
 * @version $Id: LCIORandomAccessMgr.h,v 1.3 2011-03-03 16:00:12 gaede Exp $
 */

  class LCIORandomAccessMgr {
    friend class SIOIndexHandler ;
    friend class SIORandomAccessHandler ;
    friend std::ostream & operator<<(std::ostream& os, const LCIORandomAccessMgr& ra ) ;

  public:
    typedef RunEvent::long64 long64 ;
    typedef std::list<std::shared_ptr<LCIORandomAccess>> LCIORandomAccessList ;

  public:
    LCIORandomAccessMgr() = default ;
    LCIORandomAccessMgr(const LCIORandomAccessMgr&) = delete ;
    LCIORandomAccessMgr& operator=(const LCIORandomAccessMgr&) = delete ;
    ~LCIORandomAccessMgr() = default ;

    /** Return the position of the specified Event record or Run record respectively (if EventNum == -1 ).
     *  Returns RunEventMap::NPos if no record found.
     */
    long64 getPosition( const RunEvent& re ) {
      return _runEvtMap->getPosition( re  ) ;
    }

     /** Add a new entry to the event map - if the RunEvent already exists the new position will be stored.
     */
    void add(const RunEvent& re, long64 pos ) {
      _runEvtMap->add( re , pos ) ;
    }

    /** Get the run and event header map from the stream - either by reading the random access records or by recreating
     *  it for old files.
     */
    bool createEventMap( sio::ifstream &stream ) ;

    /** Return the event map  - it will be empty, if not yet created.
     */
    std::shared_ptr<RunEventMap> getEventMap() { return _runEvtMap ; }

    /** Initialize random access for append mode: read last LCIORandomAccess record if it exists -
     *  recreate the RunEvent map from the file if not (old files).
     */
    bool initAppend( sio::ifstream &stream ) ;


    /** Write the current random access records LCIOIndex and LCIORandomAccess to the stream.
     */
    void writeRandomAccessRecords( sio::ofstream &stream ) ;

    /** Clear all lists and maps before closing a file */
    void clear() ;

    /** Seek the stream at the given position */
    void seekStream( sio::ifstream &stream, long64 pos ) ;

  protected:

    /** Prepare an LCIORandomAccess object from the current contents of RunEventMap (all file locations set to 0). */
    std::shared_ptr<LCIORandomAccess> createFromEventMap() const ;

    /** Add a new LCIORandomAccess object to the list */
    void addLCIORandomAccess( std::shared_ptr<LCIORandomAccess> ra ) ;

    /** Read the LCIORandomAccess record at the specified position */
    bool readLCIORandomAccessAt( sio::ifstream &stream , long64 pos) ;

    /** Read the LCIOIndex record at the specified position */
    bool readLCIOIndexAt( sio::ifstream &stream , long64 pos) ;

   /** Fill the RunEventMap from the event and run header records in the file */
    bool recreateEventMap( sio::ifstream &stream ) ;

    /** Helper for reading the next LCIORandomAccess record (need preceeding call to LCSIO::seek() ) */
    bool readLCIORandomAccess( sio::ifstream &stream ) ;

    /** Helper for reading the next LCIOIndex record (need preceeding call to LCSIO::seek() ) */
    bool readLCIOIndex( sio::ifstream &stream ) ;


    /** Create file record from all LCIORandomAccess records */
    void createFileRecord() ;

    /**Pointer to the last LCIORandomAccess in the list */
    const std::shared_ptr<LCIORandomAccess> lastLCIORandomAccess() const {
      return (_list.empty() ?  0 : _list.back() )  ;
    }

  private:
    sio::buffer                        _rawBuffer { 32*sio::kbyte } ;

    sio::buffer                        _compressBuffer { 32*sio::kbyte } ;
    /// Map with RunHeader and EventHeader record positions
    std::shared_ptr<RunEventMap>       _runEvtMap {std::make_shared<RunEventMap>()} ;
    /// List of LCIORandomAccess objects
    LCIORandomAccessList               _list {} ;
    /// The file record summary
    std::shared_ptr<LCIORandomAccess>  _fileRecord {nullptr} ;
  };
} // namespace
#endif
