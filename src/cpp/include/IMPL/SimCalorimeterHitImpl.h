#ifndef EVENT_CALORIMETERHITIMPL_H
#define EVENT_CALORIMETERHITIMPL_H 1


#include "EVENT/MCParticle.h"
#include "EVENT/SimCalorimeterHit.h"
#include <vector>

namespace IMPL {
  
  // helper class for particle contributions to hits
  typedef struct MCParticleCont_S{
    const EVENT::MCParticle* Particle ;
    float Energy ;
    float Time ;
    int   PDG ;
  }  MCParticleCont  ;
  
  typedef std::vector< MCParticleCont* > MCParticleContVec ;
  
/** Implementation of the generic SimCalorimeterHit.
 * 
 * @author gaede
 * @version Mar 5, 2003
 */

  class SimCalorimeterHitImpl : public EVENT::SimCalorimeterHit {
    
  public: 
    
    /** Default constructor, initializes values to 0.
     */
    SimCalorimeterHitImpl() ;

    /** Copy constructor. Not yet - needs pointer chasing ... 
     */
    //    SimCalorimeterHitImpl(const SimCalorimeterHit& hit) ;

    /// Destructor.
    virtual ~SimCalorimeterHitImpl() ;
    
    /** Create a deep copy of itself.
     * Overwrites return type in LCObject::clone().
     * Not yet - as AID genereates virtual inheritance and
     * g++ doesn't support virtual constructors with virtual inheritance
     * TODO - change org.freehep.aid.CPPHeaderGenerator !
     */
    //virtual  SimCalorimeterHit* clone() const;
    virtual LCObject* clone() const;
    
    /**Returns the detector specific (geometrical) cell id.
     */
    virtual int getCellID0() const ;
    
    /**Returns the second detector specific (geometrical) cell id.
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
    
    /** Returns the number of MC particles that caused the hit - 0 if 
     * information is not stored. Be aware of double counting if PDG
     * for secondaries is stored.
     * @see SimCalorimeterHit.getPDGCont
     */
    virtual int getNMCParticles() const ;
    
    /** Returns the i-th particle that contributed to the hit.
     */
    virtual const DATA::MCParticleData * getParticleContData(int i) const ;
    
    /** Returns the i-th particle that contributed to the hit.
     *  Same as getParticleContData() except for return type and exception.
     *
     * @throws DataNotAvailableException
     * @see MCParticle
     * @see getNMCParticles()
     */
    virtual const EVENT::MCParticle * getParticleCont(int i) const throw (EVENT::DataNotAvailableException) ;

    /** Returns the energy of the i-th particle that contributed to the hit.
     */ 
    virtual float getEnergyCont(int i) const ;
    
    /** Returns the time information of the i-th particle that contributed to the hit.
     *  TODO : definition of time. 
     */ 
    virtual float getTimeCont(int i) const ;
    
    /** Returns the PDG code of the secondary type of the i-th particle that 
     * contributed to the hit. Ask the flag word (bit CHBIT_PDG) of the collection 
     * wether this information is available. If so, every primary will show up in 
     * the list of contributions once for every type of secondaries.  If not the 
     * value returned is undefined. 
     */ 
    virtual int getPDGCont(int i) const ;

    // setters
    /** Sets the first cell id;
     */
    void setCellID0(int id0) ;

    /** Sets the second cell id;
     */
    void setCellID1(int id1) ;
    
    /** Sets the energy.
     */
    void setEnergy(float en) ;
    
    /** Sets the position. Not stored if LCIO.CHBIT_LONG isn't set.
     */
    void setPosition(float pos[3])  ;
    
    /** Adds a MCParticle contribution to the hit. If you want to store
     *  the PDG of the secondary for every hit make sure the flag word bit 
     *  LCIO::CHBIT_PDG is set.
     */
    void addMCParticleContribution( const EVENT::MCParticle *p,
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
}; // namespace IMPL
#endif /* ifndef EVENT_CALORIMETERHITIMPL_H */
