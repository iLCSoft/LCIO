#ifndef IMPL_TrackerPulseImpl_H
#define IMPL_TrackerPulseImpl_H 1

#include "EVENT/TrackerPulse.h"
#include "AccessChecked.h"

#define TRKPULSENCOVMATRIX 3

namespace IMPL {

/** Default implementation of TrackerPulse.
 * 
 * @author gaede
 * @version $Id: TrackerPulseImpl.h,v 1.4 2010-05-12 14:50:00 engels Exp $
 */

  class TrackerPulseImpl :  public EVENT::TrackerPulse , public AccessChecked {
    
  public: 
    
    /** Default Constructor - initializes all data to 0's. 
     */
    TrackerPulseImpl() ;
    
    /// default copy constructor - use with care
    TrackerPulseImpl(const TrackerPulseImpl&) = default ;

    /// default assignment operator - use with care
    TrackerPulseImpl& operator=(const TrackerPulseImpl&) = default ;

    /// Destructor.
    virtual ~TrackerPulseImpl() ;
    
    virtual int id() const { return simpleUID() ; }
    

    /** Returns the first detector specific (geometrical) cell id.
     */
    virtual int getCellID0() const { return _cellID0 ; } 
    
    /** Returns the second detector specific (geometrical) cell id. Optional, check/set 
     *  flag(LCIO::TRAWBIT_ID1)==1.
     */
    virtual int getCellID1() const { return _cellID1 ; } 
    
    /** The time of the pulse.
     */
    virtual float getTime() const { return _time ; }

    // The time error
    //virtual float getTimeError() const { return _timeError ; }

    /** The integrated charge of the pulse // FIXME: unit ?.
     */
    virtual float getCharge() const { return _charge ; }

    //The charge error
    //virtual float getChargeError() const { return _chargeError ; }

    /** Covariance matrix of the charge (c) and time (t) measurements. Stored as lower triangle matrix, i.e.
     *  cov(c,c) , cov(t,c) , cov(t,t).
     *  Optional, check/set flag(LCIO::TRAWBIT_CM)==1.
     */
    virtual const EVENT::FloatVec & getCovMatrix() const { return _cov ; }


    /** The quality bit flag of the pulse - use the defined constants for referring to the bits.
     */
    virtual int getQuality() const { return _quality ; }
    
    /** Optionally the TrackerData that has been uesed to create the pulse
     *  can be stored with the pulse - NULL if none. Check the quality bits for reason
     *  why the spectrum has been stored for the pulse.
     */
    virtual EVENT::TrackerData * getTrackerData() const { return _corrData ; } 
  

    // setter methods
    void setCellID0( int cellID0 ) ; 
    void setCellID1( int cellID1 ) ; 
    void setTime( float time ) ; 
    //void setTimeError( float timeError ) ; 
    void setCharge( float charge ) ; 
    //void setChargeError( float chargeError ) ; 
    //void setCovMatrix( const float cov[TRKPULSENCOVMATRIX] ) ;
    void setCovMatrix( const float* cov ) ;
    void setCovMatrix( const EVENT::FloatVec & ) ;
    void setQuality( int quality ) ; 
    void setQualityBit( int bit , bool val=true ) ; 
    void setTrackerData( EVENT::TrackerData * corrData ) ; 
    
protected:

    int _cellID0 ;
    int _cellID1 ;
    float _time ;
    float _charge ;
    int   _quality ;
    EVENT::FloatVec _cov ;
    EVENT::TrackerData* _corrData ;
    
  }; // class
} // namespace IMPL
#endif /* ifndef IMPL_TrackerPulseImpl_H */
