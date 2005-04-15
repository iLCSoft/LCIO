#ifndef EVENT_LCFLAGIMPL_H
#define EVENT_LCFLAGIMPL_H 1

#include "EVENT/LCFlag.h"

namespace IMPL {

/** Implementation of helper class to create and interpret the 32-bit flag word in LCCollections.
 *  Bits are defined in class LCIO.
 *
 * @see LCIO
 * @author gaede 
 * @version May 6, 2003
 */
class LCFlagImpl : public EVENT::LCFlag {

private:
  int _flag ;

public: 
    
  /** Constructor initializing flag with 0.
   */
  LCFlagImpl() ;

  /**Constructor for evaluating an existing flag word.
   */
  LCFlagImpl(int flag) ;

  /// Destructor.
  virtual ~LCFlagImpl() { /* nop */; }
  
  /** Returns true if bit at given index is set.
   */
  virtual bool bitSet(int index) const ;
  
  /**Returns the flag word.
   */
  virtual int getFlag() const ;

  /** Sets bit to 1.
   */
  virtual void setBit(int bit) ;
  
  /** Sets bit to 0.
   */
  virtual void unsetBit(int bit) ;
  
}; // class
} // namespace EVENT
#endif /* ifndef EVENT_LCFLAGIMPL_H */
