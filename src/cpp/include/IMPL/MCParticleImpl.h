// -*- C++ -*-
#ifndef EVENT_MCPARTICLEIMPL_H
#define EVENT_MCPARTICLEIMPL_H 1
#include <vector>


#include "EVENT/MCParticle.h"

namespace IMPL {
  

  /** Implementation of MCParticle.
   * 
   * @author gaede
   * @version Mar 5, 2003
   */
  class MCParticleImpl : public EVENT::MCParticle  {
    
  public: 

    MCParticleImpl() ;
    
    /// Destructor.
    virtual ~MCParticleImpl() ;

    /** Returns the parent of this particle. Null if the mother (beam particle).
     */
    virtual const EVENT::MCParticle * getParent() const ;

    /** Returns the second parent of this particle, usually Null.
     */
    virtual const EVENT::MCParticle * getSecondParent() const ;

    /** Returns the daughters of this particle.
     */
    virtual const EVENT::MCParticleVec * getDaughters() const ;

    /** Returns the PDG code of the particle.
     */
    virtual int getPDG() const ;

    /** Returns the status for particles from the generator (HepEvt)
     * and for particles added in simulation:
     * <br> 0  empty line
     * <br> 1  undecayed particle, stable in the generator
     * <br> 2  particle decayed in the generator
     * <br> 3  documentation line
     * <br> 201 stable state, but created in the tracking in the detector
     * <br> 202 particle decayed in the tracking in the detector
     */
    virtual int getHepEvtStatus() const ;

    /** Returns the production vertex of the particle.
     */
          
    virtual const double * getVertex() const ;

    /** Returns the particle momentum at the production vertex.
     */
          
    virtual const float * getMomentum() const ;

    /** Returns the energy of the particle (at the vertex).
     */
    virtual float getEnergy() const ;

    /** Returns the particle's charge.
     */
    virtual float getCharge() const ;


    // set methods
    /** Sets the parent. 
     */
    void setParent( const EVENT::MCParticle *mom0 ) ;

    /** Sets a second parent.
     */
    void setSecondParent( const EVENT::MCParticle *mom1 ) ;

    /** Adds a daughter particle.
     */

    void addDaughter( const EVENT::MCParticle* daughter) ;

    /** Sets the PDG code.
     */
    void setPDG(int pdg ) ;

    /** Sets the HepEvt status.
     * @see MCParticle::getHepEvtStatus
     */
    void setHepEvtStatus( int status ) ;

     /** Sets the production vertex.
     */
    void setVertex( double vtx[3] ) ;
    
    /** Sets the momentum.
      */
    void setMomentum( float p[3] );

    /** Sets the energy.
     */
    void setEnergy( float en ) ;

    /** Sets the charge.
     */
    void setCharge( float c ) ;


  protected:

    const EVENT::MCParticle* _mother0 ;
    const EVENT::MCParticle* _mother1 ;
    int _pdg ;
    int _status ;
    double _vertex[3] ;
    float _p[3] ;
    float _energy ;
    float _charge ;
    EVENT::MCParticleVec _daughters ;


}; // class
}; // namespace IMPL
#endif /* ifndef EVENT_MCPARTICLEIMPL_H */
