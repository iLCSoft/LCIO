#ifndef IMPL_TPCHITIMPL_H
#define IMPL_TPCHITIMPL_H 1

#include "EVENT/TPCHit.h"
#include "AccessChecked.h"

namespace IMPL {

/** Implementation of the real data TPCHit.
 * 
 * @author gaede
 * @version Sep 11, 2003
 */

class TPCHitImpl :  public EVENT::TPCHit , public AccessChecked {

public: 

  /** Default Constructor - initializes all data to 0's. 
   */
  TPCHitImpl() ;
  
  /// Destructor.
  virtual ~TPCHitImpl() ;
  
  virtual int id() const { return simpleUID() ; }
    
  /** Returns the detector specific cell id.
   */
  virtual int getCellID() const;
  
  /** Returns the  time of the hit.
   */
  virtual float getTime() const;
  
  /** Returns the integrated charge of the hit.
   */
  virtual float getCharge() const;
  
  /** Returns a quality flag for the hit.
   */
  virtual int getQuality() const;
  
  /** Return the number of raw data (32-bit) words stored for the hit.
   *  Check the flag word (bit TPCBIT_RAW) of the collection if raw data is
   *  stored at all.
   */
  virtual int getNRawDataWords()const;
  
  /** Return the raw data (32-bit) word at i.
   *  Check the flag word (bit TPCBIT_RAW) of the collection if raw data is
   *  stored at all.
   */
  virtual int getRawDataWord(int i) const;

  /** Set the cell id.
   */
  virtual void setCellID(int cellID) ;

  /** Set the time.
   */
  virtual void setTime(float time);

  /** Set the charge.
   */
  virtual void setCharge(float charge);

  /** Set the cell quality word.
   */
  virtual void setQuality(int quality);

  /** Set the raw data.
   */
  virtual void setRawData(const int* raw, int size ) ;

protected:
  /** initialize the raw data array - for faster reading
   */
  virtual void initRawArray(int size) ;


  int _cellID ;
  float _time ;
  float _charge ;
  int _quality ;
  int _rawSize ;
  int* _rawArray ;

}; // class
} // namespace IMPL
#endif /* ifndef IMPL_TPCHITIMPL_H */
