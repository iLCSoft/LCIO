#ifndef SIO_SIOCONTEXT_H
#define SIO_SIOCONTEXT_H 1

// -- sio headers
#include "SIO_definitions.h"

// -- lcio headers
#include "SIO/LCSIO.h"

namespace SIO {
  
  /**
   *  @brief  SIORecordType enum
   */
  enum class SIORecordType {
    Event = 0,
    Header,
    Run,
    Access,
    Index,
    All
  };

  /**
   *  @brief  SIOContext class.
   *          Performs local LCIO records book-keeping for 
   *          LCIO read and writer instances
   */
  class SIOContext {
  private:
    SIOContext& operator=(const SIOContext &) = delete;
    SIOContext(const SIOContext &) = delete;

  public:
    using RecordMap = sio::record_map;
    using RecordNameMap = std::map<SIORecordType, std::string>;
    
  public:
    /**
     *  @brief  Constructor
     */
    SIOContext();
    
    /**
     *  @brief  Set the default compression flag to apply on 
     *          record creation (default is true)
     *
     *  @param  flag the default compression flag
     */
    void setDefaultCompress( bool flag );
    
    /**
     *  @brief  Finds and returns a registered record for this context
     *  
     *  @param  recordType the record type to lookup
     */
    sio::record_ptr getRecord( SIORecordType recordType );
    
    /**
     *  @brief  Get the record name given the record type
     *  
     *  @param  recordType the record type
     */
    std::string getRecordName( SIORecordType recordType ) const;
    
    /**
     *  @brief  Get the map of registered records
     */
    const RecordMap &getRecordMap() const;
    
    /**
     *  @brief  Set the compression flag for either a particluar registered 
     *          record or for all registered records (SIORecordType::All)
     *          
     *  @param  flag whether to set the compression
     *  @param  recordType the record type for compression
     */
    SIOContext &setCompress( bool flag = true, SIORecordType recordType = SIORecordType::All );
    
  private:
    
    void setupLCIORecords();
    
  private:
    /// The list of registered records in this context
    RecordMap               _records{};
    /// The map of record types to names for LCIO
    RecordNameMap           _allRecordNames{};
    /// The default compression mode on record creation
    bool                    _defaultCompress{true};
  };
  
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  
  inline const SIOContext::RecordMap &SIOContext::getRecordMap() const {
    return _records;
  }

} // namespace 

#endif // ifndef SIO_SIOCONTEXT_H
