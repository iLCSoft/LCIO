#ifndef SIO_LCIORECORDS_H
#define SIO_LCIORECORDS_H 1

// -- sio headers
#include <sio/definitions.h>

// -- lcio headers
#include "SIO/LCSIO.h"
#include "SIO/SIOHandlerMgr.h"

namespace IOIMPL {
  class LCEventIOImpl;
  class LCRunHeaderIOImpl;
}

namespace EVENT {
  class LCEvent;
  class LCRunHeader;
}

namespace SIO {
  
  class SIOEventHeaderRecord {
    // static API only
    SIOEventHeaderRecord() = delete ;
  public:
    /**
     *  @brief  Read an event header record.
     *          Decode the buffer and setup the collections in the event.
     *          
     *  @param  buffer the raw SIO buffer to decode
     *  @param  event the event to populate 
     *  @param  readCol the collections to read only
     */
    static void read( const sio::buffer_span &buffer, IOIMPL::LCEventIOIMPL *event, const std::vector<std::string> &readCol ) ;
    
    /**
     *  @brief  Write an event header to the raw sio buffer.
     *  
     *  @param  outbuf the buffer to fill
     *  @param  event the event to write
     *  @param  rec_info the record info to receive
     */
    static void write( sio::buffer &outbuf, const EVENT::LCEvent *event, sio::record_info &rec_info ) ;
  };
  
  class LCIORandomAccessMgr;

  /**
   *  @brief  LCIORecords class.
   *          Holds instances of SIO records for the LCIO event data model.
   *          Manage creation of records and associated blocks
   *
   *  @author R. Ete, DESY. 01/2019
   */
  class LCIORecords {
  private:
    LCIORecords& operator=(const LCIORecords &) = delete;
    LCIORecords(const LCIORecords &) = delete;
    
  public:
    /**
     *  @brief  Constructor
     */
    LCIORecords() = default;
    
    /**
     *  @brief  Create an SIO record for the event header (writing case)
     * 
     *  @param  event the event to use for event header writing
     *  @return a prepared SIO record
     */
    // sio::record_ptr createEventHeaderRecord( const EVENT::LCEvent *event ) const;
    // 
    // /**
    //  *  @brief  Create an SIO record for the event header (reading case)
    //  * 
    //  *  @param  event the event to use for event header reading
    //  *  @param  readCol a subset of collection names to read only (empty means read all)
    //  *  @return a prepared SIO record
    //  */
    // sio::record_ptr createEventHeaderRecord( IOIMPL::LCEventIOImpl **event , const std::vector<std::string> &readCol ) const;
    // 
    // /**
    //  *  @brief  Create an SIO record for the event (writing case)
    //  * 
    //  *  @param  event the event to write 
    //  *  @return a prepared SIO record
    //  */
    // sio::record_ptr createEventRecord( const EVENT::LCEvent *event ) const;
    // 
    // /**
    //  *  @brief  Create an SIO record for the event (reading case)
    //  * 
    //  *  @param  event the event to read
    //  *  @return a prepared SIO record
    //  */
    // sio::record_ptr createEventRecord( IOIMPL::LCEventIOImpl **event ) const;
    // 
    // /**
    //  *  @brief  Create an SIO record for the run header (writing case)
    //  * 
    //  *  @param  runHeader the run header to write 
    //  *  @return a prepared SIO record
    //  */
    // sio::record_ptr createRunRecord( const EVENT::LCRunHeader *runHeader ) const;
    // 
    // /**
    //  *  @brief  Create an SIO record for the run header (reading case)
    //  * 
    //  *  @param  runHeader the run header to read
    //  *  @return a prepared SIO record
    //  */
    // sio::record_ptr createRunRecord( IOIMPL::LCRunHeaderIOImpl **runHeader ) const;
    // 
    // /**
    //  *  @brief  Create a random access record (reading and writing case)
    //  *  
    //  *  @param  raMgr the random access manager for reading/writing a random access object
    //  *  @return a prepared SIO record
    //  */
    // sio::record_ptr createRandomAccessRecord( LCIORandomAccessMgr *raMgr ) const;
    // 
    // /**
    //  *  @brief  Create a index record (reading and writing case)
    //  *  
    //  *  @param  raMgr the random access manager for reading/writing an index
    //  *  @return a prepared SIO record
    //  */
    // sio::record_ptr createIndexRecord( LCIORandomAccessMgr *raMgr ) const;

  private:
    /// The LCIO object handler manager
    SIOHandlerMgr           _handlerMgr{};
  };

} // namespace 

#endif // ifndef SIO_LCIORECORDS_H
