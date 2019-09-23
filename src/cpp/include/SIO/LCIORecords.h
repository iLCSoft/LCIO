#ifndef SIO_LCIORECORDS_H
#define SIO_LCIORECORDS_H 1

// -- std headers
#include <set>

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
     *          The buffer must have been uncompressed before hand and must
     *          start on the first block
     *
     *  @param  buffer the raw SIO buffer to decode
     *  @param  event the event to populate
     *  @param  readCol the collections to read only
     */
    static void readBlocks( const sio::buffer_span &buffer, EVENT::LCEvent *event, const std::vector<std::string> &readCol ) ;

    /**
     *  @brief  Write an event header to the raw sio buffer.
     *          Write the full record to the buffer. The record is not compressed
     *
     *  @param  outbuf the buffer to fill
     *  @param  event the event to write
     *  @param  rec_info the record info to receive
     *  @param  opts an optional word with encoded options
     */
    static void writeRecord( sio::buffer &outbuf, EVENT::LCEvent *event, sio::record_info& rec_info, sio::options_type opts = 0 ) ;

    /**
     *  @brief  Write an event header to the raw sio buffer.
     *          Write the full record to the buffer. The record is not compressed
     *
     *  @param  outbuf the buffer to fill
     *  @param  event the event to write
     *  @param  rec_info the record info to receive
     *  @param  colsOnly the list of collections to write only
     *  @param  opts an optional word with encoded options
     */
    static void writeRecord( sio::buffer &outbuf, EVENT::LCEvent *event, sio::record_info& rec_info, const std::set<std::string> &colsOnly, sio::options_type opts = 0 ) ;
  };

  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------

  class SIOEventRecord {
    // static API only
    SIOEventRecord() = delete ;
  public:
    /**
     *  @brief  Setup the record blocks for the given event.
     *          Works for reading and writing
     *
     *  @param  event the event containing the collections
     *  @param  handlerMgr the handler manager to get the collection block handler
     *  @param  blocks the blocks to receive
     */
    static void setupBlocks( EVENT::LCEvent *event, const SIOHandlerMgr &handlerMgr, sio::block_list &blocks, const std::set<std::string> &colsOnly = {} ) ;

    /**
     *  @brief  Read the blocks (collections) from the sio buffer
     *
     *  @param buffer the input raw buffer
     *  @param event the event containing the collections to populate
     *  @param handlerMgr the collection handler manager to setup the blocks
     */
    static void readBlocks( const sio::buffer_span &buffer, EVENT::LCEvent *event, const SIOHandlerMgr &handlerMgr ) ;

    /**
     *  @brief  Write an event record. Setup the blocks, write the blocks and
     *          write an event record into the buffer
     *
     *  @param  outbuf the sio buffer to write to
     *  @param  event the event to write
     *  @param  handlerMgr the collection block handler manager to setup the blocks
     *  @param  rec_info the record info object to receive from sio
     *  @param  opts an optional word with encoded options
     */
    static void writeRecord( sio::buffer &outbuf, EVENT::LCEvent *event, const SIOHandlerMgr &handlerMgr, sio::record_info& rec_info, sio::options_type opts = 0 ) ;
    
    /**
     *  @brief  Write an event record. Setup the blocks, write the blocks and
     *          write an event record into the buffer
     *
     *  @param  outbuf the sio buffer to write to
     *  @param  event the event to write
     *  @param  handlerMgr the collection block handler manager to setup the blocks
     *  @param  rec_info the record info object to receive from sio
     *  @param  colsOnly thelist of collections only to write
     *  @param  opts an optional word with encoded options
     */
    static void writeRecord( sio::buffer &outbuf, EVENT::LCEvent *event, const SIOHandlerMgr &handlerMgr, sio::record_info& rec_info, const std::set<std::string> &colsOnly, sio::options_type opts = 0 ) ;
  };

  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------

  class SIORunHeaderRecord {
    // static API only
    SIORunHeaderRecord() = delete ;
  public:
    /**
     *  @brief  Read the block(s) from the sio buffer and decode a run header
     *
     *  @param  buffer the input sio buffer to decode
     *  @param  rhdr the run header to read
     */
    static void readBlocks( const sio::buffer_span &buffer, EVENT::LCRunHeader *rhdr ) ;

    /**
     *  @brief  Write a run header record to the sio buffer
     *
     *  @param  outbuf the sio buffer to write the record to
     *  @param  rhdr the run header to write
     *  @param  rec_info the record infor to receive from sio
     *  @param  opts an optional word with encoded options
     */
    static void writeRecord( sio::buffer &outbuf, EVENT::LCRunHeader *rhdr, sio::record_info& rec_info, sio::options_type opts = 0 ) ;
  };

} // namespace

#endif // ifndef SIO_LCIORECORDS_H
