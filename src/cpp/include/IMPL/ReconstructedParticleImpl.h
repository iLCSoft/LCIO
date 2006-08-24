// -*- C++ -*-
#ifndef IMPL_RECONSTRUCTEDPARTICLEIMPL_H
#define IMPL_RECONSTRUCTEDPARTICLEIMPL_H 1


#include "EVENT/ReconstructedParticle.h"
#include "AccessChecked.h"


#define NCOVARIANCE 10


namespace IMPL {


/** Implementation of ReconstructedParticle.
 *
 * @see ReconstructedParticle
 * @author gaede
 * @version Mar 30, 2003
 */
  class ReconstructedParticleImpl : public EVENT::ReconstructedParticle, public AccessChecked {
    
  public: 
    
    /** Default constructor, initializes values to 0.
     */
    ReconstructedParticleImpl() ;
    
    /// Destructor.
    virtual ~ReconstructedParticleImpl() ; 

    virtual int id() const { return simpleUID() ; }


    /** Type of reconstructed particle.
     *  Check/set collection parameterrs ReconstructedParticleTypeNames and 
     *  ReconstructedParticleTypeValues.
     */
    virtual int getType() const ;

    /** Convenient method - same as (getParticles().size() > 0 ).
     */
    virtual bool isCompound() const { return getParticles().size() > 0 ; }

    /** The magnitude of the reconstructed particle's momentum,
     */
    virtual const double* getMomentum() const ;

    /** Energy of the  reconstructed particle.
     */
    virtual double getEnergy() const ;

    /** Covariance matrix of the reconstructed particle's 4vector (10 parameters).
     */
    virtual const EVENT::FloatVec & getCovMatrix() const ;

    /** Mass of the  reconstructed particle, set independently from four vector quantities.
     */
    virtual double getMass() const ;

    /** Charge of the reconstructed particle.
     */
    virtual float getCharge() const ;

    /** Reference point of the reconstructedParticle parameters.
     */
    virtual const float* getReferencePoint() const ;

    /** The particle Id's sorted by their likelihood.
     * @see ParticleID
     */
    virtual const EVENT::ParticleIDVec & getParticleIDs() const ;

    /** The particle Id used for the kinematics of this particle.
     * @see ParticleID
     */
    virtual  EVENT::ParticleID * getParticleIDUsed() const ;


    /** The overall goodness of the PID on a scale of [0;1].
     */
    virtual float getGoodnessOfPID() const ;

    /** The reconstructed particles that have been combined to this particle.
     */
    virtual const EVENT::ReconstructedParticleVec & getParticles() const ;

    /** The clusters that have been used for this particle.
     */
    virtual const EVENT::ClusterVec & getClusters() const ;


    /** The tracks that have been used for this particle.
     */
    virtual const EVENT::TrackVec & getTracks() const ;

    /** The start vertex associated to this particle
    */
    virtual EVENT::Vertex * getStartVertex() const ;

    /** The vertex where the particle decays
     *  This method actually returns the start vertex from the first daughter particle found
    */
    virtual EVENT::Vertex * getEndVertex() const ;


    // setters
    void setType(int type) ;
    //     void setPrimary(bool primary) ;
    //    void setTypeFlag( int typeFlag)  ;
    void setMomentum( const float* momentum ) ;
    void setMomentum( const double* momentum ) ;
    void setEnergy( float energy) ;
    void setCovMatrix( const float* cov ) ;
    void setCovMatrix( const EVENT::FloatVec& ) ;
    void setMass( float mass ) ;
    void setCharge( float charge ) ;
    void setReferencePoint( const float* reference ) ;
    void addParticleID( EVENT::ParticleID*  pid ) ;
    void setParticleIDUsed( EVENT::ParticleID*  pid ) ;
    void setGoodnessOfPID( float goodness ) ;
    void addParticle( EVENT::ReconstructedParticle* particle ) ;
    void addCluster( EVENT::Cluster* cluster) ;
    void addTrack( EVENT::Track* track) ;
//     void addMCParticle( EVENT::MCParticle* mcParticle , float weight = 1.0 ) ;
    void setStartVertex( EVENT::Vertex * sv ) ;
    //void setEndVertex( EVENT::Vertex * ev ) ;

  protected:

    int _type ;
    double _momentum[3] ;
    double _energy ;
    EVENT::FloatVec _cov ;
    double _mass ;
    float _charge ;
    float _reference[3] ;
    EVENT::ParticleID* _pidUsed ;
    float _goodnessOfPID ;
    EVENT::ParticleIDVec _pid ;
    EVENT::ReconstructedParticleVec _particles ;
//     EVENT::FloatVec _particleWeights ;
    EVENT::ClusterVec _clusters ;
//     EVENT::FloatVec _clusterWeights ;
    EVENT::TrackVec _tracks ;
//     EVENT::FloatVec _trackWeights ;
//     EVENT::MCParticleVec _mcParticles ;
//     EVENT::FloatVec _mcParticleWeights ;

    EVENT::Vertex* _sv ;
    //EVENT::Vertex* _ev ;
    
}; // class

} // namespace IMPL
#endif /* ifndef IMPL_RECONSTRUCTEDPARTICLEIMLP_H */
