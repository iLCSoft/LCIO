#ifndef IMPL_SIMCALORIMETERHITIMPL_H
#define IMPL_SIMCALORIMETERHITIMPL_H 1


#include "EVENT/MCParticle.h"
#include "EVENT/SimCalorimeterHit.h"
#include "AccessChecked.h"
#include <vector>

namespace IMPL {
  
  // helper class for particle contributions to hits
  typedef struct MCParticleCont_S{
    EVENT::MCParticle* Particle ;
    float Energy ;
    float Time ;
    int   PDG ;
  }  MCParticleCont  ;
  
  typedef std::vector< IMPL::MCParticleCont* > MCParticleContVec ;
  
/** Implementation of the generic SimCalorimeterHit.
 * 
 * @author gaede
 * @version Mar 5, 2003
 */

  class SimCalorimeterHitImpl : public EVENT::SimCalorimeterHit
			      , public AccessChecked {
    
  public: 
    
    /** Default constructor, initializes values to 0.
     */
    SimCalorimeterHitImpl() ;

    /** Copy constructor - shallow copy. 
     */
    SimCalorimeterHitImpl(const EVENT::SimCalorimeterHit& hit) ;

    /// Destructor.
    virtual ~SimCalorimeterHitImpl() ;
    
    virtual int id() const { return simpleUID() ; }

//     /** Create a deep copy of itself.
//      * Overwrites return type in LCObject::clone().
//      * Not yet - as AID genereates virtual inheritance and
//      * g++ doesn't support virtual constructors with virtual inheritance
//      * TODO - change org.freehep.aid.CPPHeaderGenerator !
//      */
//     //virtual  SimCalorimeterHit* clone() const;
//     virtual LCObject* clone() const;
    
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
    
    /** Returns the number of MC contributions to the hit. 0 if 
     * information is not stored. Renamed to getNMCContributions.
     * @deprecated
     * @see getNMCContributions()
     */
    virtual int getNMCParticles() const ;
    
    /** Returns the number of MC contributions to the hit. 0 if 
     * information is not stored. There are two levels of detail: if 
     * collection flag bit LCIO.CHBIT_PDG==1 then all simulator steps' 
     * contributions to the hit are stored, otherwise there is only one 
     * contribution for every particle entering the calorimeter.
     */
    virtual int getNMCContributions() const ;

    /** Returns the energy in [GeV] of the i-th contribution to the hit.
     * @see getNMCContributions()
     */ 
    virtual float getEnergyCont(int i) const ;

    /** Returns the time of the i-th in [ns]  contribution to the hit.
     * @see getNMCContributions()
     */ 
    virtual float getTimeCont(int i) const ;

    /** Returns the PDG code of the shower particle that caused this contribution.
     *  Check the flag word bit LCIO.CHBIT_PDG of the collection whether this information 
     *  is available. 
     * @see getNMCContributions()
     */ 
    virtual int getPDGCont(int i) const ;

    /** Returns the MCParticle that caused the shower responsible for this contribution to the hit.
     *  This is the particle that flew into the calorimeter and not the shower particle that made the 
     *  energy deposition.
     * @see getNMCContributions()
     */
    virtual EVENT::MCParticle * getParticleCont(int i) const ;

    // setters
    /** Sets the first cell id;
     */
    void setCellID0(int id0) ;

    /** Sets the second cell id;
     *  Only store if the flag word (bit CHBIT_ID1) 
     *  of the collection is set. Default is to store only cellid0.
     */
    void setCellID1(int id1) ;
    
    /** Sets the energy.
     */
    void setEnergy(float en) ;
    
    /** Sets the position. Not stored if LCIO.CHBIT_LONG isn't set.
     */
    void setPosition(float pos[3])  ;
    
    /** Adds an MCParticle contribution to the hit.
     *  There are three different ways to use this method:<br>
     *  <ol>
     *  <li>p==0, just add the energy to the total energy - create no MCParticleContribution.
     *      Note: this results in non standard LCIO files - use one of the following:</li>
     *  <li>p!=0, pdg==0, add the energy contribution for this primary MCParticle if it exists, 
     *      create a new contribution if it doesn't</li>
     *  <li>p!=0, pdg!=0, create a new MCParticleContribution in any case - this is used for 
     *      the detailed mode, where every simulator step produces a contribution.</li>
     *  </ol>
     *  If you want to store the PDG of the secondary (case 3.) set the flag word bit LCIO::CHBIT_PDG.<br>
     */
    void addMCParticleContribution( EVENT::MCParticle *p,
				    float en,
				    float t,
				    int pdg=0 ) ; 

  protected:

    int _cellID0 ;
    int _cellID1 ;
    float _energy ;
    float _position[3] ;

    MCParticleContVec _vec ;
    
  }; // class
} // namespace IMPL
#endif /* ifndef IMPL_SIMCALORIMETERHITIMPL_H */
