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
 * @version Aug 8, 2003
 */

  class CalorimeterHitImpl : public EVENT::CalorimeterHit , public AccessChecked {
    
  public: 
    
    /** Default constructor, initializes values to 0.
     */
    CalorimeterHitImpl() ;

    /** Copy constructor. Not yet - needs pointer chasing ... 
     */
    //    CalorimeterHitImpl(const CalorimeterHit& hit) ;

    /// Destructor.
    virtual ~CalorimeterHitImpl() ;
    
    /**Returns the detector specific (geometrical) cell id.
     */
    virtual int getCellID0() const ;
    
    /**Returns the second detector specific (geometrical) cell id.
     *  0 if information is not stored -  check the flag word (bit CHBIT_ID1) 
     *  of the collection. Default is to store only cellid0.
     */
    virtual int getCellID1() const ;
    
    /** Returns the energy of the hit.
     */
    virtual float getEnergy() const ;
    
    /** Returns the position of the hit in world coordinates.
     *  NULL if information is not stored. Ask collection for flag, only 
     * available if bit LCIO.CHBIT_LONG is set.
     */
    
    virtual const float * getPosition() const ;
    // setters
    /** Sets the first cell id;
     */
    void setCellID0(int id0) ;

    /** Sets the second cell id;
     *  Only store if the flag word (bit CHBIT_ID1) 
     *  of the collection is set. Default is to store only cellid0.
     */
    void setCellID1(int id1) ;
    
    /** Sets the energy/amplitude.
     */
    void setEnergy(float en) ;
    
    /** Sets the position. Not stored if LCIO.CHBIT_LONG isn't set.
     */
    void setPosition(float pos[3])  ;
    

  protected:

    int _cellID0 ;
    int _cellID1 ;
    float _energy ;
    float _position[3] ;

    
  }; // class
}; // namespace IMPL
#endif /* ifndef EVENT_CALORIMETERHITIMPL_H */
