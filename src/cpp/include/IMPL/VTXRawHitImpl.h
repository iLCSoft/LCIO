#ifndef IMPL_VTXRawHitImpl_H
#define IMPL_VTXRawHitImpl_H 1

#include "EVENT/VTXRawHit.h"
#include "AccessChecked.h"

namespace IMPL {

/** Default implementation of VTXRawHit.
 * 
 * @author gaede
 * @version $Id: VTXRawHitImpl.h,v 1.1 2005-04-26 10:50:15 gaede Exp $
 */
  
  class VTXRawHitImpl :  public EVENT::VTXRawHit , public AccessChecked {
    
  public: 
    
    /** Default Constructor - initializes all data to 0's. 
     */
    VTXRawHitImpl() ;
    
    /// Destructor.
    virtual ~VTXRawHitImpl() ;
    
    virtual int id() { return simpleUID() ; }
    
    /** The id of the module that recorded the hit.
     */
    virtual int getModuleID() const  { return _moduleID ; }

    /** The row number of the hit.
     */
    virtual int getRow() const { return _row ; }

    /** The column number of the hit - undefined for strip detectors.
     */
    virtual int getColumn() const { return _column ; }

    /** The ADC counts of the hit.
     */
    virtual int getADCCounts() const { return _adcCounts ; }


    // setters
    void setModuleID( int moduleID ) ; 
    void setRow( int row ) ; 
    void setColumn( int column ) ; 
    void setADCCounts( int adcCounts ) ; 


  protected:

    int _moduleID ;
    int  _row ;
    int  _column ;
    int  _adcCounts ;


  }; // class
} // namespace IMPL
#endif /* ifndef IMPL_VTXRawHitImpl_H */
