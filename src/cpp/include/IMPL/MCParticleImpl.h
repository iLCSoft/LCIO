// -*- C++ -*-
#ifndef IMPL_MCPARTICLEIMPL_H
#define IMPL_MCPARTICLEIMPL_H 1
#include <vector>

#include "EVENT/LCIO.h"
#include "AccessChecked.h"

#include "EVENT/MCParticle.h"
#include <bitset>

namespace IMPL {
  
  // fg 01042004 : changed to use vector of pointers !
  //   typedef std::vector<EVENT::MCParticle**> MCParticlePVec ;
  // use  pointers to pointer to MCParticle 
  // as SIO needs the address of the pointer for pointer reallocation....

  /** Implementation of MCParticle.
   * 
   * @author gaede
   * @version $Id: MCParticleImpl.h,v 1.24 2010-01-21 16:28:25 gaede Exp $
   */
  class MCParticleImpl : public EVENT::MCParticle, public AccessChecked {
    
  public: 

    MCParticleImpl() ;

//     /** Copy c'tor */
//     MCParticleImpl(const EVENT::MCParticle& p) ;
    
    /// Destructor.
    virtual ~MCParticleImpl() ;

    virtual int id() const { return simpleUID() ; }

    /** Returns the parents of this particle. 
     */
    virtual const EVENT::MCParticleVec & getParents() const ;

    /** Returns the daughters of this particle. 
     */
    virtual const EVENT::MCParticleVec & getDaughters() const ;


    // /** Returns the i-th daughter of this particle.
    //  * Same as getDaughter() except for return type.
    //  *
    //  * @see getNumberOfDaughters
    //  */
    // virtual EVENT::MCParticle * getDaughter(int i) const ;


    // /** Returns the number of parents of this particle - 0 if mother.
    //  */
    // virtual int getNumberOfParents() const ;


    // /** Returns the i-th parent of this particle.
    //  */
    // virtual EVENT::MCParticle * getParent(int i) const ;


    /** Returns the endpoint of the particle in [mm] - returns NULL if
     *  undefined (not set). 
     *  Definition of the enpoint depends on the application that created 
     *  the particle, e.g. the start point of the shower in a calorimeter.
     */
    virtual const double* getEndpoint() const ;

    // /** Returns the number of daughters of this particle.
    //  */
    // virtual int getNumberOfDaughters() const ;


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
     * decayed in flight. Bits 31-16 are used to decode the information.
     * Use  the followng boolean functions to determine the 
     * proper simulator status:<br>
     * @see isCreatedInSimulation() 
     * @see isBackscatter() 
     * @see vertexIsNotEndpointOfParent() 
     * @see isDecayedInTracker() 
     * @see isDecayedInCalorimeter() 
     * @see hasLeftDetector() 
     * @see isStopped() 
     * @see isOverlay() 
     */
    virtual int getSimulatorStatus() const ;

    /** True if the particle has been created by the simulation program (rather than the generator).
     */
    virtual bool isCreatedInSimulation() const ;

    /** True if the particle is the result of a backscatter from a calorimeter shower.
     */
    virtual bool isBackscatter() const ;

    /** True if the particle's vertex is not the endpoint of the  parent particle.
     */
    virtual bool vertexIsNotEndpointOfParent() const ;

    /** True if the particle has interacted in a tracking region.
     */
    virtual bool isDecayedInTracker() const ;

    /** True if the particle has interacted in a calorimeter region.
     */
    virtual bool isDecayedInCalorimeter() const ;

    /** True if the particle has left the world volume undecayed.
     */
    virtual bool hasLeftDetector() const ;

    /** True if the particle has been stopped by the simulation program.
     */
    virtual bool isStopped() const ;

    /** True if the particle has been overlayed by the simulation (or digitization)  program.
     */
    virtual bool isOverlay() const ;

    /** Returns the production vertex of the particle.
     */
    virtual const double* getVertex() const ;

    /** The creation time of the particle in [ns] wrt. the event,
     *  e.g. for preassigned decays or decays in flight from 
     *  the simulator.
     */
    virtual float getTime() const ;

    /** Returns the particle momentum at the production vertex.
     */
    virtual const double * getMomentum() const ;


    /** Returns the particle momentum at the endpoint.
     */
    virtual const double* getMomentumAtEndpoint() const ;

    /** Returns the mass of the particle in [GeV].
     */
    virtual double getMass() const ;

    /** Returns the particle's charge.
     */
    virtual float getCharge() const ;


    /** Returns the energy of the particle (at the vertex) in [GeV] computed from
     * the particle's momentum and mass.
     */
    virtual double getEnergy() const ;

   /** Returns the spin
     */
    virtual const float* getSpin() const ;


   /** Returns the color flow
     */
    virtual const int* getColorFlow() const ;


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
    void setVertex( const double vtx[3] ) ;
    
    /** Sets the createion time.
     */
    void setTime( float time) ;

     /** Sets the particle endpoint.
      *  
      */
    void setEndpoint( const double pnt[3] ) ;
    
    /** Sets the momentum.
      */
    void setMomentum( const float p[3] );

    /** Sets the momentum.
     */
    void setMomentum( const double p[3] );

    /** Sets the momentum at the endpoint.
      */
    void setMomentumAtEndpoint( const float p[3] );

    /** Sets the momentum at the endpoint.
      */
    void setMomentumAtEndpoint( const double p[3] );

    /** Sets the mass.
     */
    void setMass( float m ) ;

    /** Sets the charge.
     */
    void setCharge( float c ) ;

    /** Sets the spin
      */
    void setSpin( const float spin[3] );

    /** Sets the color flow
      */
    void setColorFlow( const int cflow[2] );



    // setters for simulator status
    virtual void setCreatedInSimulation(bool val) ;

    virtual void setBackscatter(bool val) ;

    virtual void setVertexIsNotEndpointOfParent(bool val) ;

    virtual void setDecayedInTracker(bool val) ;

    virtual void setDecayedInCalorimeter(bool val) ;

    virtual void setHasLeftDetector(bool val) ;

    virtual void setStopped(bool val) ;

    virtual void setOverlay(bool val) ;

  protected:

    /** Adds a daughter particle - only called from addParent().
     */
    void addDaughter(  EVENT::MCParticle* daughter) ;

//     EVENT::MCParticle* _mother0 ;
//     EVENT::MCParticle* _mother1 ;
    int _pdg ;
    int _genstatus ;
    //    int _simstatus ;
    std::bitset<32> _simstatus ;
    double _vertex[3] ;
    double _endpoint[3] ;
    double _pEndpoint[3] ;
    double _p[3] ;
    double _mass ;
    float _charge ;
    float _time ;
    EVENT::MCParticleVec _parents ;
    EVENT::MCParticleVec _daughters ;
    bool _endpointSet ;
    float _spin[3] ;
    int _colorFlow[2] ;

}; // class
} // namespace IMPL
#endif /* ifndef IMPL_MCPARTICLEIMPL_H */
