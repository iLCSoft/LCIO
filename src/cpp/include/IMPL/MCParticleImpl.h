#ifndef IMPL_MCPARTICLEIMPL_H
#define IMPL_MCPARTICLEIMPL_H 1
#include <vector>

#include "EVENT/LCIO.h"
#include "AccessChecked.h"

#include "EVENT/MCParticle.h"


namespace IMPL {
  
  typedef std::vector<EVENT::MCParticle**> MCParticlePVec ;
  // use  pointers to pointer to MCParticle 
  // as SIO needs the address of the pointer for pointer reallocation....

  /** Implementation of MCParticle.
   * 
   * @author gaede
   * @version Mar 5, 2003
   */
  class MCParticleImpl : public EVENT::MCParticle, public AccessChecked {
    
  public: 

    MCParticleImpl() ;
    
    /// Destructor.
    virtual ~MCParticleImpl() ;

    /** Returns the i-th parent of this particle.
     * Same as getParentData() except for return type.
     *
     * @see MCParticleData.getNumberOfParents()
     */
    virtual MCParticle * getParent(int i) const ;

//     /** Returns the parent of this particle. Returns Null if the mother (
//      * beam particle). 
//      * Same as getParentData() except for return type.
//      */
//     virtual EVENT::MCParticle * getParent() const ;

//     /** Returns the second parent of this particle. 
//      * Same as getSecondParentData() except for return type.
//      */
//     virtual EVENT::MCParticle * getSecondParent() const ; 

    /** Returns the i-th daughter of this particle.
     * Same as getDaughterData() except for return type.
     *
     * @see getNumberOfDaughters
     */
    virtual EVENT::MCParticle * getDaughter(int i) const ;


    /** Returns the number of parents of this particle - 0 if mother.
     */
    virtual int getNumberOfParents() const ;

    /** Returns the i-th parent of this particle.
     */
    virtual MCParticleData * getParentData(int i) const ;

//     /** Returns the parent of this particle. Null if the mother (beam particle).
//      */
//     virtual DATA::MCParticleData * getParentData() const ;

//     /** Returns the second parent of this particle, usually Null.
//      */
//     virtual DATA::MCParticleData * getSecondParentData() const ;


    /** Returns the i-th daughter of this particle.
     *  Unchecked access to vector holding daughters, thus faster than getDaughter(int i).
     */
    virtual DATA::MCParticleData * getDaughterData(int i) const ;


    /** Returns the endpoint of the particle in [mm].
     *  Definition of the enpoint depends on the application that created 
     *  the particle, e.g. the start point of the shower in a calorimeter.
     *  If the particle has daughters, the corresponding vertex is returned.
     *  Never returns NULL.
     */
    virtual const double* getEndpoint() const ;

    /** Returns the number of daughters of this particle.
     */
    virtual int getNumberOfDaughters() const ;


    /** Returns the PDG code of the particle.
     */
    virtual int getPDG() const ;

    /** Returns the status for particles from the generator
     * <br> 0  empty line
     * <br> 1  undecayed particle, stable in the generator
     * <br> 2  particle decayed in the generator
     * <br> 3  documentation line
     */
    virtual int getGeneratorStatus() const ;

    /** Returns the status for particles from the simulation, e.g.
     * decayed in flight.
     * FIXME: Needs to be defined.
     */
    virtual int getSimulatorStatus() const ;


    /** Returns the production vertex of the particle.
     */
          
    virtual const double * getVertex() const ;

    /** Returns the particle momentum at the production vertex.
     */
          
    virtual const float * getMomentum() const ;

    /** Returns the mass of the particle in [GeV].
     */
    virtual float getMass() const ;

    /** Returns the particle's charge.
     */
    virtual float getCharge() const ;


    /** Returns the energy of the particle (at the vertex) in [GeV] computed from
     * the particle's momentum and mass.
     */
    virtual float getEnergy() const ;

    // set methods
    /** Adds a parent particle. 
     */
    void addParent(  EVENT::MCParticle *mom ) ;

//     // set methods
//     /** Sets the parent. 
//      */
//     void setParent(  EVENT::MCParticle *mom0 ) ;

//     /** Sets a second parent.
//      */
//     void setSecondParent(  EVENT::MCParticle *mom1 ) ;


    /** Sets the PDG code.
     */
    void setPDG(int pdg ) ;

    /** Sets the Generator status.
     */
    void setGeneratorStatus( int status ) ;

    /** Sets the Simulator status.
     */
    void setSimulatorStatus( int status ) ;

     /** Sets the production vertex.
     */
    void setVertex( double vtx[3] ) ;
    
     /** Sets the particle endpoint.
      *  Will be ignored for particles that have daughters.
      */
    void setEndpoint( double pnt[3] ) ;
    
    /** Sets the momentum.
      */
    void setMomentum( float p[3] );

    /** Sets the mass.
     */
    void setMass( float m) ;

    /** Sets the charge.
     */
    void setCharge( float c ) ;


  protected:

    /** Adds a daughter particle - only called from addParent().
     */
    void addDaughter(  EVENT::MCParticle* daughter) ;

//     EVENT::MCParticle* _mother0 ;
//     EVENT::MCParticle* _mother1 ;
    int _pdg ;
    int _genstatus ;
    int _simstatus ;
    double _vertex[3] ;
    double _endpoint[3] ;
    float _p[3] ;
    float _mass ;
    float _charge ;
    MCParticlePVec _parentsP ;
    MCParticlePVec _daughtersP ;
    
}; // class
}; // namespace IMPL
#endif /* ifndef IMPL_MCPARTICLEIMPL_H */
