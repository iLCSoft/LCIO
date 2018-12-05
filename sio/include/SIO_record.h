// ----------------------------------------------------------------------------
// CVS $Id: SIO_record.h,v 1.2 2004-12-23 13:24:07 gaede Exp $
// ----------------------------------------------------------------------------
// => Controller for an SIO record.                                
// ----------------------------------------------------------------------------
//
// General Description:
//
// Each SIO_record controls one record type.                
//
// ----------------------------------------------------------------------------

#ifndef SIO_RECORD_H
#define SIO_RECORD_H 1

// -- std headers
#include <map>
#include <string>
#include <sstream>

// -- sio headers
#include <SIO_definitions.h>
#include <SIO_functions.h>
#include <SIO_block.h>

namespace sio {
  
  /**
   *  @brief  record class
   *          Describes a record content and handles the list of 
   *          associated blocks to read or write
   */
  class record {
  public:
    // typedefs for nice looking loops
    typedef block_map::iterator           iterator;
    typedef block_map::const_iterator     const_iterator;
    
  public:
    /**
     *  @brief  Constructor
     * 
     *  @param  name the record name
     */
    record(const std::string &name);
    
    /**
     *  @brief  Register a new block
     *
     *  @param  args  the arguments to pass to block constructor
     */
    template <typename BLOCK_TYPE, typename... Args, class = typename std::is_base_of<BLOCK_TYPE, block>::value>
    block_ptr add_block(Args... args);
    
    /**
     *  @brief  Get a block by name
     * 
     *  @param  name the block name
     */
    block_ptr get_block(const std::string &name) const;
    
    /**
     *  @brief  Remove a block with the target name
     *  
     *  @param  name the block name
     */
    unsigned int remove_block(const std::string &name);
    
    /**
     *  @brief  Clear blocks map
     */
    void clear_blocks();
    
    /**
     *  @brief  Iterator to beginning of block map
     */
    iterator begin();
    
    /**
     *  @brief  Iterator to end of block map
     */
    iterator end();
    
    /**
     *  @brief  Get the record name
     */
    const std::string &name() const;
    
    /**
     *  @brief  Get the compression option
     */
    bool compression_option() const;
    
    /**
     *  @brief  Whether to unpack the next records
     */
    bool unpack() const;
    
    /**
     *  @brief  Get the record option word
     */
    unsigned int options() const;
    
    /**
     *  @brief  Set the compression option
     * 
     *  @param  comp the compression option to set
     */
    bool set_compression( bool comp );
    
    /**
     *  @brief  Set whether to unpack the incoming records
     * 
     *  @param  unpack whether to unpack
     */
    bool set_unpack( bool unpack );
    
    /**
     *  @brief  Set the word option (set by sio::stream on read)
     *  
     *  @param opts the word options
     */
    void set_options( unsigned int opts );
    
  private:
    /// The record name
    std::string _name{};
    /// The registered blocks
    block_map _blocks{};
    /// The record option word (on write)
    unsigned int _options{0};
    /// Whether to unpack the incoming records (on read)
    bool _unpack{false};
  };
  
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  
  template <typename BLOCK_TYPE, typename... Args, class>
  inline block_ptr record::add_block(Args... args) {
    
    block_ptr blk = std::make_shared<BLOCK_TYPE>(args...);
    
    if (!SIO_functions::validateName(blk->name().c_str())) {
      std::stringstream ss; ss << "SIO [context] Invalid block name '" << blk->name() << "'";
      throw std::runtime_error(ss.str().c_str());
    }
    
    block_map::value_type entry = {blk->name(), blk};
    auto res = _blocks.insert(entry);
    
    if (!res.second) {
      std::stringstream ss; ss << "SIO [context] Block '" << blk->name() << "' already exists";
      throw std::runtime_error(ss.str().c_str());
    }
    
    return blk;
  }
  
  //----------------------------------------------------------------------------
  
  inline const std::string &record::name() const {
    return _name;
  }
  
  //----------------------------------------------------------------------------
  
  inline bool record::compression_option() const {
    return ((_options & SIO_OPT_COMPRESS) != 0 );
  }
  
  //----------------------------------------------------------------------------
  
  inline bool record::unpack() const {
    return _unpack;
  }
  
  //----------------------------------------------------------------------------
  
  inline unsigned int record::options() const {
    return _options;
  }
  
  //----------------------------------------------------------------------------
  
  inline bool record::set_compression( bool comp ) {
    unsigned int out = (_options & SIO_OPT_COMPRESS);
    _options &= ~SIO_OPT_COMPRESS;
    if( comp ) {
      _options |=  SIO_OPT_COMPRESS;
    }
    return (out != 0);
  }
  
  //----------------------------------------------------------------------------
  
  inline bool record::set_unpack( bool unp ) {
    bool out = _unpack;
    _unpack = unp;
    return out;
  }
  
  //----------------------------------------------------------------------------
  
  inline block_ptr record::get_block(const std::string &name) const {
    auto iter = _blocks.find(name);
    return (_blocks.end() != iter) ? iter->second : nullptr;
  }
  
  //----------------------------------------------------------------------------
  
  inline unsigned int record::remove_block(const std::string &name) {
    auto iter = _blocks.find(name);
    if(_blocks.end() != iter) {
      _blocks.erase(iter);
      return SIO_BLOCK_SUCCESS;
    }
    return SIO_BLOCK_NOTFOUND;
  }
  
  //----------------------------------------------------------------------------
  
  inline void record::clear_blocks() {
    _blocks.clear();
  }
  
  //----------------------------------------------------------------------------
  
  inline record::iterator record::begin() {
    return _blocks.begin();
  }
  
  //----------------------------------------------------------------------------
  
  inline record::iterator record::end() {
    return _blocks.end();
  }
  
  //----------------------------------------------------------------------------
  
  inline void record::set_options( unsigned int opts ) {
    _options = opts;
  }
  
}

#endif
