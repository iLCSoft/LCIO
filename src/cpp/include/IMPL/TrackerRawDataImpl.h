#ifndef IMPL_TrackerRawDataImpl_H
#define IMPL_TrackerRawDataImpl_H 1

#include "EVENT/TrackerRawData.h"
#include "AccessChecked.h"

namespace IMPL {

/** Default implementation of TrackerRawData.
 * 
 * @author gaede
 * @version $Id: TrackerRawDataImpl.h,v 1.2 2006-08-04 16:52:47 gaede Exp $
 */

class TrackerRawDataImpl :  public EVENT::TrackerRawData , public AccessChecked {

public: 

  /** Default Constructor - initializes all data to 0's. 
   */
  TrackerRawDataImpl() ;
  
  /// Destructor.
  virtual ~TrackerRawDataImpl() ;
  
  virtual int id() const { return simpleUID() ; }
    
  /** Returns the first detector specific (geometrical) cell id.
   */
  virtual int getCellID0() const { return _cellID0 ; } 
  
  /** Returns the second detector specific (geometrical) cell id. Optional, check/set 
   *  flag(LCIO::TRAWBIT_ID1)==1.
   */
  virtual int getCellID1() const { return _cellID1 ; } 

  
  /** Returns the time.
   */
  virtual int getTime() const { return _time ; } 
  
  /** The measured ADC values.
   */
  virtual const EVENT::ShortVec & getADCValues() const { return _adc ; }
  
  void setCellID0( int cellID0 ) ; 
  void setCellID1( int cellID1 ) ; 
  void setTime( int time ) ; 

  /** Set the ADC vector by copying the values. 
   */
  void setADCValues( const EVENT::ShortVec & adc ) ;

  /** Allows direct access to the adc vector.
   *  Can be used e.g. to set the values:
   *  raw->adcValues().clear() ;
   *  raw->adcValues().resize( n ) ;
   *  raw->adcValues()[i] = 42 ;
   */
  EVENT::ShortVec & adcValues() ;
  

protected:

  int _cellID0 ;
  int _cellID1 ;
  int _channelID ;
  int  _time ;
  EVENT::ShortVec _adc{} ;
  
}; // class
} // namespace IMPL
#endif /* ifndef IMPL_TrackerRawDataImpl_H */
