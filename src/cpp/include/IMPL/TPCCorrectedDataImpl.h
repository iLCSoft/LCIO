#ifndef IMPL_TPCCorrectedDataImpl_H
#define IMPL_TPCCorrectedDataImpl_H 1

#include "EVENT/TPCCorrectedData.h"
#include "AccessChecked.h"

namespace IMPL {

/** Default implementation of TPCCorrectedData.
 * 
 * @author gaede
 * @version $Id: TPCCorrectedDataImpl.h,v 1.1 2005-04-26 11:01:21 gaede Exp $
 */

class TPCCorrectedDataImpl :  public EVENT::TPCCorrectedData , public AccessChecked {

public: 

  /** Default Constructor - initializes all data to 0's. 
   */
  TPCCorrectedDataImpl() ;
  
  /// Destructor.
  virtual ~TPCCorrectedDataImpl() ;
  
  virtual int id() { return simpleUID() ; }
    


  /** Returns the detector specific channel id.
   */
  virtual int getChannelID() const { return _channelID ; } 
  
  /** Returns the t0 of the spectrum.
   */
  virtual int getTime0() const { return _time0 ; } 
  
  /** The calibrated FADC spectrum.
   */
  virtual const EVENT::FloatVec & getCharge() const { return _charge ; }
  

  void setChannelID( int channelID ) ; 
  void setTime0( int time0 ) ; 

  /** Set the charge vector by copying the values. 
   */
  void setChargeVec( const EVENT::FloatVec & charge ) ;

  /** Allows direct access to the charge vector.
   *  Can be used e.g. to set the values:
   *  raw->charge().clear() ;
   *  raw->charge().resize( n ) ;
   *  raw->charge()[i] = 42 ;
   */
  EVENT::FloatVec & charge() ;
  

protected:

  int _channelID ;
  int  _time0 ;
  EVENT::FloatVec _charge ;
  
}; // class
} // namespace IMPL
#endif /* ifndef IMPL_TPCCorrectedDataImpl_H */
