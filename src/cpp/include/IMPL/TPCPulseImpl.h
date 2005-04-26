#ifndef IMPL_TPCPulseImpl_H
#define IMPL_TPCPulseImpl_H 1

#include "EVENT/TPCPulse.h"
#include "AccessChecked.h"

namespace IMPL {

/** Default implementation of TPCPulse.
 * 
 * @author gaede
 * @version $Id: TPCPulseImpl.h,v 1.1 2005-04-26 11:01:21 gaede Exp $
 */

  class TPCPulseImpl :  public EVENT::TPCPulse , public AccessChecked {
    
  public: 
    
    /** Default Constructor - initializes all data to 0's. 
     */
    TPCPulseImpl() ;
    
    /// Destructor.
    virtual ~TPCPulseImpl() ;
    
    virtual int id() { return simpleUID() ; }
    
    /** The detector specific channel id.
     */
    virtual int getChannelID() const { return _channelID ; } 
    
    /** The time of the pulse.
     */
    virtual float getTime() const { return _time ; }

    /** The integrated charge of the pulse // FIXME: unit ?.
     */
    virtual float getCharge() const { return _charge ; }

    /** The quality bit flag of the pulse - use the defined constants for referring to the bits.
     */
    virtual int getQuality() const { return _quality ; }
    
    /** Optionally the TPCCorrectedData that has been uesed to create the pulse
     *  can be stored with the pulse - NULL if none. Check the quality bits for reason
     *  why the spectrum has been stored for the pulse.
     */
    virtual EVENT::TPCCorrectedData * getTPCCorrectedData() const { return _corrData ; } 
  

    // setter methods
    void setChannelID( int channelID ) ; 
    void setTime( float time ) ; 
    void setCharge( float charge ) ; 
    void setQuality( int quality ) ; 
    void setQualityBit( int bit , bool val=true ) ; 
    void setTPCCorrectedData( EVENT::TPCCorrectedData * corrData ) ; 
    
protected:

    int   _channelID ;
    float _time ;
    float _charge ;
    int   _quality ;
    EVENT::TPCCorrectedData* _corrData ;
    
  }; // class
} // namespace IMPL
#endif /* ifndef IMPL_TPCPulseImpl_H */
