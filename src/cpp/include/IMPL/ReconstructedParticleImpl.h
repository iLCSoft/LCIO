// -*- C++ -*-
#ifndef IMPL_RECONSTRUCTEDPARTICLEIMPL_H
#define IMPL_RECONSTRUCTEDPARTICLEIMPL_H 1


#include "EVENT/ReconstructedParticle.h"
#include "IMPL/ParticleIDImpl.h"
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

    /** Flag word that decodes some particle type information<br>
     *  bit 31, primary: 0 secondary, 1 primary <br>   
     *  bits0-15: type: <br>
     *  ReconstructedParticle::SINGLE,<br>  
     *  ReconstructedParticle::V0,<br>
     *  ReconstructedParticle::COMPOUND,<br>
     *  ReconstructedParticle::JET<br>
     */
    virtual int getTypeFlag() const ;

    /** The magnitude of the reconstructed particle's momentum,
     */
    virtual const float* getMomentum() const ;

    /** Energy of the  reconstructed particle.
     */
    virtual float getEnergy() const ;

    /** Covariance matrix of the reconstructed particle's 4vector (10 parameters).
     */
    virtual const DATA::FloatVec & getCovMatrix() const ;

    /** Mass of the  reconstructed particle.
     */
    virtual float getMass() const ;

    /** Charge of the reconstructed particle.
     */
    virtual float getCharge() const ;

    /** Reference point of the reconstructedParticle parameters.
     */
    virtual const float* getReferencePoint() const ;

    /** The particle Id's sorted by their probability.
     * @see ParticleIDData
     */
    virtual const DATA::ParticleIDDataVec & getParticleIDsData() const ;

    /** The particle Id's sorted by their probability.
     * @see ParticleID
     */
    virtual const EVENT::ParticleIDVec & getParticleIDs() const ;

    /** The particles (as ReconstructedParticleData objects) that have
     *  been combined to this particle.
     */
    virtual const DATA::ReconstructedParticleDataVec & getParticlesData() const ;

    /** The reconstructed particles that have been combined to this particle.
     */
    virtual const EVENT::ReconstructedParticleVec& getParticles() const ; 


    /** The weights of the reconstructed particles combined to this particle
     */
    virtual const DATA::FloatVec & getParticleWeights() const ;

    /** The clusters (as ClusterData objects) that have been used for this particle.
     */
    virtual const DATA::ClusterDataVec & getClustersData() const ;

    /** The clusters that have been used for this particle.
     */
    virtual const EVENT::ClusterVec& getClusters() const ; 
    
    /** The weights of cluster contributions to this particle
     */
    virtual const DATA::FloatVec & getClusterWeights() const ;

    /** The tracks (as TrackData objects) that have been used for this particle.
     */
    virtual const DATA::TrackDataVec & getTracksData() const ;

    /** The tracks that have been used for this particle.
     */
    virtual const EVENT::TrackVec& getTracks() const ; 
    
    /** The weights of track contributions to this particle
     */
    virtual const DATA::FloatVec & getTrackWeights() const ;

    /** The MCParticle assigned to this reconstructed particle.
     * @see MCParticleData
     */
    virtual const DATA::MCParticleDataVec & getMCParticlesData() const ;

    /** The MCParticle assigned to this reconstructed particle.
     * @see MCParticle
     */
    virtual const EVENT::MCParticleVec& getMCParticles() const ;

    /** The weights of the MCParticle assignment.
     */
    virtual const DATA::FloatVec & getMCParticleWeights() const ;

    // setters
    void setTypeFlag( int typeFlag)  ;
    void setMomentum( const float* momentum ) ;
    void setEnergy( float energy) ;
    void setCovMatrix( const float* cov ) ;
    void setCovMatrix( const DATA::FloatVec& ) ;
    void setMass( float mass ) ;
    void setCharge( float charge ) ;
    void setReferencePoint( const float* reference ) ;
    void addParticleID( const EVENT::ParticleID*  pid ) ;
    void addParticle( const ReconstructedParticle* particle , float weight = 1.0 ) ;
    void addCluster( const EVENT::Cluster* cluster, float weight = 1.0 ) ;
    void addTrack( const EVENT::Track* track, float weight = 1.0 ) ;
    void addMCParticle( const EVENT::MCParticle* mcParticle , float weight = 1.0 ) ;

  protected:
    int _typeFlag ;
    float _momentum[3] ;
    float _energy ;
    DATA::FloatVec _cov ;
    float _mass ;
    float _charge ;
    float _reference[3] ;
    EVENT::ParticleIDVec _pid ;
    EVENT::ReconstructedParticleVec _particles ;
    DATA::FloatVec _particleWeights ;
    EVENT::ClusterVec _clusters ;
    DATA::FloatVec _clusterWeights ;
    EVENT::TrackVec _tracks ;
    DATA::FloatVec _trackWeights ;
    EVENT::MCParticleVec _mcParticles ;
    DATA::FloatVec _mcParticleWeights ;
    
}; // class

}; // namespace IMPL
#endif /* ifndef IMPL_RECONSTRUCTEDPARTICLEIMLP_H */
