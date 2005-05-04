#ifndef IMPL_SiliconRawHitImpl_H
#define IMPL_SiliconRawHitImpl_H 1

#include "EVENT/SiliconRawHit.h"
#include "AccessChecked.h"

namespace IMPL {

/** Default implementation of SiliconRawHit.
 * 
 * @author gaede
 * @version $Id: SiliconRawHitImpl.h,v 1.1 2005-05-04 10:25:35 gaede Exp $
 */
  
  class SiliconRawHitImpl :  public EVENT::SiliconRawHit , public AccessChecked {
    
  public: 
    
    /** Default Constructor - initializes all data to 0's. 
     */
    SiliconRawHitImpl() ;
    
    /// Destructor.
    virtual ~SiliconRawHitImpl() ;
    
    virtual int id() { return simpleUID() ; }
    
    /** The id0 of the cell that recorded the hit.
     */
    virtual int getCellID0() const  { return _cellID0 ; }

    /** The id1 of the cell that recorded the hit.
     */
    virtual int getCellID1() const { return _cellID1 ; }

    /** The detector specific time stamp of the hit.
     */
    virtual int getTimeStamp() const { return _timeStamp ; }

    /** The ADC counts of the hit.
     */
    virtual int getADCCounts() const { return _adcCounts ; }


    // setters
    void setCellID0( int cellID0 ) ; 
    void setCellID1( int cellID1 ) ; 
    void setTimeStamp( int timeStamp ) ; 
    void setADCCounts( int adcCounts ) ; 


  protected:

    int _cellID0 ;
    int  _cellID1 ;
    int  _timeStamp ;
    int  _adcCounts ;


  }; // class
} // namespace IMPL
#endif /* ifndef IMPL_SiliconRawHitImpl_H */
