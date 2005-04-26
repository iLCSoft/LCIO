#ifndef IMPL_TPCRawDataImpl_H
#define IMPL_TPCRawDataImpl_H 1

#include "EVENT/TPCRawData.h"
#include "AccessChecked.h"

namespace IMPL {

/** Default implementation of TPCRawData.
 * 
 * @author gaede
 * @version $Id: TPCRawDataImpl.h,v 1.1 2005-04-26 08:14:29 gaede Exp $
 */

class TPCRawDataImpl :  public EVENT::TPCRawData , public AccessChecked {

public: 

  /** Default Constructor - initializes all data to 0's. 
   */
  TPCRawDataImpl() ;
  
  /// Destructor.
  virtual ~TPCRawDataImpl() ;
  
  virtual int id() { return simpleUID() ; }
    


  /** Returns the detector specific channel id.
   */
  virtual int getChannelID() const { return _channelID ; } 
  
  /** Returns the t0 of the spectrum.
   */
  virtual int getTime0() const { return _time0 ; } 
  
  /** The actual FADC spectrum.
   */
  virtual const EVENT::ShortVec & getCharge() const { return _charge ; }
  

  void setChannelID( int channelID ) ; 
  void setTime0( int time0 ) ; 

  /** Set the charge vector by copying the values. 
   */
  void setChargeVec( const EVENT::ShortVec & charge ) ;

  /** Allows direct access to the charge vector.
   *  Can be used e.g. to set the values:
   *  raw->charge().clear() ;
   *  raw->charge().resize( n ) ;
   *  raw->charge()[i] = 42 ;
   */
  EVENT::ShortVec & charge() ;
  

protected:

  int _channelID ;
  int  _time0 ;
  EVENT::ShortVec _charge ;
  
}; // class
} // namespace IMPL
#endif /* ifndef IMPL_TPCRawDataImpl_H */
