#ifndef EVENT_CALORIMETERHITIMPL_H
#define EVENT_CALORIMETERHITIMPL_H 1


#include "EVENT/MCParticle.h"
#include "EVENT/CalorimeterHit.h"
#include <vector>
#include "AccessChecked.h"

namespace IMPL {
  
  
/** Implementation of the real data CalorimeterHit.
 * 
 * @author gaede
 * @version $Id: CalorimeterHitImpl.h,v 1.16 2008-06-04 17:37:16 engels Exp $
 */

  class CalorimeterHitImpl : public EVENT::CalorimeterHit , public AccessChecked {
    
  public: 
    
    /** Default constructor, initializes values to 0.
     */
    CalorimeterHitImpl() ;

    CalorimeterHitImpl(const IMPL::CalorimeterHitImpl&) = default ;
    CalorimeterHitImpl& operator=(const IMPL::CalorimeterHitImpl&) = default ;

    /// Destructor.
    virtual ~CalorimeterHitImpl() ;
    
    virtual int id() const { return simpleUID() ; }

    /**Returns the detector specific (geometrical) cell id.
     */
    virtual int getCellID0() const ;
    
    /**Returns the second detector specific (geometrical) cell id.
     *  0 if information is not stored -  check the flag word (bit RCHBIT_ID1) 
     *  of the collection. Default is to store only cellid0.
     */
    virtual int getCellID1() const ;
    
    /** Returns the energy of the hit.
     */
    virtual float getEnergy() const ;
    
    /** Returns the error of the hit energy. Optional, check/set
     *  flag(LCIO::RCHBIT_ENERGY_ERROR)==1.
     */
    virtual float getEnergyError() const ;
 
    /** Returns the time of the hit in [ns]. Optional, check/set 
     *  flag(LCIO::RCHBIT_TIME)==1.
     */
    virtual float getTime() const ;

    /** Returns the position of the hit in world coordinates.
     *  NULL if information is not stored. Ask collection for flag, only 
     * available if bit LCIO.RCHBIT_LONG is set.
     */
    
    virtual const float * getPosition() const ;

    /** Type of hit. Mapping of integer types to type names
     * through collection parameters "CalorimeterHitTypeNames"
     * and "CalorimeterHitTypeValues".
     */
    virtual int getType() const ;

    /** The RawCalorimeterHit. As in the future there might be other types of 
     *  raw calorimeter hits the return type is LCObject.
     */
    virtual EVENT::LCObject * getRawHit() const ;

    // setters

    /** Sets the first cell id;
     */
    void setCellID0(int id0) ;

    /** Sets the second cell id;
     *  Only store if the flag word (bit RCHBIT_ID1) 
     *  of the collection is set. Default is to store only cellid0.
     */
    void setCellID1(int id1) ;
    
    /** Sets the energy/amplitude.
     */
    void setEnergy(float en) ;

    /** Sets the error of the hit energy.
     *  Not stored if flag RCHBIT_ENERGY_ERROR isn't set
     */
    void setEnergyError(float enerr) ;

    /** Sets the time.
     */
    void setTime(float t) ;
    
    /** Sets the position. Not stored if LCIO.RCHBIT_LONG isn't set.
     */
    void setPosition(const float pos[3])  ;
    
    void setType(int type) ;

    void setRawHit(EVENT::LCObject* rawHit ) ;

  protected:

    int _cellID0 ;
    int _cellID1 ;
    float _energy ;
    float _energyError ;
    float _time ;
    float _position[3] ;
    int _type ;

    EVENT::LCObject* _rawHit ;
    
  }; // class
} // namespace IMPL
#endif /* ifndef EVENT_CALORIMETERHITIMPL_H */
